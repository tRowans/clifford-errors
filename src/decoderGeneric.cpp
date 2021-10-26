#include "decoderGeneric.h"

void depolarisingError(std::vector<int> &qubitsX, std::vector<int> &qubitsZ, double p, std::mt19937& engine, std::uniform_real_distribution<double>& dist, std::vector<int>& qubitIndices)
{
    for (int i : qubitIndices)
    {
        if (dist(engine) < p)
        {
            double x = dist(engine);
            if (0 <= x  && x < 1/3) qubitsX[i] = (qubitsX[i] + 1) % 2; //X error
            else if (1/3 <= x && x < 2/3) qubitsZ[i]  = (qubitsZ[i] + 1) % 2; //Z error
            else if (2/3 <= x && x < 1) //Y error
            {
                qubitsX[i] = (qubitsX[i] + 1) % 2;
                qubitsZ[i] = (qubitsZ[i] + 1) % 2;
            }
        }
    }
}

void biasedError(std::vector<int> &qubits, double p, std::mt19937& engine, std::uniform_real_distribution<double>& dist, std::vector<int>& qubitIndices)
{
    for (int i : qubitIndices)
    {
        if (dist(engine) < p)
        {
            qubits[i] = (qubits[i] + 1) % 2;
        }
    }
}

void measError(std::vector<int> &syndrome, double q, std::mt19937& engine, std::uniform_real_distribution<double>& dist, std::vector<int> &syndromeIndices)
{
    for (int i : syndromeIndices)
    {
        if (dist(engine) < q)
        {
            syndrome[i] = (syndrome[i] + 1) % 2;
        }
    }
}

void calcSynd(std::vector<int> &syndrome, std::vector<int> &qubits, std::vector<int> &lowerQubitIndices, std::vector<int> &middleQubitIndices, std::vector<int> &upperQubitIndices, std::vector<std::vector<int>> &faceToEdges, std::vector<int>& syndromeIndices)
{
    std::fill(syndrome.begin(), syndrome.end(), 0);
    for (std::vector<int> qubitIndices : {lowerQubitIndices, middleQubitIndices, upperQubitIndices})
    {
        for (int i : qubitIndices)
        {
            if (qubits[i] == 1)
            {
                std::vector<int> &edges = faceToEdges[i];
                for (auto edgeIndex : edges)
                {
                    if (std::find(syndromeIndices.begin(), syndromeIndices.end(), edgeIndex)
                            == syndromeIndices.end()) 
                    {
                        continue;
                    }

                    syndrome[edgeIndex] = (syndrome[edgeIndex] + 1) % 2;
                }
            }
        }
    }
}

void calcXSynd(std::vector<int> &syndrome, std::vector<int> &qubits, std::vector<std::pair<int,int>> &faceToCells, std::vector<int> &syndIndices)
{
    std::fill(syndrome.begin(), syndrome.end(), 0);
    for (int i = 0; i < qubits.size(); i++)
    {
        if (qubits[i] == 1)
        {
            std::pair<int,int> &cells = faceToCells[i];
            for (int cellIndex : {cells.first, cells.second})
            {
                if (std::find(syndIndices.begin(), syndIndices.end(), cellIndex)
                        == syndIndices.end())
                {
                    continue;
                }
                syndrome[cellIndex] = (syndrome[cellIndex] + 1) % 2;
            }
        }
    }
}

vvint getSyndromeVertices(vvint &syndromes, vpint &edgeToVerticesT, vpint &edgeToVerticesR1, vpint &edgeToVerticesR2)
{
    std::vector<std::set<int>> syndromeVerticesSets = {{},{},{}};
    for (int i = 0; i < 3; i++)
    {
        for (int e = 0; e < syndromes[i].size(); e++)
        {
            if (syndromes[i][e] == 1)
            {
                std::pair<int,int> vertices;  
                if (i == 0) vertices = edgeToVerticesT[e];
                else if (i == 1) vertices = edgeToVerticesR1[e];
                else vertices = edgeToVerticesR2[e];
                syndromeVerticesSets[i].insert(vertices.first);
                syndromeVerticesSets[i].insert(vertices.second);
            }
        }
    }
    vvint syndromeVertices;
    for (std::set<int> verticesSet : syndromeVerticesSets)
    {
        vint verticesVector;
        for (int v : verticesSet) verticesVector.push_back(v);
        syndromeVertices.push_back(verticesVector);
    }
    return syndromeVertices;
}

