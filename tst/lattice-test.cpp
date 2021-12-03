#include "gtest/gtest.h"
#include "cubic.h"

//------------------------------------------------------------

//This is just a hack to make the testing software build the lattice
//and also initialise some other stuff
//Means it doesn't have to be build individually in every test

TEST(buildLattice, build)
{
    cubic::buildLattice(latCubic, 6);
    ASSERT_TRUE(true);
}

//------------------------------------------------------------

TEST(wipeTest, VectorsClear)
{
    for (int i = 0; i < qubitsX.size(); i++) qubitsX[i] = 1;
    for (int i = 0; i < qubitsZ.size(); i++) qubitsZ[i] = 1;
    for (int i = 0; i < syndromeX.size(); i++) syndromeX[i] = 1;
    for (int i = 0; i < syndromeZ.size(); i++) syndromeZ[i] = 1;
    lattice.wipe();
    for (int i : lattice.qubitsX) {if (i != 0) FAIL()};
    for (int i : lattice.qubitsZ) {if (i != 0) FAIL()};
    for (int i : lattice.syndromeX) {if (i != 0) FAIL()};
    for (int i : lattice.syndromeZ) {if (i != 0) FAIL()};
}

//------------------------------------------------------------

//Not sure about the best way to test the random functions
//Maybe just test that they do something, if not a specific error
//Could do nothing but chance is very small

TEST(depolarisingErrorTest, NonTrivialAction)
{
    lattice.wipe();
    lattice.depolarisingError(0.5,engine,dist);
    int nonZeroCheck = 0;
    for (int i : lattice.qubitsX) {if (i != 0) nonZeroCheck = 1};
    for (int i : lattice.qubitsZ) {if (i != 0) nonZeroCheck = 1};
    EXPECT_EQ(nonZeroCheck, 1);
}

//------------------------------------------------------------


TEST(biasedErrorTest, NonTrivialAction)
{
    lattice.wipe();
    lattice.biasedError(0.5, engine, dist, 'x', 0);
    int nonZeroCheck = 0;
    for (int i : lattice.qubitsX) {if (i != 0) nonZeroCheck = 1};
    EXPECT_EQ(nonZeroCheck, 1);
}
TEST(biasedErrorTest, XZOnly)
{
    lattice.wipe();
    lattice.biasedError(0.5, engine, dist, 'x', 0);
    int nonZeroCheck = 0;
    int zeroCheck = 0;
    for (int i : lattice.qubitsX) {if (i != 0) nonZeroCheck = 1};
    for (int i : lattice.qubitsZ) {if (i != 0) zeroCheck = 1};
    EXPECT_EQ(nonZeroCheck, 1);
    EXPECT_EQ(zeroCheck, 0);

    lattice.wipe();
    lattice.biasedError(0.5, engine, dist, 'z', 0);
    int nonZeroCheck = 0;
    int zeroCheck = 0;
    for (int i : lattice.qubitsZ) {if (i != 0) nonZeroCheck = 1};
    for (int i : lattice.qubitsX) {if (i != 0) zeroCheck = 1};
    EXPECT_EQ(nonZeroCheck, 1);
    EXPECT_EQ(zeroCheck, 0);
}
TEST(biasedErrorTest, InnerOnly)
{
    lattice.wipe();
    lattice.biasedError(0.5, engine, dist, 'x', 1);
    int nonZeroCheck = 0;
    int zeroCheck = 0;
    for (int i : lattice.innerQubitIndices) {if (lattice.qubitsX[i] != 0) nonZeroCheck = 1};
    for (int i : lattice.outerQubitIndices) {if (lattice.qubitsX[i] != 0) zeroCheck = 1};
    EXPECT_EQ(nonZeroCheck, 1);
    EXPECT_EQ(zeroCheck, 0);
}
TEST(biasedErrorTest, InvalidInput)
{
    EXPECT_THROW(lattice.biasedError(0.5,engine,dist,'a',0), std::invalid_argument);
}

//------------------------------------------------------------

