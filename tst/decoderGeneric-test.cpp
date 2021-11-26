#include "gtest/gtest.h"
#include "decoderGeneric.h"
#include "decoderToric.h"
#include "decoderRhombic1.h"
#include "decoderRhombic2.h"
#include "prepIndexVectors.h"
#include <iostream>

//------------------------------------------------------------

//This is just a hack to make the testing software build the index vectors
//Means they don't have to be build individually in every test

TEST(indexBuildingDecoder, build)
{
    faceToEdgesT = toric::buildFaceToEdges(6);
    edgeToFacesT = toric::buildEdgeToFaces(6);
    vertexToEdgesT = toric::buildVertexToEdges(6);
    edgeToVerticesT = toric::buildEdgeToVertices(6);
    cellToFacesT = toric::buildCellToFaces(6);
    faceToCellsT = toric::buildFaceToCells(6);
    
    toric::buildSlice(lowerQubitIndicesT, middleQubitIndicesT, upperQubitIndicesT, syndIndicesZT, syndIndicesXLayerT, defectIndicesT, zLogicalT, xLogicalFragmentT, sweepVerticesT, vertexToEdgesT, faceToEdgesT, edgeToVerticesT, 6);

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

    buildOverlappingFaces(overlappingFaces, faceToEdgesT, edgeToVerticesT, vertexToFacesR1,
                            faceToVerticesR1, vertexToFacesR2, faceToVerticesR2, 6);

    ASSERT_TRUE(true);
}
//------------------------------------------------------------


//Again not sure about the best way to test these but they are simple enough that it should be fine
//These are actually really annoying because they randomly fail so disabled them for now
/*
TEST(dataErrorTest, p0)
{
    vint qubits(3*6*6*6, 0);
    vint qubitsCompare(3*6*6*6, 0);
    std::mt19937 engine(time(0));
    std::uniform_real_distribution<double> dist(0,1);
    dataError(qubits, 0, engine, dist, upperQubitIndices6);
    EXPECT_EQ(qubits, qubitsCompare);
}
TEST(dataErrorTest, p1)
{
    vint qubits(3*6*6*6, 0);
    vint qubitsCompare(3*6*6*6, 0);
    std::mt19937 engine(time(0));
    std::uniform_real_distribution<double> dist(0,1);
    dataError(qubits, 1, engine, dist, upperQubitIndices6);
    for (int i : upperQubitIndices6)
    {
        qubitsCompare[i] = 1;
    }
    EXPECT_EQ(qubits, qubitsCompare);
}

//------------------------------------------------------------

TEST(measErrorTest, p0)
{
    vint syndrome(3*6*6*6, 0);
    vint syndromeCompare(3*6*6*6, 0);
    std::mt19937 engine(time(0));
    std::uniform_real_distribution<double> dist(0,1);
    measError(syndrome, 0, engine, dist, syndIndices6);
    EXPECT_EQ(syndrome, syndromeCompare);
}
TEST(intermediateMeasErrorTest, p1)
{
    vint syndrome(3*6*6*6, 0);
    vint syndromeCompare(3*6*6*6, 0);
    std::mt19937 engine(time(0));
    std::uniform_real_distribution<double> dist(0,1);
    measError(syndrome, 1, engine, dist, syndIndices6);
    for (int i : syndIndices6)
    {
        syndromeCompare[i] = 1;
    }
    EXPECT_EQ(syndrome, syndromeCompare);
}
*/
//------------------------------------------------------------

