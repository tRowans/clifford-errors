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
                const vint &signs, const vint &signs2, Lattice &lattice);

int findFace(vint vertices, vvint &vertexToFaces, vvint &faceToVertices);

int scalarProduct(std::vector<float> vec, int dir, int sign);

std::vector<float> differenceVector(coord c1, coord c2);

float magnitude(std::vector<float> diff);

vint shortestPath(int v1, int v2, Lattice &lattice);

vint shortestDualPath(int cell1, int cell2, Lattice &lattice, int decode2D);

void checkIn2DCodespace(Lattice &lattice);

void jumpCorrection(Lattice &lattice, std::mt19937& engine, 
                        std::uniform_real_distribution<double>& dist, int r);

}

#endif
