#include "gtest/gtest.h"
#include "cubic.h"

TEST(coordToIndexTest, HandlesExpectedInput)
{
    cubic::coord c = {4,4,1};
    EXPECT_EQ(cubic::coordToIndex(c, 6), 64);
} 
TEST(coordToIndexTest, HandlesOutOfRangeInput)
{
    cubic::coord c = {-1,0,0};
    EXPECT_THROW(cubic::coordToIndex(c, 6), std::invalid_argument);
    c.xi[0] = 6;
    EXPECT_THROW(cubic::coordToIndex(c, 6), std::invalid_argument);
}

//------------------------------------------------------------

TEST(indexToCoordTest, HandlesExpectedInput)
{
    cubic::coord c = cubic::indexToCoord(64, 6);
    EXPECT_EQ(c.xi[0], 4);
    EXPECT_EQ(c.xi[1], 4);
    EXPECT_EQ(c.xi[2], 1);
}
TEST(indexToCoordTest, HandlesOutOfRangeInput)
{
    EXPECT_THROW(cubic::indexToCoord(-1, 6), std::invalid_argument);
    EXPECT_THROW(cubic::indexToCoord(216, 6), std::invalid_argument);
}

//------------------------------------------------------------

TEST(neighTest, CorrectOutput)
{
    //129 = {3,3,3}
    EXPECT_EQ(cubic::neigh(129, 0, 1, 6), 130);
    EXPECT_EQ(cubic::neigh(129, 1, 1, 6), 135);
    EXPECT_EQ(cubic::neigh(129, 2, 1, 6), 165);
    EXPECT_EQ(cubic::neigh(129, 0, -1, 6), 128);
    EXPECT_EQ(cubic::neigh(129, 1, -1, 6), 123);
    EXPECT_EQ(cubic::neigh(129, 2, -1, 6), 93);
}

//------------------------------------------------------------

TEST(edgeIndexTest, HandlesExpectedInput)
{
    EXPECT_EQ(cubic::edgeIndex(129, 0, 1, 6), 387);
    EXPECT_EQ(cubic::edgeIndex(129, 1, 1, 6), 388);
    EXPECT_EQ(cubic::edgeIndex(129, 2, 1, 6), 389);
    EXPECT_EQ(cubic::edgeIndex(129, 0, -1, 6), 384);
    EXPECT_EQ(cubic::edgeIndex(129, 1, -1, 6), 370);
    EXPECT_EQ(cubic::edgeIndex(129, 2, -1, 6), 281);
}
TEST(edgeIndexTest, HandlesInvalidInput)
{
    EXPECT_THROW(cubic::edgeIndex(0, 3, 1, 5), std::invalid_argument);
}

//------------------------------------------------------------

TEST(buildFaceToEdgesTest, CorrectOutput)
{   
    //Not worth checking the whole thing
    //Just check this for the three directions of face and check size is right
    //Same for other functions of this type
    vvint faceToEdges; 
    cubic::buildFaceToEdges(faceToEdges, 6);
    vint edges1 = {387, 388, 391, 405};
    vint edges2 = {387, 389, 392, 495};
    vint edges3 = {388, 389, 407, 496};
    EXPECT_EQ(faceToEdges[387], edges1);
    EXPECT_EQ(faceToEdges[388], edges2);
    EXPECT_EQ(faceToEdges[389], edges3);
    EXPECT_EQ(faceToEdges.size(), 3*6*6*6);
}

//------------------------------------------------------------

TEST(buildEdgeToFaces, CorrectOutput)
{
    vvint edgeToFaces;
    cubic::buildEdgeToFaces(edgeToFaces, 6);
    vint faces1 = {387, 388, 369, 280};
    vint faces2 = {387, 389, 384, 281};
    vint faces3 = {389, 388, 371, 385};
    EXPECT_EQ(edgeToFaces[387], faces1);
    EXPECT_EQ(edgeToFaces[388], faces2);
    EXPECT_EQ(edgeToFaces[389], faces3); 
    EXPECT_EQ(edgeToFaces.size(), 3*6*6*6); 
}

//------------------------------------------------------------

TEST(buildVertexToEdgesTest, CorrectOutput)
{
    vvint vertexToEdges;
    cubic::buildVertexToEdges(vertexToEdges, 6);
    vint edges = {387, 388, 389, 384, 370, 281};
    EXPECT_EQ(vertexToEdges[129], edges);
    EXPECT_EQ(vertexToEdges.size(), 6*6*6);
}

//------------------------------------------------------------

