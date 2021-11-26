#include "gtest/gtest.h"
#include "rhombic2.h"

TEST(coordToIndexTest, HandlesCorrectInput)
{
    rhombic2::coord c0;
    rhombic2::coord c1;
    rhombic2::coord c2;

    c0.xi[0] = 4;
    c0.xi[1] = 4;
    c0.xi[2] = 1;
    c0.xi[3] = 0;

    c1.xi[0] = 4;
    c1.xi[1] = 4;
    c1.xi[2] = 1;
    c1.xi[3] = 1;

    c2.xi[0] = 4;
    c2.xi[1] = 15;
    c2.xi[2] = 0;
    c2.xi[3] = 0;
    
    EXPECT_EQ(rhombic2::coordToIndex(c0, 6), 208);
    EXPECT_EQ(rhombic2::coordToIndex(c1, 6), 1288);
    EXPECT_EQ(rhombic2::coordToIndex(c2, 6), 94);
} 
TEST(coordToIndexTest, HandlesOutOfRangeInput)
{
    rhombic2::coord c0;
    rhombic2::coord c1;
    rhombic2::coord c2;
    rhombic2::coord c3;

    c0.xi[0] = -1;
    c0.xi[1] = 0;
    c0.xi[2] = 0;
    c0.xi[3] = 0;
    
    c1.xi[0] = 0;
    c1.xi[1] = 50;
    c1.xi[2] = 0;
    c1.xi[3] = 0;

    c2.xi[0] = 6;
    c2.xi[1] = 0;
    c2.xi[2] = 0;
    c2.xi[3] = 0;

    c3.xi[0] = 0;
    c3.xi[1] = 0;
    c3.xi[2] = 0;
    c3.xi[3] = 3;

    EXPECT_THROW(rhombic2::coordToIndex(c0, 6), std::invalid_argument);
    EXPECT_THROW(rhombic2::coordToIndex(c1, 6), std::invalid_argument);
    EXPECT_THROW(rhombic2::coordToIndex(c2, 6), std::invalid_argument);
    EXPECT_THROW(rhombic2::coordToIndex(c3, 6), std::invalid_argument);
}

//------------------------------------------------------------

TEST(indexToCoordTest, HandlesExpectedInput)
{
    rhombic2::coord c0 = rhombic2::indexToCoord(208, 6);
    rhombic2::coord c1 = rhombic2::indexToCoord(1288, 6);
    rhombic2::coord c2 = rhombic2::indexToCoord(94, 6);

    EXPECT_EQ(c0.xi[0], 4);
    EXPECT_EQ(c0.xi[1], 4);
    EXPECT_EQ(c0.xi[2], 1);
    EXPECT_EQ(c0.xi[3], 0);

    EXPECT_EQ(c1.xi[0], 4);
    EXPECT_EQ(c1.xi[1], 4);
    EXPECT_EQ(c1.xi[2], 1);
    EXPECT_EQ(c1.xi[3], 1);

    EXPECT_EQ(c2.xi[0], 4);
    EXPECT_EQ(c2.xi[1], 15);
    EXPECT_EQ(c2.xi[2], 0);
    EXPECT_EQ(c2.xi[3], 0);
}
TEST(indexToCoordTest, HandlesOutOfRangeInput)
{
    EXPECT_THROW(rhombic2::indexToCoord(-1, 6), std::invalid_argument);
    EXPECT_THROW(rhombic2::indexToCoord(2160, 6), std::invalid_argument);
}

//------------------------------------------------------------

TEST(cellToCoordTest, CorrectOutput)
{
    rhombic2::coord cd1 = rhombic2::cellToCoord(190, 6);
    rhombic2::coord cd2 = rhombic2::cellToCoord(193, 6);

    EXPECT_EQ(cd1.xi[0], 3);
    EXPECT_EQ(cd1.xi[1], 3);
    EXPECT_EQ(cd1.xi[2], 2);

    EXPECT_EQ(cd2.xi[0], 2);
    EXPECT_EQ(cd2.xi[1], 4);
    EXPECT_EQ(cd2.xi[2], 2);
}

//------------------------------------------------------------

