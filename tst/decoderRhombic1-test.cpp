#include "gtest/gtest.h"
#include "decoderRhombic1.h"
#include "prepRhombic1.h"

//------------------------------------------------------------

//This is just a hack to make the testing software build the index vectors
//Means they don't have to be build individually in every test

TEST(indexBuildingR1, build)
{
    rhombic::r1::buildLattice(latRhombic1);
    ASSERT_TRUE(true);
}

//------------------------------------------------------------

TEST(w1ToW0TestR1, CorrectOutput)
{
    rhombic::coord cIn = {0,0,0,1};
    rhombic::coord cOut = rhombic::r1::w1ToW0(cIn, 6);
    EXPECT_EQ(cOut.xi[0], 0);
    EXPECT_EQ(cOut.xi[1], 0);
    EXPECT_EQ(cOut.xi[2], 0);
    EXPECT_EQ(cOut.xi[3], 0);

    cIn.xi[0] = 1;
    cOut = rhombic::r1::w1ToW0(cIn,6);
    EXPECT_EQ(cOut.xi[0], 2);
    EXPECT_EQ(cOut.xi[1], 1);
    EXPECT_EQ(cOut.xi[2], 1);
    EXPECT_EQ(cOut.xi[3], 0);
}

//------------------------------------------------------------

TEST(shortestPathLengthTestR1, threeD)
{
    EXPECT_EQ(rhombic::r1::shortestPathLength(0, 216, 6, 0), 1);
    EXPECT_EQ(rhombic::r1::shortestPathLength(0, 36, 6, 0), 2);
    EXPECT_EQ(rhombic::r1::shortestPathLength(0, 217, 6, 0), 3);
    EXPECT_EQ(rhombic::r1::shortestPathLength(79, 265, 6, 0), 3);
    EXPECT_EQ(rhombic::r1::shortestPathLength(79, 252, 6, 0), 1);
    EXPECT_EQ(rhombic::r1::shortestPathLength(216, 252, 6, 0), 2);
}
TEST(shortestPathLengthTestR1, twoD)
{
    EXPECT_EQ(rhombic::r1::shortestPathLength(42, 84, 6, 1), 1);
    EXPECT_EQ(rhombic::r1::shortestPathLength(42, 54, 6, 1), 2);
}

//------------------------------------------------------------

TEST(distanceToClosestXBoundaryTestR1, threeD)
{
    vint distInfo1 = {0, -1, 1};
    vint distInfo2 = {0, 1, 1};
    vint distInfo3 = {2, -1, 1};
    vint distInfo4 = {2, 1, 1};
    
    EXPECT_EQ(rhombic::r1::distanceToClosestXBoundary(258, 6, 0), distInfo1);
    EXPECT_EQ(rhombic::r1::distanceToClosestXBoundary(253, 6, 0), distInfo2);
    EXPECT_EQ(rhombic::r1::distanceToClosestXBoundary(49, 6, 0), distInfo3);
    EXPECT_EQ(rhombic::r1::distanceToClosestXBoundary(79, 6, 0), distInfo4);
}
TEST(distanceToClosestXBoundaryTestR1, twoD)
{
    vint distInfo1 = {2, -1, 1};
    vint distInfo2 = {2, 1, 1};

    EXPECT_EQ(rhombic::r1::distanceToClosestXBoundary(42, 6, 1), distInfo1);
    EXPECT_EQ(rhombic::r1::distanceToClosestXBoundary(84, 6, 1), distInfo2);
    EXPECT_EQ(rhombic::r1::distanceToClosestXBoundary(54, 6, 1), distInfo1);
    EXPECT_EQ(rhombic::r1::distanceToClosestXBoundary(72, 6, 1), distInfo2);
}

//------------------------------------------------------------

