#ifndef RHOMBIC1_H
#define RHOMBIC1_H

#include <string>
#include <math.h>
#include "latticeGeneric.h"

namespace rhombic1 {

struct coord
{
    int xi[4];
};

enum direction {xy, xz, yz, xyz};

int coordToIndex(coord c, int L);

coord indexToCoord(int i, int L);

int neigh(int v, int dir, int sign, int L);

int neighXYZ(int v, int dir, int sign, int L);

int edgeIndex(int v, int dir, int sign, int L);

void addFace(int v, int f, const vint &dirs, const vint &dirs2, const vint &signs, const vint &signs2, vvint &faceToVertices, vvint &faceToEdges, vpint &faceToCells, vvint &vertexToFaces, vvint &edgeToFaces, int L);

void buildFaces(vvint &faceToVertices, vvint &faceToEdges, vpint &faceToCells, vvint &vertexToFaces, vvint &edgeToFaces, int L);

int faceToBaseVertex(int face, int L);

vvint buildVertexToEdges(int L);

vpint buildEdgeToVertices(int L);

vvint buildCellToFaces(vvint &cellToFaces, vvint &vertexToFaces, vvint &faceToVertices, int L);

vint buildQubitIndices(vvint &vertexToFaces, vvint &faceToVertices, int L);

vint buildXSyndIndices(int L);

vint buildZSyndIndices(int L);

vint buildLogicals(vint &xLogical, vint &zLogical, vint &qubitIndices, int L);

}

#endif