TEST(buildEdgeToVerticesTest, CorrectOutput)
{
    vpint edgeToVertices;
    cubic::buildEdgeToVertices(edgeToVertices, 6);
    std::pair<int,int> vertices1 = {129, 130};
    std::pair<int,int> vertices2 = {129, 135};
    std::pair<int,int> vertices3 = {129, 165};
    EXPECT_EQ(edgeToVertices[387], vertices1);
    EXPECT_EQ(edgeToVertices[388], vertices2);
    EXPECT_EQ(edgeToVertices[389], vertices3);
    EXPECT_EQ(edgeToVertices.size(), 3*6*6*6);
}

//------------------------------------------------------------

TEST(buildCellToFacesTest, CorrectOutput)
{
    vvint cellToFaces;
    cubic::buildCellToFaces(cellToFaces, 6);
    vint faces = {171, 172, 173, 279, 190, 176};
    EXPECT_EQ(cellToFaces[57], faces);
    EXPECT_EQ(cellToFaces.size(), 6*6*6);
}

//------------------------------------------------------------

TEST(buildFaceToCellsTest, CorrectOutput)
{
    vpint faceToCells;
    cubic::buildFaceToCells(faceToCells, 6);
    pint cells1 = {57, 93};
    pint cells2 = {87, 93};
    pint cells3 = {92, 93};
    EXPECT_EQ(faceToCells[279], cells1);
    EXPECT_EQ(faceToCells[280], cells2);
    EXPECT_EQ(faceToCells[281], cells3);
    EXPECT_EQ(faceToCells.size(), 3*6*6*6);
}

//------------------------------------------------------------

TEST(buildFaceToVerticesTest, CorrectOutput)
{
    vvint faceToVertices;
    cubic::buildFaceToVertices(faceToVertices, 6);
    vint vertices1 = {129, 130, 135, 136};
    vint vertices2 = {129, 130, 165, 166};
    vint vertices3 = {129, 135, 165, 171};
    EXPECT_EQ(faceToVertices[387], vertices1);
    EXPECT_EQ(faceToVertices[388], vertices2);
    EXPECT_EQ(faceToVertices[389], vertices3);
    EXPECT_EQ(faceToVertices.size(), 3*6*6*6);
}

//------------------------------------------------------------

TEST(buildQubitIndicesTest, CorrectOutput)
{
    vint outerQubitIndicesExpected = {2,5,8,19,20,22,23,26,37,38,40,41,44};
    vint innerQubitIndicesExpected = {108,110,111,113,116,126,127,128,129,
                                      130,131,134,144,145,146,147,148,149,
                                      152,216,218,219,221,224,234,235,236,
                                      237,238,239,242,252,253,254,255,256,
                                      257,260};
    vint outerQubitIndices;
    vint innerQubitIndices;
    cubic::buildQubitIndices(outerQubitIndices, innerQubitIndices, 6);
    EXPECT_EQ(outerQubitIndices, outerQubitIndicesExpected);
    EXPECT_EQ(innerQubitIndices, innerQubitIndicesExpected);
}

//------------------------------------------------------------

TEST(buildXSyndIndicesTest, CorrectOutput)
{
    vint xSyndIndicesExpected = {0,1,6,7,12,13,36,37,42,
                                 43,48,49,72,73,78,79,84,85};
    vint xSyndIndices;
    cubic::buildXSyndIndices(xSyndIndices, 6);
    EXPECT_EQ(xSyndIndices, xSyndIndicesExpected);
}

//------------------------------------------------------------

TEST(buildZSyndIndicesTest, CorrectOutput)
{
    vint zSyndIndicesExpected = {20,23,26,38,41,44,109,112,115,126,127,
                                 128,129,130,131,133,134,144,145,146,147,
                                 148,149,151,152,217,220,223,234,235,236,
                                 237,238,239,241,242,252,253,254,255,256,
                                 257,259,260};
    vint zSyndIndices;
    cubic::buildZSyndIndices(zSyndIndices, 6);
    EXPECT_EQ(zSyndIndices, zSyndIndicesExpected);
}

//------------------------------------------------------------

TEST(buildDefectIndicesTest, CorrectOutput)
{
    vint defectIndicesExpected = {42, 43, 44, 48, 49, 50, 78, 79, 80, 84, 85, 86};
    vint defectIndices;
    cubic::buildDefectIndices(defectIndices, 6);
    EXPECT_EQ(defectIndices, defectIndicesExpected);
}

//------------------------------------------------------------

TEST(buildXLogicalTest, CorrectOutput)
{
    vint xLogicalExpected = {2,20,38,110,128,146,218,236,254};
    vint xLogical;
    cubic::buildXLogical(xLogical, 6);
    EXPECT_EQ(xLogical, xLogicalExpected);
}

//------------------------------------------------------------

TEST(buildZLogicalTest, CorrectOutput)
{
    vint zLogicalExpected = {2,5,8};
    vint zLogical;
    cubic::buildZLogical(zLogical, 6);
    EXPECT_EQ(zLogical, zLogicalExpected);
}
