#ifndef RHOMBIC1_H
#define RHOMBIC1_H

#include "rhombicGeneric.h"

namespace rhombic1 {

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
