#include "decoderCubic.h"
#include "decoderRhombic1.h"
#include "decoderRhombic2.h"
#include "czErrorGen.h"

int main(int argc, char *argv[])
{
    if (argc != AMOUNT)
    {
        std::cout << "Invalid number of arguments." << '\n';
        return 1;
    }

    int L = std::atoi(argv[1]);
    double p = std::atof(argv[2]);
    double q = std::atof(argv[3]);
    int runs = std::atoi(argv[4]);
    int link = std::atoi(argv[5]);
    //MORE HERE?
    
    std::vector<Lattice> lattices(3);
    Lattice &cubic = lattices[0];
    Lattice &rhombic1 = lattices[1];
    Lattice &rhombic2 = lattices[2];

    cubic::buildLattice(cubic, L);
    rhombic1::buildLattice(rhombic1, L);
    rhombic2::buildLattice(rhombic2, L);

    std::map<pint,std::pair<pint,pint>> overlappingFaces = buildOverlappingFaces(lattices, L);

    for (int i = 0; i < runs; i++)
    {
        //reset error info
        cubic.wipe();
        rhombic1.wipe();
        rhombic2.wipe();
        
        //reset this stuff (is this necessary?)
        std::random_device rd{};
        std::mt19937 engine{rd()};
        std::uniform_real_distribution<double> dist(0,1);

        //qubits start in |+> --> measure Z stabilisers = random X error distribution
        cubic.biasedError(0.5, engine, dist, 'x');
        rhombic1.biasedError(0.5, engine, dist, 'x');
        rhombic2.biasedError(0.5, engine, dist, 'x');

        //Z stabiliser syndrome + measurement errors
        cubic.calcSynd('z');
        cubic.measError(q, engine, dist, 'z');
        rhombic1.calcSynd('z');
        rhombic1.measError(q, engine, dist, 'z');
        rhombic2.calcSynd('z');
        rhombic2.measError(q, engine, dist, 'z');

        //Find and fix measurement errors
        cubic.findDefects();
        cubic::measErrorDecoder(cubic.syndromeZ, cubic.defects, L);
        rhombic1.findDefects();
        rhombic1::measErrorDecoder(rhombic1.syndromeZ, rhombic1.defects, L); 
        rhombic2.findDefects();
        rhombic2::measErrorDecoder(rhombic2.syndromeZ, rhombic2.defects, L); 

        //Fix X errors (decoder for this not done yet)
        cubic::xErrorDecoder(...);
        rhombic1::xErrorDecoder(...);
        rhombic2::xErrorDecoder(...);

        //Apply CCZ --> Clifford errors + a post-gate depolarising error
        applyCCZ(lattices, overlappingFaces, L, engine, dist, link);
        cubic.depolarisingError(p, engine, dist);
        rhombic1.depolarisingError(p, engine, dist);
        rhombic2.depolarisingError(p, engine, dist);



        