void findDefects(std::vector<int> &syndrome, std::vector<std::vector<int>> &vertexToEdges, std::vector<std::pair<int, int>>& edgeToVertices, std::vector<int>& defects, std::vector<int>& defectIndices)
{
    defects.clear();
    for (auto v : defectIndices) 
    {
        std::vector<int>& edges = vertexToEdges[v];
        int count = 0;
        for (auto const& e : edges)
        {
            if (syndrome[e] == 1)
            {
                count += 1;
            }
        }
        if (count % 2 == 1)
        {
            defects.push_back(v);
        }
    }
}

bool checkXCorrection(std::vector<int> &syndrome, std::vector<int> &qubits, std::vector<int> &lowerQubitIndices, std::vector<int> &middleQubitIndices, std::vector<int> &upperQubitIndices, std::vector<std::vector<int>> &faceToEdges, std::vector<int>& syndIndices, bool r2)
{
    std::set<int> middleEdges = {};
    if (r2)
    {
        for (int f : middleQubitIndices)
        {
            vint edges = faceToEdges[f];
            for (int e : edges)
            {
                middleEdges.insert(e);
            }
        }
    }
    std::set<int> upperStabs = {};
    for (int f : upperQubitIndices)
    {
        vint edges = faceToEdges[f];
        for (int e : edges)
        {
            if (std::find(syndIndices.begin(), syndIndices.end(), e) != syndIndices.end()
                    &&
                std::find(middleEdges.begin(), middleEdges.end(), e) == middleEdges.end())
            {
                upperStabs.insert(e);
            }
        }
    }
    calcSynd(syndrome, qubits, lowerQubitIndices, 
                middleQubitIndices, upperQubitIndices, faceToEdges, syndIndices);
    
    bool notEmpty = 0;
    for (int stab : upperStabs)
    {
        if (syndrome[stab] == 1)
        {
            notEmpty = 1;
            break;
        }
    }
    return notEmpty;
}

bool checkZCorrection(std::vector<int> &syndrome, std::vector<int> &qubits, std::vector<std::pair<int,int>> &faceToCells, std::vector<int> &syndIndices)
{
    calcXSynd(syndrome, qubits, faceToCells, syndIndices);
    bool notEmpty = 0;
    for (int stab : syndrome)
    {
        if (stab != 0)
        {
            notEmpty = 1;
            break;
        }
    }
    return notEmpty;
}

bool checkLogicalError(std::vector<int> &qubits, std::vector<int> &logical)
{
    int parity = 0;
    for (int i : logical)
    {
        if (qubits[i] == 1)
        {
            parity = (parity + 1) % 2;
        }
    }
    if (parity) return true;
    else return false;
}

int scalarProduct(std::vector<float> vec, int dir)
{
    //scalar product of a vector with one of the edge directions
    //but these directions can be negative
    int sign = 1;
    if (dir < 0) 
    {
        dir = -1*dir;
        sign = -1;
    }
    if (dir == 1) return sign*vec[0] + sign*vec[1] - sign*vec[2];   //{1,1,-1}
    else if (dir == 2) return sign*vec[0] - sign*vec[1] + sign*vec[2];  //{1,-1,1}
    else if (dir == 3) return -1*sign*vec[0] + sign*vec[1] + sign*vec[2];   //{-1,1,1}
    else if (dir == 4) return sign*vec[0] + sign*vec[1] + sign*vec[2];  //{1,1,1}
    else throw std::invalid_argument("Invalid direction");
}