TEST(neighTest, CorrectOutputW0)
{
    //v=188 is (2,1,1,0)
    EXPECT_EQ(rhombic2::neigh(188, 0, 1, 6), 1088);
    EXPECT_EQ(rhombic2::neigh(188, 1, 1, 6), 1262);
    EXPECT_EQ(rhombic2::neigh(188, 2, 1, 6), 1267);
    EXPECT_EQ(rhombic2::neigh(188, 3, 1, 6), 1268);
    EXPECT_EQ(rhombic2::neigh(188, 0, -1, 6), 1261);
    EXPECT_EQ(rhombic2::neigh(188, 1, -1, 6), 1087);
    EXPECT_EQ(rhombic2::neigh(188, 2, -1, 6), 1082);
    EXPECT_EQ(rhombic2::neigh(188, 3, -1, 6), 1081);
}
TEST(neighTest, CorrectOutputW1)
{
    //v=1268 is (2,1,1,1)
    EXPECT_EQ(rhombic2::neigh(1268, 0, 1, 6), 195);
    EXPECT_EQ(rhombic2::neigh(1268, 1, 1, 6), 369);
    EXPECT_EQ(rhombic2::neigh(1268, 2, 1, 6), 374);
    EXPECT_EQ(rhombic2::neigh(1268, 3, 1, 6), 375);
    EXPECT_EQ(rhombic2::neigh(1268, 0, -1, 6), 368);
    EXPECT_EQ(rhombic2::neigh(1268, 1, -1, 6), 194);
    EXPECT_EQ(rhombic2::neigh(1268, 2, -1, 6), 189);
    EXPECT_EQ(rhombic2::neigh(1268, 3, -1, 6), 188);
}
TEST(neighTest, xGreaterThanL)
{
    //v=196 is (10,1,1,0)
    EXPECT_EQ(rhombic2::neigh(196, 0, 1, 6), 1096);
    EXPECT_EQ(rhombic2::neigh(196, 1, 1, 6), 1270);
    EXPECT_EQ(rhombic2::neigh(196, 2, 1, 6), 1275);
    EXPECT_EQ(rhombic2::neigh(196, 3, 1, 6), 1276);
    EXPECT_EQ(rhombic2::neigh(196, 0, -1, 6), 1269);
    EXPECT_EQ(rhombic2::neigh(196, 1, -1, 6), 1095);
    EXPECT_EQ(rhombic2::neigh(196, 2, -1, 6), 1090);
    EXPECT_EQ(rhombic2::neigh(196, 3, -1, 6), 1089);

    //v=1276 is (10,1,1,1)
    EXPECT_EQ(rhombic2::neigh(1276, 0, 1, 6), 203);
    EXPECT_EQ(rhombic2::neigh(1276, 1, 1, 6), 377);
    EXPECT_EQ(rhombic2::neigh(1276, 2, 1, 6), 382);
    EXPECT_EQ(rhombic2::neigh(1276, 3, 1, 6), 383);
    EXPECT_EQ(rhombic2::neigh(1276, 0, -1, 6), 376);
    EXPECT_EQ(rhombic2::neigh(1276, 1, -1, 6), 202);
    EXPECT_EQ(rhombic2::neigh(1276, 2, -1, 6), 197);
    EXPECT_EQ(rhombic2::neigh(1276, 3, -1, 6), 196);
}
TEST(neighTest, HandlesIncorrectArugments)
{
    EXPECT_THROW(rhombic2::neigh(188, -1, 1, 6), std::invalid_argument);
    EXPECT_THROW(rhombic2::neigh(188, 4, 1, 6), std::invalid_argument);
    EXPECT_THROW(rhombic2::neigh(188, 0, 0, 6), std::invalid_argument);
}

//------------------------------------------------------------

TEST(neighXYZTest, CorrectOutput)
{
    EXPECT_EQ(rhombic2::neighXYZ(187, 0, 1, 6), 188);
    EXPECT_EQ(rhombic2::neighXYZ(187, 1, 1, 6), 193);
    EXPECT_EQ(rhombic2::neighXYZ(187, 2, 1, 6), 367);
    EXPECT_EQ(rhombic2::neighXYZ(187, 0, -1, 6), 186);
    EXPECT_EQ(rhombic2::neighXYZ(187, 1, -1, 6), 181);
    EXPECT_EQ(rhombic2::neighXYZ(187, 2, -1, 6), 7);
}

//------------------------------------------------------------

