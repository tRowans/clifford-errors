#include "gtest/gtest.h"
#include "decoderRhombic2.h"
#include "prepRhombic2.h"

//------------------------------------------------------------

TEST(indexBuildingR2, building)
{
    rhombic::r2::buildLattice(latRhombic2);
    ASSERT_TRUE(true);
}

//------------------------------------------------------------

TEST(w1ToW0TestR2, CorrectOutput)
{
    rhombic::coord cIn = {0,0,0,1};
    rhombic::coord cOut = rhombic::r2::w1ToW0(cIn,6);
    vint xiExpected = {1,1,1,0};
    EXPECT_EQ(cOut.xi[0], 1);
    EXPECT_EQ(cOut.xi[1], 1);
    EXPECT_EQ(cOut.xi[2], 1);
    EXPECT_EQ(cOut.xi[3], 0);

    cIn.xi[0] = 1;
    cOut = rhombic::r2::w1ToW0(cIn,6);
    xiExpected = {1,0,0,0};
    EXPECT_EQ(cOut.xi[0], 1);
    EXPECT_EQ(cOut.xi[1], 0);
    EXPECT_EQ(cOut.xi[2], 0);
    EXPECT_EQ(cOut.xi[3], 0);
}

//------------------------------------------------------------

TEST(shortestPathLengthTestR2, threeD)
{
    EXPECT_EQ(rhombic::r2::shortestPathLength(1, 217, 6, 0), 1);
    EXPECT_EQ(rhombic::r2::shortestPathLength(1, 37, 6, 0), 2);
    EXPECT_EQ(rhombic::r2::shortestPathLength(1, 218, 6, 0), 3);
    EXPECT_EQ(rhombic::r2::shortestPathLength(85, 252, 6, 0), 3);
    EXPECT_EQ(rhombic::r2::shortestPathLength(85, 258, 6, 0), 1);
    EXPECT_EQ(rhombic::r2::shortestPathLength(216, 252, 6, 0), 2);
}
TEST(shortestPathLengthTestR2, twoD)
{
    EXPECT_EQ(rhombic::r2::shortestPathLength(48, 78, 6, 1), 1);
    EXPECT_EQ(rhombic::r2::shortestPathLength(36, 48, 6, 1), 2);
}

//------------------------------------------------------------

TEST(distanceToClosestXBoundaryTestR2, threeD)
{
    vint distInfo1 = {0, -1, 1};
    vint distInfo2 = {0, 1, 1};
    vint distInfo3 = {1, -1, 1};
    vint distInfo4 = {1, 1, 1};
    EXPECT_EQ(rhombic::r2::distanceToClosestXBoundary(222, 6, 0), distInfo1);
    EXPECT_EQ(rhombic::r2::distanceToClosestXBoundary(223, 6, 0), distInfo2);
    EXPECT_EQ(rhombic::r2::distanceToClosestXBoundary(43, 6, 0), distInfo3);
    EXPECT_EQ(rhombic::r2::distanceToClosestXBoundary(85, 6, 0), distInfo4);
}
TEST(distanceToClosestXBoundaryTestR2, twoD)
{
    vint distInfo1 = {1, -1, 1};
    vint distInfo2 = {1, 1, 1};
    EXPECT_EQ(rhombic::r2::distanceToClosestXBoundary(78, 6, 1), distInfo1);
    EXPECT_EQ(rhombic::r2::distanceToClosestXBoundary(48, 6, 1), distInfo2);
    EXPECT_EQ(rhombic::r2::distanceToClosestXBoundary(6, 6, 1), distInfo1);
    EXPECT_EQ(rhombic::r2::distanceToClosestXBoundary(120, 6, 1), distInfo2);
}

//------------------------------------------------------------

TEST(shortestPathToXBoundaryTestR2, threeD)
{
    vint path1 = {27};
    vint path2 = {895};
    vint path3 = {870};
    vint path4 = {340};
    EXPECT_EQ(rhombic::r2::shortestPathToXBoundary(222, 6, 0), path1);
    EXPECT_EQ(rhombic::r2::shortestPathToXBoundary(223, 6, 0), path2);
    EXPECT_EQ(rhombic::r2::shortestPathToXBoundary(43, 6, 0), path3);
    EXPECT_EQ(rhombic::r2::shortestPathToXBoundary(85, 6, 0), path4);
}
TEST(shortestPathToXBoundaryTestR2, twoD)
{
    vint path1 = {25};
    vint path2 = {480};
    EXPECT_EQ(rhombic::r2::shortestPathToXBoundary(6, 6, 1), path1);
    EXPECT_EQ(rhombic::r2::shortestPathToXBoundary(120, 6, 1), path2);
}

//------------------------------------------------------------

TEST(distanceToClosestZBoundaryTestR2, CorrectOutput)
{
    vint distInfoExpected1 = {-1, 1};
    vint distInfoExpected2 = {1, 1};
    EXPECT_EQ(rhombic::r2::distanceToClosestZBoundary(42, 6), distInfoExpected1);
    EXPECT_EQ(rhombic::r2::distanceToClosestZBoundary(84, 6), distInfoExpected2);
}

//------------------------------------------------------------

TEST(shortestPathToZBoundaryTestR2, CorrectOutput)
{
    vint path1 = {363};
    vint path2 = {252};
    vint path3 = {18};
    vint path4 = {147};
    EXPECT_EQ(rhombic::r2::shortestPathToZBoundary(84, latRhombic2.cellToFaces, 6), path1);
    EXPECT_EQ(rhombic::r2::shortestPathToZBoundary(91, latRhombic2.cellToFaces, 6), path2);
    EXPECT_EQ(rhombic::r2::shortestPathToZBoundary(42, latRhombic2.cellToFaces, 6), path3);
    EXPECT_EQ(rhombic::r2::shortestPathToZBoundary(54, latRhombic2.cellToFaces, 6), path4);
}

//------------------------------------------------------------

//Code for other functions in decoderRhombic2 is the same as in decoderRhombic1
//Just in a different namespace and uses different functions that are tested above
//So don't need to test again. Rhombic1 tests are enough