TEST(measErrorTest, NonTrivialAction)
{
    lattice.wipe();
    lattice.measError(0.5, engine, dist, 'x');
    int nonZeroCheck = 0;
    for (int i : lattice.syndromeX) {if (i != 0) nonZeroCheck = 1};
    EXPECT_EQ(nonZeroCheck, 1);
}
TEST(measErrorTest, XZOnly)
{
    lattice.wipe();
    lattice.measError(0.5, engine, dist, 'x');
    int nonZeroCheck = 0;
    int zeroCheck = 0;
    for (int i : lattice.syndromeX) {if (i != 0) nonZeroCheck = 1};
    for (int i : lattice.syndromeZ) {if (i != 0) zeroCheck = 1};
    EXPECT_EQ(nonZeroCheck, 1);
    EXPECT_EQ(zeroCheck, 1);
}
TEST(measErrorTest, InvalidArgument)
{
    EXPECT_THROW(lattice.measError(0.5,engine,dist,'a'), std::invalid_argument);
}

//------------------------------------------------------------

TEST(calcSyndTest, CorrectOutputOneXError)
{
    lattice.wipe();
    lattice.qubitsX[20] = 1;
    lattice.calcSynd('z', 1, 1);
    vint syndromeExpected(8*6*6*6, 0);
    EXPECT_EQ(lattice.syndromeX, syndromeExpected);
    syndromeExpected[20] = 1;
    syndromeExpected[38] = 1;
    syndromeExpected[127] = 1;
    EXPECT_EQ(lattice.syndromeZ, syndromeExpected);
}
TEST(calcSyndTest, CorrectOutputTwoXErrors)
{
    lattice.wipe();
    lattice.qubitsX[20] = 1;
    lattice.qubitsX[128] = 1;
    lattice.calcSynd('z', 1, 1);
    vint syndromeExpected(8*6*6*6, 0);
    EXPECT_EQ(lattice.syndromeX, syndromeExpected);
    syndromeExpected[20] = 1;
    syndromeExpected[38] = 1;
    syndromeExpected[128] = 1;
    syndromeExpected[146] = 1;
    syndromeExpected[235] = 1;
    EXPECT_EQ(lattice.syndromeZ, syndromeExpected);
}
TEST(calcSyndTest, CorrectOutputOneZError)
{
    lattice.wipe();
    lattice.qubitsZ[20] = 1;
    lattice.calcSynd('x', 1, 1);
    vint syndromeExpected(6*6*6, 0);
    EXPECT_EQ(lattice.syndromeZ, syndromeExpected);
    syndromeExpected[6] = 1;
    EXPECT_EQ(lattice.syndromeX, syndromeExpected);
}
TEST(calcSyndTest, CorrectOutputTwoXErrors)
{
    lattice.wipe();
    lattice.qubitsZ[20] = 1;
    lattice.qubitsZ[23] = 1;
    lattice.calcSynd('x', 1, 1);
    vint syndromeExpected(6*6*6, 0);
    EXPECT_EQ(lattice.syndromeZ, syndromeExpected);
    syndromeExpected[7] = 1;
    EXPECT_EQ(lattice.syndromeX, syndromeExpected);
}
TEST(calcSyndTest, InnerOnly)
{
    lattice.wipe();
    lattice.qubitsX[20] = 1;
    lattice.qubitsX[128] = 1;
    lattice.qubitsZ[20] = 1;
    lattice.qubitsX[128] = 1;
    lattice.calcSynd('x', 1, 0);
    lattice.calcSynd('z', 1, 0);
    vint syndromeXExpected(6*6*6, 0);
    vint syndromeZExpected(8*6*6*6, 0);
    syndromeXExpected[6] = 1;
    syndromeExpected[20] = 1;
    syndromeExpected[38] = 1;
    syndromeExpected[127] = 1;
    EXPECT_EQ(lattice.syndromeX, syndromeXExpected);
    EXPECT_EQ(lattice.syndromeZ, syndromeZExpected);
}
TEST(calcSyndTest, OuterOnly)
{
    lattice.wipe();
    lattice.qubitsX[20] = 1;
    lattice.qubitsX[128] = 1;
    lattice.qubitsZ[20] = 1;
    lattice.qubitsX[128] = 1;
    lattice.calcSynd('x', 0, 1);
    lattice.calcSynd('z', 0, 1);
    vint syndromeXExpected(6*6*6, 0);
    vint syndromeZExpected(8*6*6*6, 0);
    syndromeXExpected[42] = 1;
    syndromeZExpected[127] = 1;
    syndromeZExpected[128] = 1;
    syndromeExpected[146] = 1;
    syndromeExpected[235] = 1;
    EXPECT_EQ(lattice.syndromeX, syndromeXExpected);
    EXPECT_EQ(lattice.syndromeZ, syndromeZExpected);
}

