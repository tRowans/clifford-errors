#include "decoderRhombic1.h"

namespace rhombic1 {

int shortestPathLength(int v1, int v2, int L)
{
    coord c1 = indexToCoord(v1, L);
    coord c2 = indexToCoord(v2, L);
    int dist = 0;
    vint diff = {c2.xi[0] - c1.xi[0],
                 c2.xi[1] - c1.xi[1],
                 c2.xi[2] - c1.xi[2]};
    if (c1.xi[3] == 1)
    {
        if ((diff[0] + diff[1] + diff[2]) > 0) dist -= 1;
        else dist += 1;
    }
    if (c2.xi[3] == 1)
    {
        if((diff[0] + diff[1] + diff[2]) > 0) dist += 1;
        else dist -= 1;
    }
    for (int &i : diff) i = abs(i);
    std::sort(diff.begin(), diff.end());
    if ((diff[0] + diff[1]) > diff[2]) dist += 2*(diff[0] + diff[1]);
    else dist += 2*diff[2];
    return abs(dist); //can be -1 if both vertices have the same first 3 coords
}

vint shortestPath(int v1, int v2, vint &syndIndices, vvint &vertexToEdges, int L)
{
    int originalVertex = v1;
    coord c1 = indexToCoord(v1, L);
    coord c2 = indexToCoord(v2, L);
    std::vector<float> diff = {c2.xi[0] - c1.xi[0] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2), 
                               c2.xi[1] - c1.xi[1] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2),
                               c2.xi[2] - c1.xi[2] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2)};
    vint path = {};
    //Need to add 1 to each or xy and -xy look the same
    vint dirs1 = {xy+1, xz+1, yz+1, xyz+1, -1*(xy+1), -1*(xz+1), -1*(yz+1), -1*(xyz+1)};
    vint dirs2 = {xy+1, xz+1, yz+1, -1*(xyz+1)};
    vint dirs3 = {xyz+1, -1*(xy+1), -1*(xz+1), -1*(yz+1)};
    int turnAround = 0;
    while (abs(diff[0]) + abs(diff[1]) + abs(diff[2]) > 0)
    {
        int bestEdge;
        int bestProduct;
        vpint products;
        vint dirs;
        if (c1.xi[3] == 0) dirs = dirs1;
        else 
        {
            if ((c1.xi[0] + c1.xi[1] + c1.xi[2]) % 2 == 0) dirs = dirs2;
            else dirs = dirs3;
        }
        vint edges;
        for (int i = 0; i < dirs.size(); i++)
        {
            int edge;
            if (dirs[i] > 0) edge = edgeIndex(v1, dirs[i]-1, 1, L);
            else edge = edgeIndex(v1, -1*dirs[i]-1, -1, L);
            edges.push_back(edge);
            if (std::find(syndIndices.begin(), syndIndices.end(), edge) != syndIndices.end())
            {
                products.push_back({scalarProduct(diff, dirs[i]), i});
            }
        }
        std::sort(products.begin(), products.end());
        if (turnAround == 1) 
        {
            products.pop_back();
            turnAround = 0;
        }
        bestEdge = products.back().second;
        //I don't know if we need this here 
        //because I don't remember what the problem cases were in the slices
        //but probably best to just leave it in
        if (std::find(path.begin(), path.end(), edges[bestEdge]) != path.end())
        {
            //Sometimes gets stuck at a dead end and needs to turn around
            //we go back to the previous vertex and rerun then choose the second best edge 
            if (path.back() == edges[bestEdge]) turnAround = 1;
            //If we have gone in a larger loop just abort
            //but I don't think this should happen
            else
            {
                std::string errorMsg = "Loop occurred while building path between vertices " + std::to_string(v1) + " and " + std::to_string(v2) + '\n'+ " (vertices passed to function were " + std::to_string(originalVertex) + " and " + std::to_string(v2) + ")\n";
                throw std::runtime_error(errorMsg);
            }
        }
        if (turnAround == 0) path.push_back(edges[bestEdge]);
        else path.pop_back();
        if (dirs[bestEdge] > 0) v1 = neigh(v1, dirs[bestEdge]-1, 1, L);
        else v1 = neigh(v1, -1*dirs[bestEdge]-1, -1, L);
        c1 = indexToCoord(v1, L);
        diff = {c2.xi[0] - c1.xi[0] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2), 
                c2.xi[1] - c1.xi[1] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2),
                c2.xi[2] - c1.xi[2] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2)};
    }

    return path;
}

