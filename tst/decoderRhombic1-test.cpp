#include "gtest/gtest.h"
#include "decoderRhombic1.h"
#include <iostream>

//------------------------------------------------------------

//This is just a hack to make the testing software build the index vectors
//Means they don't have to be build individually in every test

TEST(indexBuilding, build)
{
    vertexToEdgesR1 = rhombic1::buildVertexToEdges(6);
    edgeToVerticesR1 = rhombic1::buildEdgeToVertices(6);
    rhombic1::buildFaces(faceToVerticesR1, faceToEdgesR1, faceToCellsR1, 
                            vertexToFacesR1, edgeToFacesR1, 6);
    cellToFacesR1 = rhombic1::buildCellToFaces(vertexToFacesR1, faceToVerticesR1, 6);
    //faceToCellsR1 = rhombic1::buildFaceToCells(6);

    rhombic1::buildSlice(lowerQubitIndicesR1, middleQubitIndicesR1, upperQubitIndicesR1, 
                         syndIndicesZR1, syndIndicesXLayerR1, defectIndicesR1, 
                         bulkSweepVerticesR1, middleBoundarySweepVerticesR1, 
                         upperBoundarySweepVertices1R1, upperBoundarySweepVertices2R1, 
                         zLogicalR1, xLogicalFragment1R1, xLogicalFragment2R1, vertexToFacesR1, 
                         faceToVerticesR1, faceToEdgesR1, edgeToVerticesR1, 6);

    ASSERT_TRUE(true);
}

//------------------------------------------------------------

TEST(shortestPathLengthTest, CorrectOutput)
{
    EXPECT_EQ(rhombic1::shortestPathLength(424, 1503, 6), 1);
    EXPECT_EQ(rhombic1::shortestPathLength(424, 603, 6), 2);
    EXPECT_EQ(rhombic1::shortestPathLength(424, 1682, 6), 3);
}
    
//------------------------------------------------------------

TEST(shortestPathTest, CorrectOutput)
{
    vint path1 = {1698};
    vint path2 = {1698, 2412};
    vint path3 = {1698, 2412, 2414};
    vint path4 = {1699};
    EXPECT_EQ(rhombic1::shortestPath(424, 1503, syndIndicesZR1, vertexToEdgesR1, 6), path1);
    EXPECT_EQ(rhombic1::shortestPath(424, 603, syndIndicesZR1, vertexToEdgesR1, 6), path2);
    EXPECT_EQ(rhombic1::shortestPath(424, 1682, syndIndicesZR1, vertexToEdgesR1, 6), path3);
    EXPECT_EQ(rhombic1::shortestPath(424, 1504, syndIndicesZR1, vertexToEdgesR1, 6), path4);
}

//------------------------------------------------------------

TEST(shortestDualPathTest, CorrectOutput)
{
    vint qubitIndices = {};
    for (int i : lowerQubitIndicesR1) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesR1) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesR1) qubitIndices.push_back(i);

    vint path1 = {1357};
    vint path2 = {1273, 1808}; 
    EXPECT_EQ(rhombic1::shortestDualPath(227, 316, qubitIndices, cellToFacesR1, 6), path1);
    EXPECT_EQ(rhombic1::shortestDualPath(212, 316, qubitIndices, cellToFacesR1, 6), path2);
}

//------------------------------------------------------------

