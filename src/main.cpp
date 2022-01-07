#include "decoderCubic.h"
#include "decoderRhombic1.h"
#include "decoderRhombic2.h"
#include "czErrorGen.h"
#include "vis.h"
#include "saveHz.h"

void saveCodes()
{
    vint Ls;
    int Lstart = 18;
    int Lmax = 20;
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
    int ccz = std::atoi(argv[5]);   
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
    std::string fileS = "/home/mvasmer/src/clifford-errors/alist/cubic_L=" + std::to_string(L) + ".alist";
    char *file = new char[fileS.length() + 1]();
    for (int i = 0; i < fileS.length(); ++i) {
        file[i] = fileS[i];
    }
    file[fileS.length()] = '\0';
    mod2sparse *hzC;
    hzC = load_alist(file);
    delete[] file;
    bp_osd decoderHzC(hzC, p, maxIter, osdOrder, osdMethod);
    fileS = "/home/mvasmer/src/clifford-errors/alist/rhombic1_L=" + std::to_string(L) + ".alist";
    file = new char[fileS.length() + 1]();
    for (int i = 0; i < fileS.length(); ++i) {
        file[i] = fileS[i];
    }
    file[fileS.length()] = '\0';
    mod2sparse *hzR1;
    hzR1 = load_alist(file);
    bp_osd decoderHzR1(hzR1, p, maxIter, osdOrder, osdMethod);
    delete[] file;
    fileS = "/home/mvasmer/src/clifford-errors/alist/rhombic2_L=" + std::to_string(L) + ".alist";
    file = new char[fileS.length() + 1]();
    for (int i = 0; i < fileS.length(); ++i) {
        file[i] = fileS[i];
    }
    file[fileS.length()] = '\0';
    mod2sparse *hzR2;
    hzR2 = load_alist(file);
    bp_osd decoderHzR2(hzR2, p, maxIter, osdOrder, osdMethod);
    delete[] file;

    vint cFailures = {0,0,0};
    vint r1Failures = {0,0,0};
    vint r2Failures = {0,0,0};

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    for (int i = 0; i < runs; i++)
    {
        //reset error info
        latCubic.wipe();
        latRhombic1.wipe();
        latRhombic2.wipe();

        if (vis == 1) 
        {
            out.prepErrorFiles();
            out.writeErrorInfo(lattices);
        }
        
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

        //Fix X errors 
        cubic::xErrorDecoder(decoderHzC, hzC, latCubic);
        rhombic::r1::xErrorDecoder(decoderHzR1, hzR1, latRhombic1);
        rhombic::r2::xErrorDecoder(decoderHzR2, hzR2, latRhombic2);
        
        if (vis == 1) out.writeErrorInfo(lattices);

        if (debug == 1)
        {
            try {latCubic.checkInBounds();}
            catch (std::runtime_error& e)
            {
                std::cout << "Cubic error after 3D X decoding: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
            try {latRhombic1.checkInBounds();}
            catch (std::runtime_error& e)
            {
                std::cout << "Rhombic1 error after 3D X decoding: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
            try {latRhombic2.checkInBounds();}
            catch (std::runtime_error& e)
            {
                std::cout << "Rhombic2 error after 3D X decoding: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
        }
   
        //Need to check if there should be a logical X or not
        vint expectXLogical = {0,0,0};
        latCubic.calcSynd('z',1,1);
        latRhombic1.calcSynd('z',1,1);
        latRhombic2.calcSynd('z',1,1); 
        for (int j = 0; j < 3; j++)
        {
            //First check if in codespace or not
            int inCodespace = 1;
            try {lattices[j].checkInCodespace('x',1,1);}
            catch (std::runtime_error) {inCodespace = 0;}
            //If in codespace can easily check presence/absence of logical
            if (inCodespace == 1) 
            {
                expectXLogical[j] = lattices[j].checkLogicalError('x');
            }
            //Otherwise check commutation with all disjoint logical Z reps and take majority vote
            else
            {
                expectXLogical[j] = lattices[j].checkAllZReps();
            }
        }
        //If two codes should have a logical X then the third one should have a logical Z
        //if we have link turned on, so check for this also
        vint expectZLogical = {0,0,0};
        if (ccz == 1)
        {
            if (expectXLogical[1] == 1 && expectXLogical[2] == 1) expectZLogical[0] = 1;
            if (expectXLogical[0] == 1 && expectXLogical[2] == 1) expectZLogical[1] = 1;
            if (expectXLogical[0] == 1 && expectXLogical[1] == 1) expectZLogical[2] = 1;
        }
   
        //Apply CCZ --> Clifford errors + a post-gate depolarising error
        //Although in practise only Z errors matter after this point
        //so equivalently could do a biased error with prob 2*p/3
        if (ccz == 1) applyCCZ(lattices, overlappingFaces);

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
        latCubic.calcSynd('x', 0, 1);
        latRhombic1.calcSynd('x', 0, 1);
        latRhombic2.calcSynd('x', 0, 1);
        
        if (vis == 1) out.writeErrorInfo(lattices);

        cubic::zErrorDecoder(latCubic, 1);
        rhombic::r1::zErrorDecoder(latRhombic1, 0, 1);
        rhombic::r2::zErrorDecoder(latRhombic2, 0, 1);
        
        if (vis == 1) out.writeErrorInfo(lattices);

        if (debug == 1)
        {
            try {latCubic.checkInBounds();}
            catch (std::runtime_error& e)
            {
                std::cout << "Cubic error after 3D Z decoding: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
            try {latRhombic1.checkInBounds();}
            catch (std::runtime_error& e)
            {
                std::cout << "Rhombic1 error after 3D Z decoding: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
            try {latRhombic2.checkInBounds();}
            catch (std::runtime_error& e)
            {
                std::cout << "Rhombic2 error after 3D Z decoding: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
        }

        //Find dimension jump corrections for 2D codes 
        cubic::jumpCorrection(latCubic);
        rhombic::jumpCorrection(latRhombic1, engine, dist, 1);
        rhombic::jumpCorrection(latRhombic2, engine, dist, 2);
        
        if (vis == 1) out.writeErrorInfo(lattices);

        if (debug == 1)
        {
            try
            {
                latCubic.checkInBounds();
                latCubic.checkJumpCorrection();
            }
            catch (std::runtime_error& e)
            {
                std::cout << "Cubic error after jump: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
            try 
            {
                latRhombic1.checkInBounds();
                latRhombic1.checkJumpCorrection();
            }
            catch (std::runtime_error& e)
            {
                std::cout << "Rhombic1 error after jump: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
            try 
            {
                latRhombic2.checkInBounds();
                latRhombic2.checkJumpCorrection();
            }
            catch (std::runtime_error& e)
            {
                std::cout << "Rhombic2 error after jump: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
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
            try
            {
                latCubic.checkInBounds();
                cubic::checkIn2DCodespace(latCubic);
            }
            catch (std::runtime_error& e)
            {
                std::cout << "Cubic error after perfect X decoding: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
            try
            {
                latRhombic1.checkInBounds();
                rhombic::checkIn2DCodespace(latRhombic1);
            }
            catch (std::runtime_error& e)
            {
                std::cout << "Rhombic1 error after perfect X decoding: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
            try
            {
                latRhombic2.checkInBounds();
                rhombic::checkIn2DCodespace(latRhombic2);
            }
            catch (std::runtime_error& e)
            {
                std::cout << "Rhombic2 error after perfect X decoding: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
        }
        
        //Check X logical errors
        int cXFail = 0;
        int r1XFail = 0;
        int r2XFail = 0;
        if (latCubic.checkLogicalError('x') != expectXLogical[0])
        {
            cXFail = 1;
        }
        if (latRhombic1.checkLogicalError('x') != expectXLogical[1])
        {
            r1XFail = 1;
        }
        if (latRhombic2.checkLogicalError('x') != expectXLogical[2])
        {
            r2XFail = 1;
        }

        //Can use the same decoder as for 3D here but restricted to outer qubits
        latCubic.calcSynd('x', 1, 0); 
        cubic::zErrorDecoder(latCubic, 0);
        latRhombic1.calcSynd('x', 1, 0);
        rhombic::r1::zErrorDecoder(latRhombic1, 1, 0);
        latRhombic2.calcSynd('x', 1, 0);
        rhombic::r2::zErrorDecoder(latRhombic2, 1, 0);

        if (vis == 1) out.writeErrorInfo(lattices);

        if (debug == 1)
        {
            try
            {
                latCubic.checkInBounds();
                latCubic.checkInCodespace('z', 1, 0);
            }
            catch (std::runtime_error& e)
            {
                std::cout << "Cubic error after perfect Z decoding: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
            try 
            {
                latRhombic1.checkInBounds();
                latRhombic1.checkInCodespace('z', 1, 0);
            }
            catch (std::runtime_error& e)
            {
                std::cout << "Rhombic1 error after perfect Z decoding: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
            try
            {
                latRhombic2.checkInBounds();
                latRhombic2.checkInCodespace('z', 1, 0);
            }
            catch (std::runtime_error& e)
            {
                std::cout << "Rhombic2 error after perfect Z decoding: " << e.what() << '\n';
                std::cout << "Exiting\n";
                return 1;
            }
        }
        
        //Check Z logical errors 
        int cZFail = 0;
        int r1ZFail = 0;
        int r2ZFail = 0;
        if (latCubic.checkLogicalError('z') != expectZLogical[0])
        {
            cZFail = 1;
        }
        if (latRhombic1.checkLogicalError('z') != expectZLogical[1])
        {
            r1ZFail = 1;
        }
        if (latRhombic2.checkLogicalError('z') != expectZLogical[2])
        {
            r2ZFail = 1;
        }

        //Update total counts
        if (cXFail == 1) cFailures[0] += 1;
        if (cZFail == 1) cFailures[1] += 1;
        if (cXFail == 1 || cZFail == 1) cFailures[2] += 1;
        
        if (r1XFail == 1) r1Failures[0] += 1;
        if (r1ZFail == 1) r1Failures[1] += 1;
        if (r1XFail == 1 || r1ZFail == 1) r1Failures[2] += 1;
        
        if (r2XFail == 1) r2Failures[0] += 1;
        if (r2ZFail == 1) r2Failures[1] += 1;
        if (r2XFail == 1 || r2ZFail == 1) r2Failures[2] += 1;
    }
    
    // BP-OSD cleanup
    mod2sparse_free(hzC);
    mod2sparse_free(hzR1);
    mod2sparse_free(hzR2);
    free(hzC);
    free(hzR1);
    free(hzR2);

    
    std::cout << L << ',' << p << ',' << q << ',' << runs << ',' << ccz << '\n';
    std::cout << cFailures[0] << ',' << r1Failures[0] << ',' << r2Failures[0] << '\n';
    std::cout << cFailures[1] << ',' << r1Failures[1] << ',' << r2Failures[1] << '\n';
    std::cout << cFailures[2] << ',' << r1Failures[2] << ',' << r2Failures[2] << '\n';
        
    //saveCodes();

    return 0;
}


