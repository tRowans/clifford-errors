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

void middleSweepRule(vint &qubits, vint &middleQubitIndices, vint &syndrome, vint &syndIndices, vint &bulkSweepVertices, vvint &faceToEdges, vvint &faceToVertices, vvint &vertexToFaces, int L);

void middleBoundarySweepRule(vint &qubits, vint &middleQubitIndices, vint &syndrome, vint &syndIndices, vint &middleBoundarySweepVertices, vvint &faceToEdges, vvint &faceToVertices, vvint &vertexToFaces, int L);

void upperSweepRule(vint &qubits, vint &upperQubitIndices, vint &syndrome, vint &syndIndices, vint &bulkSweepVertices, vvint &faceToEdges, vvint &faceToVertices, vvint &vertexToFaces, int L);

std::vector<int> distanceToClosestXBoundary(int v, int L);

std::vector<int> shortestPathToXB(int v, int L);

vint distanceToClosestZBoundary(int cell, int L);

vint shortestPathToZB(int cell, vvint &cellToFaces, int L);

vpint mwpm(std::vector<int> &defects, std::map<std::pair<int, int>, int> &defectPairDistances, int L, int dual);

void updatePairing(std::map<std::pair<int, int>, int> &defectPairDistances, vint &defects, int L);

void joinPair(int v1, int v2, vint &syndIndices, vvint &vertexToEdges, vint &syndrome, int L);

void joinDualPair(int cell1, int cell2, vint &qubitIndices, vvint &cellToFaces, vint &qubits, int L);

vint pathToTop(int v, vint &syndIndices, vvint &vertexToEdges, int L);

void syndromeRepair(vint &syndrome, vint &syndIndices, vvint &vertexToEdges, vpint &edgeToVertices, std::map<std::pair<int, int>, int> &defectPairDistances, int L, int cutoff, int reduction);

void zErrorDecoder(vint &qubits, vint &qubitIndices, vvint &cellToFaces, vint &xStabs, int L);

int oneRun(int L, double p, double q, vvint &faceToEdges, vvint &edgeToFaces, vvint &faceToVertices, vvint &vertexToFaces, vvint &vertexToEdges, vpint &edgeToVertices, vint lowerQubitIndices, vint middleQubitIndices, vint upperQubitIndices, vint syndIndices, vint defectIndices, vint bulkSweepVertices, vint middleBoundarySweepVertices, vint zLogical, int run, int cutoff, int reduction);

}

#endif
