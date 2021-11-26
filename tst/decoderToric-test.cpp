#include "gtest/gtest.h"
#include "decoderToric.h"
#include "prepIndexVectors.h"
#include <iostream>

//------------------------------------------------------------

//This is just a hack to make the testing software build the index vectors
//Means they don't have to be build individually in every test

TEST(indexBuilding, build)
{
    faceToEdgesT = toric::buildFaceToEdges(6);
    edgeToFacesT = toric::buildEdgeToFaces(6);
    vertexToEdgesT = toric::buildVertexToEdges(6);
    edgeToVerticesT = toric::buildEdgeToVertices(6);
    cellToFacesT = toric::buildCellToFaces(6);
    faceToCellsT = toric::buildFaceToCells(6);

    toric::buildSlice(lowerQubitIndicesT, middleQubitIndicesT, upperQubitIndicesT, syndIndicesZT, syndIndicesXLayerT, defectIndicesT, zLogicalT, xLogicalFragmentT, sweepVerticesT, vertexToEdgesT, faceToEdgesT, edgeToVerticesT, 6);
    ASSERT_TRUE(true);
}

//------------------------------------------------------------

TEST(taxiTest, CorrectOutput)
{
    vint dists1 = {-1, -1, 2};
    vint dists2 = {1, 1, -2};
    EXPECT_EQ(toric::taxi(93, 158, 6), dists1);
    EXPECT_EQ(toric::taxi(158, 93, 6), dists2);

    dists1 = {1, -1, 0};
    dists2 = {-1, 1, 0};
    EXPECT_EQ(toric::taxi(93, 88, 6), dists1);
    EXPECT_EQ(toric::taxi(88, 93, 6), dists2);
}
TEST(taxiTest, sameVertex)
{
    vint dists = {0, 0, 0};
    EXPECT_EQ(toric::taxi(58, 58, 6), dists);
}

//------------------------------------------------------------

TEST(prePathTest, CorrectOutput)
{
    vvint prePathOutput = toric::prePath(58, 123, 6);
    vvint prePathOutputExpected = {{2, 1, 2}, 
                                   {0, -1, 1}, 
                                   {1, -1, 1}}; 
    EXPECT_EQ(prePathOutput, prePathOutputExpected);

    prePathOutput = toric::prePath(93, 88, 6);
    prePathOutputExpected = {{1, -1, 1}, 
                             {2, 0, 0}, 
                             {0, 1, 1}};
    EXPECT_EQ(prePathOutput, prePathOutputExpected);
    prePathOutput = toric::prePath(88, 93, 6);
    prePathOutputExpected = {{1, 1, 1}, 
                             {2, 0, 0}, 
                             {0, -1, 1}};
    EXPECT_EQ(prePathOutput, prePathOutputExpected);
}
TEST(prePathTest, sameVertex)
{
    vvint prePathOutput = toric::prePath(58, 58, 6);
    vvint prePathOutputExpected = {{2, 0, 0}, 
                                   {0, 0, 0}, 
                                   {1, 0, 0}};
    EXPECT_EQ(prePathOutput, prePathOutputExpected);
}

//------------------------------------------------------------

TEST(shortestPathTest, sameSection)
{
    vint path = toric::shortestPath(58, 123, 6, syndIndicesZT);
    vint pathExpected = {176, 279, 281, 370};
    EXPECT_EQ(path, pathExpected);
}
TEST(shortestPathTest, sameLevel)
{
    vint path = toric::shortestPath(93, 88, 6, syndIndicesZT);
    vint pathExpected = {279, 265};
    EXPECT_EQ(path, pathExpected);
    path = toric::shortestPath(88, 93, 6, syndIndicesZT);
    pathExpected = {265, 279};
    EXPECT_EQ(path, pathExpected);
}
TEST(shortestPathTest, sameVertex)
{
    vint path = toric::shortestPath(58, 58, 6, syndIndicesZT);
    vint pathExpected = {};
    EXPECT_EQ(path, pathExpected);
}

//------------------------------------------------------------