TEST(edgeIndexTest, CorrectOutput)
{
    EXPECT_EQ(rhombic2::edgeIndex(188, 0, 1, 6), 752);
    EXPECT_EQ(rhombic2::edgeIndex(188, 1, 1, 6), 753);
    EXPECT_EQ(rhombic2::edgeIndex(188, 2, 1, 6), 754);
    EXPECT_EQ(rhombic2::edgeIndex(188, 3, 1, 6), 755);
    EXPECT_EQ(rhombic2::edgeIndex(188, 0, -1, 6), 5044);
    EXPECT_EQ(rhombic2::edgeIndex(188, 1, -1, 6), 4349);
    EXPECT_EQ(rhombic2::edgeIndex(188, 2, -1, 6), 4330);
    EXPECT_EQ(rhombic2::edgeIndex(188, 3, -1, 6), 4327);
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

    faceToVertices.assign(3 * 5 * 6 * 6 * 6, {});
    faceToEdges.assign(3 * 5 * 6 * 6 * 6,  {});
    faceToCells.assign(3 * 5 * 6 * 6 * 6, {});
    vertexToFaces.assign(2 * 5 * 6 * 6 * 6, {});
    edgeToFaces.assign(8 * 5 * 6 * 6 * 6, {});
    
    //v=187 is (1,1,1,0)
    rhombic2::addFace(187, 0, {3, 1, 1, 3}, {0, 2}, {1, 1, 1, 1}, {1, 1}, faceToVertices, 
                        faceToEdges, faceToCells, vertexToFaces, edgeToFaces, 6);
    rhombic2::addFace(187, 1, {0, 1, 1, 0}, {1, 2}, {1, -1, -1, 1}, {1, -1}, faceToVertices, 
                        faceToEdges, faceToCells, vertexToFaces, edgeToFaces, 6);

    vint vertices1 = {187, 368, 1261, 1267};
    vint edges1 = {749, 751, 5047, 5069};
    pint cells1 = {94, 183};
    vint vertices2 = {13, 187, 1086, 1087};
    vint edges2 = {53, 748, 4344, 4345};
    pint cells2 = {3, 96};
    vint faces1 = {0, 1};
    vint faces2 = {0};
    vint faces3 = {1};

    EXPECT_EQ(faceToVertices[0], vertices1);
    EXPECT_EQ(faceToEdges[0], edges1);
    EXPECT_EQ(faceToCells[0], cells1);
    EXPECT_EQ(faceToVertices[1], vertices2);
    EXPECT_EQ(faceToEdges[1], edges2);
    EXPECT_EQ(faceToCells[1], cells2);
    EXPECT_EQ(vertexToFaces[187], faces1);
    EXPECT_EQ(vertexToFaces[368], faces2);
    EXPECT_EQ(vertexToFaces[13], faces3);
    EXPECT_EQ(edgeToFaces[749], faces2);
    EXPECT_EQ(edgeToFaces[748], faces3);
}

//------------------------------------------------------------

TEST(buildFacesTest, CorrectOutput)
{   
    vvint faceToVertices;
    vvint faceToEdges;
    vpint faceToCells;
    vvint vertexToFaces;
    vvint edgeToFaces;

    rhombic2::buildFaces(faceToVertices, faceToEdges, faceToCells, vertexToFaces, edgeToFaces, 6);

    vint vertices1 = {1, 182, 1081, 1255};
    vint edges1 = {5, 7, 4325, 5023};

    EXPECT_EQ(faceToVertices[1], vertices1);
    EXPECT_EQ(faceToEdges[1], edges1);
    EXPECT_EQ(vertexToFaces[1][1], 1);
    EXPECT_EQ(vertexToFaces[182][0], 1);
    EXPECT_EQ(vertexToFaces[1081][1], 1);
    EXPECT_EQ(vertexToFaces[1255][0], 1);
    EXPECT_EQ(edgeToFaces[7][1], 1);
    EXPECT_EQ(edgeToFaces[5][0], 1);
    EXPECT_EQ(edgeToFaces[4325][0], 1);
    EXPECT_EQ(edgeToFaces[5023][0], 1);
}

//------------------------------------------------------------
    
