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
    cubic::buildLattice(latCubic);
    rhombic::r1::buildLattice(latRhombic1);
    rhombic::r2::buildLattice(latRhombic2);
    
    ASSERT_TRUE(true);
}

//------------------------------------------------------------

TEST(buildOverlappingFacesTest, CorrectOutput)
{
    std::map<pint,ppint> overlappingFaces;
    overlappingFaces = buildOverlappingFaces(lattices);

    ppint overlap1 = overlappingFaces[{0,2}];
    ppint overlap2 = overlappingFaces[{1,0}];
    ppint overlap3 = overlappingFaces[{2,111}];

    ppint overlapExpected1 = {{1,0},{2,111}};
    ppint overlapExpected2 = {{0,2},{2,111}};
    ppint overlapExpected3 = {{0,2},{1,0}};

    EXPECT_EQ(overlap1, overlapExpected1);
    EXPECT_EQ(overlap2, overlapExpected2);
    EXPECT_EQ(overlap3, overlapExpected3);
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
    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices);

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    vint qubitsZExpectedEmpty(3*6*6*6, 0);
    vint qubitsZExpectedR1(3*6*6*6, 0);
    vint qubitsZExpectedR2(3*6*6*6, 0);

    latCubic.qubitsX[2] = 1;
    qubitsZExpectedR1[0] = 1;
    qubitsZExpectedR2[111] = 1;
    
    for (int i = 0; i < 100; i++)
    {
        latRhombic1.wipe();
        latRhombic2.wipe();
        
        applyCCZ(lattices, overlappingFaces, engine, dist, 0);
        EXPECT_TRUE(latRhombic1.qubitsZ == qubitsZExpectedEmpty
                    || latRhombic1.qubitsZ == qubitsZExpectedR1);
        EXPECT_TRUE(latRhombic2.qubitsZ == qubitsZExpectedEmpty
                    || latRhombic2.qubitsZ == qubitsZExpectedR2);

    }
}
TEST(applyCCZTest, multipleCubicErrors)
{
    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices);
    
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    latCubic.qubitsX[2] = 1;
    latCubic.qubitsX[20] = 1;
    latCubic.qubitsX[110] = 1;
    latCubic.qubitsX[128] = 1;
    latCubic.qubitsX[218] = 1;
    latCubic.qubitsX[236] = 1;

    for (int i = 0; i < 100; i++)
    {
        latRhombic1.wipe();
        latRhombic2.wipe();
        
        applyCCZ(lattices, overlappingFaces, engine, dist, 0);
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
    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices);

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    vint qubitsZExpectedEmpty(3*6*6*6, 0);
    vint qubitsZExpectedC(3*6*6*6, 0);
    vint qubitsZExpectedR2(3*6*6*6, 0);

    latRhombic1.qubitsX[0] = 1;
    qubitsZExpectedC[2] = 1;
    qubitsZExpectedR2[111] = 1;
    
    for (int i = 0; i < 100; i++)
    {
        latCubic.wipe();
        latRhombic2.wipe();
        
        applyCCZ(lattices, overlappingFaces, engine, dist, 0);
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
    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices);
    
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    latRhombic1.qubitsX[0] = 1;
    latRhombic1.qubitsX[216] = 1;
    latRhombic1.qubitsX[219] = 1;
    latRhombic1.qubitsX[126] = 1;
    latRhombic1.qubitsX[129] = 1;
    latRhombic1.qubitsX[345] = 1;

    for (int i = 0; i < 100; i++)
    {
        latCubic.wipe();
        latRhombic2.wipe();
        applyCCZ(lattices, overlappingFaces, engine, dist, 0);
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
    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices);

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    vint qubitsZExpectedEmpty(3*6*6*6, 0);
    vint qubitsZExpectedC(3*6*6*6, 0);
    vint qubitsZExpectedR1(3*6*6*6, 0);

    latRhombic2.qubitsX[111] = 1;
    qubitsZExpectedC[2] = 1;
    qubitsZExpectedR1[0] = 1;
    
    for (int i = 0; i < 100; i++)
    {
        latCubic.wipe();
        latRhombic1.wipe();
        applyCCZ(lattices, overlappingFaces, engine, dist, 0);
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
    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices);
    
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    latRhombic2.qubitsX[111] = 1;
    latRhombic2.qubitsX[108] = 1;
    latRhombic2.qubitsX[327] = 1;
    latRhombic2.qubitsX[18] = 1;
    latRhombic2.qubitsX[234] = 1;
    latRhombic2.qubitsX[237] = 1;

    for (int i = 0; i < 100; i++)
    {
        latCubic.wipe();
        latRhombic1.wipe();
        applyCCZ(lattices, overlappingFaces, engine, dist, 0);
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
    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices);
    
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    latCubic.qubitsX[2] = 1;
    latRhombic1.qubitsX[0] = 1;
    latRhombic2.qubitsX[111] = 1;

    applyCCZ(lattices, overlappingFaces, engine, dist, 0);

    EXPECT_EQ(latCubic.qubitsZ[2], 1);
    EXPECT_EQ(latRhombic1.qubitsZ[0], 1);
    EXPECT_EQ(latRhombic2.qubitsZ[111], 1);
}
