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

    vint cFailures = {0,0};
    vint r1Failures = {0,0};
    vint r2Failures = {0,0};

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
        cubic.biasedError(0.5, engine, dist, 'x', 0);
        rhombic1.biasedError(0.5, engine, dist, 'x', 0);
        rhombic2.biasedError(0.5, engine, dist, 'x', 0);

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

        //Check everything working as expected (debugging step)
        cubic.checkInBounds('x');
        cubic.checkInCodespace('x');
        rhombic1.checkInBounds('x');
        rhombic1.checkInCodespace('x');
        rhombic2.checkInBounds('x');
        rhombic2.checkInCodespace('x');

        //Check X logical errors
        cFailures[0] += cubic.checkLogicalError('x');
        r1Failures[0] += rhombic1.checkLogicalError('x');
        r2Failures[0] += rhombic2.checkLogicalError('x');

        //Apply CCZ --> Clifford errors + a post-gate depolarising error
        applyCCZ(lattices, overlappingFaces, L, engine, dist, link);
        cubic.depolarisingError(p, engine, dist);
        rhombic1.depolarisingError(p, engine, dist);
        rhombic2.depolarisingError(p, engine, dist);

        //Project to stabiliser distribution + random bit flips from measurement error
        cubic.zStabPattern(engine, dist);
        cubic.biasedError(q, engine, dist, 'z', 1);
        rhombic1.zStabPattern(engine, dist);
        rhombic1.biasedError(q, engine, dist, 'z', 1);
        rhombic2.zStabPattern(engine, dist);
        rhombic2.biasedError(q, engine, dist, 'z', 1);

        //Z error decoding from single-qubit measurements
        cubic.calcSynd('x');
        cubic::zErrorDecoder(cubic, L);
        rhombic1.calcSynd('x');
        rhombic1::zErrorDecoder(rhombic1, L);
        rhombic2.calcSynd('x');
        rhombic2::zErrorDecoder(rhombic2, L);

        //Check everything working as expected (debugging step)
        cubic.checkInBounds('z');
        cubic.checkInCodespace('z');
        rhombic1.checkInBounds('z');
        rhombic1.checkInCodespace('z');
        rhombic2.checkInBounds('z');
        rhombic2.checkInCodespace('z');

        //Find 2D code corrections
        cubic::jumpCorrection(cubic, L);
        rhombicJumpCorrection(rhombic1, engine, dist, L, 1);
        rhombicJumpCorrection(rhombic2, engine, dist, L, 2);

        //Need a measurement-error free 2D code decoder here
        cubic::decoder2D(...);
        rhombic1::decoder2D(...);
        rhombic2::decoder2D(...);

        //Check Z logical errors (need to make sure logical Zs I chose are in 2D codes)
        cFailuers += cubic.checkLogicalError('z');
        r1Failures += rhombic1.checkLogicalError('z');
        r2Failures += rhombic2.checkLogicalError('z');
    }
    
    std::cout << OUTPUTS << '\n';

    return 0;
}

