#include "latticeGeneric.h"

void Lattice::depolarisingError(double p, std::mt19937& engine, std::uniform_real_distribution<double>& dist)
{
    for (int i : qubitIndices)
    {
        if (dist(engine) < p)
        {
            double x = dist(engine);
            if (0 <= x  && x < 1/3) qubitsX[i] = (qubitsX[i] + 1) % 2; 
            else if (1/3 <= x && x < 2/3) qubitsZ[i]  = (qubitsZ[i] + 1) % 2; 
            else if (2/3 <= x && x < 1) //Y error
            {
                lattice.qubitsX[i] = (lattice.qubitsX[i] + 1) % 2;
                lattice.qubitsZ[i] = (lattice.qubitsZ[i] + 1) % 2;
            }
        }
    }
}

void Lattice::biasedError(double p, std::mt19937& engine, std::uniform_real_distribution<double>& dist, char pauli)
{
    for (int i : qubitIndices)
    {
        if (dist(engine) < p)
        {
            if (pauli == 'x' || pauli == 'X') qubitsX[i] = (qubitsX[i] + 1) % 2;
            else if (pauli == 'z' || pauli = 'Z') qubitsZ[i] = (qubitsZ[i] =1 ) % 2;
            else throw std::invalid_argument("Invalid Pauli given for biased error");
        }
    }
}

void Lattice::measError(double q, std::mt19937& engine, std::uniform_real_distribution<double>& dist, char pauli)
{
    for (int i : syndromeIndices)
    {
        if (dist(engine) < q)
        {
            if (pauli == 'x' || pauli == 'X') syndromeX[i] = (syndromeX[i] + 1) % 2;
            else if (pauli == 'z' || pauli == 'Z') syndromesZ[i] = (syndromesZ[i] + 1) % 2;
            else throw std::invalid_argument("Invalid Pauli given for measError");
        }
    }
}

void Lattice::calcSynd(char pauli)
{
    //X stabilisers/Z errors
    if (pauli == 'x' || pauli == 'X')
    {
        std::fill(syndromeX.begin(), syndromeX.end(), 0);
        for (int i : qubitIndices)
        {
            if (qubitsZ[i] == 1)
            {
                std::pair<int,int> &cells = faceToCells[i];
                for (int cell : {cells.first, cells.second})
                {
                    if (std::find(xSyndIndices.begin(), xSyndIndices.end(), cell) 
                            == xSyndIndices.end()) continue;
                    syndromeX[cell] = (syndrome[cell] + 1) % 2;
                }
            }
        }
    }
    //Z stabilisers/X errors
    else if (pauli == 'z' || pauli == 'Z')
    {
        std::fill(syndromeZ.begin(), syndromeZ.end(), 0);
        for (int i : qubitIndices)
        {
            if (qubitsX[i] == 1)
            {
                std::vector<int> &edges = faceToEdges[i];
                for (auto edgeIndex : edges)
                {
                    if (std::find(zSyndIndices.begin(), zSyndIndices.end(), edgeIndex)
                            == zSyndIndices.end()) continue;
                    syndromeZ[edgeIndex] = (syndrome[edgeIndex] + 1) % 2;
                }
            }
        }
    }

    else throw std::invalid_argument("Invalid Pauli given for calcSynd");
}

void Lattice::findDefects()
{
    defects.clear();
    for (auto v : defectIndices) 
    {
        std::vector<int>& edges = vertexToEdges[v];
        int count = 0;
        for (auto const& e : edges)
        {
            if (syndromeZ[e] == 1) count += 1;
        }

        if (count % 2 == 1) defects.push_back(v);
    }
}

void Lattice::checkInBounds()
{
    for (int i=0; i < 8*L*L*L; i++)
    {
        if (i < 3*L*L*L)
        {
            if (i < L*L*L)
            {
                if (syndromeX[i] == 1 && 
                        std::find(xSyndIndices.begin(), xSyndIndices.end(), i) 
                        == xSyndIndices.end())
                {
                    throw std::runtime_error("stabiliser out of bounds (syndromeX[" 
                                                    + std::to_string(i) + "]\n");

                }
            }
            if (qubitsX[i] == 1 && 
                    std::find(qubitIndices.begin(), qubitIndices.end(), i) 
                    == qubitIndices.end())
            {
                throw std::runtime_error("qubit out of bounds (qubitsX[" 
                                                + std::to_string(i) + "]\n");
            }
            if (qubitsZ[i] == 1 &&
                    std::find(qubitIndices.begin(), qubitIndices.end(), i) 
                    == qubitIndices.end())
            {
                throw std::runtime_error("qubit out of bounds (qubitsZ["
                                                + std::to_string(i) + "]\n");
            }
        }
        if (syndromeZ[i] == 1 &&
                std::find(zSyndIndices.begin(), zSyndIndices.end(), i) == zSyndIndices.end())
        {
            throw std::runtime_error("stabiliser out of bounds (syndromeZ["
                                            + std::to_string(i) + "]\n");
        }
    }
    for (int i : defects)
    {
        if (std::find(defectIndices.begin(), defectIndices.end(), i) == defectIndices.end())
        {
            throw std::runtime_error("defect out of bounds (" + std::to_string(i) + ")\n");
        }
    }
}

void Lattice::checkInCodespace()
{
    calcSynd('x');
    calcSynd('z');
    for (int i = 0; i < 8*L*L*L; i++)
    {
        if (i < L*L*L)
        {
            if (syndromeX[i] == 1)
            {
                throw std::runtime_error("Out of codespace (bad Z correction)");
            }
        }
        if (syndromeZ[i] == 1)
        {
            throw std::runtime_error("Out of codespace (bad X correction)");
        }
    }
}

bool Lattice::checkLogicalError(char pauli)
{
    int parity = 0;
    if (pauli == 'x' || pauli == 'X')
    {
        for (int i : zLogical)
        {
            if (qubitsX[i] == 1) parity = (parity + 1) % 2;
        }
    }
    else if (pauli == 'z' || pauli == 'Z')
    {
        for (int i : xLogical)
        {
            if (qubitsZ[i] == 1) parity = (parity + 1) % 2;
        }
    }
    if (parity) return true;
    else return false;
}