//------------------------------------------------------------

TEST(findDefectsTest, OneDefect)
{
    lattice.wipe();
    vint defectsExpected = {};
    EXPECT_EQ(lattice.defects, defectsExpected);
    lattice.syndromeZ[109] = 1;
    lattice.findDefects();
    defectsExpected = {42};
    EXPECT_EQ(lattice.defects, defectsExpected);
    lattice.syndromeZ[126] = 1;
    lattice.findDefects();
    defectsExpected = {43};
    EXPECT_EQ(lattice.defects, defectsExpected);
    lattice.syndromeZ[109] = 0;
    lattice.findDefects();
    defectsExpected = {42,43};
    EXPECT_EQ(lattice.defects, defectsExpected);
}

//------------------------------------------------------------

TEST(applyZStabTest, CorrectAction)
{
    lattice.wipe();
    lattice.applyZStab(20);
    vint qubitsExpected(3*6*6*6, 0);
    EXPECT_EQ(lattice.qubitsX, qubitsExpected);
    qubitsExpected[2] = 1;
    qubitsExpected[19] = 1;
    qubitsExpected[20] = 1;
    EXPECT_EQ(lattice.qubitsZ, qubitsExpected);
}

//------------------------------------------------------------

//Just check something happens for this one too
//There's probably a better way to test random functions but I'm not sure what
//Can check the syndrome is empty as well because this shouldn't make errors

TEST(zStabPatternTest, NonTrivialAction)
{
    lattice.wipe();
    lattice.zStabPattern(engine, dist);
    int nonZeroCheck = 0;
    int zeroCheck = 0;
    for (int i : lattice.qubitsX) {if (i != 0) zeroCheck = 1};
    for (int i : lattice.qubitsZ) {if (i != 0) nonZeroCheck = 1};
    EXPECT_EQ(zeroCheck, 0);
    EXPECT_EQ(nonZeroCheck, 1);
}
TEST(zStabPatternTest, NoSyndrome)
{
    lattice.wipe();
    lattice.zStabPattern(engine, dist);
    lattice.calcSynd('x', 1, 1);
    lattice.calcSynd('z', 1, 1);
    for (int i : lattice.syndromeX) {if (i != 0) FAIL()};
    for (int i : lattice.syndromeZ) {if (i != 0) FAIL()};
}

//------------------------------------------------------------

TEST(checkInBoundsTest, InBounds)
{
    lattice.wipe();
    lattice.qubitsX[2] = 1;     //test for outer
    lattice.qubitsX[110] = 1;   //and inner qubits
    lattice.qubitsZ[2] = 1;
    lattice.qubitsZ[110] = 1;
    lattice.syndromeX[0] = 1;
    lattice.syndromeZ[20] = 1;
    EXPECT_NO_THROW(lattice.checkInBounds());
}
TEST(checkInBoundsTest, XErrorOutOfBounds)
{
    lattice.wipe();
    lattice.qubitsX[0] = 1;
    EXPECT_THROW(lattice.checkInBounds(), std::runtime_error);
}
TEST(checkInBoundsTest, ZErrorOutOfBounds)
{
    lattice.wipe();
    lattice.qubitsZ[0] = 1;
    EXPECT_THROW(lattice.checkInBounds(), std::runtime_error);
}
TEST(checkInBoundsTest, XStabOutOfBounds)
{
    lattice.wipe();
    lattice.syndromeX[2] = 1;
    EXPECT_THROW(lattice.checkInBounds(), std::runtime_error);
}
TEST(checkInBoundsTest, ZStabOutOfBounds)
{
   lattice.wipe();
   lattice.syndromeZ[0] = 1;
   EXPECT_THROW(lattice.checkInBounds(), std::runtime_error);
}

