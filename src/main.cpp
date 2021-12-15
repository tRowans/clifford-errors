#include "decoderCubic.h"
#include "decoderRhombic1.h"
#include "decoderRhombic2.h"
#include "czErrorGen.h"
#include "vis.h"
#include "saveHz.h"

void saveCodes()
{
    vint Ls;
    int Lstart = 2;
    int Lmax = 4;
    int Lstep = 2;
    for (int i = Lstart; i <= Lmax; i += Lstep) Ls.push_back(i);
    saveHzLs(Ls);
}

int main(int argc, char *argv[])
{
    if (argc != 11)
    {
        std::cout << "Invalid number of arguments." << '\n';
        return 1;
    }

    int L = std::atoi(argv[1]);
    double p = std::atof(argv[2]);
    double q = std::atof(argv[3]);
    int runs = std::atoi(argv[4]);
    int linking = std::atoi(argv[5]);
    int debug = std::atoi(argv[6]);
    int vis = std::atoi(argv[7]);
    int maxIter = std::atoi(argv[8]); // In the past I used 50
    int osdOrder = std::atoi(argv[9]); // In the past I used 10
    int osdMethod = std::atoi(argv[10]); // In the past I used 0
    
    std::vector<Lattice> lattices(3,L);
    Lattice &latCubic = lattices[0];
    Lattice &latRhombic1 = lattices[1];
    Lattice &latRhombic2 = lattices[2];

    cubic::buildLattice(latCubic);
    rhombic::r1::buildLattice(latRhombic1);
    rhombic::r2::buildLattice(latRhombic2);

    Outbox out;

    if (vis == 1) out.writeLatticeInfo(lattices);

    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices);

    // Setup BP-OSD
    std::string fileS = "/home/mvasmer/dev/clifford-errors/alist/cubic_L=" + std::to_string(L) + ".alist";
    char *file = new char[fileS.length() + 1]();
    for (int i = 0; i < fileS.length(); ++i) {
        file[i] = fileS[i];
    }
    file[fileS.length()] = '\0';
    mod2sparse *hzC;
    hzC = load_alist(file);
    delete[] file;
    bp_osd decoderHzC(hzC, p, maxIter, osdOrder, osdMethod);
    fileS = "/home/mvasmer/dev/clifford-errors/alist/rhombic1_L=" + std::to_string(L) + ".alist";
    file = new char[fileS.length() + 1]();
    for (int i = 0; i < fileS.length(); ++i) {
        file[i] = fileS[i];
    }
    file[fileS.length()] = '\0';
    mod2sparse *hzR1;
    hzR1 = load_alist(file);
    bp_osd decoderHzR1(hzR1, p, maxIter, osdOrder, osdMethod);
    delete[] file;
    fileS = "/home/mvasmer/dev/clifford-errors/alist/rhombic2_L=" + std::to_string(L) + ".alist";
    file = new char[fileS.length() + 1]();
    for (int i = 0; i < fileS.length(); ++i) {
        file[i] = fileS[i];
    }
    file[fileS.length()] = '\0';
    mod2sparse *hzR2;
    hzR2 = load_alist(file);
    bp_osd decoderHzR2(hzR2, p, maxIter, osdOrder, osdMethod);
    delete[] file;

    vint cFailures = {0,0};
    vint r1Failures = {0,0};
    vint r2Failures = {0,0};

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    for (int i = 0; i < runs; i++)
    {
        //reset error info
        latCubic.wipe();
        latRhombic1.wipe();
        latRhombic2.wipe();
        
        if (vis == 1) out.writeErrorInfo(lattices);

        //qubits start in |+> --> measure Z stabilisers = random X error distribution
        latCubic.biasedError(0.5, engine, dist, 'x', 0);
        latRhombic1.biasedError(0.5, engine, dist, 'x', 0);
        latRhombic2.biasedError(0.5, engine, dist, 'x', 0);

        //Z stabiliser syndrome + measurement errors
        latCubic.calcSynd('z', 1, 1);
        latRhombic1.calcSynd('z', 1, 1);
        latRhombic2.calcSynd('z', 1, 1);

        if (vis == 1) out.writeErrorInfo(lattices);

        latCubic.measError(q, engine, dist, 'z');
        latRhombic1.measError(q, engine, dist, 'z');
        latRhombic2.measError(q, engine, dist, 'z');

        if (vis == 1) out.writeErrorInfo(lattices);

        //Find and fix measurement errors
        latCubic.findDefects();
        cubic::measErrorDecoder(latCubic);
        latRhombic1.findDefects();
        rhombic::r1::measErrorDecoder(latRhombic1);
        latRhombic2.findDefects();
        rhombic::r2::measErrorDecoder(latRhombic2);
        
        if (vis == 1) out.writeErrorInfo(lattices);

        //Fix X errors (decoder for this not done yet)
        cubic::xErrorDecoder(decoderHzC, hzC, latCubic);
        rhombic::r1::xErrorDecoder(decoderHzR1, hzR1, latRhombic1);
        rhombic::r2::xErrorDecoder(decoderHzR2, hzR2, latRhombic2);
        
        if (vis == 1) out.writeErrorInfo(lattices);

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

        //Apply CCZ --> Clifford errors + a post-gate depolarising error
        //Although in practise only Z errors matter after this point
        //so equivalently could do a biased error with prob 2*p/3
      
        //Need perfect syndromes for CZ error calculation
        //This doesn't correspond to an actual stabiliser measurement 
        latCubic.calcSynd('z',1,1);
        latRhombic1.calcSynd('z',1,1);
        latRhombic2.calcSynd('z',1,1); 
        applyCCZ(lattices, overlappingFaces, engine, dist, linking);
        
        if (vis == 1) out.writeErrorInfo(lattices);

        latCubic.depolarisingError(p, engine, dist);
        latRhombic1.depolarisingError(p, engine, dist);
        latRhombic2.depolarisingError(p, engine, dist);
        
        if (vis == 1) out.writeErrorInfo(lattices);

        //Project to stabiliser distribution + random bit flips from measurement error
        latCubic.zStabPattern(engine, dist);
        latRhombic1.zStabPattern(engine, dist);
        latRhombic2.zStabPattern(engine, dist);

        if (vis == 1) out.writeErrorInfo(lattices);

        latCubic.biasedError(q, engine, dist, 'z', 1);
        latRhombic1.biasedError(q, engine, dist, 'z', 1);
        latRhombic2.biasedError(q, engine, dist, 'z', 1);
        
        if (vis == 1) out.writeErrorInfo(lattices);

        //Z error decoding from single-qubit measurements (only on inner qubits)
        //cubic decoder does not need to specify inner/outer qubits
        //because if no z=0 stabilisers are violated no outer qubits will be used in pathing
        //and if only z=0 stabilisers are used then only outer qubits will be used
        latCubic.calcSynd('x', 0, 1);
        latRhombic1.calcSynd('x', 0, 1);
        latRhombic2.calcSynd('x', 0, 1);
        
        if (vis == 1) out.writeErrorInfo(lattices);

        cubic::zErrorDecoder(latCubic); 
        rhombic::r1::zErrorDecoder(latRhombic1, 0, 1);
        rhombic::r2::zErrorDecoder(latRhombic2, 0, 1);
        
        if (vis == 1) out.writeErrorInfo(lattices);

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
        
        if (vis == 1) out.writeErrorInfo(lattices);

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
        //so we need a measurement-error free 2D decoding step before checking for success.

        latCubic.calcSynd('z',1,0);
        latRhombic1.calcSynd('z',1,0);
        latRhombic2.calcSynd('z',1,0);
        vvint syndromeVertices = getSyndromeVertices(lattices);
        cubic::xErrorDecoder2D(latCubic, syndromeVertices[0]);
        rhombic::r1::xErrorDecoder2D(latRhombic1, syndromeVertices[1]);
        rhombic::r2::xErrorDecoder2D(latRhombic2, syndromeVertices[2]);

        if (vis == 1) out.writeErrorInfo(lattices);

        if (debug == 1)
        {
            latCubic.checkInBounds();
            cubic::checkIn2DCodespace(latCubic);
            latRhombic1.checkInBounds();
            rhombic::checkIn2DCodespace(latRhombic1);
            latRhombic2.checkInBounds();
            rhombic::checkIn2DCodespace(latRhombic2);
        }
        
        //Check X logical errors
        cFailures[0] += latCubic.checkLogicalError('x');
        r1Failures[0] += latRhombic1.checkLogicalError('x');
        r2Failures[0] += latRhombic2.checkLogicalError('x');

        //Can use the same decoder as for 3D here but restricted to outer qubits
        latCubic.calcSynd('x', 1, 0); 
        cubic::zErrorDecoder(latCubic);
        latRhombic1.calcSynd('x', 1, 0);
        rhombic::r1::zErrorDecoder(latRhombic1, 1, 0);
        latRhombic2.calcSynd('x', 1, 0);
        rhombic::r2::zErrorDecoder(latRhombic2, 1, 0);

        if (vis == 1) out.writeErrorInfo(lattices);

        if (debug == 1)
        {
            latCubic.checkInBounds();
            latCubic.checkInCodespace('z', 1, 0);
            latRhombic1.checkInBounds();
            latRhombic1.checkInCodespace('z', 1, 0);
            latRhombic2.checkInBounds();
            latRhombic2.checkInCodespace('z', 1, 0);
        }
        
        //Check Z logical errors 
        cFailures[1] += latCubic.checkLogicalError('z');
        r1Failures[1] += latRhombic1.checkLogicalError('z');
        r2Failures[1] += latRhombic2.checkLogicalError('z');
    }
    
    // BP-OSD cleanup
    mod2sparse_free(hzC);
    mod2sparse_free(hzR1);
    mod2sparse_free(hzR2);
    free(hzC);
    free(hzR1);
    free(hzR1);

    std::cout << L << ',' << p << ',' << q << ',' << runs << ',' << linking << '\n';
    std::cout << cFailures[0] << ',' << r1Failures[0] << ',' << r2Failures[0] << '\n';
    std::cout << cFailures[1] << ',' << r1Failures[1] << ',' << r2Failures[1] << '\n';

    // saveCodes();

    return 0;
}