TEST(shortestDualPathTest, length1)
{
    vint qubitIndices = {};
    for (int i : lowerQubitIndicesT) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesT) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesT) qubitIndices.push_back(i);
    vint path = toric::shortestDualPath(57, 93, qubitIndices, cellToFacesT, 6);

    vint pathExpected = {279};
    
    EXPECT_EQ(path, pathExpected);
}
//This might be wrong because I don't remember how it prioritises faces
//but I can just check if the path is a valid path or not after running it
//and change the test if I need to
TEST(shortestDualPathTest, length2)
{
    vint qubitIndices = {};
    for (int i : lowerQubitIndicesT) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesT) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesT) qubitIndices.push_back(i);
    vint path1 = toric::shortestDualPath(57, 92, qubitIndices, cellToFacesT, 6);
    vint path2 = toric::shortestDualPath(57, 87, qubitIndices, cellToFacesT, 6);

    vint pathExpected1 = {279, 281};
    vint pathExpected2 = {279, 280};

    EXPECT_EQ(path1, pathExpected1);
    EXPECT_EQ(path2, pathExpected2);
}
   
//------------------------------------------------------------ 

TEST(checkLogicalErrorTest, noError)
{
    vint qubits(3*5*6*6*6, 0);
    EXPECT_FALSE(checkLogicalError(qubits, zLogicalT));
}
TEST(checkLogicalErrorTest, logicalError)
{
    vint qubits(3*5*6*6*6, 0);
    qubits[284] = 1;
    qubits[374] = 1;
    qubits[464] = 1;
    EXPECT_TRUE(checkLogicalError(qubits, zLogicalT));
}

//------------------------------------------------------------

//This is specifically testing how the sweep decoder deals with our layers
TEST(sweepRuleTest, correctsError)
{
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::mt19937 engine(time(0));
    std::uniform_real_distribution<double> dist(0,1);
    
    qubits[371] = 1;
    qubits[479] = 1;
    syndrome[370] = 1;
    syndrome[371] = 1;
    syndrome[389] = 1;
    syndrome[479] = 1;
    toric::sweepRule(qubits, middleQubitIndicesT, syndrome, syndIndicesZT, sweepVerticesT, 
                faceToEdgesT, 6, engine, dist);
    toric::sweepRule(qubits, upperQubitIndicesT, syndrome, syndIndicesZT, sweepVerticesT,
                faceToEdgesT, 6, engine, dist);

    EXPECT_EQ(qubits, qubitsExpected);
    EXPECT_EQ(syndrome, syndromeExpected);
}

//------------------------------------------------------------

TEST(distanceToClosestXBoundaryTest, correctOutput)
{
    vint distanceExpected1 = {1, 1};
    vint distanceExpected2 = {-1, 2};
    EXPECT_EQ(toric::distanceToClosestXBoundary(58, 6), distanceExpected1);
    EXPECT_EQ(toric::distanceToClosestXBoundary(88, 6), distanceExpected2);
}

//------------------------------------------------------------

TEST(shortestPathToXBTest, correctOutput)
{
    vint pathExpected1 = {175};
    vint pathExpected2 = {266, 355};
    EXPECT_EQ(toric::shortestPathToXB(58, 6), pathExpected1);
    EXPECT_EQ(toric::shortestPathToXB(88, 6), pathExpected2);
}

//------------------------------------------------------------

TEST(distanceToClosestZBoundary, correctOutput)
{
    vint distanceExpected1 = {1, 1, 0};
    vint distanceExpected2 = {-1, 2, 0};
    vint distanceExpected3 = {-1, 0, 2};
    vint distanceExpected4 = {-1, 1, 0};
    EXPECT_EQ(toric::distanceToClosestZBoundary(57, 6), distanceExpected1);
    EXPECT_EQ(toric::distanceToClosestZBoundary(777, 6), distanceExpected2);
    EXPECT_EQ(toric::distanceToClosestZBoundary(56, 6), distanceExpected3);
    EXPECT_EQ(toric::distanceToClosestZBoundary(158, 6), distanceExpected4);
}

//------------------------------------------------------------

//Need something in here to test the zigzag paths I think?
TEST(shortestPathToZB, correctOutput)
{
    vint pathExpected1 = {176};
    vint pathExpected2 = {2333, 2330};
    vint pathExpected3 = {276, 278};
    EXPECT_EQ(toric::shortestPathToZB(57, 6), pathExpected1);
    EXPECT_EQ(toric::shortestPathToZB(777, 6), pathExpected2);
    EXPECT_EQ(toric::shortestPathToZB(56, 6), pathExpected3);
}

