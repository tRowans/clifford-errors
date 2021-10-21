#ifndef TORICCODESLICE_H
#define TORICCODESLICE_H

#include "latticeGeneric.h"

namespace toric {

struct coord
{
    int xi[3];
};

enum direction { x, y, z, };

int coordToIndex(coord c, int L);

coord indexToCoord(int i, int L);

int neigh(int v, int dir, int sign, int L);

int edgeIndex(int v, int dir, int sign, int L);

vvint buildFaceToEdges(int L);

vvint buildEdgeToFaces(int L);

vvint buildVertexToEdges(int L);

vpint buildEdgeToVertices(int L);

vvint buildCellToFaces(int L);

vpint buildFaceToCells(int L);

vvint buildFaceToVertices(int L);

vint buildQubitIndices(int L);

vint buildXSyndIndices(int L);

vint buildZSyndIndices(int L);

vint buildXLogical(int L);

vint buildZLogical(int L);

}

#endif
