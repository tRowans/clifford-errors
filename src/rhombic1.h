#ifndef RHOMBIC1_H
#define RHOMBIC1_H

#include "rhombicGeneric.h"

namespace rhombic1 {

void buildFaces(vvint &faceToVertices, vvint &faceToEdges, vpint &faceToCells, vvint &vertexToFaces, vvint &edgeToFaces, int L);

int faceToBaseVertex(int face, int L);

void buildVertexToEdges(vvint &vertexToEdges, int L);

void buildEdgeToVertices(vpint &edgeToVertices, int L);

void buildCellToFaces(vvint &cellToFaces, vvint &vertexToFaces, vvint &faceToVertices, int L);

void buildQubitIndices(vint &qubitIndices, vvint &vertexToFaces, vvint &faceToVertices, int L);

void buildXSyndIndices(vint &xSyndIndices, int L);

void buildZSyndIndices(vint &zSyndIndices, int L);

void buildLogicals(vint &xLogical, vint &zLogical, vint &qubitIndices, int L);

}

#endif