void updateQubitHistory(vint &lowerQubitIndicesT, vint &middleQubitIndicesT, vint &lowerQubitIndicesR1, vint &middleQubitIndicesR1, vint &lowerQubitIndicesR2, vint &middleQubitIndicesR2, vvint &qubitIndexHistory, int laststep)
{
    for (int qubit : lowerQubitIndicesT) qubitIndexHistory[0].push_back(qubit);
    for (int qubit : lowerQubitIndicesR1) qubitIndexHistory[1].push_back(qubit);
    for (int qubit : lowerQubitIndicesR2) qubitIndexHistory[2].push_back(qubit);
    if (laststep == 0)
    {
        for (int qubit : middleQubitIndicesT) qubitIndexHistory[0].push_back(qubit);
        for (int qubit : middleQubitIndicesR1) qubitIndexHistory[1].push_back(qubit);
        for (int qubit : middleQubitIndicesR2) qubitIndexHistory[2].push_back(qubit);
    }
}

void updateStabiliserHistory(vint &syndIndicesXLayerT, vint &syndIndicesXLayerR1, vint &syndIndicesXLayerR2, vint &syndIndicesXT, vint &syndIndicesXR1, vint &syndIndicesXR2, int L, int j, int laststep)
{
    for (int stab : syndIndicesXLayerT)
    {
        if (j == 0) syndIndicesXT.push_back(stab - L*L); //bottom boundary X stabs
        syndIndicesXT.push_back(stab);
        if (laststep == 0) syndIndicesXT.push_back(stab + L*L);
    }
    for (int stab : syndIndicesXLayerR1)
    {
        int zCoord = (2*stab / (5*L*L)) % L;
        if (j == 0) 
        {
            if (zCoord != (L-2))
            {
                syndIndicesXR1.push_back(stab - 1);
            }
        }
        if (zCoord == 1)
        {
            if (laststep == 0) syndIndicesXR1.push_back(stab);
        }
        else syndIndicesXR1.push_back(stab);
    }
    for (int stab : syndIndicesXLayerR2)
    {
        if (j == 0) syndIndicesXR2.push_back(stab - L);
        if (laststep == 0) syndIndicesXR2.push_back(stab);
    }
}

void updateLogicalHistory(vint &xLogicalFragmentT, vint &xLogicalFragment1R1, vint &xLogicalFragment2R1, vint &xLogicalFragment1R2, vint &xLogicalFragment2R2, vint &xLogicalT, vint &xLogicalR1, vint &xLogicalR2, int L, int laststep)
{   
    for (int qubit : xLogicalFragmentT) 
    {
        xLogicalT.push_back(qubit);
        if (laststep == 0) xLogicalT.push_back(qubit + 3*L*L);
    }
    for (int qubit : xLogicalFragment1R1) xLogicalR1.push_back(qubit);
    for (int qubit : xLogicalFragment1R2) xLogicalR2.push_back(qubit);
    if (laststep == 0)
    {
        for (int qubit : xLogicalFragment2R1) xLogicalR1.push_back(qubit);
        for (int qubit : xLogicalFragment2R2) xLogicalR2.push_back(qubit);
    }
}

void moveXIndices(vint &syndIndicesXLayerT, vint &syndIndicesXLayerR1, vint &syndIndicesXLayerR2, 
                  vint &xLogicalFragmentT, vint &xLogicalFragment1R1, vint &xLogicalFragment2R1,
                  vint &xLogicalFragment1R2, vint &xLogicalFragment2R2, int L)
{
    for (int &stab : syndIndicesXLayerT) stab += 2*L*L;
    for (int &stab : syndIndicesXLayerR1) stab += 1;
    for (int &stab : syndIndicesXLayerR2) stab += L;

    for (int &qubit : xLogicalFragmentT) qubit += 6*L*L;
    for (int &qubit : xLogicalFragment1R1) qubit += 6;
    for (int &qubit : xLogicalFragment2R1) qubit += 6;
    for (int &qubit : xLogicalFragment1R2) qubit += 6*L;
    for (int &qubit : xLogicalFragment2R2) qubit += 6*L;
}

