#ifndef DECODER_H
#define DECODER_H

#include "cubic.h"
#include "decoderGeneric.h"
#include "PerfectMatching.h"
#include <iostream>

namespace cubic {

vint taxi(int v1, int v2, int L);

vint shortestPath(int v1, int v2, int L);

vint shortestDualPath(int c1, int c2, int L);

vint distanceToClosestXBoundary(int v, int L);

vint shortestPathToXBoundary(int v, int L);

vint distanceToClosestZBoundary(int cell, int L);

vint shortestPathToZBoundary(int cell, int L);

vpint mwpm(vint &defects, int L, int dual);

void joinPair(int v1, int v2, vint &syndrome, int L);

void joinDualPair(int cell1, int cell2, vint &qubits, int L);

void zErrorDecoder(vint &qubits, vint &qubitIndices, vvint &cellToFaces, vint &xStabs, int L);

}

#endif