TEST(buildVertexToEdgesTest, CorrectOutput)
{
    vvint vertexToEdges = rhombic2::buildVertexToEdges(6);
    
    vint edges1 = {};
    vint edges2 = {748, 749, 750, 751, 4323, 4326, 4345, 5040};
    vint edges3 = {758, 777, 1472, 5075};
    vint edges4 = {751, 5068, 5069, 5070};
    
    EXPECT_EQ(vertexToEdges[188], edges1);
    EXPECT_EQ(vertexToEdges[187], edges2);
    EXPECT_EQ(vertexToEdges[1268], edges3);
    EXPECT_EQ(vertexToEdges[1267], edges4);
}

//------------------------------------------------------------

TEST(buildEdgeToVerticesTest, CorrectOutput)
{
    vpint edgeToVertices = rhombic2::buildEdgeToVertices(6);

    std::pair<int, int> vertices1 = {187, 1087};
    std::pair<int, int> vertices2 = {187, 1261};
    std::pair<int, int> vertices3 = {187, 1266};
    std::pair<int, int> vertices4 = {187, 1267};
    
    EXPECT_EQ(edgeToVertices[748], vertices1);
    EXPECT_EQ(edgeToVertices[749], vertices2);
    EXPECT_EQ(edgeToVertices[750], vertices3);
    EXPECT_EQ(edgeToVertices[751], vertices4);
}

//------------------------------------------------------------

TEST(findFaceTest, CorrectOutput)
{
    vvint faceToVertices;
    vvint faceToEdges;
    vpint faceToCells;
    vvint vertexToFaces;
    vvint edgeToFaces;

    rhombic2::buildFaces(faceToVertices, faceToEdges, faceToCells, vertexToFaces, edgeToFaces, 6);
    vint vertices = {1, 182, 1081, 1255};
    EXPECT_EQ(findFace(vertices, vertexToFaces, faceToVertices), 1);
}

//------------------------------------------------------------

TEST(buildCellToFacesTest, CorrectOutput)
{
    vvint faceToVertices;
    vvint faceToEdges;
    vpint faceToCells;
    vvint vertexToFaces;
    vvint edgeToFaces;

    rhombic2::buildFaces(faceToVertices, faceToEdges, faceToCells, vertexToFaces, edgeToFaces, 6);
    vvint cellToFaces = rhombic2::buildCellToFaces(vertexToFaces, faceToVertices, 6);

    EXPECT_EQ(cellToFaces[283][0], 1721);
    EXPECT_EQ(cellToFaces[283][1], 1682);
    EXPECT_EQ(cellToFaces[283][2], 1700);
    EXPECT_EQ(cellToFaces[283][3], 1703);
    EXPECT_EQ(cellToFaces[283][4], 2242);
    EXPECT_EQ(cellToFaces[283][5], 1699);
    EXPECT_EQ(cellToFaces[283][6], 2241);
    EXPECT_EQ(cellToFaces[283][7], 1680);
    EXPECT_EQ(cellToFaces[283][8], 1159);
    EXPECT_EQ(cellToFaces[283][9], 1702);
    EXPECT_EQ(cellToFaces[283][10], 1158);
    EXPECT_EQ(cellToFaces[283][11], 1683);
}


//------------------------------------------------------------

