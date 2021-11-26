#include "gtest/gtest.h"
#include "decoderRhombic2.h"
#include "prepIndexVectors.h"
#include <iostream>

//------------------------------------------------------------

TEST(indexBuilding, building)
{
    vertexToEdgesR2 = rhombic2::buildVertexToEdges(6);
    edgeToVerticesR2 = rhombic2::buildEdgeToVertices(6);
    rhombic2::buildFaces(faceToVerticesR2, faceToEdgesR2, faceToCellsR2, 
                            vertexToFacesR2, edgeToFacesR2, 6);
    cellToFacesR2 = rhombic2::buildCellToFaces(vertexToFacesR2, faceToVerticesR2, 6);
    //faceToCellsR2 = rhombic2::buildFaceToCells(6);

    rhombic2::buildSlice(lowerQubitIndicesR2, middleQubitIndicesR2, upperQubitIndicesR2, 
                         syndIndicesZR2, syndIndicesXLayerR2,  defectIndicesR2, 
                         bulkSweepVerticesR2, middleBoundarySweepVerticesR2, zLogicalR2, 
                         xLogicalFragment1R2, xLogicalFragment2R2, vertexToFacesR2, 
                         faceToVerticesR2, faceToEdgesR2, edgeToVerticesR2, 6);
    
    ASSERT_TRUE(true);
}

//------------------------------------------------------------

TEST(shortestPathLengthTest, CorrectOutput)
{
    EXPECT_EQ(rhombic2::shortestPathLength(387, 1461, 6), 1);
    EXPECT_EQ(rhombic2::shortestPathLength(387, 561, 6), 2);
    EXPECT_EQ(rhombic2::shortestPathLength(387, 1635, 6), 3);
}

//------------------------------------------------------------

TEST(shortestPathTest, CorrectOutput)
{
    vint path1 = {1549};
    vint path2 = {5840, 5841};
    vint path3 = {1549, 2244, 2245};
    EXPECT_EQ(rhombic2::shortestPath(387, 1461, syndIndicesZR2, vertexToEdgesR2, 6), path1);
    EXPECT_EQ(rhombic2::shortestPath(387, 561, syndIndicesZR2, vertexToEdgesR2, 6), path2);
    EXPECT_EQ(rhombic2::shortestPath(387, 1635, syndIndicesZR2, vertexToEdgesR2, 6), path3);
}

//------------------------------------------------------------

TEST(shortestDualPathTest, CorrectOutput)
{
    vint qubitIndices = {};
    for (int i : lowerQubitIndicesR2) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesR2) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesR2) qubitIndices.push_back(i);

    vint path1 = {1158};
    vint path2 = {1158, 1682}; //might not be the right path
    EXPECT_EQ(rhombic2::shortestDualPath(196, 283, qubitIndices, cellToFacesR2, 6), path1);
    EXPECT_EQ(rhombic2::shortestDualPath(196, 281, qubitIndices, cellToFacesR2, 6), path2);
}

//------------------------------------------------------------

TEST(middleSweepRuleTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint nonBlankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
    vint nonBlankSyndrome(8*5*6*6*6, 0);

    qubits[1683] = 1;
    syndrome[5840] = 1;
    syndrome[5841] = 1;
    syndrome[1549] = 1;
    syndrome[2244] = 1;
    rhombic2::middleSweepRule(qubits, middleQubitIndicesR2, syndrome, syndIndicesZR2, 
                        bulkSweepVerticesR2, faceToEdgesR2, faceToVerticesR2, vertexToFacesR2, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1683] = 1;
    qubits[1702] = 1;
    syndrome[5841] = 1;
    syndrome[5842] = 1;
    syndrome[1549] = 1;
    syndrome[1550] = 1;
    syndrome[2244] = 1;
    syndrome[2264] = 1;
    nonBlankQubits[1683] = 1;
    nonBlankQubits[1702] = 1;
    nonBlankQubits[1703] = 1;
    nonBlankSyndrome[1549] = 1;
    nonBlankSyndrome[1550] = 1;
    nonBlankSyndrome[2244] = 1;
    nonBlankSyndrome[2246] = 1;
    nonBlankSyndrome[2264] = 1;
    nonBlankSyndrome[2265] = 1;
    rhombic2::middleSweepRule(qubits, middleQubitIndicesR2, syndrome, syndIndicesZR2, 
                        bulkSweepVerticesR2, faceToEdgesR2, faceToVerticesR2, vertexToFacesR2, 6);
    
    EXPECT_EQ(qubits, nonBlankQubits);
    EXPECT_EQ(syndrome, nonBlankSyndrome);
}
 