//------------------------------------------------------------

TEST(mwpmTest, matchingToDefects)
{
    vint defects = {88, 123};
    std::map<std::pair<int, int>, int> defectPairDistances;
    vpint defectPairsExpected = {{88, 123}};
    EXPECT_EQ(toric::mwpm(defects, defectPairDistances, 6, 0), defectPairsExpected);
}
TEST(mwpmTest, matchingToBoundaries)
{
    vint defects = {57, 158};
    std::map<std::pair<int, int>, int> defectPairDistances;
    vpint defectPairsExpected = {{57, -1}, {158, -1}};
    EXPECT_EQ(toric::mwpm(defects, defectPairDistances, 6, 0), defectPairsExpected);
}

//------------------------------------------------------------

TEST(updatePairingTest, addPair)
{
    std::map<std::pair<int, int>, int> defectPairDistances;
    std::map<std::pair<int, int>, int> defectPairDistancesExpected;
    defectPairDistancesExpected[{88,123}] = 2;
    vint defects = {88, 123};
    toric::updatePairing(defectPairDistances, defects, syndIndicesZT, 6);
    EXPECT_EQ(defectPairDistances, defectPairDistancesExpected);
}
TEST(updatePairingTest, removePair)
{
    std::map<std::pair<int, int>, int> defectPairDistances;
    std::map<std::pair<int, int>, int> defectPairDistancesExpected;
    defectPairDistances[{88, 123}] = 2;
    vint defects = {};
    toric::updatePairing(defectPairDistances, defects, syndIndicesZT, 6);
    EXPECT_EQ(defectPairDistances, defectPairDistancesExpected);
}
TEST(updatePairingTest, existingPair)
{
    std::map<std::pair<int, int>, int> defectPairDistances;
    std::map<std::pair<int, int>, int> defectPairDistancesExpected;
    defectPairDistances[{88, 123}] = 1;
    defectPairDistancesExpected[{88, 123}] = 1;
    vint defects = {88, 123};
    toric::updatePairing(defectPairDistances, defects, syndIndicesZT, 6);
    EXPECT_EQ(defectPairDistances, defectPairDistancesExpected);
}

//------------------------------------------------------------

TEST(joinPairTest, joinToDefect)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    syndrome[281] = 1;
    syndrome[370] = 1;
    toric::joinPair(93, 123, syndIndicesZT, syndrome, 6);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(joinPairTest, joinToBoundary)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    syndrome[280] = 1;
    toric::joinPair(93, -1, syndIndicesZT, syndrome, 6);
    EXPECT_EQ(syndrome, syndromeExpected);
}

//------------------------------------------------------------

TEST(joinDualPairTest, joinInBulk)
{
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint qubitIndices = {};
    for (int i : lowerQubitIndicesT) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesT) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesT) qubitIndices.push_back(i);

    qubits[279] = 1;
    qubits[387] = 1;
    toric::joinDualPair(57, 129, qubitIndices, cellToFacesT, qubits, 6);
    EXPECT_EQ(qubits, qubitsExpected);
}
TEST(joinDualPairTest, joinToBoundary)
{
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint qubitIndices = {};
    for (int i : lowerQubitIndicesT) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesT) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesT) qubitIndices.push_back(i);
    
    qubits[176] = 1;
    toric::joinDualPair(57, -1, qubitIndices, cellToFacesT, qubits, 6);
    EXPECT_EQ(qubits, qubitsExpected);
}

//------------------------------------------------------------

