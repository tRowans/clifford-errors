#ifndef RHOMBIC2_H
#define RHOMBIC2_H

#include "rhombicGeneric.h"

namespace rhombic {

namespace r2 {

void buildFaces(Lattice &lattice);

int faceToBaseVertex(int face, int L);

void buildVertexToEdges(vvint &vertexToEdges, int L);

void buildEdgeToVertices(vpint &edgeToVertices, int L);

void buildCellToFaces(vvint &cellToFaces, vvint &vertexToFaces, vvint &faceToVertices, int L);

void buildQubitIndices(vint &outerQubitIndices, vint &innerQubitIndices, vint &allQubitIndices, int L);

void buildXSyndIndices(vint &xSyndIndices, int L);

void buildZSyndIndices(vint &zSyndIndices, int L);

void buildDefectIndices(vint &defectIndices, int L);

void buildXLogical(Lattice &lattice);

void buildZLogicals(Lattice &lattice);

void buildLattice(Lattice &lattice);

}

}

#endif
