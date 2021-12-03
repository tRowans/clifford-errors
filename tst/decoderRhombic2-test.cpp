#include "gtest/gtest.h"
#include "decoderRhombic2.h"
#include "prepLattices.h"

//------------------------------------------------------------

TEST(indexBuilding, building)
{
    rhombic::r2::buildLattice(latRhombic2, 6);
    ASSERT_TRUE(true);
}

//------------------------------------------------------------

TEST(distanceToClosestXBoundaryTest, CorrectOutput)
{
    vint distInfo1 = {0, -1, 1};
    vint distInfo2 = {0, 1, 1};
    vint distInfo3 = {1, -1, 1};
    vint distInfo4 = {1, 1, 1};
    EXPECT_EQ(rhombic::r2::distanceToClosestXBoundary(222, 6), distInfo1);
    EXPECT_EQ(rhombic::r2::distanceToClosestXBoundary(223, 6), distInfo2);
    EXPECT_EQ(rhombic::r2::distanceToClosestXBoundary(43, 6), distInfo3);
    EXPECT_EQ(rhombic::r2::distanceToClosestXBoundary(85, 6), distInfo4);
}

//------------------------------------------------------------

TEST(shortestPathToXBoundaryTest, CorrectOutput)
{
    vint path1 = {27};
    vint path2 = {895};
    vint path3 = {867};
    vint path4 = {340};
    EXPECT_EQ(rhombic::r2::shortestPathToXBoundary(222, 6), path1);
    EXPECT_EQ(rhombic::r2::shortestPathToXBoundary(223, 6), path2);
    EXPECT_EQ(rhombic::r2::shortestPathToXBoundary(43, 6), path3);
    EXPECT_EQ(rhombic::r2::shortestPathToXBoundary(85, 6), path4);
}

//------------------------------------------------------------

TEST(distanceToClosestZBoundaryTest, CorrectOutput)
{
    EXPECT_EQ(rhombic::r2::distanceToClosestZBoundary(42, 6), -1);
    EXPECT_EQ(rhombic::r2::distanceToClosestZBoundary(84, 6), 1);
}

//------------------------------------------------------------

TEST(shortestPathToZBTest, CorrectOutput)
{
    vint path1 = {18};
    vint path2 = {363};
    vint path3 = {252};
    vint path4 = {147};
    EXPECT_EQ(rhombic::r2::shortestPathToZB(196, latRhombic2.cellToFacesR2, 6), path1);
    EXPECT_EQ(rhombic::r2::shortestPathToZB(283, latRhombic2.cellToFacesR2, 6), path2);
    EXPECT_EQ(rhombic::r2::shortestPathToZB(91, latRhombic2.cellToFacesR2, 6), path3);
    EXPECT_EQ(rhombic::r2::shortestPathToZB(54, latRhombic2.cellToFacesR2, 6), path4);
}

//------------------------------------------------------------

//Code for other functions in decoderRhombic2 is the same as in decoderRhombic1
//Just in a different namespace and uses different functions that are tested above
//So don't need to test again. Rhombic1 tests are enough
