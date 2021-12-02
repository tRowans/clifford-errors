#include "gtest/gtest.h"
#include "rhombic2.h"

//------------------------------------------------------------

TEST(buildFacesTest, CorrectOutput)
{   
    Lattice lattice;
    rhombic::r2::buildFaces(lattice, 6);

    vint vertices = {1, 38, 217, 247};
    vint edges = {5, 7, 868, 991};
    pint cells = {2, 37};

    EXPECT_EQ(lattice.faceToVertices[1], vertices1);
    EXPECT_EQ(lattice.faceToEdges[1], edges1);
    EXPECT_EQ(lattice.faceToCells[1], cells);
    EXPECT_EQ(lattice.vertexToFaces[1][1], 1);
    EXPECT_EQ(lattice.vertexToFaces[38][0], 1);
    EXPECT_EQ(lattice.vertexToFaces[217][1], 1);
    EXPECT_EQ(lattice.vertexToFaces[247][0], 1);
    EXPECT_EQ(lattice.edgeToFaces[7][1], 1);
    EXPECT_EQ(lattice.edgeToFaces[5][0], 1);
    EXPECT_EQ(lattice.edgeToFaces[868][0], 1);
    EXPECT_EQ(lattice.edgeToFaces[991][0], 1);
}

//------------------------------------------------------------

TEST(faceToBaseVertexTest, CorrectOutput)
{
    //Matching indices
    EXPECT_EQ(faceToBaseVertex(18, 6), 6);
    EXPECT_EQ(faceToBaseVertex(22, 6), 6);

    //Non-matching indices
    EXPECT_EQ(faceToBaseVertex(0, 6), 1);
    EXPECT_EQ(faceToBaseVertex(4, 6), 1);
}

//------------------------------------------------------------
    
TEST(buildVertexToEdgesTest, CorrectOutput)
{
    vvint vertexToEdges;
    rhombic::r2::buildVertexToEdges(vertexToEdges, 6);
    
    vint edges1 = {};
    vint edges2 = {172, 173, 174, 175, 1008, 889, 870, 867};
    vint edges3 = {320, 201, 182, 1043};
    vint edges4 = {1036, 1037, 1038, 175};
    
    EXPECT_EQ(vertexToEdges[44], edges1);
    EXPECT_EQ(vertexToEdges[43], edges2);
    EXPECT_EQ(vertexToEdges[260], edges3);
    EXPECT_EQ(vertexToEdges[259], edges4);
}

//------------------------------------------------------------

TEST(buildEdgeToVerticesTest, CorrectOutput)
{
    vpint edgeToVertices = rhombic::r2::buildEdgeToVertices(6);

    std::pair<int, int> vertices1 = {43, 223};
    std::pair<int, int> vertices2 = {43, 253};
    std::pair<int, int> vertices3 = {43, 258};
    std::pair<int, int> vertices4 = {43, 259};
    
    EXPECT_EQ(edgeToVertices[172], vertices1);
    EXPECT_EQ(edgeToVertices[173], vertices2);
    EXPECT_EQ(edgeToVertices[174], vertices3);
    EXPECT_EQ(edgeToVertices[175], vertices4);
}

//------------------------------------------------------------

TEST(buildCellToFacesTest, CorrectOutput)
{
    Lattice lattice; 
    rhombic::r2::buildFaces(lattice, 6);
    vvint cellToFaces = rhombic::r2::buildCellToFaces(vertexToFaces, faceToVertices, 6);

    EXPECT_EQ(cellToFaces[44][0], 155);
    EXPECT_EQ(cellToFaces[44][1], 116);
    EXPECT_EQ(cellToFaces[44][2], 122);
    EXPECT_EQ(cellToFaces[44][3], 125);
    EXPECT_EQ(cellToFaces[44][4], 244);
    EXPECT_EQ(cellToFaces[44][5], 121);
    EXPECT_EQ(cellToFaces[44][6], 243);
    EXPECT_EQ(cellToFaces[44][7], 114);
    EXPECT_EQ(cellToFaces[44][8], 25);
    EXPECT_EQ(cellToFaces[44][9], 124);
    EXPECT_EQ(cellToFaces[44][10], 24);
    EXPECT_EQ(cellToFaces[44][11], 117);
}


//------------------------------------------------------------

TEST(buildQubitIndicesTest, CorrectOutput)
{
    Lattice lattice;
    rhombic::r2::buildQubitIndices(lattice, 6);

    vint outerQubitIndices = {18, 108, 111, 144, 147, 234, 237, 327, 363};
    vint innerQubitIndices = {0, 19, 24, 36, 37, 110, 114, 117, 126, 127, 
                              128, 129, 130, 131, 145, 146, 148, 149, 150, 
                              153, 167, 216, 218, 219, 235, 236, 238, 239, 
                              240, 243, 252, 253, 254, 255, 256, 257, 275, 
                              333, 345, 346, 364, 369};

    EXPECT_EQ(lattice.outerQubitIndices, outerQubitIndicesExpected);
    EXPECT_EQ(lattice.innerQubitIndices, innerQubitIndicesExpected);
}

//------------------------------------------------------------

TEST(buildXSyndIndicesTest, CorrectOutput)
{
    vint xSyndIndices = {};
    rhombic::r2::buildXSyndIndices(xSyndIndices, 6);

    vint xSyndIndicesExpected = {37, 42, 44, 49, 54, 56, 72, 74, 79, 84, 86, 91};
    EXPECT_EQ(xSyndIndices, xSyndIndicesExpected);
}

//------------------------------------------------------------

TEST(buildZSyndIndicesTest, CorrectOutput)
{
    vint zSyndIndices = {};
    rhombic::r2::buildZSyndIndices(zSyndIndices, 6);

    vint zSyndIndicesExpected = {25, 867, 890, 868, 27, 889, 34, 172, 890, 888,
                                 53, 895, 54, 192, 55, 917, 1008, 173, 193, 174,
                                 175, 1036, 195, 1056, 202, 1010, 1015, 312, 1037,
                                 1035, 1038, 1057, 340, 313, 1156, 315, 1176, 341,
                                 322, 342, 343, 1155, 1158, 1177, 460, 1178, 1183,
                                 480, 1205};
    EXPECT_EQ(zSyndIndices, zSyndIndicesExpected);
}

//------------------------------------------------------------

TEST(buildLogicalsTest, CorrectOutput)
{
    Lattice lattice;
    buildLogicals(lattice, 6);

    vint xLogicalExpected = {0, 18, 19, 24, 36, 40, 111, 117, 129, 130, 147, 148, 153};
    vint zLogicalExpected = {108, 111, 327};
    EXPECT_EQ(lattice.xLogical, xLogicalExpected);
    EXPECT_EQ(lattice.zLogical, zLogicalExpected);
}