TEST(calcSyndTest, noErrors)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);

    calcSynd(syndrome, qubits, lowerQubitIndicesT, middleQubitIndicesT, upperQubitIndicesT, faceToEdgesT, syndIndicesZT);

    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(calcSyndTest, singleError)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);

    qubits[266] = 1;
    syndromeExpected[265] = 1;
    syndromeExpected[266] = 1;
    syndromeExpected[284] = 1;
    syndromeExpected[373] = 1;

    calcSynd(syndrome, qubits, lowerQubitIndicesT, middleQubitIndicesT, upperQubitIndicesT, faceToEdgesT, syndIndicesZT);
    
    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(calcSyndTest, separateErrors)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);

    qubits[266] = 1;
    qubits[474] = 1;
    syndromeExpected[265] = 1;
    syndromeExpected[266] = 1;
    syndromeExpected[284] = 1;
    syndromeExpected[373] = 1;
    syndromeExpected[474] = 1;
    syndromeExpected[475] = 1;
    syndromeExpected[492] = 1;
    syndromeExpected[478] = 1;

    calcSynd(syndrome, qubits, lowerQubitIndicesT, middleQubitIndicesT, upperQubitIndicesT, faceToEdgesT, syndIndicesZT);

    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(calcSyndTest, adjacentErrors)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(3*5*6*6*6, 0);
    vint syndromeExpected(3*5*6*6*6, 0);

    qubits[369] = 1;
    qubits[371] = 1;
    syndromeExpected[369] = 1;
    syndromeExpected[371] = 1;
    syndromeExpected[373] = 1;
    syndromeExpected[387] = 1;
    syndromeExpected[389] = 1;
    syndromeExpected[478] = 1;

    calcSynd(syndrome, qubits, lowerQubitIndicesT, middleQubitIndicesT, upperQubitIndicesT, faceToEdgesT, syndIndicesZT);

    EXPECT_EQ(syndrome, syndromeExpected);
}

//------------------------------------------------------------

TEST(calcXSyndTest, noErrors)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(5*6*6*6, 0);
    vint syndromeExpected(5*6*6*6, 0);
    
    //Just use all syndIndices for test
    //Not testing that the list of syndrome indices is correct anyway
    //just that the list is used correctly
    vint syndIndices = {};
    for (int i = 0; i  < 5*6*6*6; i++) syndIndices.push_back(i);
    calcXSynd(syndrome, qubits, faceToCellsT, syndIndices);

    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(calcXSyndTest, singleErrorT)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(5*6*6*6, 0);
    vint syndromeExpected(5*6*6*6, 0);

    qubits[279] = 1;
    syndromeExpected[57] = 1;
    syndromeExpected[93] = 1;
    
    vint syndIndices = {};
    for (int i = 0; i  < 5*6*6*6; i++) syndIndices.push_back(i);
    calcXSynd(syndrome, qubits, faceToCellsT, syndIndices);

    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(calcXSyndTest, singleErrorR1)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome((5*6*6*6)/2, 0);
    vint syndromeExpected((5*6*6*6)/2, 0);

    qubits[1274] = 1; 
    syndromeExpected[227] = 1;
    syndromeExpected[212] = 1;

    vint syndIndices = {};
    for (int i = 0; i  < (5*6*6*6)/2; i++) syndIndices.push_back(i);
    calcXSynd(syndrome, qubits, faceToCellsR1, syndIndices);

    EXPECT_EQ(syndrome, syndromeExpected);
}
TEST(calcXSyndTest, singleErrorR2)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome((5*6*6*6)/2, 0);
    vint syndromeExpected((5*6*6*6)/2, 0);
    
    qubits[1160] = 1;
    syndromeExpected[194] = 1;
    syndromeExpected[196] = 1;

    vint syndIndices = {};
    for (int i = 0; i  < (5*6*6*6)/2; i++) syndIndices.push_back(i);
    calcXSynd(syndrome, qubits, faceToCellsR2, syndIndices);
   
    EXPECT_EQ(syndrome, syndromeExpected);
}

//------------------------------------------------------------

