#include "gtest/gtest.h"
#include "rhombicGeneric.h"
#include "rhombic1.h" //for use of lattice building only (see pathing function tests)
#include "rhombic2.h" //same

//------------------------------------------------------------

TEST(coordToIndexTest, HandlesCorrectInput)
{
    rhombic::coord c0 = {4,4,1,0};
    rhombic::coord c1 = {4,4,1,1};
    rhombic::coord c2 = {0,0,0,0};

    EXPECT_EQ(rhombic::coordToIndex(c0, 6), 64);
    EXPECT_EQ(rhombic::coordToIndex(c1, 6), 280);
    EXPECT_EQ(rhombic::coordToIndex(c2, 6), 0);
} 
TEST(coordToIndexTest, HandlesOutOfRangeInput)
{
    rhombic::coord c0 = {-1,0,0,0};
    rhombic::coord c1 = {6,0,0,0};
    rhombic::coord c2 = {0,6,0,0};
    rhombic::coord c3 = {0,0,0,3};

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
    rhombic::coord c2 = rhombic::indexToCoord(0, 6);

    EXPECT_EQ(c0.xi[0], 4);
    EXPECT_EQ(c0.xi[1], 4);
    EXPECT_EQ(c0.xi[2], 1);
    EXPECT_EQ(c0.xi[3], 0);

    EXPECT_EQ(c1.xi[0], 4);
    EXPECT_EQ(c1.xi[1], 4);
    EXPECT_EQ(c1.xi[2], 1);
    EXPECT_EQ(c1.xi[3], 1);

    EXPECT_EQ(c2.xi[0], 0);
    EXPECT_EQ(c2.xi[1], 0);
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
    Lattice lattice(6);

    lattice.faceToVertices.assign(3*6*6*6,{});
    lattice.faceToEdges.assign(3*6*6*6,{});
    lattice.faceToCells.assign(3*6*6*6,{});
    lattice.vertexToFaces.assign(2*6*6*6,{});
    lattice.edgeToFaces.assign(8*6*6*6,{});
    lattice.cellToFaces.assign(6*6*6,{});
    
    //v=44 is (2,1,1,0)
    rhombic::addFace(44, 0, {3, 1, 1, 3}, {0, 2}, {1, 1, 1, 1}, {1, 1},  lattice);
    rhombic::addFace(44, 1, {0, 1, 1, 0}, {1, 2}, {1, -1, -1, 1}, {1, -1},  lattice);

    vint vertices1 = {44, 81, 254, 260};
    vint edges1 = {177, 179, 1019, 1041};
    pint cells1 = {45, 80};
    vint vertices2 = {14, 44, 223, 224};
    vint edges2 = {57, 176, 892, 893};
    pint cells2 = {8, 50};
    vint faces1 = {0,1};
    vint faces2 = {0};
    vint faces3 = {1};

    EXPECT_EQ(lattice.faceToVertices[0], vertices1);
    EXPECT_EQ(lattice.faceToEdges[0], edges1);
    EXPECT_EQ(lattice.faceToCells[0], cells1);
    EXPECT_EQ(lattice.faceToVertices[1], vertices2);
    EXPECT_EQ(lattice.faceToEdges[1], edges2);
    EXPECT_EQ(lattice.faceToCells[1], cells2);
    EXPECT_EQ(lattice.vertexToFaces[44], faces1);
    EXPECT_EQ(lattice.vertexToFaces[81], faces2);
    EXPECT_EQ(lattice.vertexToFaces[14], faces3);
    EXPECT_EQ(lattice.edgeToFaces[177], faces2);
    EXPECT_EQ(lattice.edgeToFaces[57], faces3);
}

//------------------------------------------------------------

TEST(findFaceTest, CorrectOutput)
{
    Lattice lattice(6);

    lattice.faceToVertices.assign(3*6*6*6,{});
    lattice.faceToEdges.assign(3*6*6*6,{});
    lattice.faceToCells.assign(3*6*6*6,{});
    lattice.vertexToFaces.assign(2*6*6*6,{});
    lattice.edgeToFaces.assign(8*6*6*6,{});
    lattice.cellToFaces.assign(6*6*6,{});

    rhombic::addFace(44, 0, {3, 1, 1, 3}, {0, 2}, {1, 1, 1, 1}, {1, 1},  lattice);
    rhombic::addFace(44, 1, {0, 1, 1, 0}, {1, 2}, {1, -1, -1, 1}, {1, -1},  lattice);

    EXPECT_EQ(rhombic::findFace({44,81}, lattice.vertexToFaces, lattice.faceToVertices), 0);
    EXPECT_EQ(rhombic::findFace({14,44}, lattice.vertexToFaces, lattice.faceToVertices), 1);
}

//------------------------------------------------------------

