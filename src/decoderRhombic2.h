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

void joinPair(int v1, int v2, Lattice &lattice, int L);

void joinDualPair(int cell1, int cell2, Lattice &lattice, int L, int useOuter, int useInner);

//void xErrorDecoder(...)

void zErrorDecoder(Lattice &lattice, int L, int useOuter, int useInner);

void measErrorDecoder(Lattice &lattice, int L);

}

#endif