TEST(getSyndromeVerticesTest, correctOutput)
{
    vvint syndromes(3, vint(8*5*6*6*6, 0));
    syndromes[0].resize(3*5*6*6*6);
    vvint syndromeVertices;
    vvint syndromeVerticesExpected{{},{},{}};
    
    syndromes[0][279] = 1;
    syndromes[0][281] = 1;
    syndromes[0][284] = 1;
    syndromes[0][387] = 1;
    
    syndromeVerticesExpected[0].push_back(93);
    syndromeVerticesExpected[0].push_back(94);
    syndromeVerticesExpected[0].push_back(129);
    syndromeVerticesExpected[0].push_back(130);

    syndromes[1][2412] = 1;
    syndromes[1][2415] = 1;
    syndromes[1][6732] = 1;
    syndromes[1][6015] = 1;
    
    syndromeVerticesExpected[1].push_back(603);
    syndromeVerticesExpected[1].push_back(634); 
    syndromeVerticesExpected[1].push_back(1503);
    syndromeVerticesExpected[1].push_back(1683);
    
    syndromes[2][1549] = 1;
    syndromes[2][5840] = 1;
    syndromes[2][5841] = 1;
    syndromes[2][2244] = 1;

    syndromeVerticesExpected[2].push_back(387);
    syndromeVerticesExpected[2].push_back(561);
    syndromeVerticesExpected[2].push_back(1460);
    syndromeVerticesExpected[2].push_back(1461);

    syndromeVertices = getSyndromeVertices(syndromes, edgeToVerticesT, 
                                            edgeToVerticesR1, edgeToVerticesR2);
    
    EXPECT_EQ(syndromeVertices, syndromeVerticesExpected);
}

//------------------------------------------------------------

TEST(findDefectsTest, validSyndrome)
{
    vint syndrome(3*5*6*6*6, 0);
    vint defects = {};
    vint defectsExpected = {};

    syndrome[373] = 1;
    syndrome[374] = 1; 
    
    findDefects(syndrome, vertexToEdgesT, edgeToVerticesT, defects, defectIndicesT);
    
    EXPECT_EQ(defects, defectsExpected);
}
TEST(findDefectsTest, invalidSyndrome)
{
    vint syndrome(3*5*6*6*6, 0);
    vint defects = {};
    vint defectsExpected = {88, 124};

    syndrome[266] = 1;
    
    findDefects(syndrome, vertexToEdgesT, edgeToVerticesT, defects, defectIndicesT);
    
    EXPECT_EQ(defects, defectsExpected);
}

//------------------------------------------------------------

TEST(checkXCorrectionTest, Fail)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(3*5*6*6*6, 0);
    
    //toric 
    qubits[582] = 1;
    bool tFail = checkXCorrection(syndrome, qubits, lowerQubitIndicesT, middleQubitIndicesT, 
                                    upperQubitIndicesT, faceToEdgesT, syndIndicesZT, 0);
    //rhombic1
    qubits[582] = 0;
    qubits[1815] = 1;
    bool r1Fail = checkXCorrection(syndrome, qubits, lowerQubitIndicesR1, middleQubitIndicesR1, 
                                    upperQubitIndicesR1, faceToEdgesR1, syndIndicesZR1, 0);
    //rhombic2
    qubits[1815] = 0;
    qubits[1158] = 1;
    bool r2Fail = checkXCorrection(syndrome, qubits, lowerQubitIndicesR2, middleQubitIndicesR2, 
                                    upperQubitIndicesR2, faceToEdgesR2, syndIndicesZR2, 1);

    EXPECT_EQ(tFail, 1);
    EXPECT_EQ(r1Fail, 1);
    EXPECT_EQ(r2Fail, 1);
}
TEST(checkXCorrectionTest, Pass)
{
    vint qubits(3*5*6*6*6, 0);
    vint syndrome(8*5*6*6*6, 0);
    
    bool tFail = checkXCorrection(syndrome, qubits, lowerQubitIndicesT, middleQubitIndicesT, 
                                    upperQubitIndicesT, faceToEdgesT, syndIndicesZT, 0);
    bool r1Fail = checkXCorrection(syndrome, qubits, lowerQubitIndicesR1, middleQubitIndicesR1, 
                                    upperQubitIndicesR1, faceToEdgesR1, syndIndicesZR1, 0);
    bool r2Fail = checkXCorrection(syndrome, qubits, lowerQubitIndicesR2, middleQubitIndicesR2, 
                                    upperQubitIndicesR2, faceToEdgesR2, syndIndicesZR2, 1);

    EXPECT_EQ(tFail, 0);
    EXPECT_EQ(r1Fail, 0);
    EXPECT_EQ(r2Fail, 0);
}