//------------------------------------------------------------

TEST(middleBoundarySweepRuleTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
    
    qubits[2205] = 1;
    syndrome[2940] = 1;
    syndrome[6536] = 1;
    syndrome[6537] = 1;
    syndrome[2245] = 1;
    rhombic2::middleBoundarySweepRule(qubits, middleQubitIndicesR2, syndrome, syndIndicesZR2, 
                                middleBoundarySweepVerticesR2, faceToEdgesR2, faceToVerticesR2, 
                                vertexToFacesR2, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[2205] = 1;
    qubits[2206] = 1;
    syndrome[6536] = 1;
    syndrome[6537] = 1;
    syndrome[2245] = 1;
    syndrome[6516] = 1;
    syndrome[6518] = 1;
    syndrome[2226] = 1;
    rhombic2::middleBoundarySweepRule(qubits, middleQubitIndicesR2, syndrome, syndIndicesZR2,
                                middleBoundarySweepVerticesR2, faceToEdgesR2, faceToVerticesR2, 
                                vertexToFacesR2, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);
    
    qubits[2193] = 1;
    qubits[2223] = 1;
    syndrome[6516] = 1;
    syndrome[6517] = 1;
    syndrome[2960] = 1;
    syndrome[2265] = 1;
    rhombic2::middleBoundarySweepRule(qubits, middleQubitIndicesR2, syndrome, syndIndicesZR2,
                                middleBoundarySweepVerticesR2, faceToEdgesR2, faceToVerticesR2, 
                                vertexToFacesR2, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[2223] = 1;
    qubits[2224] = 1;
    syndrome[2246] = 1;
    syndrome[2265] = 1;
    syndrome[6536] = 1;
    syndrome[6538] = 1;
    rhombic2::middleBoundarySweepRule(qubits, middleQubitIndicesR2, syndrome, syndIndicesZR2,
                                middleBoundarySweepVerticesR2, faceToEdgesR2, faceToVerticesR2, 
                                vertexToFacesR2, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(upperSweepRuleTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankQubits(3*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
    
    qubits[1680] = 1;
    syndrome[2246] = 1;
    syndrome[2247] = 1;
    syndrome[6563] = 1;
    rhombic2::upperSweepRule(qubits, upperQubitIndicesR2, syndrome, syndIndicesZR2, 
                        bulkSweepVerticesR2, faceToEdgesR2, faceToVerticesR2, vertexToFacesR2, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);
    
    qubits[1680] = 1;
    qubits[1682] = 1;
    syndrome[2244] = 1;
    syndrome[2246] = 1;
    syndrome[5847] = 1;
    syndrome[6563] = 1;
    rhombic2::upperSweepRule(qubits, upperQubitIndicesR2, syndrome, syndIndicesZR2, 
                        bulkSweepVerticesR2, faceToEdgesR2, faceToVerticesR2, vertexToFacesR2, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1680] = 1;
    qubits[1681] = 1;
    qubits[1682] = 1;
    syndrome[2244] = 1;
    syndrome[2245] = 1;
    syndrome[2246] = 1;
    syndrome[2247] = 1;
    syndrome[5847] = 1;
    syndrome[6563] = 1;
    syndrome[6543] = 1;
    rhombic2::upperSweepRule(qubits, upperQubitIndicesR2, syndrome, syndIndicesZR2, 
                        bulkSweepVerticesR2, faceToEdgesR2, faceToVerticesR2, vertexToFacesR2, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);

    qubits[1698] = 1;
    qubits[1700] = 1;
    syndrome[2264] = 1;
    syndrome[5867] = 1;
    rhombic2::upperSweepRule(qubits, upperQubitIndicesR2, syndrome, syndIndicesZR2, 
                        bulkSweepVerticesR2, faceToEdgesR2, faceToVerticesR2, vertexToFacesR2, 6);
    EXPECT_EQ(qubits, blankQubits);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(distanceToClosestXBoundaryTest, CorrectOutput)
{
    vint distInfo1 = {-1, 1};
    vint distInfo2 = {1, 2};
    vint distInfo3 = {-1, 1};
    EXPECT_EQ(rhombic2::distanceToClosestXBoundary(1460, 6), distInfo1);
    EXPECT_EQ(rhombic2::distanceToClosestXBoundary(561, 6), distInfo2);
    EXPECT_EQ(rhombic2::distanceToClosestXBoundary(1466, 6), distInfo3);
}

//------------------------------------------------------------

TEST(shortestPathToXBTest, CorrectOutput)
{
    vint path1 = {5842};
    vint path2 = {2245, 2226};
    vint path3 = {1569};
    EXPECT_EQ(rhombic2::shortestPathToXB(1460, 6), path1);
    EXPECT_EQ(rhombic2::shortestPathToXB(561, 6), path2);
    EXPECT_EQ(rhombic2::shortestPathToXB(1466, 6), path3);
}

//------------------------------------------------------------

TEST(distanceToClosestZBoundaryTest, CorrectOutput)
{
    vint path1 = {-1, 1};
    vint path2 = {1, 1};
    EXPECT_EQ(rhombic2::distanceToClosestZBoundary(196, 6), path1);
    EXPECT_EQ(rhombic2::distanceToClosestZBoundary(283, 6), path2);
}

//------------------------------------------------------------

TEST(shortestPathToZBTest, CorrectOutput)
{
    vint path1 = {636};
    vint path2 = {1680};
    EXPECT_EQ(rhombic2::shortestPathToZB(196, cellToFacesR2, 6), path1);
    EXPECT_EQ(rhombic2::shortestPathToZB(283, cellToFacesR2, 6), path2);
}

//------------------------------------------------------------

TEST(mwpmTest, CorrectOutput)
{
    vint defects;
    vpint expectedPairing;
    std::map<std::pair<int, int>, int> defectPairDistances;

    defects = {387, 1460};
    expectedPairing = {{387, 1460}};
    EXPECT_EQ(rhombic2::mwpm(defects, defectPairDistances, 6, 0), expectedPairing);

    defects = {387, 1286, 561, 1635};
    expectedPairing = {{387, 1286}, {561, 1635}};
    EXPECT_EQ(rhombic2::mwpm(defects, defectPairDistances, 6, 0), expectedPairing);

    defects = {1286, 1635};
    expectedPairing = {{1286, -1}, {1635, -1}};
    EXPECT_EQ(rhombic2::mwpm(defects, defectPairDistances, 6, 0), expectedPairing);

    defectPairDistances[{1286, 1635}] = 1;
    expectedPairing = {{1286, 1635}};
    EXPECT_EQ(rhombic2::mwpm(defects, defectPairDistances, 6, 0), expectedPairing);
}
TEST(mwpmTest, DualLattice)
{
    vint violatedStabs;
    vpint expectedMatching;
    std::map<std::pair<int, int>, int> emptyMap;
    
    violatedStabs = {196, 283};
    expectedMatching = {{196, 283}};
    EXPECT_EQ(rhombic2::mwpm(violatedStabs, emptyMap, 6, 1), expectedMatching);

    violatedStabs = {196, 199, 277, 275};
    expectedMatching = {{196, 199}, {277, 275}};
    EXPECT_EQ(rhombic2::mwpm(violatedStabs, emptyMap, 6, 1), expectedMatching);

    violatedStabs = {196};
    expectedMatching = {{196, -1}};
    EXPECT_EQ(rhombic2::mwpm(violatedStabs, emptyMap, 6, 1), expectedMatching);
}

//------------------------------------------------------------

TEST(updatePairingTest, CorrectOutput)
{
    vint defects;
    std::map<std::pair<int, int>, int> defectPairDistances;
    std::map<std::pair<int, int>, int> expectedDefectPairDistances;
    
    defects = {393, 1292, 567, 1641};
    defectPairDistances[{393, 1292}] = 1;
    defectPairDistances[{741, 1641}] = 1;
    expectedDefectPairDistances[{393, 1292}] = 1;
    expectedDefectPairDistances[{567, 1641}] = 1;
    rhombic2::updatePairing(defectPairDistances, defects, 6);
    EXPECT_EQ(defectPairDistances, expectedDefectPairDistances);
}

//------------------------------------------------------------

TEST(joinPairTest, CorrectOutput)
{
    vint syndrome(8*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);

    syndrome[5840] = 1;
    rhombic2::joinPair(387, 1460, syndIndicesZR2, vertexToEdgesR2, syndrome, 6);
    EXPECT_EQ(syndrome, blankSyndrome);

    syndrome[5842] = 1;
    rhombic2::joinPair(1460, -1, syndIndicesZR2, vertexToEdgesR2, syndrome, 6);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(joinDualPairTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint qubitIndices = {};
    for (int i : lowerQubitIndicesR2) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesR2) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesR2) qubitIndices.push_back(i);

    qubits[1158] = 1;
    rhombic2::joinDualPair(196, 283, qubitIndices, cellToFacesR2, qubits, 6);
    EXPECT_EQ(qubits, qubitsExpected);

    qubits[636] = 1;
    rhombic2::joinDualPair(196, -1, qubitIndices, cellToFacesR2, qubits, 6);
    EXPECT_EQ(qubits, qubitsExpected);
}

//------------------------------------------------------------

TEST(pathToTopTest, CorrectOutput)
{
    vint path1 = {1549, 1551};
    vint path2 = {5841, 5842, 2264, 5867};
    vint path3 = {5822, 2244, 1549, 1551};
    vint path4 = {};
    EXPECT_EQ(rhombic2::pathToTop(1461, syndIndicesZR2, vertexToEdgesR2, 6), path1);
    EXPECT_EQ(rhombic2::pathToTop(561, syndIndicesZR2, vertexToEdgesR2, 6), path2);
    //I actually think this one will fail and it will defer upwards too early and get stuck
    //Can either hard-code deferral paths or make pathfinding stuff cleverer
    //i.e. when it loops/turns back you delete those edges from the path
    //and then force it to choose other edges 
    EXPECT_EQ(rhombic2::pathToTop(1455, syndIndicesZR2, vertexToEdgesR2, 6), path3);
    EXPECT_EQ(rhombic2::pathToTop(-1, syndIndicesZR2, vertexToEdgesR2, 6), path4);
}

//------------------------------------------------------------

TEST(syndromeRepairTest, CorrectOutput)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    vint blankSyndrome(8*5*6*6*6, 0);
    vint nonBlankSyndrome(8*5*6*6*6, 0);

    syndrome[5840] = 1;
    syndrome[5841] = 1;
    syndrome[5822] = 1;
    nonBlankSyndrome[5840] = 1;
    nonBlankSyndrome[5841] = 1;
    nonBlankSyndrome[2244] = 1;
    nonBlankSyndrome[1549] = 1;
    nonBlankSyndrome[1551] = 1;
    nonBlankSyndrome[5145] = 1;
    nonBlankSyndrome[5146] = 1;
    nonBlankSyndrome[1568] = 1;
    nonBlankSyndrome[5171] = 1;
    std::map<std::pair<int, int>, int> defectPairDistances;
    defectPairDistances[{387, 1455}] = 3;
    std::map<std::pair<int, int>, int> updatedDefectPairDistances;
    updatedDefectPairDistances[{399, 1467}] = 2;
    rhombic2::syndromeRepair(syndrome, syndIndicesZR2, vertexToEdgesR2, edgeToVerticesR2, defectPairDistances, 6,2,1);
    EXPECT_EQ(syndrome, nonBlankSyndrome);
    EXPECT_EQ(defectPairDistances, updatedDefectPairDistances);
    
    for (int i=0; i < syndrome.size(); i++) syndrome[i] = 0;
    syndrome[5840] = 1;
    syndrome[5841] = 1;
    syndrome[5822] = 1;
    
    std::map<std::pair<int, int>, int> defectPairDistances2;
    defectPairDistances2[{387, 1455}] = 1;
    rhombic2::syndromeRepair(syndrome, syndIndicesZR2, vertexToEdgesR2, edgeToVerticesR2, defectPairDistances2,6,2,1);
    rhombic2::middleBoundarySweepRule(qubits, middleQubitIndicesR2, syndrome, syndIndicesZR2, 
                                middleBoundarySweepVerticesR2, faceToEdgesR2, faceToVerticesR2, 
                                vertexToFacesR2, 6);
    rhombic2::middleSweepRule(qubits, middleQubitIndicesR2, syndrome, syndIndicesZR2, 
                                bulkSweepVerticesR2, faceToEdgesR2, faceToVerticesR2, 
                                vertexToFacesR2, 6);
    rhombic2::upperSweepRule(qubits, upperQubitIndicesR2, syndrome, syndIndicesZR2, 
                                bulkSweepVerticesR2, faceToEdgesR2, faceToVerticesR2, 
                                vertexToFacesR2, 6);
    EXPECT_EQ(syndrome, blankSyndrome);
}

//------------------------------------------------------------

TEST(zErrorDecoderTest, matchPair)
{
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint qubitIndices = {};
    vint xStabs((5*6*6*6)/2, 0);
    for (int i : lowerQubitIndicesR2) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesR2) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesR2) qubitIndices.push_back(i);

    qubits[1158] = 1;
    xStabs[196] = 1;
    xStabs[283] = 1;

    rhombic2::zErrorDecoder(qubits, qubitIndices, cellToFacesR2, xStabs, 6);
    EXPECT_EQ(qubits, qubitsExpected);
}
/*
TEST(zErrorDecoderTest, matchTwoPairs)
{
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint qubitIndices = {};
    vint xStabs((5*6*6*6)/2, 0);
    for (int i : lowerQubitIndicesR2) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesR2) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesR2) qubitIndices.push_back(i);

    qubits[1160] = 1;
    xStabs[196] = 1;
    xStabs[199] = 1;

    qubits[1646] = 1;
    xStabs[277] = 1;
    xStabs[275] = 1;

    rhombic2::zErrorDecoder(qubits, qubitIndices, cellToFacesR2, xStabs, 6);
    EXPECT_EQ(qubits, qubitsExpected);
}
*/
TEST(zErrorDecoderTest, matchToBoundary)
{
    vint qubits(3*5*6*6*6, 0);
    vint qubitsExpected(3*5*6*6*6, 0);
    vint qubitIndices = {};
    vint xStabs((5*6*6*6)/2, 0);
    for (int i : lowerQubitIndicesR2) qubitIndices.push_back(i);
    for (int i : middleQubitIndicesR2) qubitIndices.push_back(i);
    for (int i : upperQubitIndicesR2) qubitIndices.push_back(i);

    qubits[636] = 1;
    xStabs[196] = 1;

    rhombic2::zErrorDecoder(qubits, qubitIndices, cellToFacesR2, xStabs, 6);
    
    EXPECT_EQ(qubits, qubitsExpected);
}

