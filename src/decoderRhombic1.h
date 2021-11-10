#ifndef DECODERRHOMBIC1_H
#define DECODERRHOMBIC1_H

#include "rhombic1.h"
#include "PerfectMatching.h"

namespace rhombic1 {

vint distanceToClosestXBoundary(int v, int L);

vint shortestPathToXBoundary(int v, int L);

vint distanceToClosestZBoundary(int cell, int L);

vint shortestPathToZB(int cell, vvint &cellToFaces, int L);

//LOOP DECODER

vpint mwpm(vint &defects, int L, int dual);

void joinPair(int v1, int v2, vint &syndIndices, vvint &vertexToEdges, vint &syndrome, int L);

void joinDualPair(int cell1, int cell2, vint &qubitIndices, vvint &cellToFaces, vint &qubits, int L);

//void xErrorDecoder(...)

void zErrorDecoder(vint &qubits, vint &qubitIndices, vvint &cellToFaces, vint &xStabs, int L);

void measErrorDecoder(vint &syndromeZ, vint &zSyndIndices, vvint &vertexToEdges, vint &defects, int L);

}
#endif