TEST(shortestPathToXBoundaryTestR1, threeD)
{
    vint path1 = {171};
    vint path2 = {1012};
    vint path3 = {894};
    vint path4 = {317};
    EXPECT_EQ(rhombic::r1::shortestPathToXBoundary(258, 6, 0), path1);
    EXPECT_EQ(rhombic::r1::shortestPathToXBoundary(253, 6, 0), path2);
    EXPECT_EQ(rhombic::r1::shortestPathToXBoundary(49, 6, 0), path3);
    EXPECT_EQ(rhombic::r1::shortestPathToXBoundary(79, 6, 0), path4);
}
TEST(shortestPathToXBoundaryTestR1, twoD)
{
    vint path1 = {866};
    vint path2 = {337};
    EXPECT_EQ(rhombic::r1::shortestPathToXBoundary(42, 6, 1), path1);
    EXPECT_EQ(rhombic::r1::shortestPathToXBoundary(84, 6, 1), path2);
} 

//------------------------------------------------------------

TEST(distanceToClosestZBoundaryTestR1, CorrectOutput)
{
    vint distInfoExpected1 = {-1,1};
    vint distInfoExpected2 = {1,1};
    EXPECT_EQ(rhombic::r1::distanceToClosestZBoundary(43, 6), distInfoExpected1);
    EXPECT_EQ(rhombic::r1::distanceToClosestZBoundary(85, 6), distInfoExpected2);
    EXPECT_EQ(rhombic::r1::distanceToClosestZBoundary(6, 6), distInfoExpected1);
    EXPECT_EQ(rhombic::r1::distanceToClosestZBoundary(120, 6), distInfoExpected2);
    EXPECT_EQ(rhombic::r1::distanceToClosestZBoundary(115, 6), distInfoExpected1);
}

//------------------------------------------------------------

TEST(shortestPathToZBoundaryTestR1, CorrectOutput)
{
    vint path1 = {108};
    vint path2 = {363};
    vint path3 = {0};
    vint path4 = {252};
    vint path5 = {327};
    EXPECT_EQ(rhombic::r1::shortestPathToZBoundary(43, latRhombic1.cellToFaces, 6), path1);
    EXPECT_EQ(rhombic::r1::shortestPathToZBoundary(85, latRhombic1.cellToFaces, 6), path2);
    EXPECT_EQ(rhombic::r1::shortestPathToZBoundary(6, latRhombic1.cellToFaces, 6), path3);
    EXPECT_EQ(rhombic::r1::shortestPathToZBoundary(120, latRhombic1.cellToFaces, 6), path4);
    EXPECT_EQ(rhombic::r1::shortestPathToZBoundary(115, latRhombic1.cellToFaces, 6), path5);
}

//------------------------------------------------------------

TEST(mwpmTestR1, PrimalLattice)
{
    vint defects;
    vpint expectedMatching;

    defects = {79, 258};
    expectedMatching = {{79, 258}};
    EXPECT_EQ(rhombic::r1::mwpm(defects, 6, 0, 0), expectedMatching);

    defects = {49, 79, 252, 265};
    expectedMatching = {{49, 265}, {79, 252}};
    EXPECT_EQ(rhombic::r1::mwpm(defects, 6, 0, 0), expectedMatching);

    defects = {49, 252};
    expectedMatching = {{49, -1}, {252, -1}};
    EXPECT_EQ(rhombic::r1::mwpm(defects, 6, 0, 0), expectedMatching);
}
TEST(mwpmTestR1, DualLattice)
{
    vint violatedStabs;
    vpint expectedMatching;

    violatedStabs = {43, 48};
    expectedMatching = {{43, 48}};
    EXPECT_EQ(rhombic::r1::mwpm(violatedStabs, 6, 1, 0), expectedMatching);

    violatedStabs = {8, 43, 48, 50};
    expectedMatching = {{8, 50}, {43, 48}};
    EXPECT_EQ(rhombic::r1::mwpm(violatedStabs, 6, 1, 0), expectedMatching);

    violatedStabs = {6};
    expectedMatching = {{6, -1}};
    EXPECT_EQ(rhombic::r1::mwpm(violatedStabs, 6, 1, 0), expectedMatching);
}