TEST(buildSliceTest, CorrectOutput)
{
    vint lowerQubitIndices;
    vint middleQubitIndices;
    vint upperQubitIndices;
    vint syndIndicesZ;
    vint syndIndicesXLayer;
    vint defectIndices;
    vint bulkSweepVertices;
    vint middleBoundarySweepVertices;
    vint zLogical;
    vint xLogicalFragment1;
    vint xLogicalFragment2;

    vvint faceToVertices;
    vvint faceToEdges;
    vpint faceToCells;
    vvint vertexToFaces;
    vvint edgeToFaces;
    rhombic2::buildFaces(faceToVertices, faceToEdges, faceToCells, vertexToFaces, edgeToFaces, 6);

    vvint vertexToEdges = rhombic2::buildVertexToEdges(6);
    vpint edgeToVertices = rhombic2::buildEdgeToVertices(6);

    rhombic2::buildSlice(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndicesZ, syndIndicesXLayer, defectIndices, bulkSweepVertices, middleBoundarySweepVertices, zLogical, xLogicalFragment1, xLogicalFragment2, vertexToFaces, faceToVertices, faceToEdges, edgeToVertices, 6);

    vint expectedLowerQubitIndices = {618, 619, 600, 601, 1140, 1141, 1142, 588, 1122, 1123,
                                 1124, 1662, 1663, 1664, 1110, 1644, 1645, 1646, 1632};
    vint expectedMiddleQubitIndices = {1179, 1161, 1181, 1180, 1701, 1149, 1163, 1162, 1683, 
                                  1703, 1702, 2223, 1671, 1685, 1684, 2205, 2224, 2193, 2206};
    vint expectedUpperQubitIndices = {654, 655, 636, 637, 1176, 1177, 1178, 624, 1158, 1159,
                                      1160, 1698, 1699, 1700, 1146, 1680, 1681, 1682, 1668};
    vint expectedSyndIndicesZ = {873, 875, 853, 855, 5144, 854, 5124, 834, 2960, 6538, 
                           6563, 2940, 6518, 6543, 6537, 6517, 1530, 1548, 1549,
                           1550, 1551, 1568, 1569, 1571, 2226, 2244, 2245, 2246, 2247, 2264, 
                           2265, 2267, 5125, 5126, 5145, 5146, 5151, 5171, 5820, 5821, 5822, 
                           5840, 5841, 5842, 5847, 5867, 6516, 6536};
    vint expectedSyndIndicesXLayer = {199, 196, 286, 194, 283, 281};
    vint expectedDefectIndices = {1286, 1281, 735, 213, 387, 561, 1287, 1292, 1455, 1460, 1461, 
                            1466, 1629, 1634, 1635, 1640};
    vint expectedBulkSweepVertices = {208, 213, 218, 382, 387, 392, 556, 561, 566, 1281, 1286,
                                 1291, 1455, 1460, 1465, 1629, 1634, 1639};
    vint expectedMiddleBoundarySweepVertices = {730, 735, 740};
    vint expectedZLogical = {654, 1176, 1698};
    vint expectedXLogicalFragment1 = {1662, 1663, 1644, 1645, 1632};
    vint expectedXLogicalFragment2 = {2223, 2224, 2205, 2206, 2193};
    
    EXPECT_EQ(lowerQubitIndices, expectedLowerQubitIndices);
    EXPECT_EQ(middleQubitIndices, expectedMiddleQubitIndices);
    EXPECT_EQ(upperQubitIndices, expectedUpperQubitIndices);
    EXPECT_EQ(syndIndicesZ, expectedSyndIndicesZ);
    EXPECT_EQ(syndIndicesXLayer, expectedSyndIndicesXLayer);
    EXPECT_EQ(defectIndices, expectedDefectIndices);
    EXPECT_EQ(bulkSweepVertices, expectedBulkSweepVertices);
    EXPECT_EQ(middleBoundarySweepVertices, expectedMiddleBoundarySweepVertices);
    EXPECT_EQ(zLogical, expectedZLogical);
    EXPECT_EQ(xLogicalFragment1, expectedXLogicalFragment1);
    EXPECT_EQ(xLogicalFragment2, expectedXLogicalFragment2);
}

//------------------------------------------------------------

TEST(moveFacesTest, CorrectOutput)
{
    vvint faceToVertices;
    vvint faceToEdges;
    vpint faceToCells;
    vvint vertexToFaces;
    vvint edgeToFaces;
    rhombic2::buildFaces(faceToVertices, faceToEdges, faceToCells, vertexToFaces, edgeToFaces, 6);
    vint lowerQubitIndices = {636, 637, 618, 619, 1158, 1159, 1160, 606, 1140, 1141,
                                 1142, 1680, 1681, 1682, 1128, 1662, 1663, 1664, 1650};
    vint upperQubitIndices = {672, 673, 654, 655, 1194, 1195, 1196, 642, 1176, 1177,
                                      1178, 1716, 1717, 1718, 1164, 1698, 1699, 1700, 1686};

    EXPECT_EQ(rhombic2::moveFaces(lowerQubitIndices, vertexToFaces, faceToVertices, 6), upperQubitIndices);
}

//------------------------------------------------------------