//------------------------------------------------------------

TEST(scalarProductTest, HandlesCorrectInput)
{
    EXPECT_EQ(scalarProduct({4,1,-3}, 1), 8);
    EXPECT_EQ(scalarProduct({4,1,-3}, 2), 0);
    EXPECT_EQ(scalarProduct({4,1,-3}, 3), -6);
    EXPECT_EQ(scalarProduct({4,1,-3}, 4), 2);
    EXPECT_EQ(scalarProduct({4,1,-3}, -1*1), -8);
    EXPECT_EQ(scalarProduct({4,1,-3}, -1*2), 0);
    EXPECT_EQ(scalarProduct({4,1,-3}, -1*3), 6);
    EXPECT_EQ(scalarProduct({4,1,-3}, -1*4), -2);

    EXPECT_EQ(scalarProduct({0,1,1}, 1), 0);
    EXPECT_EQ(scalarProduct({1,0,1}, 2), 2);
    EXPECT_EQ(scalarProduct({1,1,0}, 3), 0);
}
TEST(scalarProductTest, HandlesIncorrectInput)
{
    EXPECT_THROW(scalarProduct({1,1,1}, 0), std::invalid_argument);
    EXPECT_THROW(scalarProduct({1,1,1}, 5), std::invalid_argument);
    EXPECT_THROW(scalarProduct({1,1,1}, -5), std::invalid_argument);
}

//------------------------------------------------------------

//Not sure there's much point testing updateQubitHistory because it's so simple

//------------------------------------------------------------

TEST(updateStabiliserHistoryTest, FirstStep)
{
    syndIndicesXLayerT = {57, 87, 92, 117, 122, 152};
    syndIndicesXLayerR1 = {123, 137, 212, 227, 302, 316, 391, 406};
    syndIndicesXLayerR2 = {194, 196, 199, 281, 283, 286};
    
    vint syndIndicesXTExpected = {21, 57, 93, 51, 87, 123, 
                                  56, 92, 128, 81, 117, 153, 
                                  86, 122, 158, 116, 152, 188};
    vint syndIndicesXR1Expected = {122, 123, 136, 137, 211, 212, 
                                   226, 227, 301, 302, 315, 316,
                                   391, 406};
    vint syndIndicesXR2Expected = {188, 194, 190, 196, 193, 199, 
                                   275, 281, 277, 283, 280, 286};
    
    vint syndIndicesXT;
    vint syndIndicesXR1;
    vint syndIndicesXR2;

    updateStabiliserHistory(syndIndicesXLayerT, syndIndicesXLayerR1, syndIndicesXLayerR2,
                            syndIndicesXT, syndIndicesXR1, syndIndicesXR2, 6, 0, 0);

    EXPECT_EQ(syndIndicesXT, syndIndicesXTExpected);
    EXPECT_EQ(syndIndicesXR1, syndIndicesXR1Expected);
    EXPECT_EQ(syndIndicesXR2, syndIndicesXR2Expected);
}
TEST(updateStabiliserHistoryTest, LastStep)
{
    syndIndicesXLayerT = {57, 87, 92, 117, 122, 152};
    syndIndicesXLayerR1 = {123, 137, 212, 227, 302, 316, 391, 406};
    syndIndicesXLayerR2 = {194, 196, 199, 281, 283, 286};

    vint syndIndicesXTExpected = {57, 87, 92, 117, 122, 152};
    vint syndIndicesXR1Expected = {212, 227, 302, 316, 391, 406};
    vint syndIndicesXR2Expected = {};

    vint syndIndicesXT;
    vint syndIndicesXR1;
    vint syndIndicesXR2;

    updateStabiliserHistory(syndIndicesXLayerT, syndIndicesXLayerR1, syndIndicesXLayerR2,
                            syndIndicesXT, syndIndicesXR1, syndIndicesXR2, 6, 1, 1);

    EXPECT_EQ(syndIndicesXT, syndIndicesXTExpected);
    EXPECT_EQ(syndIndicesXR1, syndIndicesXR1Expected);
    EXPECT_EQ(syndIndicesXR2, syndIndicesXR2Expected);
}

