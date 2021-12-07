#include <fstream>
#include "lattice.h"
#include "cubic.h"
#include "rhombic1.h"
#include "rhombic2.h"

vvint listToBin(vvint &in)
{
    vvint bin;
    int maxIndex = 0;
    for (auto const &vec : in)
    {
        for (auto const &index : vec)
        {
            if (index > maxIndex)
                maxIndex = index;
        }
    }
    for (auto const &vec : in)
    {
        vint row(maxIndex, 0);
        for (auto const &index : vec)
        {
            row[index] = 1;
        }
        bin.push_back(row);
    }
    return bin;
}

void saveHz(Lattice &lat, int L, std::string file) // Save in numpy readable format
{
    vint &qubitIndices = lat.innerQubitIndices;
    qubitIndices.insert(qubitIndices.end(), lat.outerQubitIndices.begin(), lat.outerQubitIndices.end()); 
    // Make one vector containing all the qubit indices, in-place modification so be careful

    std::ofstream out(file);
    std::streambuf *coutbuf = std::cout.rdbuf(); // Save old buffer
    std::cout.rdbuf(out.rdbuf());                // Redirect std::cout to out

    vvint hz;
    for (auto const &e : lat.zSyndIndices)
    {
        vint row;
        auto faces = lat.edgeToFaces[e];
        for (int i = 0; i < faces.size(); ++i)
        {
            if (std::find(qubitIndices.begin(), qubitIndices.end(), faces[i]) == qubitIndices.end())
            {
                faces[i] = -1;
            }
        }
        for (auto const &f : faces)
        {
            if (f == -1)
                continue;
            row.push_back(f);
        }
        hz.push_back(row);
    }
    auto hzBin = listToBin(hz);
    for (auto const &row : hzBin)
    {
        for (auto const &b : row)
        {
            std::cout << b << " ";
        }
        std::cout << std::endl;
    }
}

void saveHzLs(vint Ls)
{
    for (auto const &L : Ls)
    {
        Lattice latCubic = Lattice(L);
        Lattice latRhombic1 = Lattice(L);
        Lattice latRhombic2 = Lattice(L);
        cubic::buildLattice(latCubic);
        rhombic::r1::buildLattice(latRhombic1);
        rhombic::r2::buildLattice(latRhombic2);

        std::string file = "~/dev/clifford-errors/alist/cubic_L=" + std::to_string(L) + ".txt";
        saveHz(latCubic, L, file);
        file = "~/dev/clifford-errors/alist/rhombic1_L=" + std::to_string(L) + ".txt";
        saveHz(latRhombic1, L, file);
        file = "~/dev/clifford-errors/alist/rhombic2_L=" + std::to_string(L) + ".txt";
        saveHz(latRhombic2, L, file);
    }
}