#include "decoderCubic.h"
#include "decoderRhombic1.h"
#include "decoderRhombic2.h"
#include "czErrorGen.h"

int main(int argc, char *argv[])
{
    if (argc != 7)
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
    
    std::vector<Lattice> lattices(3,L);
    Lattice &latCubic = lattices[0];
    Lattice &latRhombic1 = lattices[1];
    Lattice &latRhombic2 = lattices[2];

    cubic::buildLattice(latCubic);
    rhombic::r1::buildLattice(latRhombic1);
    rhombic::r2::buildLattice(latRhombic2);

    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices);

    vint cFailures = {0,0};
    vint r1Failures = {0,0};
    vint r2Failures = {0,0};

    for (int i = 0; i < runs; i++)
    {
        //reset error info
        latCubic.wipe();
        latRhombic1.wipe();
        latRhombic2.wipe();
        
        //reset this stuff (is this necessary?)
        std::random_device rd{};
        std::mt19937 engine{rd()};
        std::uniform_real_distribution<double> dist(0,1);

        //qubits start in |+> --> measure Z stabilisers = random X error distribution
        latCubic.biasedError(0.5, engine, dist, 'x', 0);
        latRhombic1.biasedError(0.5, engine, dist, 'x', 0);
        latRhombic2.biasedError(0.5, engine, dist, 'x', 0);

        //Z stabiliser syndrome + measurement errors
        latCubic.calcSynd('z', 1, 1);
        latCubic.measError(q, engine, dist, 'z');
        latRhombic1.calcSynd('z', 1, 1);
        latRhombic1.measError(q, engine, dist, 'z');
        latRhombic2.calcSynd('z', 1, 1);
        latRhombic2.measError(q, engine, dist, 'z');

        //Find and fix measurement errors
        latCubic.findDefects();
        cubic::measErrorDecoder(latCubic);
        latRhombic1.findDefects();
        rhombic::r1::measErrorDecoder(latRhombic1);
        latRhombic2.findDefects();
        rhombic::r2::measErrorDecoder(latRhombic2);

        //Fix X errors (decoder for this not done yet)
        //cubic::xErrorDecoder(...);
        //rhombic::r1::xErrorDecoder(...);
        //rhombic::r2::xErrorDecoder(...);

        //Check everything working as expected (debugging step)
        if (debug == 1)
        {
            latCubic.checkInBounds();
            latCubic.checkInCodespace('x', 1, 1);
            latRhombic1.checkInBounds();
            latRhombic1.checkInCodespace('x', 1, 1);
            latRhombic2.checkInBounds();
            latRhombic2.checkInCodespace('x', 1, 1);
        }

        //Check X logical errors
        cFailures[0] += latCubic.checkLogicalError('x');
        r1Failures[0] += latRhombic1.checkLogicalError('x');
        r2Failures[0] += latRhombic2.checkLogicalError('x');

        //Apply CCZ --> Clifford errors + a post-gate depolarising error
        //Although in practise only Z errors matter after this point
        //so equivalently could do a biased error with prob 2*p/3
        applyCCZ(lattices, overlappingFaces, engine, dist, link);
        latCubic.depolarisingError(p, engine, dist);
        latRhombic1.depolarisingError(p, engine, dist);
        latRhombic2.depolarisingError(p, engine, dist);

        //Project to stabiliser distribution + random bit flips from measurement error
        latCubic.zStabPattern(engine, dist);
        latCubic.biasedError(q, engine, dist, 'z', 1);
        latRhombic1.zStabPattern(engine, dist);
        latRhombic1.biasedError(q, engine, dist, 'z', 1);
        latRhombic2.zStabPattern(engine, dist);
        latRhombic2.biasedError(q, engine, dist, 'z', 1);

        //Z error decoding from single-qubit measurements (only on inner qubits)
        //cubic decoder does not need to specify inner/outer qubits
        //because if no z=0 stabilisers are violated no outer qubits will be used in pathing
        //and if only z=0 stabilisers are used then only outer qubits will be used
        latCubic.calcSynd('x', 0, 1);
        cubic::zErrorDecoder(latCubic); 
        latRhombic1.calcSynd('x', 0, 1);
        rhombic::r1::zErrorDecoder(latRhombic1, 0, 1);
        latRhombic2.calcSynd('x', 0, 1);
        rhombic::r2::zErrorDecoder(latRhombic2, 0, 1);

        //Check step
        if (debug == 1)
        {
            latCubic.checkInBounds();
            latCubic.checkInCodespace('z', 0, 1);
            latRhombic1.checkInBounds();
            latRhombic1.checkInCodespace('z', 0, 1);
            latRhombic2.checkInBounds();
            latRhombic2.checkInCodespace('z', 0, 1);
        }

        //Find dimension jump corrections for 2D codes 
        cubic::jumpCorrection(latCubic);
        rhombic::jumpCorrection(latRhombic1, engine, dist, 1);
        rhombic::jumpCorrection(latRhombic2, engine, dist, 2);

        //Another check step
        if (debug == 1)
        {
            latCubic.checkInBounds();
            latCubic.checkJumpCorrection();
            latRhombic1.checkInBounds();
            latRhombic1.checkJumpCorrection();
            latRhombic2.checkInBounds();
            latRhombic2.checkJumpCorrection();
        }
        
        //We do not expect the 2D code to be error free even if we make no mistakes
        //because it will still have errors from the CZ + depolarising error
        //so we need a measurement-error free decoding step before checking for success.
        //Can use the same decoder as for 3D here as long as errors are only on outer qubits
        latCubic.calcSynd('x', 1, 0); 
        cubic::zErrorDecoder(latCubic);
        latRhombic1.calcSynd('x', 1, 0);
        rhombic::r1::zErrorDecoder(latRhombic1, 1, 0);
        latRhombic2.calcSynd('x', 1, 0);
        rhombic::r2::zErrorDecoder(latRhombic2, 1, 0);

        //Check Z logical errors 
        cFailures[1] += latCubic.checkLogicalError('z');
        r1Failures[1] += latRhombic1.checkLogicalError('z');
        r2Failures[1] += latRhombic2.checkLogicalError('z');
    }
    
    std::cout << L << ',' << p << ',' << q << ',' << runs << ',' << link << '\n';
    std::cout << cFailures[0] << ',' << r1Failures[0] << ',' << r2Failures[0] << '\n';
    std::cout << cFailures[1] << ',' << r1Failures[1] << ',' << r2Failures[1] << '\n';

    return 0;
}

