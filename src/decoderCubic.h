#ifndef DECODER_H
#define DECODER_H

#include "cubic.h"
#include "PerfectMatching.h"

namespace cubic {

vint taxi(int v1, int v2, int L);

vint shortestPath(int v1, int v2, int L);

vint shortestDualPath(int c1, int c2, int L);

vint distanceToClosestXBoundary(int v, int L, int yOnly);

vint shortestPathToXBoundary(int v, int L, int yOnly);

vint distanceToClosestZBoundary(int cell, int L);

vint shortestPathToZBoundary(int cell, int L);

//LOOP DECODER

vpint mwpm(vint &defects, int L, int dual, int yOnly);

void joinPair(int v1, int v2, Lattice &lattice);

void joinDualPair(int cell1, int cell2, Lattice &lattice);

//void xErrorDecoder(...)

void zErrorDecoder(Lattice &lattice);

void xErrorDecoder2D(Lattice &lattice, vint &syndromeVertices);

void measErrorDecoder(Lattice &lattice);

void jumpCorrection(Lattice &lattice);

}

#endif
