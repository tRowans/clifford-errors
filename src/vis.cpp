#include "vis.h"

vint Outbox::getNonZeroElements(std::vector<int> &elements, std::vector<int> &indexVector)
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

void Outbox::writeCSV(std::vector<int> &indices)
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

void Outbox::writeLatticeInfo(std::vector<Lattice> &lattices)
{
    std::vector<std::string> latticeNames = {"C", "R1", "R2"};
    for (int i = 0; i < 3; i++)
    {
        Lattice &lattice = lattices[i];
        std::string latticeName = latticeNames[i];
        std::set<int> latticeVerticesSet;
        std::set<pint> latticeEdgesSet;
        for (const vint qubitIndices : {lattice.outerQubitIndices, lattice.innerQubitIndices})
        {
            for (int i : qubitIndices)
            {
                vint corners = lattice.faceToVertices[i];
                for (int j : corners) latticeVerticesSet.insert(j);
                vint sides = lattice.faceToEdges[i];
                for (int j : sides) latticeEdgesSet.insert(lattice.edgeToVertices[j]);
            }
        }
        vint latticeVertices;
        for (int i : latticeVerticesSet) latticeVertices.push_back(i);
        file.open("vertices_" + latticeName + ".csv");
        writeCSV(latticeVertices);
        file.close();
        file.open("edges_" + latticeName + ".csv");
        for (pint i : latticeEdgesSet) file << i.first << ',' << i.second << '\n';
        file.close();
    }
}

void Outbox::prepErrorFiles()
{
    std::vector<std::string> latticeNames = {"C", "R1", "R2"};
    for (int i = 0; i < 3; i++)
    {
        std::string latticeName = latticeNames[i];

        file.open("xErrors_" + latticeName + ".csv", std::ios::app);
        file.close();

        file.open("zErrors_" + latticeName + ".csv", std::ios::app);
        file.close();
        
        file.open("xSyndrome_" + latticeName + ".csv", std::ios::app);
        file.close();
        
        file.open("zSyndrome_" + latticeName + ".csv", std::ios::app);
        file.close();
        
        file.open("defects_" + latticeName + ".csv", std::ios::app);
        file.close();
    }
}

void Outbox::writeErrorInfo(std::vector<Lattice> &lattices)
{
    std::vector<std::string> latticeNames = {"C", "R1", "R2"};
    for (int i = 0; i < 3; i++)
    {
        Lattice &lattice = lattices[i];
        std::string latticeName = latticeNames[i];

        vint xErrors = getNonZeroElements(lattice.qubitsX, lattice.outerQubitIndices);
        vint innerErrors = getNonZeroElements(lattice.qubitsX, lattice.innerQubitIndices);
        xErrors.insert(xErrors.end(), innerErrors.begin(), innerErrors.end());
        vint zErrors = getNonZeroElements(lattice.qubitsZ, lattice.outerQubitIndices);
        innerErrors = getNonZeroElements(lattice.qubitsZ, lattice.innerQubitIndices);
        zErrors.insert(zErrors.end(), innerErrors.begin(), innerErrors.end());

        vint xSyndrome = getNonZeroElements(lattice.syndromeX, lattice.xSyndIndices);
        vint zSyndrome = getNonZeroElements(lattice.syndromeZ, lattice.zSyndIndices);

        file.open("xErrors_" + latticeName + ".csv", std::ios::app);
        writeCSV(xErrors);
        file.close();

        file.open("zErrors_" + latticeName + ".csv", std::ios::app);
        writeCSV(zErrors);
        file.close();
        
        file.open("xSyndrome_" + latticeName + ".csv", std::ios::app);
        writeCSV(xSyndrome);
        file.close();
        
        file.open("zSyndrome_" + latticeName + ".csv", std::ios::app);
        writeCSV(zSyndrome);
        file.close();
        
        file.open("defects_" + latticeName + ".csv", std::ios::app);
        writeCSV(lattice.defects);
        file.close();
    }
}
