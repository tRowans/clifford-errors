#ifndef DECODERRHOMBIC1_H
#define DECODERRHOMBIC1_H

#include "rhombic1.h"
#include "PerfectMatching.h"

namespace rhombic {

namespace r1 {

coord w1ToW0(coord cd, int L);

int shortestPathLength(int v1, int v2, int L);

vint distanceToClosestXBoundary(int v, int L);

vint shortestPathToXBoundary(int v, int L);

vint distanceToClosestZBoundary(int cell, int L);

vint shortestPathToZBoundary(int cell, vvint &cellToFaces, int L);

//LOOP DECODER

vpint mwpm(vint &defects, int L, int dual);

void joinPair(int v1, int v2, Lattice &lattice);

void joinDualPair(int cell1, int cell2, Lattice &lattice, int useOuter, int useInner);

//void xErrorDecoder(...)

void zErrorDecoder(Lattice &lattice, int useOuter, int useInner);

void measErrorDecoder(Lattice &lattice);

}

}

#endif
