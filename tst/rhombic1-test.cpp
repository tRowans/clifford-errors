#include "gtest/gtest.h"
#include "rhombic1.h"

//------------------------------------------------------------

TEST(buildFacesTestR1, CorrectOutput)
{   
    Lattice lattice(6);
    rhombic::r1::buildFaces(lattice);
    
    vint vertices = {0, 37, 216, 246};
    vint edges = {1, 3, 649, 741};
    pint cells = {1, 36};

    EXPECT_EQ(lattice.faceToVertices[1], vertices);
    EXPECT_EQ(lattice.faceToEdges[1], edges);
    EXPECT_EQ(lattice.faceToCells[1], cells);
    EXPECT_EQ(lattice.vertexToFaces[0][1], 1);
    EXPECT_EQ(lattice.vertexToFaces[37][0], 1);
    EXPECT_EQ(lattice.vertexToFaces[216][1], 1);
    EXPECT_EQ(lattice.vertexToFaces[246][0], 1);
    EXPECT_EQ(lattice.edgeToFaces[3][1], 1);
    EXPECT_EQ(lattice.edgeToFaces[1][0], 1);
    EXPECT_EQ(lattice.edgeToFaces[649][0], 1);
    EXPECT_EQ(lattice.edgeToFaces[741][0], 1);
}

//------------------------------------------------------------

TEST(faceToBaseVertexTestR1, CorrectOutput)
{
    //Matching indices
    EXPECT_EQ(rhombic::r1::faceToBaseVertex(132, 6), 44);
    EXPECT_EQ(rhombic::r1::faceToBaseVertex(136, 6), 44);

    //Non-matching indices
    EXPECT_EQ(rhombic::r1::faceToBaseVertex(108, 6), 37);
    EXPECT_EQ(rhombic::r1::faceToBaseVertex(112, 6), 37);
}

//------------------------------------------------------------

TEST(buildVertexToEdgesTestR1, CorrectOutput)
{
    vvint vertexToEdges;
    rhombic::r1::buildVertexToEdges(vertexToEdges, 6);
    
    vint edges1 = {176, 177, 178, 179, 1012, 893, 874, 871};
    vint edges2 = {};
    vint edges3 = {1040, 1041, 1042, 179};
    vint edges4 = {316, 197, 178, 1039};
    
    EXPECT_EQ(vertexToEdges[44], edges1);
    EXPECT_EQ(vertexToEdges[43], edges2);
    EXPECT_EQ(vertexToEdges[260], edges3);
    EXPECT_EQ(vertexToEdges[259], edges4);
}

//------------------------------------------------------------

TEST(buildEdgeToVerticesTestR1, CorrectOutput)
{
    vpint edgeToVertices; 
    rhombic::r1::buildEdgeToVertices(edgeToVertices, 6);
    
    pint vertices1 = {44, 224};
    pint vertices2 = {44, 254};
    pint vertices3 = {44, 259};
    pint vertices4 = {44, 260};

    EXPECT_EQ(edgeToVertices[176], vertices1);
    EXPECT_EQ(edgeToVertices[177], vertices2);
    EXPECT_EQ(edgeToVertices[178], vertices3);
    EXPECT_EQ(edgeToVertices[179], vertices4);
}

//------------------------------------------------------------

TEST(buildCellToFacesTestR1, CorrectOutput)
{
    Lattice lattice(6);
    rhombic::r1::buildFaces(lattice);
    rhombic::r1::buildCellToFaces(lattice.cellToFaces, 
            lattice.vertexToFaces, lattice.faceToVertices, 6);
    
    EXPECT_EQ(lattice.cellToFaces[43][0], 149);
    EXPECT_EQ(lattice.cellToFaces[43][1], 110);
    EXPECT_EQ(lattice.cellToFaces[43][2], 128);
    EXPECT_EQ(lattice.cellToFaces[43][3], 131);
    EXPECT_EQ(lattice.cellToFaces[43][4], 238);
    EXPECT_EQ(lattice.cellToFaces[43][5], 127);
    EXPECT_EQ(lattice.cellToFaces[43][6], 237);
    EXPECT_EQ(lattice.cellToFaces[43][7], 108);
    EXPECT_EQ(lattice.cellToFaces[43][8], 19);
    EXPECT_EQ(lattice.cellToFaces[43][9], 130);
    EXPECT_EQ(lattice.cellToFaces[43][10], 18);
    EXPECT_EQ(lattice.cellToFaces[43][11], 111);
} 


//------------------------------------------------------------

TEST(buildQubitIndicesTestR1, CorrectOutput)
{
    Lattice lattice(6);
    rhombic::r1::buildQubitIndices(lattice.outerQubitIndices, 
                                    lattice.innerQubitIndices, 6);

    vint outerQubitIndicesExpected = {0, 36, 126, 129, 216, 219, 252, 255, 345};
    vint innerQubitIndicesExpected = {6, 18, 19, 37, 42, 108, 110, 111, 127, 128, 
                                      130, 131, 132, 135, 144, 145, 146, 147, 148, 
                                      149, 167, 218, 222, 225, 234, 235, 236, 237, 
                                      238, 239, 253, 254, 256, 257, 261, 258, 275, 
                                      327, 346, 351, 363, 364};

    EXPECT_EQ(lattice.outerQubitIndices, outerQubitIndicesExpected);
    EXPECT_EQ(lattice.innerQubitIndices, innerQubitIndicesExpected);
}

//------------------------------------------------------------

TEST(buildXSyndIndicesTestR1, CorrectOutput)
{
    vint xSyndIndices = {};
    rhombic::r1::buildXSyndIndices(xSyndIndices, 6);
    
    vint xSyndIndicesExpected = {6, 8, 13, 43, 48, 50, 78, 80, 85, 115, 120, 122};
    EXPECT_EQ(xSyndIndices, xSyndIndicesExpected);
}

//------------------------------------------------------------

TEST(buildZSyndIndicesTestR1, CorrectOutput)
{
    vint zSyndIndices = {};
    rhombic::r1::buildZSyndIndices(zSyndIndices, 6);

    vint zSyndIndicesExpected = {148, 150, 151, 168, 169, 171, 178, 196, 197, 198, 
                                 199, 217, 288, 291, 298, 316, 317, 318, 319, 336, 
                                 337, 339, 346, 365, 865, 866, 871, 891, 893, 894, 
                                 913, 914, 919, 1011, 1012, 1013, 1014, 1032, 1033,
                                 1034, 1039, 1059, 1060, 1061, 1062, 1152, 1180, 1200};
    EXPECT_EQ(zSyndIndices, zSyndIndicesExpected);
}

//------------------------------------------------------------

TEST(buildLogicalsTestR1, CorrectOutput)
{
    Lattice lattice(6);
    rhombic::r1::buildLogicals(lattice);

    vint xLogicalExpected = {0, 6, 108, 110, 111, 131, 216, 218, 219, 222, 225, 239, 327};
    vint zLogicalExpected = {0, 36, 219};
    EXPECT_EQ(lattice.xLogical, xLogicalExpected);
    EXPECT_EQ(lattice.zLogical, zLogicalExpected);
}