TEST(middleSweepRuleTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
   
    qubits[1808] = 1;
    syndrome[2412] = 1;
    syndrome[2415] = 1;
    syndrome[6015] = 1;
    syndrome[6732] = 1;
    rhombic1::middleSweepRule(qubits, middleQubitIndicesR1, syndrome, syndIndicesZR1, 
                            bulkSweepVerticesR1, faceToEdgesR1, faceToVerticesR1, vertexToFacesR1,6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1807] = 1;
    qubits[1808] = 1;
    syndrome[2412] = 1;
    syndrome[2413] = 1;
    syndrome[6015] = 1;
    syndrome[6732] = 1;
    rhombic1::middleSweepRule(qubits, middleQubitIndicesR1, syndrome, syndIndicesZR1, 
                            bulkSweepVerticesR1, faceToEdgesR1, faceToVerticesR1, vertexToFacesR1,6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1806] = 1;
    qubits[1807] = 1;
    qubits[1808] = 1;
    syndrome[2412] = 1;
    syndrome[2413] = 1;
    syndrome[2414] = 1;
    syndrome[2415] = 1;
    syndrome[6732] = 1;
    syndrome[6015] = 1;
    rhombic1::middleSweepRule(qubits, middleQubitIndicesR1, syndrome, syndIndicesZR1, 
                            bulkSweepVerticesR1, faceToEdgesR1, faceToVerticesR1, vertexToFacesR1,6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(middleBoundarySweepRuleTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint nonBlankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
    vint nonBlankSyndrome(8*5*6*6*6, 0);

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    qubits[822] = 1;
    syndrome[5415] = 1;
    syndrome[5418] = 1;
    rhombic1::middleBoundarySweepRule(qubits, middleQubitIndicesR1, syndrome, syndIndicesZR1, 
                                middleBoundarySweepVerticesR1, faceToEdgesR1, faceToVerticesR1, 
                                vertexToFacesR1, 6, engine, dist);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[823] = 1;
    syndrome[5299] = 1;
    syndrome[5417] = 1;
    nonBlankQubits[823] = 1;
    nonBlankQubits[732] = 1;
    nonBlankSyndrome[5417] = 1;
    nonBlankSyndrome[5302] = 1;
    rhombic1::middleBoundarySweepRule(qubits, middleQubitIndicesR1, syndrome, syndIndicesZR1, 
                                middleBoundarySweepVerticesR1, faceToEdgesR1, faceToVerticesR1, 
                                vertexToFacesR1, 6, engine, dist);
    if (qubits == blankQubits || qubits == nonBlankQubits) SUCCEED();
    else FAIL();
    if (syndrome == blankSyndrome || syndrome == nonBlankSyndrome) SUCCEED();
    else FAIL();
}

//------------------------------------------------------------

TEST(upperSweepRuleTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint nonBlankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);

    qubits[1815] = 1;
    syndrome[6016] = 1;
    syndrome[6017] = 1;
    rhombic1::upperSweepRule(qubits, upperQubitIndicesR1, syndrome, syndIndicesZR1, 
                        bulkSweepVerticesR1, faceToEdgesR1, faceToVerticesR1, vertexToFacesR1, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1815] = 1;
    qubits[1906] = 1;
    syndrome[6017] = 1;
    syndrome[6018] = 1;
    nonBlankQubits[1815] = 1;
    nonBlankQubits[1906] = 1;
    nonBlankQubits[1907] = 1;
    rhombic1::upperSweepRule(qubits, upperQubitIndicesR1, syndrome, syndIndicesZR1, 
                        bulkSweepVerticesR1, faceToEdgesR1, faceToVerticesR1, vertexToFacesR1, 6);
    EXPECT_EQ(qubits, nonBlankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(upperBoundarySweepRule1Test, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);

    qubits[2439] = 1;
    syndrome[2653] = 1;
    syndrome[6848] = 1;
    rhombic1::upperBoundarySweepRule1(qubits, upperQubitIndicesR1, syndrome, syndIndicesZR1,
                                upperBoundarySweepVertices1R1, faceToEdgesR1, faceToVerticesR1,
                                vertexToFacesR1, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[2439] = 1;
    qubits[1991] = 1;
    syndrome[6848] = 1;
    syndrome[6133] = 1;
    syndrome[6134] = 1;
    rhombic1::upperBoundarySweepRule1(qubits, upperQubitIndicesR1, syndrome, syndIndicesZR1,
                                upperBoundarySweepVertices1R1, faceToEdgesR1, faceToVerticesR1,
                                vertexToFacesR1, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1991] = 1;
    qubits[1905] = 1;
    syndrome[2653] = 1;
    syndrome[6132] = 1;
    syndrome[6134] = 1;
    syndrome[1937] = 1;
    rhombic1::upperBoundarySweepRule1(qubits, upperQubitIndicesR1, syndrome, syndIndicesZR1,
                                upperBoundarySweepVertices1R1, faceToEdgesR1, faceToVerticesR1,
                                vertexToFacesR1, 6);
    
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(upperBoundarySweepRule2Test, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint nonBlankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
    
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    qubits[1281] = 1;
    syndrome[5301] = 1;
    rhombic1::upperBoundarySweepRule2(qubits, upperQubitIndicesR1, syndrome, syndIndicesZR1, 
                                upperBoundarySweepVertices2R1, faceToEdgesR1, faceToVerticesR1, 
                                vertexToFacesR1, 6, engine, dist);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[2355] = 1;
    syndrome[6732] = 1;
    nonBlankQubits[2355] = 1;
    nonBlankQubits[2440] = 1;
    rhombic1::upperBoundarySweepRule2(qubits, upperQubitIndicesR1, syndrome, syndIndicesZR1, 
                                upperBoundarySweepVertices2R1, faceToEdgesR1, faceToVerticesR1, 
                                vertexToFacesR1, 6, engine, dist);
    if (qubits == blankQubits || qubits == nonBlankQubits) SUCCEED();
    else FAIL();
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(distanceToClosestXBoundaryTest, CorrectOutput)
{
    vint distInfo1 = {1, 2};
    vint distInfo2 = {1, 1};
    vint distInfo3 = {1, 2};
    vint distInfo4 = {-1, 1};
    EXPECT_EQ(rhombic1::distanceToClosestXBoundary(1504, 6), distInfo1);
    EXPECT_EQ(rhombic1::distanceToClosestXBoundary(634, 6), distInfo2);
    EXPECT_EQ(rhombic1::distanceToClosestXBoundary(1503, 6), distInfo3);
    EXPECT_EQ(rhombic1::distanceToClosestXBoundary(453, 6), distInfo4);
}

//------------------------------------------------------------

TEST(shortestPathToXBTest, CorrectOutput)
{
    vint path1 = {6018, 6732};
    vint path2 = {6732};
    vint path3 = {2412, 2414};
    vint path4 = {1812};
    EXPECT_EQ(rhombic1::shortestPathToXB(1504, syndIndicesZR1, 6), path1);
    EXPECT_EQ(rhombic1::shortestPathToXB(634, syndIndicesZR1, 6), path2);
    EXPECT_EQ(rhombic1::shortestPathToXB(1503, syndIndicesZR1, 6), path3);
    EXPECT_EQ(rhombic1::shortestPathToXB(453, syndIndicesZR1, 6), path4);
}

//------------------------------------------------------------

TEST(distanceToClosestZBoundaryTest, CorrectOutput)
{
    vint path;
    path = {1, 1};
    EXPECT_EQ(rhombic1::distanceToClosestZBoundary(146, 6), path); //293
    path = {-1, 1};
    EXPECT_EQ(rhombic1::distanceToClosestZBoundary(132, 6), path); //264
    path = {1, 2};
    EXPECT_EQ(rhombic1::distanceToClosestZBoundary(237, 6), path); //474
    EXPECT_EQ(rhombic1::distanceToClosestZBoundary(326, 6), path); //653
    path = {1, 1};
    EXPECT_EQ(rhombic1::distanceToClosestZBoundary(227, 6), path); //454
    path = {-1, 1};
    EXPECT_EQ(rhombic1::distanceToClosestZBoundary(391, 6), path); //783
    path = {1, 1};
    EXPECT_EQ(rhombic1::distanceToClosestZBoundary(406, 6), path); //812
    path = {1, 2};
    EXPECT_EQ(rhombic1::distanceToClosestZBoundary(301, 6), path); //602
    EXPECT_EQ(rhombic1::distanceToClosestZBoundary(211, 6), path); //423
    path = {-1, 1};
    EXPECT_EQ(rhombic1::distanceToClosestZBoundary(302, 6), path); //604
}

//------------------------------------------------------------

TEST(shortestPathToZBTest, CorrectOutput)
{
    vint path;
    path = {1419};
    EXPECT_EQ(rhombic1::shortestPathToZB(146, cellToFacesR1, 6), path);
    path = {702};
    EXPECT_EQ(rhombic1::shortestPathToZB(132, cellToFacesR1, 6), path);
    path = {1420, 1419};
    EXPECT_EQ(rhombic1::shortestPathToZB(237, cellToFacesR1, 6), path);
    path = {1960, 1412};
    EXPECT_EQ(rhombic1::shortestPathToZB(326, cellToFacesR1, 6), path);
    path = {1358};
    EXPECT_EQ(rhombic1::shortestPathToZB(227, cellToFacesR1, 6), path);
    path = {2259};
    EXPECT_EQ(rhombic1::shortestPathToZB(391, cellToFacesR1, 6), path);
    path = {1896};
    EXPECT_EQ(rhombic1::shortestPathToZB(406, cellToFacesR1, 6), path);
    path = {1901, 1898};
    EXPECT_EQ(rhombic1::shortestPathToZB(301, cellToFacesR1, 6), path);
    path = {1361, 1358};
    EXPECT_EQ(rhombic1::shortestPathToZB(211, cellToFacesR1, 6), path);
    path = {1724};
    EXPECT_EQ(rhombic1::shortestPathToZB(302, cellToFacesR1, 6), path);
}

//------------------------------------------------------------

TEST(mwpmTest, PrimalLattice)
{
    vint defects;
    vpint expectedMatching;
    std::map<std::pair<int, int>, int> defectPairDistances;

    defects = {1503, 603};
    expectedMatching = {{1503, 603}};
    EXPECT_EQ(rhombic1::mwpm(defects, defectPairDistances, 6, 0), expectedMatching);

    defects = {1503, 603, 395, 1475};
    expectedMatching = {{1503, 603}, {395, 1475}};
    EXPECT_EQ(rhombic1::mwpm(defects, defectPairDistances, 6, 0), expectedMatching);

    defects = {424, 632};
    expectedMatching = {{424, -1}, {632, -1}};
    EXPECT_EQ(rhombic1::mwpm(defects, defectPairDistances, 6, 0), expectedMatching);

    defectPairDistances[{424, 632}] = 1;
    expectedMatching = {{424, 632}};
    EXPECT_EQ(rhombic1::mwpm(defects, defectPairDistances, 6, 0), expectedMatching);
}
TEST(mwpmTest, DualLattice)
{
    vint violatedStabs;
    vpint expectedMatching;
    std::map<std::pair<int, int>, int> emptyMap;

    violatedStabs = {227, 316};
    expectedMatching = {{227, 316}};
    EXPECT_EQ(rhombic1::mwpm(violatedStabs, emptyMap, 6, 1), expectedMatching);

    violatedStabs = {227, 316, 213, 123};
    expectedMatching = {{227, 316}, {213, 123}};
    EXPECT_EQ(rhombic1::mwpm(violatedStabs, emptyMap, 6, 1), expectedMatching);

    violatedStabs = {227};
    expectedMatching = {{227, -1}};
    EXPECT_EQ(rhombic1::mwpm(violatedStabs, emptyMap, 6, 1), expectedMatching);
}

//------------------------------------------------------------

TEST(updatePairingTest, CorrectOutput)
{
    vint defects;
    std::map<std::pair<int, int>, int> defectPairDistances;
    std::map<std::pair<int, int>, int> expectedDefectPairDistances;
    
    defects = {603, 1503, 395, 1475};
    defectPairDistances[{603, 1503}] = 1;
    defectPairDistances[{424, 1504}] = 1;
    expectedDefectPairDistances[{603, 1503}] = 1;
    expectedDefectPairDistances[{395, 1475}] = 1;
    rhombic1::updatePairing(defectPairDistances, defects, 6);
    EXPECT_EQ(defectPairDistances, expectedDefectPairDistances);
}

//------------------------------------------------------------

TEST(joinPairTest, CorrectOutput)
{
    vint syndrome(8*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);

    syndrome[2412] = 1;
    rhombic1::joinPair(1503, 603, syndIndicesZR1, vertexToEdgesR1, syndrome, 6);
    EXPECT_EQ(syndrome, blankSyndrome);

    syndrome[2530] = 1;
    rhombic1::joinPair(632, -1, syndIndicesZR1, vertexToEdgesR1, syndrome, 6);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(joinDualPairTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint qubitIndices = {};
    for (int i : lowerQubitIndicesR1) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesR1) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesR1) qubitIndices.push_back(i);

    qubits[1357] = 1;
    rhombic1::joinDualPair(227, 316, qubitIndices, cellToFacesR1, qubits, 6);
    EXPECT_EQ(qubits, qubitsExpected);

    qubits[1358] = 1;
    rhombic1::joinDualPair(227, -1, qubitIndices, cellToFacesR1, qubits, 6);
    EXPECT_EQ(qubits, qubitsExpected);
}

//------------------------------------------------------------

TEST(pathToTopTest, CorrectOutput)
{
    vint path1 = {2528, 6131, 6134, 6133};
    vint path2 = {6131, 6134, 6133};
    vint path3 = {6133};
    vint path4 = {};
    EXPECT_EQ(rhombic1::pathToTop(632, 6), path1);
    EXPECT_EQ(rhombic1::pathToTop(1532, 6), path2);
    EXPECT_EQ(rhombic1::pathToTop(1533, 6), path3);
    EXPECT_EQ(rhombic1::pathToTop(-1, 6), path4);
}

//------------------------------------------------------------

TEST(syndromeRepairTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
    vint nonBlankSyndrome(8*5*6*6*6, 0);

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    syndrome[5902] = 1;
    syndrome[6017] = 1;
    syndrome[6018] = 1;
    syndrome[6133] = 1;
    nonBlankSyndrome[5901] = 1;
    nonBlankSyndrome[5902] = 1;
    nonBlankSyndrome[6017] = 1;
    nonBlankSyndrome[6018] = 1;

    std::map<std::pair<int, int>, int> defectPairDistances;
    defectPairDistances[{1475, 1533}] = 4;
    rhombic1::syndromeRepair(syndrome, syndIndicesZR1, vertexToEdgesR1, edgeToVerticesR1, 
                    defectPairDistances, 6, 2, 0);
    EXPECT_EQ(syndrome, nonBlankSyndrome);
    int distance = defectPairDistances[{576, 634}];
    EXPECT_EQ(distance, 2);
    
    syndrome[5901] = 0;
    syndrome[6133] = 1;
    std::map<std::pair<int, int>, int> defectPairDistances2;
    defectPairDistances2[{1475, 1533}] = 2;
    rhombic1::syndromeRepair(syndrome, syndIndicesZR1, vertexToEdgesR1, edgeToVerticesR1, 
                    defectPairDistances2, 6, 2, 0);
    rhombic1::middleSweepRule(qubits, middleQubitIndicesR1, syndrome, syndIndicesZR1, 
                        bulkSweepVerticesR1, faceToEdgesR1, faceToVerticesR1, vertexToFacesR1, 6);
    rhombic1::middleBoundarySweepRule(qubits, middleQubitIndicesR1, syndrome, syndIndicesZR1, 
                                middleBoundarySweepVerticesR1, faceToEdgesR1, faceToVerticesR1, 
                                vertexToFacesR1, 6, engine, dist);
    rhombic1::upperSweepRule(qubits, upperQubitIndicesR1, syndrome, syndIndicesZR1, 
                        bulkSweepVerticesR1, faceToEdgesR1, faceToVerticesR1, vertexToFacesR1, 6);
    rhombic1::upperBoundarySweepRule1(qubits, upperQubitIndicesR1, syndrome, syndIndicesZR1, 
                                upperBoundarySweepVertices1R1, faceToEdgesR1, faceToVerticesR1, 
                                vertexToFacesR1, 6);
    rhombic1::upperBoundarySweepRule2(qubits, upperQubitIndicesR1, syndrome, syndIndicesZR1, 
                                upperBoundarySweepVertices2R1, faceToEdgesR1, faceToVerticesR1, 
                                vertexToFacesR1, 6, engine, dist);

    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(zErrorDecoderTest, matchPair)
{
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint qubitIndices = {};
    vint xStabs((5*6*6*6)/2, 0);
    for (int i : lowerQubitIndicesR1) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesR1) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesR1) qubitIndices.push_back(i);

    qubits[1357] = 1;
    xStabs[227] = 1;
    xStabs[316] = 1;
    
    rhombic1::zErrorDecoder(qubits, qubitIndices, cellToFacesR1, xStabs, 6);
    
    EXPECT_EQ(qubits, qubitsExpected);
}
TEST(zErrorDecoderTest, matchTwoPairs)
{
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint qubitIndices = {};
    vint xStabs((5*6*6*6)/2, 0);
    for (int i : lowerQubitIndicesR1) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesR1) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesR1) qubitIndices.push_back(i);
    
    qubits[1357] = 1;
    xStabs[227] = 1;
    xStabs[316] = 1;
    qubits[1282] = 1;
    xStabs[213] = 1;
    xStabs[123] = 1;

    rhombic1::zErrorDecoder(qubits, qubitIndices, cellToFacesR1, xStabs, 6);
    
    EXPECT_EQ(qubits, qubitsExpected);
}
TEST(zErrorDecoderTest, matchToBoundary)
{
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint qubitIndices = {};
    vint xStabs((5*6*6*6)/2, 0);
    for (int i : lowerQubitIndicesR1) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesR1) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesR1) qubitIndices.push_back(i);

    qubits[1358] = 1;
    xStabs[227] = 1;

    rhombic1::zErrorDecoder(qubits, qubitIndices, cellToFacesR1, xStabs, 6);
    
    EXPECT_EQ(qubits, qubitsExpected);
}
