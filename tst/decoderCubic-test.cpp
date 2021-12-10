#include "gtest/gtest.h"
#include "decoderCubic.h"
#include "prepCubic.h"

//------------------------------------------------------------
//This is just a hack to make the testing software build the index vectors
//Means they don't have to be build individually in every test

TEST(indexBuilding, build)
{
    cubic::buildLattice(latCubic);
    ASSERT_TRUE(true);
}

//------------------------------------------------------------

TEST(taxiTest, CorrectOutput)
{
    vint dists1 = {-1, -1, 2};
    vint dists2 = {1, 1, -2};
    EXPECT_EQ(cubic::taxi(93, 158, 6), dists1);
    EXPECT_EQ(cubic::taxi(158, 93, 6), dists2);

    dists1 = {1, -1, 0};
    dists2 = {-1, 1, 0};
    EXPECT_EQ(cubic::taxi(93, 88, 6), dists1);
    EXPECT_EQ(cubic::taxi(88, 93, 6), dists2);
}
TEST(taxiTest, sameVertex)
{
    vint dists = {0, 0, 0};
    EXPECT_EQ(cubic::taxi(58, 58, 6), dists);
}

//------------------------------------------------------------

TEST(shortestPathTest, OneAxis)
{
    vint path = cubic::shortestPath(0, 2, 6);
    vint pathExpected = {0, 3};
    EXPECT_EQ(path, pathExpected);
}
TEST(shortestPathTest, MultipleAxes)
{
    vint path = cubic::shortestPath(0, 43, 6);
    vint pathExpected = {0, 4, 23};
    EXPECT_EQ(path, pathExpected);
}
TEST(shortestPathTest, sameVertex)
{
    vint path = cubic::shortestPath(58, 58, 6);
    vint pathExpected = {};
    EXPECT_EQ(path, pathExpected);
}

//------------------------------------------------------------

TEST(shortestDualPathTest, length1)
{
    vint path = cubic::shortestDualPath(0, 1, 6);
    vint pathExpected = {5};
    EXPECT_EQ(path, pathExpected);
}
TEST(shortestDualPathTest, length2)
{
    vint path = cubic::shortestDualPath(0, 43, 6); 
    vint pathExpected = {5, 22, 129};
    EXPECT_EQ(path, pathExpected);
}

//------------------------------------------------------------ 

TEST(distanceToClosestXBoundaryTest, AllDirections)
{
    vint distInfoExpected1 = {2, -1, 1};
    vint distInfoExpected2 = {2, 1, 1};
    
    EXPECT_EQ(cubic::distanceToClosestXBoundary(43, 6, 0), distInfoExpected1);
    EXPECT_EQ(cubic::distanceToClosestXBoundary(79, 6, 0), distInfoExpected2);
}
TEST(distanceToClosestXBoundaryTest, yOnly)
{
    vint distInfoExpected1 = {1, -1, 1};
    vint distInfoExpected2 = {1, 1, 1};

    EXPECT_EQ(cubic::distanceToClosestXBoundary(42, 6, 1), distInfoExpected1);
    EXPECT_EQ(cubic::distanceToClosestXBoundary(48, 6, 1), distInfoExpected2);
    EXPECT_EQ(cubic::distanceToClosestXBoundary(6, 6, 1), distInfoExpected1);
}

//------------------------------------------------------------

TEST(shortestPathToXBoundaryTest, AllDirections)
{
    vint pathExpected1 = {23};
    vint pathExpected2 = {239};
    EXPECT_EQ(cubic::shortestPathToXBoundary(43, 6, 0), pathExpected1);
    EXPECT_EQ(cubic::shortestPathToXBoundary(79, 6, 0), pathExpected2);
}
TEST(shortestPathToXBoundary, yOnly)
{
    vint pathExpected1 = {109};
    vint pathExpected2 = {145};
    vint pathExpected3 = {1};
    EXPECT_EQ(cubic::shortestPathToXBoundary(42, 6, 1), pathExpected1);
    EXPECT_EQ(cubic::shortestPathToXBoundary(48, 6, 1), pathExpected2);
    EXPECT_EQ(cubic::shortestPathToXBoundary(6, 6, 1), pathExpected3);
}

