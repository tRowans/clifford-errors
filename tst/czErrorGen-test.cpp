#include "gtest/gtest.h"
#include "cubic.h"
#include "rhombic1.h"
#include "rhombic2.h"
#include "czErrorGen.h"
#include "prepCZ.h"

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

    overlap1 = overlappingFaces[{0,108}];
    overlap2 = overlappingFaces[{1,131}];
    overlap3 = overlappingFaces[{2,110}];

    overlapExpected1 = {{1,131},{2,110}}; 
    overlapExpected2 = {{0,108},{2,110}}; 
    overlapExpected3 = {{0,108},{1,131}}; 
    
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
    latRhombic1.syndromeZ[1033] = 1;

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

/*
 * Not in use
TEST(latticeWhereCellTest, CorrectOutput)
{
    EXPECT_EQ(latticeWhereCell(0, 0, 6), 2);
    EXPECT_EQ(latticeWhereCell(1, 0, 6), 1);
    EXPECT_EQ(latticeWhereCell(0, 1, 6), 2);
    EXPECT_EQ(latticeWhereCell(216, 1, 6), 0);
    EXPECT_EQ(latticeWhereCell(1, 2, 6), 1);
    EXPECT_EQ(latticeWhereCell(216, 2, 6), 0);
}
*/

//------------------------------------------------------------

