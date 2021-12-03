#include "gtest/gtest.h"
#include "cubic.h"
#include "rhombic1.h"
#include "rhombic2.h"
#include "czErrorGen.h"
#include "prepLattices.h"

//------------------------------------------------------------

//This is just a hack to make the testing software build the index vectors
//Means they don't have to be build individually in every test

TEST(indexBuilding, build)
{
    cubic::buildLattice(latCubic, 6);
    rhombic::r1::buildLattice(latRhombic1, 6);
    rhombic::r2::buildLattice(latRhombic2, 6);
    
    ASSERT_TRUE(true);
}

//------------------------------------------------------------

TEST(buildOverlappingFacesTest, CorrectOutput)
{
    std::map<pint,ppint> overlappingFaces;
    overlappingFaces = buildOverlappingFaces(lattices, 6);
    ppint overlap1 = {{1,0},{2,111}};
    ppint overlap2 = {{0,2},{2,111}};
    ppint overlap3 = {{0,2},{1,0}};
    EXPECT_EQ(overlappingFaces[{0,2}], overlap1);
    EXPECT_EQ(overlappingFaces[{1,0}], overlap2);
    EXPECT_EQ(overlappingFaces[{2,111}], overlap3);
}

//------------------------------------------------------------

TEST(getSyndromeVerticesTest, correctOutput)
{
    vvint syndromeVerticesExpected{{},{},{}};
   
    latCubic.syndromeZ[127] = 1;
    latCubic.syndromeZ[128] = 1;
    latCubic.syndromeZ[146] = 1;
    latCubic.syndromeZ[235] = 1;

    syndromeVerticesExpected[0].push_back(42);
    syndromeVerticesExpected[0].push_back(48);
    syndromeVerticesExpected[0].push_back(78);
    syndromeVerticesExpected[0].push_back(84);
    
    latRhombic1.syndromeZ[169] = 1;
    latRhombic1.syndromeZ[171] = 1;
    latRhombic1.syndromeZ[1011] = 1;
    latRhombic1.syndromeZ[1032] = 1;
    
    syndromeVerticesExpected[1].push_back(42);
    syndromeVerticesExpected[1].push_back(79); 
    syndromeVerticesExpected[1].push_back(252);
    syndromeVerticesExpected[1].push_back(258);

    latRhombic2.syndromeZ[193] = 1;
    latRhombic2.syndromeZ[195] = 1;
    latRhombic2.syndromeZ[1035] = 1;
    latRhombic2.syndromeZ[1057] = 1;

    syndromeVerticesExpected[2].push_back(48);
    syndromeVerticesExpected[2].push_back(85);
    syndromeVerticesExpected[2].push_back(258);
    syndromeVerticesExpected[2].push_back(264);

    vvint syndromeVertices = getSyndromeVertices(lattices);
    
    EXPECT_EQ(syndromeVertices, syndromeVerticesExpected);
}

//------------------------------------------------------------

TEST(latticeWhereCellTest, CorrectOutput)
{
    EXPECT_EQ(latticeWhereCell(0, 0, 6), 2);
    EXPECT_EQ(latticeWhereCell(1, 0, 6), 1);
    EXPECT_EQ(latticeWhereCell(0, 1, 6), 2);
    EXPECT_EQ(latticeWhereCell(216, 1, 6), 0);
    EXPECT_EQ(latticeWhereCell(1, 2, 6), 1);
    EXPECT_EQ(latticeWhereCell(216, 2, 6), 0);
}

//------------------------------------------------------------