vint shortestDualPath(int cell1, int cell2, vint &qubitIndices, vvint &cellToFaces, int L)
{
    int originalCell = cell1;
    coord cd1 = indexToCoord(cell1, L);
    coord cd2 = indexToCoord(cell2, L);
    vint diff = {cd2.xi[0] - cd1.xi[0], 
                 cd2.xi[1] - cd1.xi[1],
                 cd2.xi[2] - cd1.xi[2]};
    vint path = {};
    vvint dirs = {{1,1,0},{1,-1,0},{-1,1,0},{-1,-1,0},
                  {1,0,1},{-1,0,1},{0,1,1},{0,-1,1},
                  {1,0,-1},{-1,0,-1},{0,1,-1},{0,-1,-1}};
    int turnAround = 0;
    while (abs(diff[0]) + abs(diff[1]) + abs(diff[2]) > 0)
    {
        int bestDir;
        int bestFace;
        vpint products;
        for (int i = 0; i < 12; i++)
        {
            if (std::find(qubitIndices.begin(), qubitIndices.end(), cellToFaces[cell1][i]) 
                        != qubitIndices.end())
            {
                int product = dirs[i][0]*diff[0] + dirs[i][1]*diff[1] + dirs[i][2]*diff[2];
                products.push_back({product, i});
            }
        }
        std::sort(products.begin(), products.end());
        if (turnAround == 1) 
        {
            products.pop_back();
            turnAround = 0;
        }
        bestDir = products.back().second;
        bestFace = cellToFaces[cell1][bestDir];
        //Once again, probably don't need this
        //but no harm in leaving it in just in case 
        if (std::find(path.begin(), path.end(), bestFace) != path.end())
        {
            //Sometimes gets stuck at a dead end and needs to turn around
            //we go back to the previous vertex and rerun then choose the second best edge 
            if (path.back() == bestFace) turnAround = 1;
            //If we have gone in a larger loop just abort
            //but I don't think this should happen
            else
            {
                std::string errorMsg = "Loop occurred while building path between cells " + std::to_string(cell1) + " and " + std::to_string(cell2) + '\n'+ " (cells passed to function were " + std::to_string(originalCell) + " and " + std::to_string(cell2) + ")\n";
                throw std::runtime_error(errorMsg);
            }
        }
        if (turnAround == 0) path.push_back(bestFace);
        else path.pop_back();
        cd1.xi[0] += dirs[bestDir][0];
        cd1.xi[1] += dirs[bestDir][1];
        cd1.xi[2] += dirs[bestDir][2];
        cell1 = coordToIndex(cd1, L);
        diff = {cd2.xi[0] - cd1.xi[0], 
                cd2.xi[1] - cd1.xi[1], 
                cd2.xi[2] - cd1.xi[2]};
    }
    return path;
}

std::vector<int> distanceToClosestXBoundary(int v, int L)
{
    //strings of edges can terminate at w=0 vertices at +/- x and w=1 at +/- z
    coord cd = indexToCoord(v, L);
    int xDist, zDist;

    if (cd.xi[3] == 0)
    {
        if (cd.xi[0] < (L-4)/2) xDist = -2*cd.xi[0];
        else xDist = 2*((L-4)/2 - cd.xi[0]);

        if (cd.xi[2] < (L-4)/2) zDist = -2*cd.xi[2] - 1;
        else zDist = 2*((L-4)/2 - cd.xi[2]) + 1;
    }

    else
    {
        if (cd.xi[0] < (L-4)/2) xDist = -2*cd.xi[0] + 1;
        else xDist = 2*((L-4)/2 - cd.xi[0]) - 1;

        if (cd.xi[2] < (L-4)/2) zDist = -2*cd.xi[2];
        else zDist = 2*((L-4)/2 - cd.xi[2]);
    }

    vint distInfo = {0,0,0}; //dir, sign, dist
    if (abs(xDist) < abs(zDist))
    {
        distInfo[0] = 0;
        distInfo[1] = (0 < xDist) - (0 > xDist);
        distInfo[2] = abs(xDist);
    }
    else 
    {
        distInfo[0] = 2;
        distInfo[1] = (0 < zDist) - (0 > zDist);
        distInfo[2] = abs(zDist);
    }
    
    return distInfo;
}

