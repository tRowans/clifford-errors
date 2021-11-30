#include "gtest/gtest.h"
#include "rhombicGeneric.h"
#include "rhombic1.h" //for use of lattice building only (see pathing function tests)
#include "rhombic2.h" //same

TEST(coordToIndexTest, HandlesCorrectInput)
{
    rhombic::coord c0;
    rhombic::coord c1;
    rhombic::coord c2;

    c0.xi[0] = 4;
    c0.xi[1] = 4;
    c0.xi[2] = 1;
    c0.xi[3] = 0;

    c1.xi[0] = 4;
    c1.xi[1] = 4;
    c1.xi[2] = 1;
    c1.xi[3] = 1;

    c2.xi[0] = 15;
    c2.xi[1] = 3;
    c2.xi[2] = 0;
    c2.xi[3] = 0;
    
    EXPECT_EQ(rhombic::coordToIndex(c0, 6), 64);
    EXPECT_EQ(rhombic::coordToIndex(c1, 6), 280);
    EXPECT_EQ(rhombic::coordToIndex(c2, 6), 33);
} 
TEST(coordToIndexTest, HandlesOutOfRangeInput)
{
    rhombic::coord c0;
    rhombic::coord c1;
    rhombic::coord c2;
    rhombic::coord c3;

    c0.xi[0] = -1;
    c0.xi[1] = 0;
    c0.xi[2] = 0;
    c0.xi[3] = 0;
    
    c1.xi[0] = 6;
    c1.xi[1] = 0;
    c1.xi[2] = 0;
    c1.xi[3] = 0;

    c2.xi[0] = 0;
    c2.xi[1] = 6;
    c2.xi[2] = 0;
    c2.xi[3] = 0;

    c3.xi[0] = 0;
    c3.xi[1] = 0;
    c3.xi[2] = 0;
    c3.xi[3] = 3;

    EXPECT_THROW(rhombic::coordToIndex(c0, 6), std::invalid_argument);
    EXPECT_THROW(rhombic::coordToIndex(c1, 6), std::invalid_argument);
    EXPECT_THROW(rhombic::coordToIndex(c2, 6), std::invalid_argument);
    EXPECT_THROW(rhombic::coordToIndex(c3, 6), std::invalid_argument);
}

//------------------------------------------------------------

TEST(indexToCoordTest, HandlesExpectedInput)
{
    rhombic::coord c0 = rhombic::indexToCoord(64, 6);
    rhombic::coord c1 = rhombic::indexToCoord(280, 6);
    rhombic::coord c2 = rhombic::indexToCoord(33, 6);

    EXPECT_EQ(c0.xi[0], 4);
    EXPECT_EQ(c0.xi[1], 4);
    EXPECT_EQ(c0.xi[2], 1);
    EXPECT_EQ(c0.xi[3], 0);

    EXPECT_EQ(c1.xi[0], 4);
    EXPECT_EQ(c1.xi[1], 4);
    EXPECT_EQ(c1.xi[2], 1);
    EXPECT_EQ(c1.xi[3], 1);

    EXPECT_EQ(c2.xi[0], 15);
    EXPECT_EQ(c2.xi[1], 3);
    EXPECT_EQ(c2.xi[2], 0);
    EXPECT_EQ(c2.xi[3], 0);
}
TEST(indexToCoordTest, HandlesOutOfRangeInput)
{
    EXPECT_THROW(rhombic::indexToCoord(-1, 6), std::invalid_argument);
    EXPECT_THROW(rhombic::indexToCoord(2160, 6), std::invalid_argument);
}

//------------------------------------------------------------