TEST(applyCCZTest, singleCubicXError)
{
    overlappingFaces = buildOverlappingFaces(lattices, 6);

    vint qubitsZExpectedEmpty(3*6*6*6, 0);
    vint qubitsZExpectedR1(3*6*6*6, 0);
    vint qubitsZExpectedR2(3*6*6*6, 0);

    latCubic.qubitsX[2] = 1;
    vint qubitsZExpectedR1[0] = 1;
    vint qubitsZExpectedR2[111] = 1;
    
    for (int i = 0; i < 100; i++)
    {
        latRhombic1.wipe();
        latRhombic2.wipe();
        applyCCZ(lattices, overlappingFaces, 6, engine, dist, 0);
        EXPECT_TRUE(latRhombic1.qubitsZ == qubitsZExpectedEmpty
                    || latRhombic1.qubitsZ == qubitsZExpectedR1);
        EXPECT_TRUE(latRhombic2.qubitsZ == qubitsZExpectedEmpty
                    || latRhombic2.qubitsZ == qubitsZExpectedR2);

    }
}
TEST(applyCCZTest, multipleCubicErrors)
{
    overlappingFaces = buildOverlappingFaces(lattices, 6);
    
    latCubic.qubitsX[2] = 1;
    latCubic.qubitsX[20] = 1;
    latCubic.qubitsX[110] = 1;
    latCubic.qubitsX[128] = 1;
    latCubic.qubitsX[218] = 1;
    latCubic.qubtisX[236] = 1;

    for (int i = 0; i < 100; i++)
    {
        latRhombic1.wipe();
        latRhombic2.wipe();
        applyCCZ(lattices, overlappingFaces, 6, engine, dist, 0);
        if (latRhombic1.qubitsZ[0] == 1
            || latRhombic1.qubitsZ[216] == 1
            || latRhombic1.qubitsZ[219] == 1) FAIL();
        if (latRhombic2.qubitsZ[111] == 1
            || latRhombic2.qubitsZ[108] == 1
            || latRhombic2.qubitsZ[327] == 1) FAIL();
    }
}
TEST(applyCCZTest, singleR1XError)
{
    overlappingFaces = buildOverlappingFaces(lattices, 6);

    vint qubitsZExpectedEmpty(3*6*6*6, 0);
    vint qubitsZExpectedC(3*6*6*6, 0);
    vint qubitsZExpectedR2(3*6*6*6, 0);

    latRhombic1.qubitsX[0] = 1;
    vint qubitsZExpectedC[2] = 1;
    vint qubitsZExpectedR2[111] = 1;
    
    for (int i = 0; i < 100; i++)
    {
        latCubic.wipe();
        latRhombic2.wipe();
        applyCCZ(lattices, overlappingFaces, 6, engine, dist, 0);
        EXPECT_TRUE(latCubic.qubitsZ == qubitsZExpectedEmpty
                    ||
                    latCubic.qubitsZ == qubitsZExpectedC);
        EXPECT_TRUE(latRhombic2.qubitsZ == qubitsZExpectedEmpty
                    ||
                    latRhombic2.qubitsZ == qubitsZExpectedR2);
    }
}
TEST(applyCCZTest, multipleR1XError)
{
    overlappingFaces = buildOverlappingFaces(lattices, 6);
    
    latRhombic1.qubitsX[0] = 1;
    latRhombic1.qubitsX[216] = 1;
    latRhombic1.qubitsX[219] = 1;
    latRhombic1.qubitsX[126] = 1;
    latRhombic1.qubitsX[129] = 1;
    latRhombic1.qubtisX[345] = 1;

    for (int i = 0; i < 100; i++)
    {
        latCubic.wipe();
        latRhombic2.wipe();
        applyCCZ(lattices, overlappingFaces, 6, engine, dist, 0);
        if (latCubic.qubitsZ[2] == 1
            || latCubic.qubitsZ[110] == 1
            || latCubic.qubitsZ[218] == 1) FAIL();
        if (latRhombic2.qubitsZ[111] == 1
            || latRhombic2.qubitsZ[108] == 1
            || latRhombic2.qubitsZ[327] == 1) FAIL();
    }
}
TEST(applyCCZTest, singleR2XError)
{
    overlappingFaces = buildOverlappingFaces(lattices, 6);

    vint qubitsZExpectedEmpty(3*6*6*6, 0);
    vint qubitsZExpectedC(3*6*6*6, 0);
    vint qubitsZExpectedR1(3*6*6*6, 0);

    latRhombic2.qubitsX[111] = 1;
    vint qubitsZExpectedC[2] = 1;
    vint qubitsZExpectedR1[0] = 1;
    
    for (int i = 0; i < 100; i++)
    {
        latCubic.wipe();
        latRhombic1.wipe();
        applyCCZ(lattices, overlappingFaces, 6, engine, dist, 0);
        EXPECT_TRUE(latCubic.qubitsZ == qubitsZExpectedEmpty
                    ||
                    latCubic.qubitsZ == qubitsZExpectedC);
        EXPECT_TRUE(latRhombic1.qubitsZ == qubitsZExpectedEmpty
                    ||
                    latRhombic1.qubitsZ == qubitsZExpectedR1);
    }
}
TEST(applyCCZTest, multipleR2XError)
{
    overlappingFaces = buildOverlappingFaces(lattices, 6);
    
    latRhombic2.qubitsX[111] = 1;
    latRhombic2.qubitsX[108] = 1;
    latRhombic2.qubitsX[327] = 1;
    latRhombic2.qubitsX[18] = 1;
    latRhombic2.qubitsX[234] = 1;
    latRhombic2.qubtisX[237] = 1;

    for (int i = 0; i < 100; i++)
    {
        latCubic.wipe();
        latRhombic1.wipe();
        applyCCZ(lattices, overlappingFaces, 6, engine, dist, 0);
        if (latCubic.qubitsZ[2] == 1
            || latCubic.qubitsZ[110] == 1
            || latCubic.qubitsZ[218] == 1) FAIL();
        if (latRhombic1.qubitsZ[0] == 1
            || latRhombic1.qubitsZ[216] == 1
            || latRhombic1.qubitsZ[219] == 1) FAIL();
    }
}
TEST(applyCCZTest, link)
{
    overlappingFaces = buildOverlappingFaces(lattices, 6);
    
    latCubic.qubitsX[2] = 1;
    latRhombic1.qubitsX[0] = 1;
    latRhombic2.qubitsX[111] = 1;

    applyCCZ(lattices, overlappingFaces, 6, engine, dist, 0);

    EXPECT_EQ(latCubic.qubitsZ[2] == 1);
    EXPECT_EQ(latRhombic1.qubitsZ[0] == 1);
    EXPECT_EQ(latRhombic2.qubitsZ[111] == 1);
}