//------------------------------------------------------------

TEST(joinPairTestR1, CorrectOutput)
{
    vint blankSyndrome(8*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.syndromeZ[1033] = 1;
    rhombic::r1::joinPair(258, 79, latRhombic1);
    EXPECT_EQ(latRhombic1.syndromeZ, blankSyndrome);

    latRhombic1.wipe();
    latRhombic1.syndromeZ[171] = 1;
    rhombic::r1::joinPair(258, -1, latRhombic1);
    EXPECT_EQ(latRhombic1.syndromeZ, blankSyndrome);
}

//------------------------------------------------------------

TEST(joinDualPairTestR1, CorrectOutputAll)
{
    vint qubitsExpected(3*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.qubitsZ[128] = 1;
    rhombic::r1::joinDualPair(43, 48, latRhombic1, 0, 0);
    EXPECT_EQ(latRhombic1.qubitsZ, qubitsExpected);

    latRhombic1.wipe();
    latRhombic1.qubitsZ[0] = 1;
    rhombic::r1::joinDualPair(6, -1, latRhombic1, 0, 0);
    EXPECT_EQ(latRhombic1.qubitsZ, qubitsExpected);
}
TEST(joinDualPairTestR1, CorrectOutputOuterOnly)
{
    vint qubitsExpected(3*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.qubitsZ[126] = 1;
    rhombic::r1::joinDualPair(48, 78, latRhombic1, 1, 0);
    EXPECT_EQ(latRhombic1.qubitsZ, qubitsExpected);

    latRhombic1.wipe();
    latRhombic1.qubitsZ[0] = 1;
    rhombic::r1::joinDualPair(6, -1, latRhombic1, 1, 0);
    EXPECT_EQ(latRhombic1.qubitsZ, qubitsExpected);
}
TEST(joinDualPairTestR1, CorrectOutputInnerOnly)
{
    vint qubitsExpected(3*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.qubitsZ[128] = 1;
    rhombic::r1::joinDualPair(43, 48, latRhombic1, 0, 1);
    EXPECT_EQ(latRhombic1.qubitsZ, qubitsExpected);

    latRhombic1.wipe();
    latRhombic1.qubitsZ[108] = 1;
    rhombic::r1::joinDualPair(43, -1, latRhombic1, 0, 1);
    EXPECT_EQ(latRhombic1.qubitsZ, qubitsExpected);
}

//------------------------------------------------------------

TEST(zErrorDecoderTestR1, matchPair)
{
    vint qubitsExpected(3*6*6*6, 0);
    
    latRhombic1.wipe();
    latRhombic1.qubitsZ[128] = 1;
    latRhombic1.syndromeX[43] = 1;
    latRhombic1.syndromeX[48] = 1;
    
    rhombic::r1::zErrorDecoder(latRhombic1, 0, 0);
    EXPECT_EQ(latRhombic1.qubitsZ, qubitsExpected);
}
TEST(zErrorDecoderTestR1, matchTwoPairs)
{
    vint qubitsExpected(3*6*6*6, 0);
    
    latRhombic1.wipe();
    latRhombic1.qubitsZ[128] = 1;
    latRhombic1.syndromeX[43] = 1;
    latRhombic1.syndromeX[48] = 1;
    latRhombic1.qubitsZ[135] = 1;
    latRhombic1.syndromeX[8] = 1;
    latRhombic1.syndromeX[50] = 1;

    rhombic::r1::zErrorDecoder(latRhombic1, 0, 0);
    EXPECT_EQ(latRhombic1.qubitsZ, qubitsExpected);
}
TEST(zErrorDecoderTestR1, matchToBoundary)
{
    vint qubitsExpected(3*6*6*6, 0);
    
    latRhombic1.wipe();
    latRhombic1.qubitsZ[0] = 1;
    latRhombic1.syndromeX[6] = 1;

    rhombic::r1::zErrorDecoder(latRhombic1, 0, 0);
    EXPECT_EQ(latRhombic1.qubitsZ, qubitsExpected);
}
TEST(zErrorDecoderTestR1, decode2D)
{
    vint qubitsExpected(3*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.qubitsZ[255] = 1;
    latRhombic1.qubitsZ[252] = 1;
    latRhombic1.syndromeX[48] = 1;
    latRhombic1.syndromeX[120] = 1;

    rhombic::r1::zErrorDecoder(latRhombic1, 1, 0);
    EXPECT_EQ(latRhombic1.qubitsZ, qubitsExpected);
}
TEST(zErrorDecoderTestR1, ignoreOuter)
{
    vint qubitsExpected(3*6*6*6, 0);
    qubitsExpected[126] = 1;
    qubitsExpected[127] = 1;
    qubitsExpected[128] = 1;

    latRhombic1.wipe();
    latRhombic1.qubitsZ[126] = 1;
    latRhombic1.qubitsZ[127] = 1;
    latRhombic1.qubitsZ[128] = 1;
    latRhombic1.syndromeX[48] = 1;
    latRhombic1.syndromeX[78] = 1;

    rhombic::r1::zErrorDecoder(latRhombic1, 0, 1);
    EXPECT_EQ(latRhombic1.qubitsZ, qubitsExpected);
}

//------------------------------------------------------------

TEST(xErrorDecoder2DTestR1, matchPair)
{
    vint qubitsExpected(3*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.qubitsX[126] = 1;
    latRhombic1.syndromeZ[171] = 1;
    latRhombic1.syndromeZ[1034] = 1;
    vint syndromeVertices = {42, 84, 258};

    rhombic::r1::xErrorDecoder2D(latRhombic1, syndromeVertices);
    std::cout << "non-zero qubits: ";
    for (int i = 0; i < latRhombic1.qubitsX.size(); i++)
    {
        if (latRhombic1.qubitsX[i] == 1) std::cout << i << ',';
    }
    std::cout << '\n';

    EXPECT_EQ(latRhombic1.qubitsX, qubitsExpected);
}
//Not enough space for matching two pairs in the bulk
TEST(xErrorDecoder2DTestR1, matchTwoToBoundary) 
{
    vint qubitsExpected(3*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.qubitsX[36] = 1;
    latRhombic1.qubitsX[216] = 1;
    latRhombic1.syndromeZ[914] = 1;
    latRhombic1.syndromeZ[291] = 1;
    vint syndromeVertices = {54, 72, 228, 288};

    rhombic::r1::xErrorDecoder2D(latRhombic1, syndromeVertices);
    EXPECT_EQ(latRhombic1.qubitsX, qubitsExpected);
}

//------------------------------------------------------------

TEST(measErrorDecoderTestR1, matchPair)
{
    vint syndromeExpected(8*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.syndromeZ[1033] = 1;
    latRhombic1.defects = {258, 79};
    rhombic::r1::measErrorDecoder(latRhombic1);
    EXPECT_EQ(latRhombic1.syndromeZ, syndromeExpected);
}
TEST(measErrorDecoderTestR1, matchTwoPairs)
{
    vint syndromeExpected(8*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.syndromeZ[1011] = 1;
    latRhombic1.syndromeZ[199] = 1;
    latRhombic1.defects = {49, 79, 252, 265};
    rhombic::r1::measErrorDecoder(latRhombic1);
    EXPECT_EQ(latRhombic1.syndromeZ, syndromeExpected);
}
TEST(measErrorDecoderTestR1, matchToBoundary)
{
    vint syndromeExpected(8*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.syndromeZ[171] = 1;
    latRhombic1.defects = {258};
    rhombic::r1::measErrorDecoder(latRhombic1);
    EXPECT_EQ(latRhombic1.syndromeZ, syndromeExpected);
}
