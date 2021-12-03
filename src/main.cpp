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
    int debug = std::atoi(argv[6]);
    
    std::vector<Lattice> lattices(3);
    Lattice &cubic = lattices[0];
    Lattice &rhombic1 = lattices[1];
    Lattice &rhombic2 = lattices[2];

    cubic::buildLattice(cubic, L);
    rhombic::r1::buildLattice(rhombic1, L);
    rhombic::r2::buildLattice(rhombic2, L);

    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices, L);

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
        cubic.calcSynd('z', 1, 1);
        cubic.measError(q, engine, dist, 'z');
        rhombic1.calcSynd('z', 1, 1);
        rhombic1.measError(q, engine, dist, 'z');
        rhombic2.calcSynd('z', 1, 1);
        rhombic2.measError(q, engine, dist, 'z');

        //Find and fix measurement errors
        cubic.findDefects();
        cubic::measErrorDecoder(cubic, L);
        rhombic1.findDefects();
        rhombic::r1::measErrorDecoder(rhombic1, L);
        rhombic2.findDefects();
        rhombic::r2::measErrorDecoder(rhombic2, L);

        //Fix X errors (decoder for this not done yet)
        cubic::xErrorDecoder(...);
        rhombic::r1::xErrorDecoder(...);
        rhombic::r2::xErrorDecoder(...);

        //Check everything working as expected (debugging step)
        if (debug == 1)
        {
            cubic.checkInBounds();
            cubic.checkInCodespace('x', 1, 1);
            rhombic1.checkInBounds();
            rhombic1.checkInCodespace('x', 1, 1);
            rhombic2.checkInBounds();
            rhombic2.checkInCodespace('x', 1, 1);
        }

        //Check X logical errors
        cFailures[0] += cubic.checkLogicalError('x');
        r1Failures[0] += rhombic1.checkLogicalError('x');
        r2Failures[0] += rhombic2.checkLogicalError('x');

        //Apply CCZ --> Clifford errors + a post-gate depolarising error
        //Although in practise only Z errors matter after this point
        //so equivalently could do a biased error with prob 2*p/3
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

        //Z error decoding from single-qubit measurements (only on inner qubits)
        cubic.calcSynd('x', 0, 1);
        cubic::zErrorDecoder(cubic, L, 0, 1);
        rhombic1.calcSynd('x', 0, 1);
        rhombic::r1::zErrorDecoder(rhombic1, L, 0, 1);
        rhombic2.calcSynd('x', 0, 1);
        rhombic::r2::zErrorDecoder(rhombic2, L, 0, 1);

        //Check step
        if (debug == 1)
        {
            cubic.checkInBounds();
            cubic.checkInCodespace('z', 0, 1);
            rhombic1.checkInBounds();
            rhombic1.checkInCodespace('z', 0, 1);
            rhombic2.checkInBounds();
            rhombic2.checkInCodespace('z', 0, 1);
        }

        //Find dimension jump corrections for 2D codes 
        cubic::jumpCorrection(cubic, L);
        rhombic::jumpCorrection(rhombic1, engine, dist, L, 1);
        rhombic::jumpCorrection(rhombic2, engine, dist, L, 2);

        //Another check step
        if (debug == 1)
        {
            cubic.checkInBounds();
            cubic.checkJumpCorrection();
            rhombic1.checkInBounds();
            rhombic1.checkJumpCorrection();
            rhombic2.checkInBounds();
            rhombic2.checkJumpCorrection();
        }
        
        //We do not expect the 2D code to be error free even if we make no mistakes
        //because it will still have errors from the CZ + depolarising error
        //so we need a measurement-error free decoding step before checking for success.
        //Can use the same decoder as for 3D here as long as errors are only on outer qubits
        cubic.calcSynd('x', 1, 0); 
        cubic::zErrorDecoder(cubic, L, 1, 0);
        rhombic1.calcSynd('x', 1, 0);
        rhombic::r1::zErrorDecoder(rhombic1, L, 1, 0);
        rhombic2.calcSynd('x', 1, 0);
        rhombic::r2::zErrorDecoder(rhombic2, L, 1, 0);

        //Check Z logical errors 
        cFailuers[1] += cubic.checkLogicalError('z');
        r1Failures[1] += rhombic1.checkLogicalError('z');
        r2Failures[1] += rhombic2.checkLogicalError('z');
    }
    
    std::cout << L << ',' << p << ',' << q << ',' << runs << ',' << link << '\n';
    std::cout << cFailures[0] << ',' << r1Failures[0] << ',' << r2Failures[0] << '\n';
    std::cout << cFailures[1] << ',' << r1Failures[1] << ',' << r2Failures[1] << '\n';

    return 0;
}

