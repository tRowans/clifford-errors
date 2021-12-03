#include "gtest/gtest.h"
#include "decoderRhombic1.h"
#include <iostream>

//------------------------------------------------------------

//This is just a hack to make the testing software build the index vectors
//Means they don't have to be build individually in every test

TEST(indexBuilding, build)
{
    rhombic::r1::buildLattice(latRhombic1, 6);
    ASSERT_TRUE(true);
}

//------------------------------------------------------------

TEST(distanceToClosestXBoundaryTest, CorrectOutput)
{
    vint distInfo1 = {0, -1, 1};
    vint distInfo2 = {0, 1, 1};
    vint distInfo3 = {2, -1, 1}
    vint distInfo4 = {2, 1, 1};
    
    EXPECT_EQ(rhombic::r1::distanceToClosestXBoundary(258, 6), distInfo1);
    EXPECT_EQ(rhombic::r1::distanceToClosestXBoundary(253, 6), distInfo2);
    EXPECT_EQ(rhombic::r1::distanceToClosestXBoundary(49, 6), distInfo3);
    EXPECT_EQ(rhombic::r1::distanceToClosestXBoundary(79, 6), distInfo4);
}

//------------------------------------------------------------

TEST(shortestPathToXBoundaryTest, CorrectOutput)
{
    //These might be wrong because I'm not sure which path it will choose
    vint path1 = {171};
    vint path2 = {1012};
    vint path3 = {894};
    vint path4 = {317};
    EXPECT_EQ(rhombic::r1::shortestPathToXB(258, 6), path1);
    EXPECT_EQ(rhombic::r1::shortestPathToXB(253, 6), path2);
    EXPECT_EQ(rhombic::r1::shortestPathToXB(49, 6), path3);
    EXPECT_EQ(rhombic::r1::shortestPathToXB(79, 6), path4);
}

//------------------------------------------------------------

TEST(distanceToClosestZBoundaryTest, CorrectOutput)
{
    EXPECT_EQ(rhombic::r1::distanceToClosestZBoundary(43, 6), 1);
    EXPECT_EQ(rhombic::r1::distanceToClosestZBoundary(85, 6), 1);
    EXPECT_EQ(rhombic::r1::distanceToClosestZBoundary(6, 6), 1);
    EXPECT_EQ(rhombic::r1::distanceToClosestZBoundary(120, 6), 1);
    EXPECT_EQ(rhombic::r1::distanceToClosestZBoundary(115, 6), 1);
}

//------------------------------------------------------------

TEST(shortestPathToZBoundaryTest, CorrectOutput)
{
    EXPECT_EQ(rhombic::r1::shortestPathToZBoundary(43, latRhombic1.cellToFaces, 6), 108);
    EXPECT_EQ(rhombic::r1::shortestPathToZBoundary(85, latRhombic1.cellToFaces, 6), 363);
    EXPECT_EQ(rhombic::r1::shortestPathToZBoundary(6, latRhombic1.cellToFaces, 6), 0);
    EXPECT_EQ(rhombic::r1::shortestPathToZBoundary(120, latRhombic1.cellToFaces, 6), 252);
    EXPECT_EQ(rhombic::r1::shortestPathToZBoundary(115, latRhombic1.cellToFaces, 6), 327);
}

//------------------------------------------------------------

TEST(mwpmTest, PrimalLattice)
{
    vint defects;
    vpint expectedMatching;

    defects = {79, 258};
    expectedMatching = {{79, 258}};
    EXPECT_EQ(rhombic::r1::mwpm(defects, 6, 0), expectedMatching);

    defects = {49, 79, 252, 265};
    expectedMatching = {{79, 258}, {49, 265}};
    EXPECT_EQ(rhombic::r1::mwpm(defects, 6, 0), expectedMatching);

    defects = {49, 252};
    expectedMatching = {{49, -1}, {252, -1}};
    EXPECT_EQ(rhombic::r1::mwpm(defects, 6, 0), expectedMatching);
}
TEST(mwpmTest, DualLattice)
{
    vint violatedStabs;
    vpint expectedMatching;

    violatedStabs = {43, 48};
    expectedMatching = {{43, 48}};
    EXPECT_EQ(rhombic::r1::mwpm(violatedStabs, 6, 1), expectedMatching);

    violatedStabs = {8, 43, 48, 50};
    expectedMatching = {{43, 48}, {8, 50}};
    EXPECT_EQ(rhombic::r1::mwpm(violatedStabs, 6, 1), expectedMatching);

    violatedStabs = {6};
    expectedMatching = {{6, -1}};
    EXPECT_EQ(rhombic::r1::mwpm(violatedStabs, 6, 1), expectedMatching);
}

//------------------------------------------------------------

