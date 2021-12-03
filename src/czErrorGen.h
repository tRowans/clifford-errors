#ifndef CZERRORGEN_H
#define CZERRORGEN_H

#include "lattice.h"

std::map<pint,ppint> buildOverlappingFaces(std::vector<Lattice> &lattices, int L);

vvint getSyndromeVertices(std::vector<Lattice> &lattices);

int latticeWhereCell(int v, int latticeA, int L);

void applyCCZ(std::vector<Lattice> &lattices, std::map<pint,ppint> &overlappingFaces, int L, std::mt19937 &engine, std::uniform_real_distribution<double> &dist, int link);

#endif