TEST(neighTest, CorrectOutputW0)
{
    //v=44 is (2,1,1,0)
    EXPECT_EQ(rhombic::neigh(44, 0, 1, 6), 224);
    EXPECT_EQ(rhombic::neigh(44, 1, 1, 6), 254);
    EXPECT_EQ(rhombic::neigh(44, 2, 1, 6), 259);
    EXPECT_EQ(rhombic::neigh(44, 3, 1, 6), 260);
    EXPECT_EQ(rhombic::neigh(44, 0, -1, 6), 253);
    EXPECT_EQ(rhombic::neigh(44, 1, -1, 6), 223);
    EXPECT_EQ(rhombic::neigh(44, 2, -1, 6), 218);
    EXPECT_EQ(rhombic::neigh(44, 3, -1, 6), 217);
}
TEST(neighTest, CorrectOutputW1)
{
    //v=260 is (2,1,1,1)
    EXPECT_EQ(rhombic::neigh(260, 0, 1, 6), 51);
    EXPECT_EQ(rhombic::neigh(260, 1, 1, 6), 81);
    EXPECT_EQ(rhombic::neigh(260, 2, 1, 6), 86);
    EXPECT_EQ(rhombic::neigh(260, 3, 1, 6), 87);
    EXPECT_EQ(rhombic::neigh(260, 0, -1, 6), 80);
    EXPECT_EQ(rhombic::neigh(260, 1, -1, 6), 50);
    EXPECT_EQ(rhombic::neigh(260, 2, -1, 6), 45);
    EXPECT_EQ(rhombic::neigh(260, 3, -1, 6), 44);
}
TEST(neighTest, HandlesIncorrectArugments)
{
    EXPECT_THROW(rhombic::neigh(44, -1, 1, 6), std::invalid_argument);
    EXPECT_THROW(rhombic::neigh(44, 4, 1, 6), std::invalid_argument);
    EXPECT_THROW(rhombic::neigh(44, 0, 0, 6), std::invalid_argument);
}

//------------------------------------------------------------

TEST(neighXYZTest, CorrectOutput)
{
    EXPECT_EQ(rhombic::neighXYZ(44, 0, 1, 6), 45);
    EXPECT_EQ(rhombic::neighXYZ(44, 1, 1, 6), 50);
    EXPECT_EQ(rhombic::neighXYZ(44, 2, 1, 6), 80);
    EXPECT_EQ(rhombic::neighXYZ(44, 0, -1, 6), 43);
    EXPECT_EQ(rhombic::neighXYZ(44, 1, -1, 6), 38);
    EXPECT_EQ(rhombic::neighXYZ(44, 2, -1, 6), 8);
}

//------------------------------------------------------------

TEST(edgeIndexTest, CorrectOutput)
{
    EXPECT_EQ(rhombic::edgeIndex(44, 0, 1, 6), 176);
    EXPECT_EQ(rhombic::edgeIndex(44, 1, 1, 6), 177);
    EXPECT_EQ(rhombic::edgeIndex(44, 2, 1, 6), 178);
    EXPECT_EQ(rhombic::edgeIndex(44, 3, 1, 6), 179);
    EXPECT_EQ(rhombic::edgeIndex(44, 0, -1, 6), 1012);
    EXPECT_EQ(rhombic::edgeIndex(44, 1, -1, 6), 893);
    EXPECT_EQ(rhombic::edgeIndex(44, 2, -1, 6), 874);
    EXPECT_EQ(rhombic::edgeIndex(44, 3, -1, 6), 871);
}
//Incorrect inputs will be detected by neigh inside the function 
//so don't need to test for them again here

//------------------------------------------------------------

