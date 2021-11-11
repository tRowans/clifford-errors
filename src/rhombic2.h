#ifndef RHOMBIC2_H
#define RHOMBIC2_H

#include "rhombicGeneric.h"

namespace rhombic2 {

void buildFaces(vvint &faceToVertices, vvint &faceToEdges, vpint &faceToCells, vvint &vertexToFaces, vvint &edgeToFaces, int L);

int faceToBaseVertex(int face, int L);

void buildVertexToEdges(vvint &vertexToEdges, int L);

void buildEdgeToVertices(vpint &edgeToVertices, int L);

void buildCellToFaces(vvint &cellToFaces, vvint &vertexToFaces, vvint &faceToVertices, int L);

void buildQubitIndices(vint &outerQubitIndices, vint &innerQubitIndices, 
                        vvint &vertexToFaces, vvint &faceToVertices, int L);

void buildXSyndIndices(vint &xSyndIndices, int L);

void buildZSyndIndices(vint &zSyndIndices, int L);

void buildLogicals(vint &xLogical, vint &zLogical, 
                    vint &outerQubitIndices, vint &innerQubitIndices, int L);

}

#endif
