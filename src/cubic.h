#ifndef TORICCODESLICE_H
#define TORICCODESLICE_H

#include "lattice.h"

namespace cubic {

struct coord
{
    int xi[3];
};

enum direction { x, y, z, };

int coordToIndex(coord c, int L);

coord indexToCoord(int i, int L);

int neigh(int v, int dir, int sign, int L);

int edgeIndex(int v, int dir, int sign, int L);

void buildFaceToEdges(vvint &faceToEdges, int L);

void buildEdgeToFaces(vvint &edgeToFaces, int L);

void buildVertexToEdges(vvint &vertexToEdges, int L);

void buildEdgeToVertices(vpint &edgeToVertices, int L);

void buildCellToFaces(vvint &cellToFaces, int L);

void buildFaceToCells(vpint &faceToCells, int L);

void buildFaceToVertices(vvint &faceToVertices, int L);

void buildQubitIndices(vint &outerQubitIndices, vint &innerQubitIndices, vint& allQubitIndices, int L);

void buildXSyndIndices(vint &xSyndIndices, int L);

void buildZSyndIndices(vint &zSyndIndices, int L);

void buildDefectIndices(vint &defectIndices, int L);

void buildXLogical(vint &xLogical, int L);

void buildZLogicals(vvint &zLogicals, int L);

void buildLattice(Lattice &lattice);

}

#endif