TEST(scalarProductTest, HandlesCorrectInput)
{
    EXPECT_EQ(rhombic::scalarProduct({4,1,-3}, 0, 1), 8);
    EXPECT_EQ(rhombic::scalarProduct({4,1,-3}, 1, 1), 0);
    EXPECT_EQ(rhombic::scalarProduct({4,1,-3}, 2, 1), -6);
    EXPECT_EQ(rhombic::scalarProduct({4,1,-3}, 3, 1), 2);
    EXPECT_EQ(rhombic::scalarProduct({4,1,-3}, 0, -1), -8);
    EXPECT_EQ(rhombic::scalarProduct({4,1,-3}, 1, -1), 0);
    EXPECT_EQ(rhombic::scalarProduct({4,1,-3}, 2, -1), 6);
    EXPECT_EQ(rhombic::scalarProduct({4,1,-3}, 3, -1), -2);

    EXPECT_EQ(rhombic::scalarProduct({0,1,1}, 0, 1), 0);
    EXPECT_EQ(rhombic::scalarProduct({1,0,1}, 1, 1), 2);
    EXPECT_EQ(rhombic::scalarProduct({1,1,0}, 2, 1), 0);
}
TEST(scalarProductTest, HandlesIncorrectInput)
{
    EXPECT_THROW(rhombic::scalarProduct({1,1,1}, 100, 1), std::invalid_argument);
    EXPECT_THROW(rhombic::scalarProduct({1,1,1}, 0, 0), std::invalid_argument);
    EXPECT_THROW(rhombic::scalarProduct({1,1,1}, 0, 2), std::invalid_argument);
}

//------------------------------------------------------------

TEST(differenceVectorTest, CorrectOutput)
{
    rhombic::coord cd1;
    rhombic::coord cd2;

    cd1.xi[0] = 1;
    cd1.xi[1] = 0;
    cd1.xi[2] = 1;
    cd1.xi[3] = 0;

    cd2.xi[0] = 0;
    cd2.xi[1] = 0;
    cd2.xi[2] = 2;
    cd2.xi[3] = 0;

    std::vector<float> diffExpected = {-1,0,1};
    EXPECT_EQ(rhombic::differenceVector(cd1, cd2), diffExpected);
    diffExpected = {1,0,-1};
    EXPECT_EQ(rhombic::differenceVector(cd2, cd1), diffExpected);

    cd1.xi[3] = 1;
    diffExpected = {-1.5,-0.5,0.5};
    EXPECT_EQ(rhombic::differenceVector(cd1, cd2), diffExpected);
    diffExpected = {1.5,0.5,-0.5};
    EXPECT_EQ(rhombic::differenceVector(cd2, cd1), diffExpected);
}

//------------------------------------------------------------

TEST(magnitudeTest, CorrectOutput)
{
    std::vector<float> diff1 = {1,0,0};
    std::vector<float> diff2 = {1,1,1};
    std::vector<float> diff3 = {1,-1,1};
    std::vector<float> diff4 = {2,-3,-4};
    EXPECT_FLOAT_EQ(rhombic::magnitude(diff1), 1);
    EXPECT_FLOAT_EQ(rhombic::magnitude(diff2), sqrt(3));
    EXPECT_FLOAT_EQ(rhombic::magnitude(diff3), sqrt(3));
    EXPECT_FLOAT_EQ(rhombic::magnitude(diff4), sqrt(29));
}

//------------------------------------------------------------

TEST(shortestPathTest, CorrectOutputNoBuild)
{
    //This is independent from the index vector building functions
    //but means lattice index vectors are not fully detailed
    vint path1 = {3};
    vint path2 = {3, 865};
    vint path3 = {3, 865, 148};
    
    Lattice lattice(6);
    lattice.zSyndIndices = {3, 865, 148};

    lattice.vertexToEdges.assign(2*6*6*6,{});
    lattice.edgeToVertices.assign(8*6*6*6,{});

    lattice.vertexToEdges[0] = {3};
    lattice.vertexToEdges[216] = {3,865};
    lattice.vertexToEdges[37] = {148, 865};
    lattice.vertexToEdges[217] = {148};

    lattice.edgeToVertices[3] = {0,216};
    lattice.edgeToVertices[865] = {37, 216};
    lattice.edgeToVertices[148] = {37, 217};

    EXPECT_EQ(rhombic::shortestPath(0, 216, lattice), path1);
    EXPECT_EQ(rhombic::shortestPath(0, 37, lattice), path2);
    EXPECT_EQ(rhombic::shortestPath(0, 217, lattice), path3);
}
TEST(shortestPathTest, CorrectOutputBuild)
{
    //Uses the vector building functions so is not independent but can capture more complex errors
    //Result only matters if lattice building functions pass all their individual tests
   
    //bulk paths
    vint path1 = {171};
    vint path2 = {171, 1033};
    vint path3 = {171, 1033, 316};

    Lattice lattice(6);
    rhombic::r1::buildLattice(lattice);

    EXPECT_EQ(rhombic::shortestPath(42, 258, lattice), path1);
    EXPECT_EQ(rhombic::shortestPath(42, 79, lattice), path2);
    EXPECT_EQ(rhombic::shortestPath(42, 259, lattice), path3);

    //boundary paths
    vint path4 = {865};
    vint path5 = {865, 148};
   
    EXPECT_EQ(rhombic::shortestPath(216, 37, lattice), path4);
    EXPECT_EQ(rhombic::shortestPath(216, 217, lattice), path5);
}

