#ifndef RHOMBICGENERIC.H
#define RHOMBICGENERIC.H

#include <math.h>
#include "lattice.h"

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

int findFace(vint &vertices, vvint &vertexToFaces, vvint &faceToVertices);

int scalarProduct(std::vector<float> vec, int dir);

#endif
