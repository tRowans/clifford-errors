#include "gtest/gtest.h"
#include "rhombic2.h"

//------------------------------------------------------------

TEST(buildFacesTestR2, CorrectOutput)
{   
    Lattice lattice(6);
    rhombic::r2::buildFaces(lattice);

    vint vertices = {1, 38, 217, 247};
    vint edges = {5, 7, 869, 991};
    pint cells = {2, 37};

    EXPECT_EQ(lattice.faceToVertices[1], vertices);
    EXPECT_EQ(lattice.faceToEdges[1], edges);
    EXPECT_EQ(lattice.faceToCells[1], cells);
    EXPECT_EQ(lattice.vertexToFaces[1][1], 1);
    EXPECT_EQ(lattice.vertexToFaces[38][0], 1);
    EXPECT_EQ(lattice.vertexToFaces[217][1], 1);
    EXPECT_EQ(lattice.vertexToFaces[247][0], 1);
    EXPECT_EQ(lattice.edgeToFaces[7][1], 1);
    EXPECT_EQ(lattice.edgeToFaces[5][0], 1);
    EXPECT_EQ(lattice.edgeToFaces[869][0], 1);
    EXPECT_EQ(lattice.edgeToFaces[991][0], 1);
}

//------------------------------------------------------------

TEST(faceToBaseVertexTestR2, CorrectOutput)
{
    //Matching indices
    EXPECT_EQ(rhombic::r2::faceToBaseVertex(18, 6), 6);
    EXPECT_EQ(rhombic::r2::faceToBaseVertex(22, 6), 6);

    //Non-matching indices
    EXPECT_EQ(rhombic::r2::faceToBaseVertex(0, 6), 1);
    EXPECT_EQ(rhombic::r2::faceToBaseVertex(4, 6), 1);
}

//------------------------------------------------------------
    
TEST(buildVertexToEdgesTestR2, CorrectOutput)
{
    vvint vertexToEdges;
    rhombic::r2::buildVertexToEdges(vertexToEdges, 6);
    
    vint edges1 = {};
    vint edges2 = {172, 173, 174, 175, 867, 870, 889, 1008};
    vint edges3 = {182, 201, 320, 1043};
    vint edges4 = {175, 1036, 1037, 1038};
    
    EXPECT_EQ(vertexToEdges[44], edges1);
    EXPECT_EQ(vertexToEdges[43], edges2);
    EXPECT_EQ(vertexToEdges[260], edges3);
    EXPECT_EQ(vertexToEdges[259], edges4);
}

//------------------------------------------------------------

TEST(buildEdgeToVerticesTestR2, CorrectOutput)
{
    vpint edgeToVertices;
    rhombic::r2::buildEdgeToVertices(edgeToVertices, 6);

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

TEST(buildCellToFacesTestR2, CorrectOutput)
{
    Lattice lattice(6); 
    rhombic::r2::buildFaces(lattice);
    rhombic::r2::buildCellToFaces(lattice.cellToFaces, 
            lattice.vertexToFaces, lattice.faceToVertices, 6);

    EXPECT_EQ(lattice.cellToFaces[44][0], 155);
    EXPECT_EQ(lattice.cellToFaces[44][1], 116);
    EXPECT_EQ(lattice.cellToFaces[44][2], 128);
    EXPECT_EQ(lattice.cellToFaces[44][3], 131);
    EXPECT_EQ(lattice.cellToFaces[44][4], 244);
    EXPECT_EQ(lattice.cellToFaces[44][5], 127);
    EXPECT_EQ(lattice.cellToFaces[44][6], 243);
    EXPECT_EQ(lattice.cellToFaces[44][7], 114);
    EXPECT_EQ(lattice.cellToFaces[44][8], 25);
    EXPECT_EQ(lattice.cellToFaces[44][9], 130);
    EXPECT_EQ(lattice.cellToFaces[44][10], 24);
    EXPECT_EQ(lattice.cellToFaces[44][11], 117);
}


//------------------------------------------------------------

TEST(buildQubitIndicesTestR2, CorrectOutput)
{
    vint outerQubitIndices;
    vint innerQubitIndices;
    vint allQubitIndices;
    rhombic::r2::buildQubitIndices(outerQubitIndices, innerQubitIndices, allQubitIndices, 6);

    vint outerQubitIndicesExpected = {18,108,111,144,147,234,237,327,363};
    vint innerQubitIndicesExpected = {0,19,24,36,37,110,114,117,126,127, 
                                      128,129,130,131,145,146,148,149,150,
                                      153,167,216,218,219,235,236,238,239,
                                      240,243,252,253,254,255,256,257,275,
                                      333,345,346,364,369};

    EXPECT_EQ(outerQubitIndices, outerQubitIndicesExpected);
    EXPECT_EQ(innerQubitIndices, innerQubitIndicesExpected);
}

//------------------------------------------------------------

TEST(buildXSyndIndicesTestR2, CorrectOutput)
{
    vint xSyndIndices = {};
    rhombic::r2::buildXSyndIndices(xSyndIndices, 6);

    vint xSyndIndicesExpected = {37,42,44,49,54,56,72,74,79,84,86,91};
    EXPECT_EQ(xSyndIndices, xSyndIndicesExpected);
}

//------------------------------------------------------------

TEST(buildZSyndIndicesTestR2, CorrectOutput)
{
    vint zSyndIndices = {};
    rhombic::r2::buildZSyndIndices(zSyndIndices, 6);

    vint zSyndIndicesExpected = {25,27,34,53,54,55,172,173,174,175,192,
                                 193,195,202,312,313,315,322,340,341,342,
                                 343,460,480,867,868,870,888,889,890,895,
                                 917,1008,1010,1015,1035,1036,1037,1038,
                                 1057,1155,1156,1158,1176,1177,1178,1183,
                                 1205};
    EXPECT_EQ(zSyndIndices, zSyndIndicesExpected);
}

//------------------------------------------------------------

TEST(buildDefectIndicesTestR2, CorrectOutput)
{
    vint defectIndices;
    rhombic::r2::buildDefectIndices(defectIndices, 6);

    vint defectIndicesExpected = {13,43,85,115,222,223,258,259,294,295};
    EXPECT_EQ(defectIndices, defectIndicesExpected);
}

//------------------------------------------------------------

TEST(buildXLogicalTestR2, CorrectOutput)
{
    Lattice lattice(6);
    rhombic::r2::buildFaces(lattice);
    rhombic::r2::buildQubitIndices(lattice.outerQubitIndices,
                                   lattice.innerQubitIndices,
                                   lattice.allQubitIndices,
                                   lattice.L);
    rhombic::r2::buildXLogical(lattice);

    vint xLogicalExpected = {18,111,147,0,19,24,36,37,117,129,130,148,153};
    EXPECT_EQ(lattice.xLogical, xLogicalExpected);
}

//------------------------------------------------------------

TEST(buildZLogicalTestR2, CorrectOutput)
{
    Lattice lattice(6);
    rhombic::r2::buildFaces(lattice);
    rhombic::r2::buildZLogicals(lattice);

    vvint zLogicalsExpected = {{111,108,327},
                             {0,219,216},
                             {117,114,333},
                             {18,237,234},
                             {129,126,345},
                             {24,243,240},
                             {147,144,363},
                             {36,255,252},
                             {153,150,369}};
    EXPECT_EQ(lattice.zLogicals, zLogicalsExpected);
}