void applyCCZ(vvint &qubitsX, vvint &qubitsZ, vvint &syndromeVertices, vvint &xStabs, std::map<std::pair<int,int>,std::pair<std::pair<int,int>,std::pair<int,int>>> &overlappingFaces, vvint &cellToFacesT, vvint &cellToFacesR1, vvint &cellToFacesR2, vint &lowerQubitIndicesT, vint &middleQubitIndicesT, int L)
{
    for (int i = 0; i < 3; i++)
    {
        for (int v : syndromeVertices[i])
        {
            std::pair<int,int> cell = latticeSwitchCell(v,i,L);
            if (cell.second < 0) continue;  //input vertex not in overlapping cube so not in CCZ support
            if (xStabs[cell.first][cell.second] == 1)
            {
                vint faces;
                if (cell.first == 0) faces = cellToFacesT[cell.second];
                else if (cell.first == 1) faces = cellToFacesR1[cell.second];
                else faces = cellToFacesR2[cell.second];
                for (int face : faces)
                {
                    auto overlap = overlappingFaces[{cell.first,face}];

                    if (overlap.first.first == i && 
                        qubitsX[overlap.first.first][overlap.first.second] == 1)
                    {
                        qubitsZ[overlap.second.first][overlap.second.second] = (
                                qubitsZ[overlap.second.first][overlap.second.second] + 1) % 2;
                    }
                    else if (overlap.second.first == i &&
                             qubitsX[overlap.second.first][overlap.second.second] == 1)
                    {
                        qubitsZ[overlap.first.first][overlap.first.second] = (
                                qubitsZ[overlap.first.first][overlap.first.second] + 1) % 2;
                    }
                }
            }
        }
    }
}

//------------------------------Z ERROR VISUALISER------------------------------

std::vector<int> getNonZeroElements(std::vector<int> &elements, std::vector<int> &indexVector)
{
    std::vector<int> nonZeroElements = {};
    for (int i : indexVector)
    {
        if (elements[i] == 1)
        {
            nonZeroElements.push_back(i);
        }
    }
    return nonZeroElements;
}

void writeCSV(std::ofstream &file, std::vector<int> &indices)
{
    if (indices.size())
    {
        file << indices[0];
        for (int i = 1; i < indices.size(); i++)
        {
            file << ',' << indices[i];
        }
    }
    file << '\n';
}

void openFiles(std::ofstream& qubitsOut, std::ofstream& xStabsOut, std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, int i, bool decoded)
{
    if (i == 0 && decoded == 0)
    {
        qubitsOut.open("qubits.csv");
        xStabsOut.open("xStabs.csv");
    }
    if (decoded)
    {
        zErrorsOut.open("zErrors_" + std::to_string(i) + "_decoded.csv");
        violatedXStabsOut.open("violatedXStabs_" + std::to_string(i) + "_decoded.csv");
    }
    else 
    {
        zErrorsOut.open("zErrors_" + std::to_string(i) + "_undecoded.csv");
        violatedXStabsOut.open("violatedXStabs_" + std::to_string(i) + "_undecoded.csv");
    }
}

void writeLatticeInfo(std::ofstream& qubitsOut, std::ofstream& xStabsOut, vint& qubitIndices, vpint& faceToCells, vint& stabIndices)
{
    for (int qubit : qubitIndices)
    {
        pint cells = faceToCells[qubit];
        qubitsOut << cells.first << ',' << cells.second << '\n';
    }
    writeCSV(xStabsOut, stabIndices);
}

void writeErrorInfo(std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, vint& qubits, vint& stabs, vint& qubitIndices, vpint& faceToCells, vint& stabIndices)
{
    vint nonZeroQubits = getNonZeroElements(qubits, qubitIndices);
    vint nonZeroStabs = getNonZeroElements(stabs, stabIndices);
    for (int qubit : nonZeroQubits)
    {
        pint cells = faceToCells[qubit];
        zErrorsOut << cells.first << ',' << cells.second << '\n';
    }
    writeCSV(violatedXStabsOut, nonZeroStabs);
}

void closeFiles(std::ofstream& qubitsOut, std::ofstream& xStabsOut, std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, int i, bool decoded)
{
    if (i == 0 && decoded == 0)
    {
        qubitsOut.close();
        xStabsOut.close();
    }
    zErrorsOut.close();
    violatedXStabsOut.close();
}

