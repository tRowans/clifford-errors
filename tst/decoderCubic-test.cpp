#include "gtest/gtest.h"
#include "decoderCubic.h"
#include "prepCubic.h"

//------------------------------------------------------------
//This is just a hack to make the testing software build the index vectors
//Means they don't have to be build individually in every test

TEST(indexBuildingC, build)
{
    cubic::buildLattice(latCubic);
    ASSERT_TRUE(true);
}

//------------------------------------------------------------

TEST(taxiTestC, CorrectOutput)
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
TEST(taxiTestC, sameVertex)
{
    vint dists = {0, 0, 0};
    EXPECT_EQ(cubic::taxi(58, 58, 6), dists);
}

//------------------------------------------------------------

TEST(shortestPathTestC, OneAxis)
{
    vint path = cubic::shortestPath(0, 2, 6);
    vint pathExpected = {0, 3};
    EXPECT_EQ(path, pathExpected);
}
TEST(shortestPathTestC, MultipleAxes)
{
    vint path = cubic::shortestPath(0, 43, 6);
    vint pathExpected = {0, 4, 23};
    EXPECT_EQ(path, pathExpected);
}
TEST(shortestPathTestC, sameVertex)
{
    vint path = cubic::shortestPath(58, 58, 6);
    vint pathExpected = {};
    EXPECT_EQ(path, pathExpected);
}

//------------------------------------------------------------

TEST(shortestDualPathTestC, length1)
{
    vint path = cubic::shortestDualPath(0, 1, 6);
    vint pathExpected = {5};
    EXPECT_EQ(path, pathExpected);
}
TEST(shortestDualPathTestC, length2)
{
    vint path = cubic::shortestDualPath(0, 43, 6); 
    vint pathExpected = {5, 22, 129};
    EXPECT_EQ(path, pathExpected);
}

//------------------------------------------------------------ 

TEST(distanceToClosestXBoundaryTestC, CorrectOutput)
{
    vint distInfoExpected1 = {1, -1, 1};
    vint distInfoExpected2 = {1, 1, 1};
    
    EXPECT_EQ(cubic::distanceToClosestXBoundary(43, 6), distInfoExpected1);
    EXPECT_EQ(cubic::distanceToClosestXBoundary(49, 6), distInfoExpected2);
}

//------------------------------------------------------------

TEST(shortestPathToXBoundaryTestC, correctOutput)
{
    vint pathExpected1 = {112};
    vint pathExpected2 = {148};
    EXPECT_EQ(cubic::shortestPathToXBoundary(43, 6), pathExpected1);
    EXPECT_EQ(cubic::shortestPathToXBoundary(49, 6), pathExpected2);
}

//------------------------------------------------------------

TEST(distanceToClosestZBoundaryC, correctOutput)
{
    vint distanceExpected1 = {-1, 1};
    vint distanceExpected2 = {1, 1};
    EXPECT_EQ(cubic::distanceToClosestZBoundary(0, 6), distanceExpected1);
    EXPECT_EQ(cubic::distanceToClosestZBoundary(1, 6), distanceExpected2);
}

//------------------------------------------------------------

TEST(shortestPathToZBoundaryTestC, correctOutput)
{
    vint pathExpected1 = {2};
    vint pathExpected2 = {8};
    EXPECT_EQ(cubic::shortestPathToZBoundary(0, 6), pathExpected1);
    EXPECT_EQ(cubic::shortestPathToZBoundary(1, 6), pathExpected2);
}

//------------------------------------------------------------

TEST(mwpmTestC, matchingToDefects)
{
    vint defects = {88, 123};
    vpint defectPairsExpected = {{88, 123}};
    EXPECT_EQ(cubic::mwpm(defects, 6, 0), defectPairsExpected);
}
TEST(mwpmTestC, matchingToBoundaries)
{
    vint defects = {57, 158};
    vpint defectPairsExpected = {{57, -1}, {158, -1}};
    EXPECT_EQ(cubic::mwpm(defects, 6, 0), defectPairsExpected);
}
TEST(mwpmTestC, dualMatching)
{
    vint cells = {0, 6};
    vpint cellPairsExpected = {{0, 6}};
    EXPECT_EQ(cubic::mwpm(cells, 6, 1), cellPairsExpected);
}
TEST(mwpmTestC, dualMatchingToBoundaries)
{
    vint cells = {0, 1};
    vpint cellPairsExpected = {{0, -1}, {1, -1}};
    EXPECT_EQ(cubic::mwpm(cells, 6, 1), cellPairsExpected);
}

