#ifndef DECODERRHOMBIC2_H
#define DECODERRHOMBIC2_H

#include "rhombic2.h"
#include "PerfectMatching.h"

namespace rhombic2 {

std::vector<int> distanceToClosestXBoundary(int v, int L);

std::vector<int> shortestPathToXBoundary(int v, int L);

vint distanceToClosestZBoundary(int cell, int L);

vint shortestPathToZBoundary(int cell, vvint &cellToFaces, int L);

//LOOP DECODER

vpint mwpm(std::vector<int> &defects, int L, int dual);

void joinPair(int v1, int v2, vint &syndIndices, vvint &vertexToEdges, vint &syndrome, int L);

void joinDualPair(int cell1, int cell2, vint &outerQubitIndices, 
                    vint &innerQubitIndices, vvint &cellToFaces, vint &qubits, int L);

//void xErrorDecoder(...)

void zErrorDecoder(vint &qubits, vint &outerQubitIndices, 
                    vint &innerQubitIndices, vvint &cellToFaces, vint &xStabs, int L);

void measErrorDecoder(vint &syndromeZ, vint &zSyndIndices, vvint &vertexToEdges, vint &defects, int L);

}

#endif