//------------------------------------------------------------

TEST(shortestDualPathTest, CorrectOutputNoBuild)
{
    //As above
    vint path1 = {129};
    vint path2 = {129,126};

    Lattice lattice(6);
    lattice.outerQubitIndices = {126, 129};

    lattice.cellToFaces.assign(6*6*6,{});

    //These things need 12 elements or we get a segfault
    //and also ordering is used to infer direction so need to be in the right place
    lattice.cellToFaces[6] = {0,0,0,0,0,0,129,0,0,0,0,0};
    lattice.cellToFaces[48] = {0,0,0,0,0,0,0,126,0,0,0,129,};
    lattice.cellToFaces[78] = {0,0,0,0,0,0,0,0,0,0,126,0};

    EXPECT_EQ(rhombic::shortestDualPath(6, 48, lattice, 1, 1), path1);
    EXPECT_EQ(rhombic::shortestDualPath(6, 78, lattice, 1, 1), path2);
}
TEST(shortestDualPathTest, CorrectOutputBuild)
{
    //bulk paths
    vint path1 = {126};
    vint path2 = {126, 345};

    Lattice lattice(6);
    rhombic::r1::buildLattice(lattice);

    EXPECT_EQ(rhombic::shortestDualPath(48, 78, lattice, 1, 1), path1);
    EXPECT_EQ(rhombic::shortestDualPath(48, 120, lattice, 1, 1), path2);

    //boundary path
    vint path3 = {129, 37};

    EXPECT_EQ(rhombic::shortestDualPath(6, 13, lattice, 1, 1), path3);
}
TEST(shortestDualPathTest, CorrectOutputInnerOnly)
{
    Lattice lattice(6);
    rhombic::r1::buildLattice(lattice);
    vint path = rhombic::shortestDualPath(48, 78, lattice, 0, 1);
    for (int i : path) {if (i == 126) FAIL();}
}
TEST(shortestDualPathTest, CorrectOutputOuterOnly)
{
    Lattice lattice(6);
    rhombic::r1::buildLattice(lattice);
    vint path = {126};
    EXPECT_EQ(rhombic::shortestDualPath(48, 78, lattice, 1, 0), path);
}

//------------------------------------------------------------

TEST(rhombicJumpCorrectionTest, CorrectOutputExample)
{
    //Tests action on a specific example
    Lattice lattice(6);
    rhombic::r1::buildLattice(lattice);
    
    //weight-2 boundary stab
    lattice.qubitsZ[222] = 1;
    lattice.qubitsZ[239] = 1;
    //weight-3 bulk stab
    lattice.qubitsZ[144] = 1;
    lattice.qubitsZ[145] = 1;
    lattice.qubitsZ[146] = 1;

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    rhombic::jumpCorrection(lattice, engine, dist, 1);
    vint zErrors;
    vint zErrorsExpected = {};
    for (int i : lattice.innerQubitIndices) 
    {
        if (lattice.qubitsZ[i] != 0) zErrors.push_back(i);
    }
    EXPECT_EQ(zErrors, zErrorsExpected);
}
TEST(rhombicJumpCorrectionTest, CorrectOutputR1)
{
    Lattice lattice(6);
    rhombic::r1::buildLattice(lattice);

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    lattice.zStabPattern(engine, dist);     //tested separately
    rhombic::jumpCorrection(lattice, engine, dist, 1);
    vint zErrors;
    vint zErrorsExpected = {};
    for (int i : lattice.innerQubitIndices) 
    {
        if (lattice.qubitsZ[i] != 0) zErrors.push_back(i);
    }
    EXPECT_EQ(zErrors, zErrorsExpected);
}
TEST(rhombicJumpCorrectionTest, CorrectOutputR2)
{
    Lattice lattice(6);
    rhombic::r2::buildLattice(lattice);

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    lattice.zStabPattern(engine, dist);     //tested separately
    rhombic::jumpCorrection(lattice, engine, dist, 2);
    vint zErrors;
    vint zErrorsExpected = {};
    for (int i : lattice.innerQubitIndices) 
    {
        if (lattice.qubitsZ[i] != 0) zErrors.push_back(i);
    }
    EXPECT_EQ(zErrors, zErrorsExpected);
}

//------------------------------------------------------------
