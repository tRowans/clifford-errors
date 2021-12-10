#include "vis.h"

std::vector<int> getNonZeroElements(std::vector<int> &elements, std::vector<int> &indexVector)
{
    std::vector<int> nonZeroElements = {};
    for (int i : indexVector)
    {
        if (elements[i] == 1)
        {
            nonZeroElements.push_back(i);
        }
    }
    return nonZeroElements;
}

void writeCSV(std::ofstream &file, std::vector<int> &indices)
{
    if (indices.size())
    {
        file << indices[0];
        for (int i = 1; i < indices.size(); i++)
        {
            file << ',' << indices[i];
        }
    }
    file << '\n';
}

void openFiles(std::ofstream& qubitsOut, std::ofstream& xStabsOut, std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, int i, bool decoded)
{
    if (i == 0 && decoded == 0)
    {
        qubitsOut.open("qubits.csv");
        xStabsOut.open("xStabs.csv");
    }
    if (decoded)
    {
        zErrorsOut.open("zErrors_" + std::to_string(i) + "_decoded.csv");
        violatedXStabsOut.open("violatedXStabs_" + std::to_string(i) + "_decoded.csv");
    }
    else 
    {
        zErrorsOut.open("zErrors_" + std::to_string(i) + "_undecoded.csv");
        violatedXStabsOut.open("violatedXStabs_" + std::to_string(i) + "_undecoded.csv");
    }
}

void writeLatticeInfo(std::ofstream& qubitsOut, std::ofstream& xStabsOut, vint& qubitIndices, vpint& faceToCells, vint& stabIndices)
{
    for (int qubit : qubitIndices)
    {
        pint cells = faceToCells[qubit];
        qubitsOut << cells.first << ',' << cells.second << '\n';
    }
    writeCSV(xStabsOut, stabIndices);
}

void writeErrorInfo(std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, vint& qubits, vint& stabs, vint& qubitIndices, vpint& faceToCells, vint& stabIndices)
{
    vint nonZeroQubits = getNonZeroElements(qubits, qubitIndices);
    vint nonZeroStabs = getNonZeroElements(stabs, stabIndices);
    for (int qubit : nonZeroQubits)
    {
        pint cells = faceToCells[qubit];
        zErrorsOut << cells.first << ',' << cells.second << '\n';
    }
    writeCSV(violatedXStabsOut, nonZeroStabs);
}

void closeFiles(std::ofstream& qubitsOut, std::ofstream& xStabsOut, std::ofstream& zErrorsOut, std::ofstream& violatedXStabsOut, int i, bool decoded)
{
    if (i == 0 && decoded == 0)
    {
        qubitsOut.close();
        xStabsOut.close();
    }
    zErrorsOut.close();
    violatedXStabsOut.close();
}

