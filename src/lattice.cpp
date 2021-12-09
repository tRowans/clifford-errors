#include "lattice.h"

void Lattice::depolarisingError(double p, std::mt19937& engine, std::uniform_real_distribution<double>& dist)
{
    for (const vint &qubitIndices : {outerQubitIndices, innerQubitIndices})
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
                    qubitsX[i] = (qubitsX[i] + 1) % 2;
                    qubitsZ[i] = (qubitsZ[i] + 1) % 2;
                }
            }
        }
    }
}

void Lattice::biasedError(double p, std::mt19937& engine, 
                            std::uniform_real_distribution<double>& dist, char pauli, int innerOnly)
{
    if (innerOnly == 0)
    {
        for (int i : outerQubitIndices)
        {
            if (dist(engine) < p)
            {
                if (pauli == 'x' || pauli == 'X') qubitsX[i] = (qubitsX[i] + 1) % 2;
                else if (pauli == 'z' || pauli == 'Z') qubitsZ[i] = (qubitsZ[i] =1 ) % 2;
                else throw std::invalid_argument("Invalid Pauli given for biased error");
            }
        }
    }
    for (int i : innerQubitIndices)
    {
        if (dist(engine) < p)
        {
            if (pauli == 'x' || pauli == 'X') qubitsX[i] = (qubitsX[i] + 1) % 2;
            else if (pauli == 'z' || pauli == 'Z') qubitsZ[i] = (qubitsZ[i] =1 ) % 2;
            else throw std::invalid_argument("Invalid Pauli given for biased error");
        }
    }
}

void Lattice::measError(double q, std::mt19937& engine, std::uniform_real_distribution<double>& dist, char pauli)
{
    if (pauli == 'x' || pauli == 'X')
    {
        for (int i : xSyndIndices)
        {
            if (dist(engine) < q) syndromeX[i] = (syndromeX[i] + 1) % 2;
        }
    }
    else if (pauli == 'z' || pauli == 'Z')
    {
        for (int i : zSyndIndices)
        {
            if (dist(engine) < q) syndromeZ[i] = (syndromeZ[i] + 1) % 2;
        }
    }
    else throw std::invalid_argument("Invalid Pauli given for measError");
}

void Lattice::calcSynd(char pauli, int useOuter, int useInner)
{
    vint whichQubits = {useOuter, useInner};
    int j = 0;
    //X stabilisers/Z errors
    if (pauli == 'x' || pauli == 'X')
    {
        std::fill(syndromeX.begin(), syndromeX.end(), 0);
        for (const vint &qubitIndices : {outerQubitIndices, innerQubitIndices})
        {
            if (whichQubits[j] == 1)
            {
                for (int i : qubitIndices)
                {
                    if (qubitsZ[i] == 1)
                    {
                        pint &cells = faceToCells[i];
                        for (int cell : {cells.first, cells.second})
                        {
                            if (std::find(xSyndIndices.begin(), xSyndIndices.end(), cell)
                                    == xSyndIndices.end()) continue;
                            syndromeX[cell] = (syndromeX[cell] + 1) % 2;
                        }
                    }
                }
            }
            j = 1;
        }
    }
    //Z stabilisers/X errors
    else if (pauli == 'z' || pauli == 'Z')
    {
        std::fill(syndromeZ.begin(), syndromeZ.end(), 0);
         for (const vint &qubitIndices : {outerQubitIndices, innerQubitIndices})
        {
            if (whichQubits[j] == 1)
            {
                for (int i : qubitIndices)
                {
                    if (qubitsX[i] == 1)
                    {
                        vint &edges = faceToEdges[i];
                        for (auto edge : edges)
                        {
                            if (std::find(zSyndIndices.begin(), zSyndIndices.end(), edge)
                                    == zSyndIndices.end()) continue;
                            syndromeZ[edge] = (syndromeZ[edge] + 1) % 2;
                        }
                    }
                }
            }
            j = 1;
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

void Lattice::applyZStab(int edge)
{
    if (std::find(zSyndIndices.begin(), zSyndIndices.end(), edge) == zSyndIndices.end())
    {
        throw std::invalid_argument("Non-stabiliser edge passed to applyZStab");
    }
    vint &stabQubits = edgeToFaces[edge];
    for (int q : stabQubits)
    {
        if (std::find(outerQubitIndices.begin(), outerQubitIndices.end(), q) 
              != outerQubitIndices.end() ||
            std::find(innerQubitIndices.begin(), innerQubitIndices.end(), q)
              != innerQubitIndices.end())
        {
            qubitsZ[q] = (qubitsZ[q] + 1) % 2;
        }
    }
}

void Lattice::zStabPattern(std::mt19937& engine, std::uniform_real_distribution<double>& dist)
{
    for (int stab : zSyndIndices)
    {
        if (dist(engine) < 0.5) applyZStab(stab);
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
                    std::find(outerQubitIndices.begin(), outerQubitIndices.end(), i) 
                    == outerQubitIndices.end() &&
                    std::find(innerQubitIndices.begin(), innerQubitIndices.end(), i)
                    == innerQubitIndices.end())
            {
                throw std::runtime_error("qubit out of bounds (qubitsX[" 
                                                + std::to_string(i) + "]\n");
            }
            if (qubitsZ[i] == 1 &&
                    std::find(outerQubitIndices.begin(), outerQubitIndices.end(), i) 
                    == outerQubitIndices.end() &&
                    std::find(innerQubitIndices.begin(), innerQubitIndices.end(), i)
                    == innerQubitIndices.end())
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

void Lattice::checkInCodespace(char pauli, int useOuter, int useInner)
{
    if (pauli == 'x' || pauli == 'X')
    {
        calcSynd('z', useOuter, useInner);
        for (int i = 0; i < 8*L*L*L; i++)
        {
            if (syndromeZ[i] == 1)
            {
                throw std::runtime_error("Out of codespace (bad Z correction)");
            }
        }
    }
    else if (pauli == 'z' || pauli == 'Z')
    {
        calcSynd('x', useOuter, useInner);
        for (int i = 0; i < L*L*L; i++)
        {
            if (syndromeX[i] == 1)
            {
                throw std::runtime_error("Out of codespace (bad X Correction)");
            }
        }
    }
    else throw std::invalid_argument("Invalid Pauli given for checkInCodespace");
}

void Lattice::checkJumpCorrection()
{
    for (int i : innerQubitIndices)
    {
        if (qubitsZ[i] == 1) throw std::runtime_error("Bad jump correction");
    }
}

int Lattice::checkLogicalError(char pauli)
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
    else throw std::invalid_argument("Invalid Pauli given for checkLogicalError");
    return parity;
}

void Lattice::wipe()
{
    for (int i = 0; i < 8*L*L*L; i++)
    {
        if (i < 3*L*L*L)
        {
            if (i < L*L*L) syndromeX[i] = 0;
            qubitsX[i] = 0;
            qubitsZ[i] = 0;
        }
        syndromeZ[i] = 0;
    }
    defects.clear();
}