TEST(joinPairTest, CorrectOutput)
{
    vint blankSyndrome(8*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.syndrome[1033] = 1;
    rhombic::r1::joinPair(258, 79, latRhombic1, 6);
    EXPECT_EQ(syndrome, blankSyndrome);

    latRhombic1.wipe();
    latRhombic.syndrome[171] = 1;
    rhombic::r1::joinPair(258, -1, latRhombic1, 6);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(joinDualPairTest, CorrectOutputAll)
{
    vint qubitsExpected(3*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.qubits[128] = 1;
    rhombic::r1::joinDualPair(43, 48, latRhombic1, 6, 1, 1);
    EXPECT_EQ(qubits, qubitsExpected);

    latRhombic1.wipe();
    latRhombic1.qubits[0] = 1;
    rhombic::r1::joinDualPair(6, -1, latRhombic1, 6, 1, 1);
    EXPECT_EQ(qubits, qubitsExpected);
}
TEST(joinDualPairTest, CorrectOutputOuterOnly)
{
    vint qubitsExpected(3*6*6*6, 0);

    latRhombic1.qubits[126] = 1;
    rhombic::r1::joinDualPair(48, 78, latRhombic1, 6, 1, 0);
    EXPECT_EQ(qubits, qubitsExpected);

    latRhombic1.qubits[0] = 1;
    rhombic::r1::joinDualPair(6, -1, latRhombic1, 6, 1, 0);
    EXPECT_EQ(qubits, qubitsExpected);
}
TEST(joinDualPairTest, CorrectOutputInnerOnly)
{
    vint qubitsExpected(3*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.qubits[128] = 1;
    rhombic::r1::joinDualPair(43, 48, latRhombic1, 6, 0, 1);
    EXPECT_EQ(qubits, qubitsExpected);

    latRhombic1.wipe();
    latRhombic1.qubits[108] = 1;
    rhombic::r1::joinDualPair(43, -1, latRhombic1, 6, 0, 1);
    EXPECT_EQ(qubits, qubitsExpected);
}

//------------------------------------------------------------

TEST(zErrorDecoderTest, matchPair)
{
    vint qubitsExpected(3*6*6*6, 0);
    
    latRhombic1.wipe();
    latRhombic1.qubits[128] = 1;
    latRhombic1.syndromeX[43] = 1;
    latRhombic1.syndromeX[48] = 1;
    
    rhombic::r1::zErrorDecoder(latRhombic1, 6, 1, 1);
    EXPECT_EQ(qubits, qubitsExpected);
}
TEST(zErrorDecoderTest, matchTwoPairs)
{
    vint qubitsExpected(3*6*6*6, 0);
    
    latRhombic1.wipe();
    latRhombic1.qubits[128] = 1;
    latRhombic1.syndromeX[43] = 1;
    latRhombic1.syndromeX[48] = 1;
    latRhombic1.qubits[135] = 1;
    latRhombic1.syndromeX[8] = 1;
    latRhombic1.syndromeX[50] = 1;

    rhombic::r1::zErrorDecoder(latRhombic1, 6, 1, 1);
    EXPECT_EQ(qubits, qubitsExpected);
}
TEST(zErrorDecoderTest, matchToBoundary)
{
    vint qubitsExpected(3*6*6*6, 0);
    
    latRhombic1.wipe();
    latRhombic1.qubits[0] = 1;
    latRhombic1.syndromeX[6] = 1;

    rhombic::r1::zErrorDecoder(latRhombic1, 6, 1, 1);
    EXPECT_EQ(qubits, qubitsExpected);
}

//------------------------------------------------------------

TEST(measErrorDecoderTest, matchPair)
{
    vint syndromeExpected(8*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.syndromeZ[1033] = 1;
    latRhombic1.defects = {258, 79};
    rhombic::r1::measErrorDecoder(latRhombic1, 6);
    EXPECT_EQ(latRhombic1.syndromeZ, syndromeExpected);
}
TEST(measErrorDecoderTest, matchTwoPairs)
{
    vint syndromeExpected(8*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic1.syndromeZ[1011] = 1;
    latRhombic1.syndromeZ[199] = 1;
    latRhombic1.defects = {49, 79, 252, 265};
    rhombic::r1::measErrorDecoder(latRhombic1, 6);
    EXPECT_EQ(latRhombic1.syndromeZ, syndromeExpected);
}
TEST(measErrorDecoderTest, matchToBoundary)
{
    vint syndromeExpected(8*6*6*6, 0);

    latRhombic1.wipe();
    latRhombic.syndrome[171] = 1;
    rhombic::r1::measErrorDecoder(latRhombic1, 6);
    EXPECT_EQ(latRhombic1.syndromeZ, syndromeExpected);
}