//------------------------------------------------------------

TEST(distanceToClosestZBoundary, correctOutput)
{
    vint distanceExpected1 = {-1, 1};
    vint distanceExpected2 = {1, 1};
    EXPECT_EQ(cubic::distanceToClosestZBoundary(0, 6), distanceExpected1);
    EXPECT_EQ(cubic::distanceToClosestZBoundary(1, 6), distanceExpected2);
}

//------------------------------------------------------------

TEST(shortestPathToZBoundaryTest, correctOutput)
{
    vint pathExpected1 = {2};
    vint pathExpected2 = {8};
    EXPECT_EQ(cubic::shortestPathToZBoundary(0, 6), pathExpected1);
    EXPECT_EQ(cubic::shortestPathToZBoundary(1, 6), pathExpected2);
}

//------------------------------------------------------------

TEST(mwpmTest, matchingToDefects)
{
    vint defects = {42, 48};
    vpint defectPairsExpected = {{42, 48}};
    EXPECT_EQ(cubic::mwpm(defects, 6, 0, 0), defectPairsExpected);
}
TEST(mwpmTest, matchingToBoundaries)
{
    vint defects = {42, 86};
    vpint defectPairsExpected = {{42, -1}, {86, -1}};
    EXPECT_EQ(cubic::mwpm(defects, 6, 0, 0), defectPairsExpected);
}
TEST(mwpmTest, dualMatching)
{
    vint cells = {0, 6};
    vpint cellPairsExpected = {{0, 6}};
    EXPECT_EQ(cubic::mwpm(cells, 6, 1, 0), cellPairsExpected);
}
TEST(mwpmTest, dualMatchingToBoundaries)
{
    vint cells = {0, 73};
    vpint cellPairsExpected = {{0, -1}, {73, -1}};
    EXPECT_EQ(cubic::mwpm(cells, 6, 1, 0), cellPairsExpected);
}

//------------------------------------------------------------

TEST(joinPairTest, joinToDefect)
{
    vint syndromeExpected(8*6*6*6, 0);
    latCubic.syndromeZ[127] = 1;
    cubic::joinPair(42, 48, latCubic);
    EXPECT_EQ(latCubic.syndromeZ, syndromeExpected);
}
TEST(joinPairTest, joinToBoundary)
{
    vint syndromeExpected(8*6*6*6, 0);
    latCubic.syndromeZ[20] = 1;
    cubic::joinPair(42, -1, latCubic);
    EXPECT_EQ(latCubic.syndromeZ, syndromeExpected);
}

//------------------------------------------------------------

TEST(joinDualPairTest, joinInBulk)
{
    vint qubitsExpected(3*6*6*6, 0);
    latCubic.qubitsZ[19] = 1;
    cubic::joinDualPair(0, 6, latCubic);
    EXPECT_EQ(latCubic.qubitsZ, qubitsExpected);
}
TEST(joinDualPairTest, joinToBoundary)
{
    vint qubitsExpected(3*6*6*6, 0);
    latCubic.qubitsZ[2] = 1;
    cubic::joinDualPair(0, -1, latCubic);
    EXPECT_EQ(latCubic.qubitsZ, qubitsExpected);
}

//------------------------------------------------------------

TEST(zErrorDecoderTest, matchPair)
{
    latCubic.wipe();
    latCubic.qubitsZ[19] = 1; 
    latCubic.syndromeX[0] = 1;
    latCubic.syndromeX[6] = 1;

    cubic::zErrorDecoder(latCubic);
    vint qubitsExpected(3*6*6*6, 0);
    EXPECT_EQ(latCubic.qubitsZ, qubitsExpected);
}
TEST(zErrorDecoderTest, matchTwoPairs)
{  
    latCubic.wipe();
    latCubic.qubitsZ[19] = 1;
    latCubic.syndromeX[0] = 1;
    latCubic.syndromeX[6] = 1;
    latCubic.qubitsZ[256] = 1;
    latCubic.syndromeX[79] = 1;
    latCubic.syndromeX[85] = 1;

    cubic::zErrorDecoder(latCubic);
    vint qubitsExpected(3*6*6*6, 0);
    EXPECT_EQ(latCubic.qubitsZ, qubitsExpected);
}
TEST(zErrorDecoderTest, matchToBoundary)
{
    latCubic.wipe();
    latCubic.qubitsZ[2] = 1;
    latCubic.syndromeX[0] = 1;

    cubic::zErrorDecoder(latCubic);
    vint qubitsExpected(3*6*6*6, 0);
    EXPECT_EQ(latCubic.qubitsZ, qubitsExpected);
}

