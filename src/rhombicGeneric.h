#ifndef RHOMBICGENERIC_H
#define RHOMBICGENERIC_H

#include <math.h>
#include "lattice.h"

namespace rhombic {

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

void addFace(int v, int f, const vint &dirs, const vint &dirs2, 
                const vint &signs, const vint &signs2, Lattice &lattice, int L);

int findFace(vint &vertices, vvint &vertexToFaces, vvint &faceToVertices);

int scalarProduct(std::vector<float> vec, int dir);

int shortestPathLength(int v1, int v2, int L);

vint shortestPath(int v1, int v2, Lattice &lattice, int L);

vint shortestDualPath(int cell1, int cell2, Lattice &lattice, 
                                int L, int useOuter, int useInner);

void jumpCorrection(Lattice &lattice, std::mt19937& engine, 
                        std::uniform_real_distribution<double>& dist, int L, int r);

}

#endif