std::vector<int> shortestPathToXBoundary(int v, int L)   
{
    vint distInfo = distanceToClosestXBoundary(v, L);
    int &dir = distInfo[0];
    int &sign = distInfo[1];
    int &dist = distInfo[2];
    vint path;
    coord cd = indexToCoord(v, L);

    vint moveDir1 = {0,0};   //dir, sign
    vint moveDir2 = {0,0};
    
    if (dir == 0)
    {
        if (cd.xi[3] == 1)
        {
            if ((cd.xi[0] + cd.xi[1] + cd.xi[2]) % 2 == 0)
            {
                if (sign == 1) moveDir1 = {xy,1};
                else moveDir1 == {xyz,-1};
            }
            else 
            {
                if (sign == 1) moveDir1 = {xyz,1};
                else moveDir1 = {xy,-1};
            }
            path.push_back(edgeIndex(v, moveDir1[0], moveDir1[1], L));
            v = neigh(v, moveDir1[0], moveDir1[1], L);
            cd = indexToCoord(v, L);
            dist -= 1;
        }

        int zigzag = 0;
        if (cd.xi[1] == 0 && sign == 1) zigzag = 1;
        else if (cd.xi[1] == L-4 && sign == -1) zigzag = 1;
        while (dist > 0)
        {
            if (zigzag == 0)
            {
                if (sign == 1)
                {
                    moveDir1 = {yz, -1};
                    moveDir2 = {xz, 1};
                }
                else 
                {
                    moveDir1 = {xz, -1};
                    moveDir2 = {yz, 1};
                }
            }
            else 
            {
                if (sign == 1)
                {
                    moveDir1 = {xy, 1};
                    moveDir2 = {xyz, 1};
                }
                else 
                {
                    moveDir1 = {xyz, -1};
                    moveDir2 = {xy, -1};
                }
            }

            path.push_back(edgeIndex(v, moveDir1[0], moveDir1[1], L);
            v = neigh(v, moveDir1[0], moveDir1[1], L);
            path.push_back(edgeIndex(v, moveDir2[0], moveDir2[1], L);
            v = neigh(v, moveDir2[0], moveDir2[1], L);
            zigzag = (zigzag + 1) % 2;
            dist -= 2;
        }
    }

    else
    {
        if (cd.xi[3] == 0)
        {
            if (sign == 1) 
            {
                if (cd.xi[1] == 0) moveDir1 = {xyz, 1};
                else moveDir1 = {xz, 1};
            }
            else 
            {
                if (cd.xi[1] == 0) moveDir1 = {xy, 1};
                else moveDir1 = {yz, -1};
            }

            path.push_back(edgeIndex(v, moveDir1[0], moveDir1[1], L));
            v = neigh(v, moveDir1[0], moveDir1[1], L);
            cd = indexToCoord(v, L);
            dist -= 1;
        }

        while (dist > 0)
        {
            if ((cd.xi[0] + cd.xi[1] + cd.xi[2]) % 2 == 0)
            {
                if (sign == 1)
                {
                    moveDir1 = {xz, 1};
                    moveDir2 = {yz, 1};
                else 
                {
                    moveDir1 = {xyz, -1};
                    moveDir2 = {xy, 1};
                }
            }

            else 
            {
                if (sign == 1)
                {
                    moveDir1 = {xy, -1};
                    moveDir2 = {xyz, 1};
                }
                else
                {
                    moveDir1 = {yz, -1};
                    moveDir2 = {xz, -1};
                }
            }
            
            path.push_back(edgeIndex(v, moveDir1[0], moveDir1[1], L);
            v = neigh(v, moveDir1[0], moveDir1[1], L);
            path.push_back(edgeIndex(v, moveDir2[0], moveDir2[1], L);
            v = neigh(v, moveDir2[0], moveDir2[1], L);
            cd = indexToCoord(v, L);
            dist -= 2;
        }
    }

    return path;
}

vint distanceToClosestZBoundary(int cell, int L)
{
    coord cd = IndexToCoord(cell, L);
    int dist;

    if (cd.xi[1] > (L-3)/2) dist = (L-3) - cd.xi[1];
    else dist = -cd.xi[1];

    return dist;
}

vint shortestPathToZBoundary(int cell, vvint &cellToFaces, int L)
{
    int dist = distanceToClosestZBoundary(cell, L);
    int sign = (0 < dist) - (0 > dist);
    int dist = abs(dist);
    coord cd = indexToCoord(cell, L);
    vint path;

    if (sign == 1)
    {
        if (cd.xi[2] == 0)
        {
            path.push_back(cellToFaces[cell][6]);
            cell = cell + L + L*L;
            dist -= 1;
        }
        else if (cd.xi[2] == L-3)
        {
            path.push_back(cellToFaces[cell][8]);
            cell = cell + L - L*L;
            dist -= 1;
        }
        
        int zigzag = 0;
        if (cd.xi[0] == L-4) zigzag = 1; 
        while (dist > 0)
        {
            if (zigzag = 0)
            {
                path.push_back(cellToFaces[cell][0]);
                cell = cell + 1 + L;
            }
            else 
            {
                path.push_back(cellToFaces[cell][2]);
                cell = cell - 1 + L;
            }
            zigzag = (zigzag + 1) % 2;
            dist -= 1;
        }
    }

    else 
    {
        if (cd.xi[2] == 0)
        {
            path.push_back(cellToFaces[cell][7]);
            cell = cell - L + L*L;
            dist -= 1;
        }
        else if (cd.xi[2] == L-3)
        {
            path.push_back(cellToFaces[cell][11]);
            cell = cell - L - L*L;
            dist -= 1;
        }

        int zigzag = 0;
        if (cd.xi[0] == L-4) zigzag = 1;
        while (dist > 0)
        {
            if (zigzag = 0)
            {
                path.push_back(cellToFaces[cell][1]);
                cell = cell + 1 - L;
            }
            else
            {
                path.push_back(cellToFaces[cell][3]);
                cell = cell - 1 - L;
            }
            zigzag = (zigzag + 1) % 2;
            dist -= 1;
        }
    }

    return path;
}

vpint mwpm(vint &defects, int L, int dual)
{
    std::vector<int> edges;
    std::vector<int> weights;
    int nodeNum = defects.size();
    for (int i = 0; i < nodeNum; ++i)
    {
        for (int j = i + 1; j < nodeNum; ++j)
        {
            edges.push_back(i);
            edges.push_back(j);
            if (dual == 0) weights.push_back(shortestPathLength(defects[i], defects[j], L));
            else weights.push_back(shortestPathLength(defects[i], defects[j], L)/2);
            // Add boundary node edges with wt = 0 so they can be matched for no cost
            edges.push_back(nodeNum + i);
            edges.push_back(nodeNum + j);
            weights.push_back(0);
        }
        // Add edge to boundary node
        edges.push_back(i);
        edges.push_back(nodeNum + i);
        if (dual == 0) weights.push_back(distanceToClosestXBoundary(defects[i], L)[2]);
        else weights.push_back(abs(distanceToClosestZBoundary(defects[i], L)));
    }
    int edgeNum = edges.size() / 2;
    struct PerfectMatching::Options options;
    options.verbose = false;
    PerfectMatching *pm;
    pm = new PerfectMatching(2 * nodeNum, edgeNum);
    
    for (int e = 0; e < edgeNum; ++e)
    {
        pm->AddEdge(edges[2*e], edges[2*e+1], weights[e]);
    }
    pm->options = options;
    pm->Solve();
    
    std::vector<std::pair<int, int>> defectPairs;
    for (int i = 0; i < nodeNum; ++i)
    {
        int j = pm->GetMatch(i);
        if (i < j)
        {
            if (j >= nodeNum)
            {
                defectPairs.push_back({defects[i], -1});  //-1 means matched to boundary
            }
            else
            {
                defectPairs.push_back({defects[i], defects[j]});
            }
        }
    }
    delete pm;
    return defectPairs;
}

void joinPair(int v1, int v2, vint &syndIndices, vvint &vertexToEdges, vint &syndrome, int L)
{
    vint path;
    //If matched to boundary
    if (v2 == -1) path = shortestPathToXBoundary(v1, L);
    else path = shortestPath(v1, v2, syndIndices, vertexToEdges, L);
    for (int i : path) syndrome[i] = (syndrome[i] + 1) % 2;
}

void joinDualPair(int cell1, int cell2, vint &qubitIndices, vvint &cellToFaces, vint &qubits, int L)
{
    vint path;
    if (cell2 == -1) path = shortestPathToZBoundary(cell1, cellToFaces, L);
    else path = shortestDualPath(cell1, cell2, qubitIndices, cellToFaces, L);
    for (int i : path) qubits[i] = (qubits[i] + 1) % 2;
}

void zErrorDecoder(vint &qubits, vint &qubitIndices, vvint &cellToFaces, vint &xStabs, int L)
{
    vint violatedXStabs;
    vpint xStabPairs;
    for (int j = 0; j < xStabs.size(); j++)
    {
        if (xStabs[j] == 1) violatedXStabs.push_back(j);
    }
    xStabPairs = mwpm(violatedXStabs, L, 1);
    for (auto &pair : xStabPairs)
    {
        joinDualPair(pair.first, pair.second, qubitIndices, cellToFaces, qubits, L);
    }
}

}
