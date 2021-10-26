#ifndef LATTICEGENERIC_H
#define LATTICEGENERIC_H

#include <vector>
#include <set>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <map>

typedef std::vector<int> vint;
typedef std::vector<std::vector<int>> vvint;
typedef std::pair<int,int> pint;
typedef std::vector<std::pair<int, int>> vpint;

class Lattice
{
    public:
        vvint faceToVertices;   
        vvint vertexToFaces;    //This one not used in cubic lattice
        vvint faceToEdges;
        vvint edgeToFaces;
        vpint faceToCells;
        vvint cellToFaces;
        vvint vertexToEdges;
        vpint edgeToVertices;

        vint qubitIndices;
        vint xSyndIndices;
        vint zSyndIndices;
        vint xLogical;
        vint zLogical; 

        vint qubitsX(3*L*L*L, 0);
        vint qubitsZ(3*L*L*L, 0);
        vint syndromesX(L*L*L, 0);   //X stabiliser syndromes
        vint syndromesZ(8*L*L*L, 0); //This is bigger than cubic needs but doesn't matter
        vint defects = {};
};

int findFace(vint &vertices, vvint &vertexToFaces, vvint &faceToVertices);

std::map<pint,std::pair<pint,pint>> buildOverlappingFaces(Lattice &cubic, Lattice &rhombic1, Lattice &rhombic2, int L);

void randomiseStabs(vvint &xStabs,std::mt19937 &engine,std::uniform_real_distribution<double> &dist);

#endif