//------------------------------------------------------------

TEST(checkInCodespaceTest, Nothing)
{
    lattice.wipe();
    EXPECT_NO_THROW(lattice.checkInCodespace('x', 1, 1));
    EXPECT_NO_THROW(lattice.checkInCodespace('z', 1, 1));
}
TEST(checkInCodespaceTest, XStabiliser)
{
    lattice.wipe();
    lattice.qubitsX[2] = 1;
    lattice.qubitsX[5] = 1;
    lattice.qubitsX[19] = 1;
    lattice.qubitsX[108] = 1;
    EXPECT_NO_THROW(lattice.checkInCodespace('x', 1, 1));
}
TEST(checkInCodespaceTest, XOutOfCodespace)
{
    lattice.wipe();
    lattice.qubitsX[2] = 1;
    EXPECT_THROW(lattice.checkInCodespace('x', 1, 1), std::runtime_error);
}
TEST(checkInCodespaceTest, ZStabiliser)
{
    lattice.wipe();
    lattice.qubitsZ[2] = 1;
    lattice.qubitsZ[108] = 1;
    lattice.qubitsZ[110] = 1;
    EXPECT_NO_THROW(lattice.checkInCodespace('z', 1, 1));
}
TEST(checkInCodespaceTest ZOutOfCodespace)
{
    lattice.wipe();
    lattice.qubitsZ[2] = 1;
    EXPECT_THROW(lattice.checkInCodespace('z', 1, 1), std::runtime_error);
}
TEST(checkInCodespaceTest, InvalidArgument)
{
    EXPECT_THROW(lattice.checkInCodespace('a', 1, 1), std::invalid_argument);
}

//------------------------------------------------------------

TEST(checkJumpCorrectionTest, Nothing)
{
    lattice.wipe();
    EXPECT_NO_THROW(lattice.checkJumpCorrection());
}
TEST(checkJumpCorrectionTest, InnerEmpty)
{
    lattice.wipe();
    lattice.qubitsZ[2] = 1;
    EXPECT_NO_THROW(lattice.checkJumpCorrection());
}
TEST(checkJumpCorrectionTest, InnerNotEmpty)
{
    lattice.wipe();
    lattice.qubits[108] = 1;
    EXPECT_THROW(lattice.checkJumpCorrection(), std::runtime_error);
}

//------------------------------------------------------------

TEST(checkLogicalErrorTest, NoError)
{
    lattice.wipe();
    EXPECT_EQ(lattice.checkLogicalError('x'), 0);
    EXPECT_EQ(lattice.checkLogicalError('z'), 0);
    lattice.qubitsX[2] = 1;
    lattice.qubitsZ[2] = 1;
    EXPECT_EQ(lattice.checkLogicalError('x'), 0);
    EXPECT_EQ(lattice.checkLogicalError('z'), 0);
}
TEST(checkLogicalErrorTest, LogicalXError)
{
    lattice.wipe();
    lattice.qubitsX[2] = 1;
    lattice.qubitsX[20] = 1;
    lattice.qubitsX[38] = 1;
    lattice.qubitsX[110] = 1;
    lattice.qubitsX[128] = 1;
    lattice.qubitsX[146] = 1;
    lattice.qubitsX[218] = 1;
    lattice.qubitsX[236] = 1;
    lattice.qubitsX[254] = 1;
    EXPECT_EQ(lattice.checkLogicalError('x'), 1);
    EXPECT_EQ(lattice.checkLogicalError('z'), 0);
}
TEST(checkLogicalErrorTest, LogicalZError)
{
    lattice.wipe();
    lattice.qubitsZ[2] = 1;
    lattice.qubitsZ[5] = 1;
    lattice.qubitsZ[8] = 1;
    EXPECT_EQ(lattice.checkLogicalError('x'), 0);
    EXPECT_EQ(lattice.checkLogicalError('z'), 1);
}
