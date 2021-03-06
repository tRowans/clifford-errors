#ifndef DECODER_H
#define DECODER_H

#include "cubic.h"
#include "PerfectMatching.h"
#include "bp_osd_c.hpp"

namespace cubic {

vint taxi(int v1, int v2, int L);

vint shortestPath(int v1, int v2, int L);

vint shortestDualPath(int c1, int c2, int L);

vint distanceToClosestXBoundary(int v, int L, int yOnly);

vint shortestPathToXBoundary(int v, int L, int yOnly);

vint distanceToClosestZBoundary(int cell, int L);

vint shortestPathToZBoundary(int cell, int L);

vpint mwpm(vint &defects, int L, int dual, int yOnly);

void joinPair(int v1, int v2, Lattice &lattice);

void joinDualPair(int cell1, int cell2, Lattice &lattice, int ignoreOuter);

void xErrorDecoder(bp_osd &decoderHz, mod2sparse *hz, Lattice &lattice);

void zErrorDecoder(Lattice &lattice, int ignoreOuter);

void xErrorDecoder2D(Lattice &lattice, vint &syndromeVertices);

void measErrorDecoder(Lattice &lattice);

void checkIn2DCodespace(Lattice &lattice);

void jumpCorrection(Lattice &lattice);

}

#endif
