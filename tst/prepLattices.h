#include "lattice.h"

std::vector<Lattice> lattices(3,6);
Lattice &latCubic = lattices[0];
Lattice &latRhombic1 = lattices[1];
Lattice &latRhombic2 = lattices[2];

std::random_device rd{};
std::mt19937 engine{rd()};
std::uniform_real_distribution<double> dist(0,1);
