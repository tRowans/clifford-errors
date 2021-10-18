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

coord cellToCoord(int cell, int L);

int neigh(int v, int dir, int sign, int L);

int neighXYZ(int v, int dir, int sign, int L);

int edgeIndex(int v, int dir, int sign, int L);

void addFace(int v, int f, const vint &dirs, const vint &dirs2, const vint &signs, const vint &signs2, vvint &faceToVertices, vvint &faceToEdges, vpint &faceToCells, vvint &vertexToFaces, vvint &edgeToFaces, int L);

void buildFaces(vvint &faceToVertices, vvint &faceToEdges, vpint &faceToCells, vvint &vertexToFaces, vvint &edgeToFaces, int L);

//vvint listToBin(vvint& in, int L);

vvint buildVertexToEdges(int L);

vpint buildEdgeToVertices(int L);

vvint buildCellToFaces(vvint &vertexToFaces, vvint &faceToVertices, int L);

//vpint buildFaceToCells(int L);

//std::vector<std::vector<int>> buildZLogicals(int L);

void buildSlice(vint &lowerQubitIndices, vint &middleQubitIndices, vint &upperQubitIndices, vint &syndIndicesZ, vint &syndIndicesXLayer, vint &defectIndices, vint &bulkSweepVertices, vint &middleBoundarySweepVertices, vint &upperBoundarySweepVertices1, vint &upperBoundarySweepVertices2, vint &zLogical, vint &xLogicalFragment1, vint &xLogicalFragment2, vvint &vertexToFaces, vvint &faceToVertices, vvint &faceToEdges, vpint &edgeToVertices, int L);

vint moveFaces(vint &indexVector, vvint &vertexToFaces, vvint &faceToVertices);

void moveIndices(vint &lowerQubitIndices, vint &middleQubitIndices, vint &upperQubitIndices, vint &syndIndices, vint &defectIndices, vint &bulkSweepVertices, vint &middleBoundarySweepVertices, vint &upperBoundarySweepVertices1, vint &upperBoundarySweepVertices2, vint &zLogical, vvint &vertexToFaces, vvint &faceToVertices, int L);

}

#endif
