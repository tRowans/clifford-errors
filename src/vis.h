#ifndef VIS_H
#define VIS_H

#include <fstream>
#include "lattice.h"

class Outbox
{
    private:
        std::ofstream file;
        
        vint getNonZeroElements(std::vector<int> &elements, std::vector<int> &indexVector);
        
        void writeCSV(std::vector<int> &indices);

    public:

        void writeLatticeInfo(std::vector<Lattice> &lattices);

        void writeErrorInfo(std::vector<Lattice> &lattices);
};

#endif