TEST(addFaceTest, CorrectOutput)
{
    vvint faceToVertices;
    vvint faceToEdges;
    vpint faceToCells;
    vvint vertexToFaces;
    vvint edgeToFaces;
    vvint cellToFaces;
    
    Lattice lattice;

    //v=44 is (2,1,1,0)
    rhombic::addFace(44, 0, {3, 1, 1, 3}, {0, 2}, {1, 1, 1, 1}, {1, 1},  lattice, 6);
    rhombic::addFace(44, 1, {0, 1, 1, 0}, {1, 2}, {1, -1, -1, 1}, {1, -1},  lattice, 6);

    vint vertices1 = {44, 81, 254, 260};
    vint edges1 = {177, 179, 1019, 1041};
    pint cells1 = {45, 80};
    vint vertices2 = {2, 44, 223, 224};
    vint edges2 = {9, 176, 892, 893};
    pint cells2 = {8, 50};
    vint faces1 = {0,1};
    vint faces2 = {0};
    vint faces3 = {1};

    EXPECT_EQ(faceToVertices[0], vertices1);
    EXPECT_EQ(faceToEdges[0], edges1);
    EXPECT_EQ(faceToCells[0], cells1);
    EXPECT_EQ(faceToVertices[1], vertices2);
    EXPECT_EQ(faceToEdges[1], edges2);
    EXPECT_EQ(faceToCells[1], cells2);
    EXPECT_EQ(vertexToFaces[212], faces1);
    EXPECT_EQ(vertexToFaces[393], faces2);
    EXPECT_EQ(vertexToFaces[62], faces3);
    EXPECT_EQ(edgeToFaces[849], faces2);
    EXPECT_EQ(edgeToFaces[848], faces3);
}

//------------------------------------------------------------

