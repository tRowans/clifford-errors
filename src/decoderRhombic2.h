#ifndef DECODERRHOMBIC2_H
#define DECODERRHOMBIC2_H

#include "rhombic2.h"
#include "PerfectMatching.h"

namespace rhombic {

namespace r2 {

coord w1ToW0(coord cd, int L);

int shortestPathLength(int v1, int v2, int L, int twoD);

vint distanceToClosestXBoundary(int v, int L, int twoD);

vint shortestPathToXBoundary(int v, int L, int twoD);

vint distanceToClosestZBoundary(int cell, int L);

vint shortestPathToZBoundary(int cell, vvint &cellToFaces, int L);

//LOOP DECODER

vpint mwpm(std::vector<int> &defects, int L, int dual, int twoD);

void joinPair(int v1, int v2, Lattice &lattice);

void joinDualPair(int cell1, int cell2, Lattice &lattice, int decode2D, int ignoreOuter);

//void xErrorDecoder(...)

void zErrorDecoder(Lattice &lattice, int decode2D, int ignoreOuter);

void xErrorDecoder2D(Lattice &lattice, vint &syndromeVertices);

void measErrorDecoder(Lattice &lattice);

}

}

#endif
