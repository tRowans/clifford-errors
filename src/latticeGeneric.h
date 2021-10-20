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

int latticeSwitchVertex(int i, int currentLattice, int newLattice, int L);

std::pair<int,int> latticeSwitchCell(int i, int currentLattice, int L);

int findFace(vint vertices, vvint &vertexToFaces, vvint &faceToVertices);

void buildOverlappingFaces(std::map<pint,std::pair<pint,pint>> &overlappingFaces, vvint &faceToEdgesT, vpint &edgeToVerticesT, vvint &vertexToFacesR1, vvint &faceToVerticesR1, vvint &vertexToFacesR2, vvint &faceToVerticesR2, int L);

void randomiseStabs(vvint &xStabs,std::mt19937 &engine,std::uniform_real_distribution<double> &dist);

#endif
