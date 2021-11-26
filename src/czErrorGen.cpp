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

void applyCCZ(std::vector<Lattice> &lattices, std::map<pint,std::pair<pint,pint>> &overlappingFaces, int L, std::mt19937 &engine, std::uniform_real_distribution<double> &dist, int link)
{
    //membrane boundary errors
    vvint syndromeVertices = getSyndromeVertices(lattices);
    for (int i = 0; i < 3; i++)
    {
        for (int v : syndromeVertices[i])
        {
            std::pair<int,int> cell = {latticeWhereCell(v,i,L), v};
            if (dist(engine) < 0.5)
            {
                vint faces = lattices[cell.first].cellToFaces[cell.second];
                for (int face : faces)
                {
                    auto overlap = overlappingFaces[{cell.first,face}];
                    std::pair<int,int> &xFace;
                    std::pair<int,int> &zFace;
                    if (overlap.first.first == i)
                    {
                        xFace = overlap.first;
                        zFace = overlap.second;
                    }
                    else 
                    {
                        xFace = overlap.second;
                        zFace = overlap.first;
                    }

                    if (lattices[xFace.first].qubitsX[xFace.second] == 1)
                    {
                        lattices[zFace.first].qubitsZ[zFace.second] = (
                                lattices[zFace.first].qubitsZ[zFace.second] + 1) % 2;
                    }
                }
            }
        }
    }
    //linking charge
    if (link)
    {
        for (int fC = 0; fC < 3*L*L*L; fC++)
        {
            auto overlap = overlappingFaces[{0,fC}];
            int fR1 = overlap.first.second;
            int fR2 = overlap.second.second;
            if (lattices[0].qubitsX[fC] == 1 && lattices[1].qubitsX[fR1] == 1)
            {
                lattices[2].qubitsZ[fR2] = (lattices[2].qubitsZ[fR2] + 1) % 2;
            }
            if (lattices[0].qubitsX[fC] == 1 && lattices[2].qubitsX[fR2] == 1)
            {
                lattices[1].qubitsZ[fR1] = (lattices[1].qubitsZ[fR1] + 1) % 2;
            }
            if (lattices[1].qubitsX[fR1] == 1 && lattices[2].qubitsX[fR2] == 1)
            {
                lattices[0].qubitsZ[fC] = (lattices[0].qubitsZ[fC] + 1) % 2;
            }
        }
    }
}