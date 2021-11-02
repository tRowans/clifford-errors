#ifndef DECODERRHOMBIC2_H
#define DECODERRHOMBIC2_H

#include "rhombic2.h"
#include "decoderGeneric.h"
#include "PerfectMatching.h"
#include <iostream>

namespace rhombic2 {

int shortestPathLength(int v1, int v2, int L);

vint shortestPath(int v1, int v2, vint &syndIndices, vvint &vertexToEdges, int L);

vint shortestDualPath(int cell1, int cell2, vint &qubitIndices, vvint &cellToFaces, int L);

std::vector<int> distanceToClosestXBoundary(int v, int L);

std::vector<int> shortestPathToXBoundary(int v, int L);

vint distanceToClosestZBoundary(int cell, int L);

vint shortestPathToZBoundary(int cell, vvint &cellToFaces, int L);

vpint mwpm(std::vector<int> &defects, int L, int dual);

void joinPair(int v1, int v2, vint &syndIndices, vvint &vertexToEdges, vint &syndrome, int L);

void joinDualPair(int cell1, int cell2, vint &qubitIndices, vvint &cellToFaces, vint &qubits, int L);

void zErrorDecoder(vint &qubits, vint &qubitIndices, vvint &cellToFaces, vint &xStabs, int L);

}

#endif
