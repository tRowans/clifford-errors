#ifndef RHOMBIC2_H
#define RHOMBIC2_H

#include "rhombicGeneric.h"

namespace rhombic2 {

void buildFaces(vvint &faceToVertices, vvint &faceToEdges, vpint &faceToCells, vvint &vertexToFaces, vvint &edgeToFaces, int L);

int faceToBaseVertex(int face, int L);

vvint buildVertexToEdges(int L);

vpint buildEdgeToVertices(int L);

vvint buildCellToFaces(vvint &cellToFaces, vvint &vertexToFaces, vvint &faceToVertices, int L);

vint buildQubitIndices(vvint &vertexToFaces, vvint &faceToVertices, int L);

vint buildXSyndIndices(int L);

vint buildZSyndIndices(int L);

vint buildLogicals(int L);

}

#endif