//------------------------------------------------------------

TEST(xErrorDecoder2DTest, matchPair)
{
    latCubic.wipe();
    latCubic.qubitsX[20] = 1;
    vint syndromeVertices = {6, 12, 42, 48};

    cubic::xErrorDecoder2D(latCubic, syndromeVertices);
    vint qubitsExpected(3*6*6*6, 0);
    EXPECT_EQ(latCubic.qubitsX, qubitsExpected);
}
TEST(xErrorDecoder2DTest, matchTwoPairs)
{
    latCubic.wipe();
    latCubic.qubitsX[20] = 1;
    latCubic.qubitsX[26] = 1;
    vint syndromeVertices = {6, 12, 42, 48, 8, 14, 44, 50};

    cubic::xErrorDecoder2D(latCubic, syndromeVertices);
    vint qubitsExpected(3*6*6*6, 0);
    EXPECT_EQ(latCubic.qubitsX, qubitsExpected);
}
TEST(xErrorDecoder2DTest, matchToBoundary)
{
    latCubic.wipe();
    latCubic.qubitsX[2] = 1;
    vint syndromeVertices = {6, 36};

    cubic::xErrorDecoder2D(latCubic, syndromeVertices);
    vint qubitsExpected(3*6*6*6, 0);
    EXPECT_EQ(latCubic.qubitsX, qubitsExpected);
}

//------------------------------------------------------------

TEST(measErrorDecoderTest, matchPair)
{
    latCubic.wipe();
    latCubic.syndromeZ[127] = 1;
    latCubic.defects = {42, 48};

    cubic::measErrorDecoder(latCubic);
    vint syndromeExpected(8*6*6*6, 0);
    EXPECT_EQ(latCubic.syndromeZ, syndromeExpected);
}
TEST(measErrorDecoderTest, matchTwoPairs)
{
    latCubic.wipe();
    latCubic.syndromeZ[127] = 1;
    latCubic.syndromeZ[241] = 1;
    latCubic.defects = {42, 48, 80, 86};

    cubic::measErrorDecoder(latCubic);
    vint syndromeExpected(8*6*6*6, 0);
    EXPECT_EQ(latCubic.syndromeZ, syndromeExpected);
}
TEST(measErrorDecoderTest, matchToBoundary)
{
    latCubic.wipe();
    latCubic.syndromeZ[20] = 1;
    latCubic.defects = {42};

    cubic::measErrorDecoder(latCubic);
    vint syndromeExpected(8*6*6*6, 0);
    EXPECT_EQ(latCubic.syndromeZ, syndromeExpected);
}

//------------------------------------------------------------
//X ERROR DECODER TEST HERE?
//------------------------------------------------------------

TEST(jumpCorrectionTest, CorrectOutputExample)
{
    //Tests a specific example
    latCubic.wipe();
    latCubic.qubitsZ[110] = 1;
    latCubic.qubitsZ[216] = 1;
    latCubic.qubitsZ[218] = 1;

    cubic::jumpCorrection(latCubic);
    vint qubitsExpected(3*6*6*6, 0);
    EXPECT_EQ(latCubic.qubitsZ, qubitsExpected);
}
TEST(jumpCorrectionTest, CorrectOutput)
{
    latCubic.wipe();

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    latCubic.zStabPattern(engine, dist);
    cubic::jumpCorrection(latCubic);
    //These all tested separately in lattice-test.cpp
    EXPECT_NO_THROW(latCubic.checkJumpCorrection());
    EXPECT_NO_THROW(latCubic.checkInCodespace('z',1,1));
    EXPECT_EQ(latCubic.checkLogicalError('z'), 0);
}
