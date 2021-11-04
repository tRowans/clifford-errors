
#include <fstream>


std::vector<int> getNonZeroElements(std::vector<int> &elements, std::vector<int> &indexVector);

void writeCSV(std::ofstream &file, std::vector<int> &indices);

void openFiles(std::ofstream& qubitsOut, std::ofstream& xStabsOut, std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, int i, bool decoded);

void writeLatticeInfo(std::ofstream& qubitsOut, std::ofstream& xStabsOut, vint& qubitIndices, vpint& faceToCells, vint& stabIndices);

void writeErrorInfo(std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, vint& qubits, vint& stabs, vint& qubitIndices, vpint& faceToCells, vint& stabIndices);

void closeFiles(std::ofstream& qubitsOut, std::ofstream& xStabsOut, std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, int i, bool decoded);

