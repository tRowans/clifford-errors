#ifndef DECODERRHOMBIC1_H
#define DECODERRHOMBIC1_H

#include "rhombic1.h"
#include "PerfectMatching.h"
#include "bp_osd_c.hpp"

namespace rhombic {

namespace r1 {

coord w1ToW0(coord cd, int L);

int shortestPathLength(int v1, int v2, int L, int twoD);

vint distanceToClosestXBoundary(int v, int L, int twoD);

vint shortestPathToXBoundary(int v, int L, int twoD);

vint distanceToClosestZBoundary(int cell, int L);

vint shortestPathToZBoundary(int cell, vvint &cellToFaces, int L);

vpint mwpm(vint &defects, int L, int dual, int twoD);

void joinPair(int v1, int v2, Lattice &lattice);

void joinDualPair(int cell1, int cell2, Lattice &lattice, int useOuter, int useInner);

void xErrorDecoder(bp_osd &decoderHz, mod2sparse *hz, Lattice &lattice);

void zErrorDecoder(Lattice &lattice, int useOuter, int useInner);

void xErrorDecoder2D(Lattice &lattice, vint &syndromeVertices);

void measErrorDecoder(Lattice &lattice);

}

}

#endif