/*
 * Simple version doesn't need all these
 *
TEST(applyCCZTest, singleCubicXError)
{
    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices);

    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    vint qubitsZExpectedEmpty(3*6*6*6, 0);
    vint qubitsZExpectedR1(3*6*6*6, 0);
    vint qubitsZExpectedR2(3*6*6*6, 0);

    latCubic.wipe();
    latCubic.qubitsX[2] = 1;
    latCubic.calcSynd('z',1,1);
   
    int r1QubitTotal = 0;
    int r2QubitTotal = 0; 
    std::set<int> r1WrongQubits;
    std::set<int> r2WrongQubits;
    for (int i = 0; i < 100; i++)
    {
        latRhombic1.wipe();
        latRhombic2.wipe();
    
        applyCCZ(lattices, overlappingFaces, engine, dist, 0);
        r1QubitTotal += latRhombic1.qubitsZ[0];
        r2QubitTotal += latRhombic2.qubitsZ[111];
        for (int j = 0; j < 3*6*6*6; j++)
        {
            if (latRhombic1.qubitsZ[j] == 1 && j != 0) r1WrongQubits.insert(j);
            if (latRhombic2.qubitsZ[j] == 1 && j != 111) r2WrongQubits.insert(j);
        }
    }
    std::cout << "#R1 Z errors: " << r1QubitTotal << '\n';
    std::cout << "#R2 Z errors: " << r2QubitTotal << '\n';
    std::set<int> emptySet = {};
    EXPECT_EQ(r1WrongQubits, emptySet);
    EXPECT_EQ(r2WrongQubits, emptySet);
}
TEST(applyCCZTest, membraneCubic)
{
    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices);
    
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    latCubic.wipe();
    latCubic.qubitsX[2] = 1;
    latCubic.qubitsX[20] = 1;
    latCubic.qubitsX[110] = 1;
    latCubic.qubitsX[128] = 1;
    latCubic.qubitsX[218] = 1;
    latCubic.qubitsX[236] = 1;
    latCubic.calcSynd('z',1,1);
    
    vint r1QubitTotals = {0,0,0};
    vint r2QubitTotals = {0,0,0};
    std::set<int> r1WrongQubits;
    std::set<int> r2WrongQubits;
    for (int i = 0; i < 100; i++)
    {
        latRhombic1.wipe();
        latRhombic2.wipe();
        
        applyCCZ(lattices, overlappingFaces, engine, dist, 0);
        r1QubitTotals[0] += latRhombic1.qubitsZ[126];
        r1QubitTotals[1] += latRhombic1.qubitsZ[129];
        r1QubitTotals[2] += latRhombic1.qubitsZ[345];
        r2QubitTotals[0] += latRhombic2.qubitsZ[18];
        r2QubitTotals[1] += latRhombic2.qubitsZ[234];
        r2QubitTotals[2] += latRhombic2.qubitsZ[237];
        for (int j = 0; j < 3*6*6*6; j++)
        {
            if (latRhombic1.qubitsZ[j] == 1 &&
                    j != 126 && j != 129 && j != 345) r1WrongQubits.insert(j);
            if (latRhombic2.qubitsZ[j] == 1 &&
                    j != 18 && j != 234 && j != 237) r2WrongQubits.insert(j);
        }
    }
    std::cout << "R1 Z errors: ";
    for (int i : r1QubitTotals) std::cout << i << ' ';
    std::cout << '\n';
    std::cout << "R2 Z errors: ";
    for (int i : r2QubitTotals) std::cout << i << ' ';
    std::cout << '\n';
    std::set<int> emptySet = {};
    EXPECT_EQ(r1WrongQubits, emptySet);
    EXPECT_EQ(r2WrongQubits, emptySet);
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

    latRhombic1.wipe();
    latRhombic1.qubitsX[0] = 1;
    latRhombic1.calcSynd('z',1,1);
    
    int cQubitTotal = 0;
    int r2QubitTotal = 0; 
    std::set<int> cWrongQubits;
    std::set<int> r2WrongQubits;
    for (int i = 0; i < 100; i++)
    {
        latCubic.wipe();
        latRhombic2.wipe();
        
        applyCCZ(lattices, overlappingFaces, engine, dist, 0);
        cQubitTotal += latCubic.qubitsZ[2];
        r2QubitTotal += latRhombic2.qubitsZ[111];
        for (int j = 0; j < 3*6*6*6; j++)
        {
            if (latCubic.qubitsZ[j] == 1 && j != 2) cWrongQubits.insert(j);
            if (latRhombic2.qubitsZ[j] == 1 && j != 111) r2WrongQubits.insert(j);
        }
    }
    std::cout << "#Cb Z errors: " << cQubitTotal << '\n';
    std::cout << "#R2 Z errors: " << r2QubitTotal << '\n';
    std::set<int> emptySet = {};
    EXPECT_EQ(cWrongQubits, emptySet);
    EXPECT_EQ(r2WrongQubits, emptySet);
}
TEST(applyCCZTest, membraneR1)
{
    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices);
    
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    latRhombic1.wipe();
    latRhombic1.qubitsX[0] = 1;
    latRhombic1.qubitsX[111] = 1;
    latRhombic1.qubitsX[131] = 1;
    latRhombic1.qubitsX[219] = 1;
    latRhombic1.calcSynd('z',1,1);

    vint cQubitTotals = {0,0,0};
    vint r2QubitTotals = {0,0,0};
    std::set<int> cWrongQubits;
    std::set<int> r2WrongQubits;
    for (int i = 0; i < 100; i++)
    {
        latCubic.wipe();
        latRhombic2.wipe();
        
        applyCCZ(lattices, overlappingFaces, engine, dist, 0);
        cQubitTotals[0] += latCubic.qubitsZ[5];
        cQubitTotals[1] += latCubic.qubitsZ[108];
        cQubitTotals[2] += latCubic.qubitsZ[110];
        r2QubitTotals[0] += latRhombic2.qubitsZ[0];
        r2QubitTotals[1] += latRhombic2.qubitsZ[110];
        r2QubitTotals[2] += latRhombic2.qubitsZ[108];
        for (int j = 0; j < 3*6*6*6; j++)
        {
            if (latCubic.qubitsZ[j] == 1 &&
                    j != 5 && j != 108 && j != 110) cWrongQubits.insert(j);
            if (latRhombic2.qubitsZ[j] == 1 &&
                    j != 0 && j != 110 && j != 108) r2WrongQubits.insert(j);
        }
    }
    std::cout << "Cb Z errors: ";
    for (int i : cQubitTotals) std::cout << i << ' ';
    std::cout << '\n';
    std::cout << "R2 Z errors: ";
    for (int i : r2QubitTotals) std::cout << i << ' ';
    std::cout << '\n';
    std::set<int> emptySet = {};
    EXPECT_EQ(cWrongQubits, emptySet);
    EXPECT_EQ(r2WrongQubits, emptySet);
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

    latRhombic2.wipe();
    latRhombic2.qubitsX[111] = 1;
    latRhombic2.calcSynd('z',1,1);

    int cQubitTotal = 0;
    int r1QubitTotal = 0; 
    std::set<int> cWrongQubits;
    std::set<int> r1WrongQubits;
    for (int i = 0; i < 100; i++)
    {
        latCubic.wipe();
        latRhombic1.wipe();
        
        applyCCZ(lattices, overlappingFaces, engine, dist, 0);
        cQubitTotal += latCubic.qubitsZ[2];
        r1QubitTotal += latRhombic1.qubitsZ[0];
        for (int j = 0; j < 3*6*6*6; j++)
        {
            if (latCubic.qubitsZ[j] == 1 && j != 2) cWrongQubits.insert(j);
            if (latRhombic1.qubitsZ[j] == 1 && j != 0) r1WrongQubits.insert(j);
        }
    }
    std::cout << "#Cb Z errors: " << cQubitTotal << '\n';
    std::cout << "#R1 Z errors: " << r1QubitTotal << '\n';
    std::set<int> emptySet = {};
    EXPECT_EQ(cWrongQubits, emptySet);
    EXPECT_EQ(r1WrongQubits, emptySet);
}
TEST(applyCCZTest, membraneR2)
{
    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices);
    
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    latRhombic2.wipe();
    latRhombic2.qubitsX[111] = 1;
    latRhombic2.qubitsX[0] = 1;
    latRhombic2.qubitsX[18] = 1;
    latRhombic2.qubitsX[19] = 1;
    latRhombic2.calcSynd('z',1,1);

    vint cQubitTotals = {0,0,0};
    vint r1QubitTotals = {0,0,0};
    std::set<int> cWrongQubits;
    std::set<int> r1WrongQubits;
    for (int i = 0; i < 100; i++)
    {
        latCubic.wipe();
        latRhombic1.wipe();
        
        applyCCZ(lattices, overlappingFaces, engine, dist, 0);
        cQubitTotals[0] += latCubic.qubitsZ[5];
        cQubitTotals[1] += latCubic.qubitsZ[19];
        cQubitTotals[2] += latCubic.qubitsZ[20];
        r1QubitTotals[0] += latRhombic1.qubitsZ[111];
        r1QubitTotals[1] += latRhombic1.qubitsZ[129];
        r1QubitTotals[2] += latRhombic1.qubitsZ[130];
        for (int j = 0; j < 3*6*6*6; j++)
        {
            if (latCubic.qubitsZ[j] == 1 &&
                    j != 5 && j != 19 && j != 20) cWrongQubits.insert(j);
            if (latRhombic1.qubitsZ[j] == 1 &&
                    j != 111 && j != 129 && j != 130) r1WrongQubits.insert(j);
        }
    }
    std::cout << "Cb Z errors: ";
    for (int i : cQubitTotals) std::cout << i << ' ';
    std::cout << '\n';
    std::cout << "R1 Z errors: ";
    for (int i : r1QubitTotals) std::cout << i << ' ';
    std::cout << '\n';
    std::set<int> emptySet = {};
    EXPECT_EQ(cWrongQubits, emptySet);
    EXPECT_EQ(r1WrongQubits, emptySet);
}
*/
TEST(applyCCZTest, link)
{
    std::map<pint,ppint> overlappingFaces = buildOverlappingFaces(lattices);
    
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);

    latCubic.qubitsX[2] = 1;
    latRhombic1.qubitsX[0] = 1;
    latRhombic2.qubitsX[111] = 1;

    applyCCZ(lattices, overlappingFaces);

    EXPECT_EQ(latCubic.qubitsZ[2], 1);
    EXPECT_EQ(latRhombic1.qubitsZ[0], 1);
    EXPECT_EQ(latRhombic2.qubitsZ[111], 1);
}