TEST(moveIndicesTest, CorrectOutput)
{   
    vvint faceToVertices;
    vvint faceToEdges;
    vpint faceToCells;
    vvint vertexToFaces;
    vvint edgeToFaces;
    rhombic2::buildFaces(faceToVertices, faceToEdges, faceToCells, vertexToFaces, edgeToFaces, 6);
    vint lowerQubitIndices = {636, 637, 618, 619, 1158, 1159, 1160, 606, 1140, 1141,
                                 1142, 1680, 1681, 1682, 1128, 1662, 1663, 1664, 1650};
    vint middleQubitIndices = {1197, 1179, 1199, 1198, 1719, 1167, 1181, 1180, 1701, 
                                 1721, 1720, 2241, 1689, 1703, 1702, 2223, 2242, 2211, 
                                 2224};
    vint upperQubitIndices = {672, 673, 654, 655, 1194, 1195, 1196, 642, 1176, 1177,
                                 1178, 1716, 1717, 1718, 1164, 1698, 1699, 1700, 1686};
    vint syndIndices = {897, 899, 877, 879, 5168, 878, 851, 5148, 858, 831, 2984, 6562, 
                           6587, 2964, 6542, 6567, 6561, 6539, 6541, 6519, 1554, 1572, 1573,
                           1574, 1575, 1592, 1593, 1595, 2250, 2268, 2269, 2270, 2271, 2288, 
                           2289, 2291, 5149, 5150, 5169, 5170, 5175, 5195, 5844, 5845, 5846, 
                           5864, 5865, 5866, 5871, 5891, 6540, 6560};
    vint defectIndices = {219, 393, 567, 741, 1287, 1292, 1293, 1298, 1461, 1466, 1467, 
                            1472, 1635, 1640, 1641, 1646};
    vint bulkSweepVertices = {214, 219, 224, 388, 393, 398, 562, 567, 572, 1287, 1292,
                                 1297, 1461, 1466, 1471, 1635, 1640, 1645};
    vint middleBoundarySweepVertices = {736, 741, 746};
    vint zLogical = {672, 1194, 1716};
    vint movedLowerQubitIndices = {672, 673, 654, 655, 1194, 1195, 1196, 642, 1176, 1177,
                                 1178, 1716, 1717, 1718, 1164, 1698, 1699, 1700, 1686};
    vint movedMiddleQubitIndices = {1233, 1215, 1235, 1234, 1755, 1203, 1217, 1216, 1737, 
                                 1757, 1756, 2277, 1725, 1739, 1738, 2259, 2278, 2247, 
                                 2260};
    vint movedUpperQubitIndices = {708, 709, 690, 691, 1230, 1231, 1232, 678, 1212, 1213,
                                 1214, 1752, 1753, 1754, 1200, 1734, 1735, 1736, 1722};
    vint movedSyndIndices = {945, 947, 925, 927, 5216, 926, 899, 5196, 906, 879, 3032, 6610, 
                           6635, 3012, 6590, 6615, 6609, 6587, 6589, 6567, 1602, 1620, 1621,
                           1622, 1623, 1640, 1641, 1643, 2298, 2316, 2317, 2318, 2319, 2336, 
                           2337, 2339, 5197, 5198, 5217, 5218, 5223, 5243, 5892, 5893, 5894, 
                           5912, 5913, 5914, 5919, 5939, 6588, 6608};
    vint movedDefectIndices = {231, 405, 579, 753, 1299, 1304, 1305, 1310, 1473, 1478, 1479, 
                            1484, 1647, 1652, 1653, 1658};
    vint movedBulkSweepVertices = {226, 231, 236, 400, 405, 410, 574, 579, 584, 1299, 1304,
                                 1309, 1473, 1478, 1483, 1647, 1652, 1657};
    vint movedMiddleBoundarySweepVertices = {748, 753, 758};
    vint movedZLogical = {708, 1230, 1752};

    rhombic2::moveIndices(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, defectIndices, bulkSweepVertices, middleBoundarySweepVertices, zLogical, vertexToFaces, faceToVertices, 6);

    EXPECT_EQ(lowerQubitIndices, movedLowerQubitIndices);
    EXPECT_EQ(middleQubitIndices, movedMiddleQubitIndices);
    EXPECT_EQ(upperQubitIndices, movedUpperQubitIndices);
    EXPECT_EQ(syndIndices, movedSyndIndices);
    EXPECT_EQ(defectIndices, movedDefectIndices);
    EXPECT_EQ(bulkSweepVertices, movedBulkSweepVertices);
    EXPECT_EQ(middleBoundarySweepVertices, movedMiddleBoundarySweepVertices);
    EXPECT_EQ(zLogical, movedZLogical);
}
