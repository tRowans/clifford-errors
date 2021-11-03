#ifndef CZERRORGEN_H
#define CZERRORGEN_H

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <fstream>
#include "lattice.h"


std::map<pint,std::pair<pint,pint>> buildOverlappingFaces(Lattice &cubic, Lattice &rhombic1, Lattice &rhombic2, int L);

void randomiseStabs(vvint &xStabs, std::mt19937 &engine, std::uniform_real_distribution<double> &dist);

vvint getSyndromeVertices(vvint &syndromes, vpint &edgeToVerticesT, vpint &edgeToVerticesR1, vpint &edgeToVerticesR2);

void applyCCZ(vvint &qubitsX, vvint &qubitsZ, vvint &syndromeVertices, vvint &xStabs, std::map<std::pair<int,int>,std::pair<std::pair<int,int>,std::pair<int,int>>> &overlappingFaces, vvint &cellToFacesT, vvint &cellToFacesR1, vvint &cellToFacesR2, vint &lowerQubitIndicesT, vint &middleQubitIndicesT, int L);

std::vector<int> getNonZeroElements(std::vector<int> &elements, std::vector<int> &indexVector);

void writeCSV(std::ofstream &file, std::vector<int> &indices);

void openFiles(std::ofstream& qubitsOut, std::ofstream& xStabsOut, std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, int i, bool decoded);

void writeLatticeInfo(std::ofstream& qubitsOut, std::ofstream& xStabsOut, vint& qubitIndices, vpint& faceToCells, vint& stabIndices);

void writeErrorInfo(std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, vint& qubits, vint& stabs, vint& qubitIndices, vpint& faceToCells, vint& stabIndices);

void closeFiles(std::ofstream& qubitsOut, std::ofstream& xStabsOut, std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, int i, bool decoded);

#endif