TEST(findFaceTest, CorrectOutput)
{
    rhombic::addFace(44, 0, {3, 1, 1, 3}, {0, 2}, {1, 1, 1, 1}, {1, 1},  lattice, 6);
    rhombic::addFace(44, 1, {0, 1, 1, 0}, {1, 2}, {1, -1, -1, 1}, {1, -1},  lattice, 6);

    EXPECT_EQ(findFace({44,81}, lattice.vertexToFaces, lattice.faceToVertices), 0);
    EXPECT_EQ(findFace({2,44}, lattice.vertexToFaces, lattice.faceToVertices), 1);
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

TEST(shortestPathLengthTest, CorrectOutput)
{
    EXPECT_EQ(rhombic::shortestPathLength(0, 216, 6), 1);
    EXPECT_EQ(rhombic::shortestPathLength(0, 36, 6), 2);
    EXPECT_EQ(rhombic::shortestPathLength(0, 217, 6), 3);
}
    
//------------------------------------------------------------

TEST(shortestPathTest, CorrectOutputNoBuild)
{
    //This is independent from the index vector building functions
    //but means lattice index vectors are not fully detailed
    vint path1 = {3};
    vint path2 = {3, 865};
    vint path3 = {3, 865, 148};
    
    Lattice lattice;
    lattice.zSyndIndices = {3, 865, 148};

    lattice.vertexToEdges[0] = {3};
    lattice.vertexToEdges[216] = {3,865};
    lattice.vertexToEdges[36] = {148, 865};
    lattice.vertexToEdges[217] = {148};

    lattice.edgeToVertices[3] = {0,216};
    lattice.edgeToVertices[865] = {36, 216};
    lattice.edgeToVertices[148] = {36, 217};

    EXPECT_EQ(rhombic::shortestPath(0, 216, lattice, 6), path1);
    EXPECT_EQ(rhombic::shortestPath(0, 36, lattice, 6), path2);
    EXPECT_EQ(rhombic::shortestPath(0, 217, lattice, 6), path3);
}
TEST(shortestPathTest, CorrectOutputBuild_)
{
    //Uses the vector building functions so is not independent but can capture more complex errors
    //Result only matters if lattice building functions pass all their individual tests
   
    //bulk paths
    vint path1 = {3};
    vint path2 = {3, 865};
    vint path3 = {3, 865, 148};

    Lattice lattice;
    rhombic::r1::buildLattice(lattice, 6);

    EXPECT_EQ(rhombic::shortestPath(42, 258, lattice, 6), path1);
    EXPECT_EQ(rhombic::shortestPath(42, 79, lattice, 6), path2);
    EXPECT_EQ(rhombic::shortestPath(42, 259, lattice, 6), path3);

    //boundary paths
    vint path4 = {865};
    vint path5 = {865, 148};
   
    EXPECT_EQ(rhombic::shortestPath(216, 36, lattice, 6), path4);
    EXPECT_EQ(rhombic::shortestPath(216, 217, lattice, 6), path5);
}

//------------------------------------------------------------

TEST(shortestDualPathTest, CorrectOutputNoBuild)
{
    //As above
    vint path1 = {129};
    vint path2 = {129,126};

    Lattice lattice;
    lattice.outerQubits = {126, 129};
    lattice.cellToFaces[6] = {129};
    lattice.cellToFaces[48] = {126, 129};
    lattice.cellToFaces[78] = {126};

    EXPECT_EQ(rhombic::shortestDualPath(6, 48, lattice, 6, 1, 1), path1);
    EXPECT_EQ(rhombic::shortestDualPath(6, 78, lattice, 6, 1, 1), path2);
}
TEST(shortestDualPathTest, CorrectOutputBuild)
{
    //bulk paths
    vint path1 = {126};
    vint path2 = {126, 127};

    Lattice lattice;
    rhombic::r1::buildLattice(lattice, 6);

    EXPECT_EQ(rhombic::shortestDualPath(48, 78, lattice, 6, 1, 1), path1);
    EXPECT_EQ(rhombic::shortestDualPath(48, 43, lattice, 6, 1, 1), path2);

    //boundary path
    vint path3 = {129, 37};

    EXPECT_EQ(rhombic::shortestDualPath(6, 13, lattice, 6, 1, 1), path3);
}
TEST(shortestDualPathTest, CorrectOutputInnerOnly)
{
    Lattice lattice;
    rhombic::r1::buildLattice(lattice, 6);
    vint path = rhombic::shortestDualPath(48, 78, lattice, 6, 0, 1);
    for (int i : path) {if (i == 126) FAIL()};
}
TEST(shortestDualPathTest, CorrectOutputOuterOnly)
{
    vint path = {126};
    Lattice lattice;
    rhombic::r1::buildLattice(lattice, 6);
    EXPECT_EQ(rhombic::shoretestDualPath(48, 78. lattice, 6, 1, 0), path);
}

//------------------------------------------------------------

TEST(rhombicJumpCorrectionTest, CorrectOutputExample)
{
    //Tests action on a specific example
    Lattice lattice;
    rhombic::r1::buildLattice(lattice, 6);
    
    //weight-2 boundary stab
    lattice.qubitsZ[117] = 1;
    lattice.qubitsZ[239] = 1;
    //weight-3 bulk stab
    lattice.qubitsZ[144] = 1;
    lattice.qubitsZ[145] = 1;
    lattice.qubitsZ[146] = 1;

    rhombic::jumpCorrection(lattice, engine, dist, 6, 1);
    for (int i : lattice.innerQubitIndices) {if (i != 0) FAIL()};
}
TEST(rhombicJumpCorrectionTest, CorrectOutputR1)
{
    //Tests action on a random example generated by lattice.zStabPattern()
    Lattice lattice;
    rhombic::r1::buildLattice(lattice, 6);  //tested separately

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    lattice.zStabPattern(engine, dist);     //tested separately
    rhombic::jumpCorrection(lattice, engine, dist, 6, 1);
    for (int i : lattice.innerQubitIndices) {if (i != 0) FAIL()};
}
TEST(rhombicJumpCorrectionTest, CorrectOutputR2)
{
    Lattice lattice;
    rhombic::r2::buildLattice(lattice, 6);  //tested separately

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    lattice.zStabPattern(engine, dist);     //tested separately
    rhombic::jumpCorrection(lattice, engine, dist, 6, 2);
    for (int i : lattice.innerQubitIndices) {if (i != 0) FAIL()};
}

//------------------------------------------------------------