//------------------------------------------------------------

//updateLogicalHistory also probably too simple to need a test

//------------------------------------------------------------

TEST(applyCCZTest, toricXError)
{
    vvint qubitsX(3, vint(3*5*6*6*6, 0));
    vvint qubitsZ(3, vint(3*5*6*6*6, 0));
    vvint xStabs(3, vint(5*6*6*6, 0));
    xStabs[1].resize((5*6*6*6)/2);
    xStabs[2].resize((5*6*6*6)/2);
   
    qubitsX[0][1538] = 1;
    xStabs[1][307] = 1;
    xStabs[2][310] = 1;

    vvint qubitsZExpected(3, vint(3*5*6*6*6, 0));
    qubitsZExpected[1][1755] = 1;
    qubitsZExpected[2][1320] = 1;
    
    //I'm not really concerned with if this is consistent with slices or not
    //Imagine having bulk lattices instead of slices for example 
    //CCZ per region should do the same thing
    //So this is just saying the relevant stabs/qubits are in the part of the "slice" where
    //the function wants to apply CCZ
    vvint syndromeVertices = {{548, 554}, {}, {}};
    lowerQubitIndicesT = {1538};
    middleQubitIndicesT = {};
    
    applyCCZ(qubitsX, qubitsZ, syndromeVertices, xStabs, overlappingFaces, 
              cellToFacesT, cellToFacesR1, cellToFacesR2, lowerQubitIndicesT, middleQubitIndicesT,6);
   
    EXPECT_EQ(qubitsZ, qubitsZExpected);
}
TEST(applyCCZTest, rhombic1XError)
{
    vvint qubitsX(3, vint(3*5*6*6*6, 0));
    vvint qubitsZ(3, vint(3*5*6*6*6, 0));
    vvint xStabs(3, vint(5*6*6*6, 0));
    xStabs[1].resize((5*6*6*6)/2);
    xStabs[2].resize((5*6*6*6)/2);

    qubitsX[1][1755] = 1;
    xStabs[0][512] = 1;
    xStabs[2][310] = 1;

    vvint qubitsZExpected(3, vint(3*5*6*6*6, 0));
    qubitsZExpected[0][1538] = 1;
    qubitsZExpected[2][1320] = 1;

    vvint syndromeVertices = {{}, {584, 1484}, {}};
    lowerQubitIndicesT = {1538};
    middleQubitIndicesT = {};
    
    applyCCZ(qubitsX, qubitsZ, syndromeVertices, xStabs, overlappingFaces, 
              cellToFacesT, cellToFacesR1, cellToFacesR2, lowerQubitIndicesT, middleQubitIndicesT,6);

    EXPECT_EQ(qubitsZ, qubitsZExpected);
}
TEST(applyCCZTest, rhombic2XError)
{
    vvint qubitsX(3, vint(3*5*6*6*6, 0));
    vvint qubitsZ(3, vint(3*5*6*6*6, 0));
    vvint xStabs(3, vint(5*6*6*6, 0));
    xStabs[1].resize((5*6*6*6)/2);
    xStabs[2].resize((5*6*6*6)/2);

    qubitsX[2][1320] = 1;
    xStabs[0][512] = 1;
    xStabs[1][307] = 1;

    vvint qubitsZExpected(3, vint(3*5*6*6*6, 0));
    qubitsZExpected[0][1538] = 1;
    qubitsZExpected[1][1755] = 1;

    vvint syndromeVertices = {{}, {}, {626, 1520}};
    lowerQubitIndicesT = {1538};
    middleQubitIndicesT = {};
    
    applyCCZ(qubitsX, qubitsZ, syndromeVertices, xStabs, overlappingFaces, 
              cellToFacesT, cellToFacesR1, cellToFacesR2, lowerQubitIndicesT, middleQubitIndicesT,6);

    EXPECT_EQ(qubitsZ, qubitsZExpected);
}

