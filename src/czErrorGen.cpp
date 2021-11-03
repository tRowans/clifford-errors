#include "czErrorGen.h"

std::map<pint,std::pair<pint,pint>> buildOverlappingFaces(std::vector<Lattice> &lattices, int L)
{
    std::map<pint,std::pair<pint,pint>> overlappingFaces;
    int fC, fR1, fR2
    for (int v = 0; v < L*L*L; v++)
    {
        for (int i = 0; i < 3; i++)
        fC = 3*v + i;
        vint verts = lattices[0].faceToVertices[fC];
        fR1 = findFace({verts[0],verts[3]}, 
                lattices[1].vertexToFaces, lattices[1].faceToVertices);
        if (fR1 == -1)
        {
            fR1 = findFace({verts[1],verts[2]}, 
                    lattices[1].vertexToFaces, lattices[1].faceToVertices);
            fR2 = findFace({verts[0],verts[3]}, 
                    lattices[2].vertexToFaces, lattices[2].faceToVertices);
        }
        else fR2 = findFace({verts[1],verts[2]}, 
                lattices[2].vertexToFaces, lattices[2].faceToVertices);
        overlappingFaces[{0,fC}] = {{1,fR1}, {2,fR2}};
        overlappingFaces[{1,fR1}] = {{0,fC}, {2,fR2}};
        overlappingFaces[{2,fR2}] = {{0,fC}, {1,fR1}};
    }
    return overlappingFaces;
}

//Do I need this? See how it goes. 
void randomiseStabs(vvint &xStabs, std::mt19937 &engine, std::uniform_real_distribution<double> &dist)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < xStabs[i].size(); j++)
        {
            if (dist(engine) < 0.5)
            {
                xStabs[i][j] = 1;
            }
        }
    }
}

vvint getSyndromeVertices(std::vector<Lattice> &lattices)
{
    std::vector<std::set<int>> syndromeVerticesSets = {{},{},{}};
    for (Lattice &l : lattices)
    {
        for (int e = 0; e < 8*L*L*L; e++)
        {
            if (l.syndromesZ[e] == 1)
            {
                std::pair<int,int> vertices = l.edgeToVertices[e];
                syndromeVerticesSets[i].insert(vertices.first);
                syndromeVerticesSets[i].insert(vertices.second);
            }
        }
    }
    vvint syndromeVertices;
    for (std::set<int> &verticesSet : syndromeVerticesSets)
    {
        vint verticesVector;
        for (int v : verticesSet) verticesVector.push_back(v);
        syndromeVertices.push_back(verticesVector);
    }
    return syndromeVertices;
}

int latticeWhereCell(int v, int latticeA, int L)
{
    //Each vertex of lattice A is also a vertex of lattice B but a cell of lattice C
    int latticeC;
    if (latticeA == 0)
    {
        if (((v % L) + ((v/L) % L) + ((v/(L*L)) % L)) % 2 == 0) latticeC = 2;
        else latticeC = 1;
    }
    else if (latticeA == 1)
    {
        if ((v/(L*L*L)) % L == 0) latticeC = 2;
        else latticeC = 0;
    }
    else if (latticeA == 2)
    {
        if ((v/(L*L*L)) % L == 0) latticeC = 1;
        else latticeC = 0;
    }
    return latticeC;
}

//STILL WORKING ON THIS ONE !!

void applyCCZ(std::vector<Lattice> &lattices, std::map<pint,std::pair<pint,pint>> &overlappingFaces, int L)
{
    vvint syndromeVertices = getSyndromeVertices(lattices);
    for (int i = 0; i < 3; i++)
    {
        for (int v : syndromeVertices[i])
        {
            std::pair<int,int> cell = {latticeWhereCell(v,i,L), v};
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