TEST(syndromeRepairTest, matchEffectiveDistanceOnePair)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::map<std::pair<int, int>, int> defectPairDistances;
    syndrome[266] = 1;
    syndrome[369] = 1;
    defectPairDistances[{88, 123}] = 1;
    toric::syndromeRepair(syndrome, syndIndicesZT, defectPairDistances, edgeToVerticesT, 6, 0, 0);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(syndromeRepairTest, matchTrueDistanceOneToBoundary)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::map<std::pair<int, int>, int> defectPairDistances;
    syndrome[175] = 1;
    defectPairDistances[{58,-1}] = 1;
    toric::syndromeRepair(syndrome, syndIndicesZT, defectPairDistances, edgeToVerticesT, 6, 0, 0);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(syndromeRepairTest, matchEffectiveDistanceOneToBoundary)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::map<std::pair<int, int>, int> defectPairDistances;
    syndrome[266] = 1;
    syndrome[355] = 1;
    defectPairDistances[{88,-1}] = 1;
    toric::syndromeRepair(syndrome, syndIndicesZT, defectPairDistances, edgeToVerticesT, 6, 0, 0);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(syndromeRepairTest, matchMultipleDefectsToBoundary)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::map<std::pair<int, int>, int> defectPairDistances;
    syndrome[175] = 1;
    syndrome[280] = 1;
    syndrome[385] = 1;
    defectPairDistances[{58, -1}] = 1;
    defectPairDistances[{93, -1}] = 1;
    defectPairDistances[{128, -1}] = 1;
    toric::syndromeRepair(syndrome, syndIndicesZT, defectPairDistances, edgeToVerticesT, 6, 0, 0);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(syndromeRepairTest, matchSomeDeferOthers)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::map<std::pair<int, int>, int> defectPairDistances;
    syndrome[280] = 1;
    syndrome[584] = 1;
    syndrome[673] = 1;
    syndromeExpected[673] = 1;
    defectPairDistances[{93, -1}] = 1;
    defectPairDistances[{194, -1}] = 2;
    toric::syndromeRepair(syndrome, syndIndicesZT, defectPairDistances, edgeToVerticesT, 6, 0, 0);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(syndromeRepairTest, deferDefectPair)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::map<std::pair<int, int>, int> defectPairDistances;
    syndrome[374] = 1;
    syndrome[477] = 1;
    syndrome[479] = 1;
    syndrome[582] = 1;
    syndromeExpected[477] = 1;
    syndromeExpected[479] = 1;
    syndromeExpected[582] = 1;
    syndromeExpected[584] = 1;
    defectPairDistances[{124, 194}] = 4;
    toric::syndromeRepair(syndrome, syndIndicesZT, defectPairDistances, edgeToVerticesT, 6, 0, 0);
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(syndromeRepairTest, deferSingleDefectUpwards)
{
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);
    std::map<std::pair<int, int>, int> defectPairDistances;
    defectPairDistances[{124, -1}] = 2;
    syndrome[374] = 1;
    syndrome[463] = 1;
    syndromeExpected[463] = 1;
    toric::syndromeRepair(syndrome, syndIndicesZT, defectPairDistances, edgeToVerticesT, 6, 0, 0);
    EXPECT_EQ(syndrome, syndromeExpected);
}

//------------------------------------------------------------

TEST(zErrorDecoderTest, matchPair)
{
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint qubitIndices = {};
    vint xStabs(5*6*6*6, 0);
    for (int i : lowerQubitIndicesT) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesT) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesT) qubitIndices.push_back(i);
    
    qubits[279] = 1; 
    xStabs[57] = 1;
    xStabs[93] = 1;

    toric::zErrorDecoder(qubits, qubitIndices, cellToFacesT, xStabs, 6);
    EXPECT_EQ(qubits, qubitsExpected);
}
TEST(zErrorDecoderTest, matchTwoPairs)
{  
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint qubitIndices = {};
    vint xStabs(5*6*6*6, 0);
    for (int i : lowerQubitIndicesT) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesT) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesT) qubitIndices.push_back(i);
    
    qubits[279] = 1;
    xStabs[57] = 1;
    xStabs[93] = 1;
    qubits[461] = 1;
    xStabs[152] = 1;
    xStabs[153] = 1;

    toric::zErrorDecoder(qubits, qubitIndices, cellToFacesT, xStabs, 6);
    EXPECT_EQ(qubits, qubitsExpected);
}
TEST(zErrorDecoderTest, matchToBoundary)
{
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint qubitIndices = {};
    vint xStabs(5*6*6*6, 0);
    for (int i : lowerQubitIndicesT) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesT) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesT) qubitIndices.push_back(i);

    qubits[176] = 1;
    xStabs[57] = 1;

    toric::zErrorDecoder(qubits, qubitIndices, cellToFacesT, xStabs, 6);
    EXPECT_EQ(qubits, qubitsExpected);
}

//Not sure it makes much sense to add unit tests for oneRun
//I think the simulation outcomes are essentially tests of that

