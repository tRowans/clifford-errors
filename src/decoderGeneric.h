#ifndef DECODERGENERIC_H
#define DECODERGENERIC_H

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <fstream>
#include "latticeGeneric.h"

void depolarisingError(vint &qubitsX, vint &qubitsZ, double p, std::mt19937& engine, std::uniform_real_distribution<double>& dist, vint& qubitIndices);

void biasedError(std::vector<int> &qubits, double p, std::mt19937& engine, std::uniform_real_distribution<double>& dist, std::vector<int>& qubitIndices);

void measError(vint &syndrome, double q, std::mt19937& engine, std::uniform_real_distribution<double>& dist, vint &syndromeIndices);

void calcSynd(vint &syndrome, vint &qubits, vint &lowerQubitIndices, vint &middleQubitIndices, vint &upperQubitIndices, vvint &faceToEdges, vint& syndromeIndices); 

void calcXSynd(vint &syndrome, vint &qubits, std::vector<std::pair<int,int>> &faceToCells, vint &syndIndices);

vvint getSyndromeVertices(vvint &syndromes, vpint &edgeToVerticesT, vpint &edgeToVerticesR1, vpint &edgeToVerticesR2);

void findDefects(vint &syndrome, vvint &vertexToEdges, std::vector<std::pair<int, int>>& edgeToVertices, vint& defects, vint& defectIndices);

bool checkXCorrection(std::vector<int> &syndrome, std::vector<int> &qubits, std::vector<int> &lowerQubitIndices, std::vector<int> &middleQubitIndices, std::vector<int> &upperQubitIndices, std::vector<std::vector<int>> &faceToEdges, std::vector<int>& syndIndices, bool r2);

bool checkZCorrection(std::vector<int> &syndrome, std::vector<int> &qubits, std::vector<std::pair<int,int>> &faceToCells, std::vector<int> &syndIndices);

bool checkLogicalError(vint &qubits, vint &logical);

int scalarProduct(std::vector<float> vec, int dir);

void updateQubitHistory(vint &lowerQubitIndicesT, vint &middleQubitIndicesT, vint &lowerQubitIndicesR1, vint &middleQubitIndicesR1, vint &lowerQubitIndicesR2, vint &middleQubitIndicesR2, vvint &qubitIndexHistory, int laststep);

void updateStabiliserHistory(vint &syndIndicesXLayerT, vint &syndIndicesXLayerR1, vint &syndIndicesXLayerR2, vint &syndIndicesXT, vint &syndIndicesXR1, vint &syndIndicesXR2, int L, int j, int laststep);

void updateLogicalHistory(vint &xLogicalFragmentT, vint &xLogicalFragment1R1, vint &xLogicalFragment2R1, vint &xLogicalFragment1R2, vint &xLogicalFragment2R2, vint &xLogicalT, vint &xLogicalR1, vint &xLogicalR2, int L, int laststep);

void moveXIndices(vint &syndIndicesXLayerT, vint &syndIndicesXLayerR1, vint &syndIndicesXLayerR2, 
                  vint &xLogicalFragmentT, vint &xLogicalFragment1R1, vint &xLogicalFragment2R1,
                  vint &xLogicalFragment1R2, vint &xLogicalFragment2R2, int L);

void applyCCZ(vvint &qubitsX, vvint &qubitsZ, vvint &syndromeVertices, vvint &xStabs, std::map<std::pair<int,int>,std::pair<std::pair<int,int>,std::pair<int,int>>> &overlappingFaces, vvint &cellToFacesT, vvint &cellToFacesR1, vvint &cellToFacesR2, vint &lowerQubitIndicesT, vint &middleQubitIndicesT, int L);

std::vector<int> getNonZeroElements(std::vector<int> &elements, std::vector<int> &indexVector);

void writeCSV(std::ofstream &file, std::vector<int> &indices);

void openFiles(std::ofstream& qubitsOut, std::ofstream& xStabsOut, std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, int i, bool decoded);

void writeLatticeInfo(std::ofstream& qubitsOut, std::ofstream& xStabsOut, vint& qubitIndices, vpint& faceToCells, vint& stabIndices);

void writeErrorInfo(std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, vint& qubits, vint& stabs, vint& qubitIndices, vpint& faceToCells, vint& stabIndices);

void closeFiles(std::ofstream& qubitsOut, std::ofstream& xStabsOut, std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, int i, bool decoded);

#endif
