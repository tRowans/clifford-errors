#ifndef DECODER_H
#define DECODER_H

#include "cubic.h"
#include "PerfectMatching.h"

namespace cubic {

vint taxi(int v1, int v2, int L);

vint shortestPath(int v1, int v2, int L);

vint shortestDualPath(int c1, int c2, int L);

vint distanceToClosestXBoundary(int v, int L);

vint shortestPathToXBoundary(int v, int L);

vint distanceToClosestZBoundary(int cell, int L);

vint shortestPathToZBoundary(int cell, int L);

//LOOP DECODER

vpint mwpm(vint &defects, int L, int dual);

void joinPair(int v1, int v2, vint &syndrome, int L);

void joinDualPair(int cell1, int cell2, vint &qubits, int L);

//void xErrorDecoder(...)

void zErrorDecoder(Lattice &lattice, int L);

void measErrorDecoder(Lattice &lattice, int L);

void jumpCorrection(Lattice &lattice, int L);

}

#endif