//------------------------------------------------------------

TEST(joinPairTestC, joinToDefect)
{
    vint syndrome(3*6*6*6, 0);
    vint syndromeExpected(3*6*6*6, 0);
    syndrome[281] = 1;
    syndrome[370] = 1;
    cubic::joinPair(93, 123, latCubic);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(joinPairTestC, joinToBoundary)
{
    vint syndrome(3*6*6*6, 0);
    vint syndromeExpected(3*6*6*6, 0);
    syndrome[280] = 1;
    cubic::joinPair(93, -1, latCubic);
    EXPECT_EQ(syndrome, syndromeExpected);
}

//------------------------------------------------------------

TEST(joinDualPairTestC, joinInBulk)
{
    vint qubits(3*6*6*6, 0);
    vint qubitsExpected(3*6*6*6, 0);
    qubits[279] = 1;
    qubits[387] = 1;
    cubic::joinDualPair(57, 129, latCubic);
    EXPECT_EQ(qubits, qubitsExpected);
}
TEST(joinDualPairTestC, joinToBoundary)
{
    vint qubits(3*6*6*6, 0);
    vint qubitsExpected(3*6*6*6, 0);
    qubits[176] = 1;
    cubic::joinDualPair(57, -1, latCubic);
    EXPECT_EQ(qubits, qubitsExpected);
}

//------------------------------------------------------------

TEST(zErrorDecoderTestC, matchPair)
{
    latCubic.wipe();
    latCubic.qubitsZ[279] = 1; 
    latCubic.syndromeX[57] = 1;
    latCubic.syndromeX[93] = 1;

    cubic::zErrorDecoder(latCubic);
    vint qubitsExpected(3*6*6*6, 0);
    EXPECT_EQ(latCubic.qubitsZ, qubitsExpected);
}
TEST(zErrorDecoderTestC, matchTwoPairs)
{  
    latCubic.wipe();
    latCubic.qubitsZ[279] = 1;
    latCubic.syndromeX[57] = 1;
    latCubic.syndromeX[93] = 1;
    latCubic.qubitsZ[461] = 1;
    latCubic.syndromeX[152] = 1;
    latCubic.syndromeX[153] = 1;

    cubic::zErrorDecoder(latCubic);
    vint qubitsExpected(3*6*6*6, 0);
    EXPECT_EQ(latCubic.qubitsZ, qubitsExpected);
}
TEST(zErrorDecoderTestC, matchToBoundary)
{
    latCubic.wipe();
    latCubic.qubitsZ[176] = 1;
    latCubic.syndromeX[57] = 1;

    cubic::zErrorDecoder(latCubic);
    vint qubitsExpected(3*6*6*6, 0);
    EXPECT_EQ(latCubic.qubitsZ, qubitsExpected);
}

//------------------------------------------------------------

TEST(measErrorDecoderTestC, matchPair)
{
    latCubic.wipe();
    latCubic.syndromeZ[127] = 1;
    latCubic.defects = {42, 48};

    cubic::measErrorDecoder(latCubic);
    vint syndromeExpected(8*6*6*6, 0);
    EXPECT_EQ(latCubic.syndromeZ, syndromeExpected);
}
TEST(measErrorDecoderTestC, matchTwoPair)
{
    latCubic.wipe();
    latCubic.syndromeZ[127] = 1;
    latCubic.syndromeZ[241] = 1;
    latCubic.defects = {42, 48, 80, 86};

    cubic::measErrorDecoder(latCubic);
    vint syndromeExpected(8*6*6*6, 0);
    EXPECT_EQ(latCubic.syndromeZ, syndromeExpected);
}
TEST(measErrorDecoderTestC, matchToBoundary)
{
    latCubic.wipe();
    latCubic.syndromeZ[109] = 1;
    latCubic.defects = {42};

    cubic::measErrorDecoder(latCubic);
    vint syndromeExpected(8*6*6*6, 0);
    EXPECT_EQ(latCubic.syndromeZ, syndromeExpected);
}

//------------------------------------------------------------
//X ERROR DECODER TEST HERE?
//------------------------------------------------------------

TEST(jumpCorrectionTestC, CorrectOutputExample)
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
TEST(jumpCorrectionTestC, CorrectOutput)
{
    latCubic.wipe();

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    latCubic.zStabPattern(engine, dist);
    cubic::jumpCorrection(latCubic);
    vint qubitsExpected(3*6*6*6, 0);
    EXPECT_EQ(latCubic.qubitsZ, qubitsExpected);
}
