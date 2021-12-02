#include "gtest/gtest.h"
#include "decoderCubic.h"
#include "prepLattice.h"

//------------------------------------------------------------

//This is just a hack to make the testing software build the index vectors
//Means they don't have to be build individually in every test

TEST(indexBuilding, build)
{
    cubic::buildLattice(cubic, 6);
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

TEST(distanceToClosestXBoundaryTest, CorrectOutput)
{
    vint distInfoExpected1 = {1, -1, 1};
    vint distInfoExpected2 = {1, 1, 1};
    
    EXPECT_EQ(cubic::distanceToClosestXBoundary(43, 6), distInfoExpected1);
    EXPECT_EQ(cubic::distanceToClosestXBoundary(49, 6), distInfoExpected2);
}

//------------------------------------------------------------

TEST(shortestPathToXBoundaryTest, correctOutput)
{
    vint pathExpected1 = {112};
    vint pathExpected2 = {148};
    EXPECT_EQ(cubic::shortestPathToXB(43, 6), pathExpected1);
    EXPECT_EQ(cubic::shortestPathToXB(49, 6), pathExpected2);
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

TEST(shortestPathToZB, correctOutput)
{
    vint pathExpected1 = {2};
    vint pathExpected2 = {8};
    EXPECT_EQ(cubic::shortestPathToZB(0, 6), pathExpected1);
    EXPECT_EQ(cubic::shortestPathToZB(1, 6), pathExpected2);
}

//------------------------------------------------------------

TEST(mwpmTest, matchingToDefects)
{
    vint defects = {88, 123};
    vpint defectPairsExpected = {{88, 123}};
    EXPECT_EQ(cubic::mwpm(defects, 6, 0), defectPairsExpected);
}
TEST(mwpmTest, matchingToBoundaries)
{
    vint defects = {57, 158};
    vpint defectPairsExpected = {{57, -1}, {158, -1}};
    EXPECT_EQ(cubic::mwpm(defects, 6, 0), defectPairsExpected);
}
TEST(mwpmTest, dualMatching)
{
    vint cells = {0, 6};
    vpint cellPairsExpected = {0, 6};
    EXPECT_EQ(cubic::mwpm(cells, 6, 1), cellPairsExpected);
}
TEST(mwpmTest, dualMatchingToBoundaries)
{
    vint cell = {0, 1};
    vpint cellsPairExpected = {{0, -1}, {1, -1}};
    EXPECT_EQ(cubic::mwpm(cells, 6, 1), cellPairsExpected);
}

//------------------------------------------------------------

TEST(joinPairTest, joinToDefect)
{
    vint syndrome(3*6*6*6, 0);
    vint syndromeExpected(3*6*6*6, 0);
    syndrome[281] = 1;
    syndrome[370] = 1;
    cubic::joinPair(93, 123, syndrome, 6);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(joinPairTest, joinToBoundary)
{
    vint syndrome(3*6*6*6, 0);
    vint syndromeExpected(3*6*6*6, 0);
    syndrome[280] = 1;
    cubic::joinPair(93, -1, syndrome, 6);
    EXPECT_EQ(syndrome, syndromeExpected);
}

//------------------------------------------------------------

TEST(joinDualPairTest, joinInBulk)
{
    vint qubits(3*6*6*6, 0);
    vint qubitsExpected(3*6*6*6, 0);
    qubits[279] = 1;
    qubits[387] = 1;
    cubic::joinDualPair(57, 129, qubits, 6);
    EXPECT_EQ(qubits, qubitsExpected);
}
TEST(joinDualPairTest, joinToBoundary)
{
    vint qubits(3*6*6*6, 0);
    vint qubitsExpected(3*6*6*6, 0);
    qubits[176] = 1;
    cubic::joinDualPair(57, -1, qubits, 6);
    EXPECT_EQ(qubits, qubitsExpected);
}

//------------------------------------------------------------

TEST(zErrorDecoderTest, matchPair)
{
    cubic.wipe();
    cubic.qubitsZ[279] = 1; 
    cubic.syndromeX[57] = 1;
    cubic.syndromeX[93] = 1;

    cubic::zErrorDecoder(cubic, 6);
    vint qubitsExpected = (3*6*6*6, 0);
    EXPECT_EQ(cubic.qubitsZ, qubitsExpected);
}
TEST(zErrorDecoderTest, matchTwoPairs)
{  
    cubic.wipe();
    cubic.qubitsZ[279] = 1;
    cubic.syndromeX[57] = 1;
    cubic.syndromeX[93] = 1;
    cubic.qubitsZ[461] = 1;
    cubic.syndromeX[152] = 1;
    cubic.syndromeX[153] = 1;

    cubic::zErrorDecoder(cubic, 6);
    vint qubitsExpected = (3*6*6*6, 0);
    EXPECT_EQ(cubic.qubitsZ, qubitsExpected);
}
TEST(zErrorDecoderTest, matchToBoundary)
{
    cubic.wipe();
    cubic.qubitsZ[176] = 1;
    cubic.syndromeX[57] = 1;

    cubic::zErrorDecoder(cubic, 6);
    vint qubitsExpected = (3*6*6*6, 0);
    EXPECT_EQ(cubic.qubitsZ, qubitsExpected);
}

//------------------------------------------------------------

TEST(measErrorDecoderTest, matchPair)
{
    cubic.wipe();
    cubic.syndromeZ[127] = 1;
    cubic.defects = {42, 48};

    cubic::measErrorDecoder(cubic, 6);
    vint syndromeExpected = (8*6*6*6, 0);
    EXPECT_EQ(cubic.syndromeZ, syndromeExpected);
}
TEST(measErrorDecoderTest, matchTwoPair)
{
    cubic.wipe();
    cubic.syndromeZ[127] = 1;
    cubic.syndromeZ[241] = 1;
    cubic.defects = {42, 48, 80, 86};

    cubic::measErrorDecoder(cubic, 6);
    vint syndromeExpected = (8*6*6*6, 0);
    EXPECT_EQ(cubic.syndromeZ, syndromeExpected);
}
TEST(measErrorDecoderTest, matchToBoundary)
{
    cubic.wipe();
    cubic.syndromeZ[109] = 1;
    cubic.defects = {42};

    cubic::measErrorDecoder(cubic, 6);
    vint syndromeExpected = (8*6*6*6, 0);
    EXPECT_EQ(cubic.syndromeZ, syndromeExpected);
}

//------------------------------------------------------------
//X ERROR DECODER TEST HERE?
//------------------------------------------------------------

TEST(jumpCorrectionTest, CorrectOutputExample)
{
    //Tests a specific example
    cubic.wipe();
    cubic.qubitsZ[110] = 1;
    cubic.qubitsZ[216] = 1;
    cubic.qubitsZ[218] = 1;

    cubic::jumpCorrection(cubic, 6);
    vint qubitsExpected = (3*6*6*6, 0);
    EXPECT_EQ(cubic.qubitsZ, qubitsExpected);
}
TEST(jumpCorrectionTest, CorrectOutput)
{
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    cubic.zStabPattern(engine, dist);
    cubic::jumpCorrection(cubic, 6);
    vint qubitsExpected = (3*6*6*6, 0);
    EXPECT_EQ(cubic.qubitsZ, qubitsExpected);
}
