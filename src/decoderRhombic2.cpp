#include "decoderRhombic2.h"

namespace rhombic2 {

std::vector<int> distanceToClosestXBoundary(int v, int L)
{
    //strings of edges can terminate at w=0 vertices at +/- x and w=1 at +/- y
    coord c = indexToCoord(v, L);
    int xDist, yDist

    if (cd.xi[3] == 0)
    {
        if (cd.xi[0] < (L-4)/2) xDist = -2*cd.xi[0];
        else xDist = 2*((L-4)/2 - cd.xi[0]);

        if (cd.xi[1] < (L-4)/2) yDist = -2*cd.xi[1] - 1;
        else zDist = 2*((L-4)/2 - cd.xi[1]) + 1;
    }

    else 
    {
        if (cd.xi[0] < (L-4)/2) xDist = -2*cd.xi[0] + 1;
        else xDist = 2*((L-4)/2 - cd.xi[0]) - 1;

        if (cd.xi[1] < (L-4)/2) yDist = -2*cd.xi[1];
        else yDist = 2*((L-4)/2 - cd.xi[1]);
    }

    vint distInfo = {0,0,0}; //dir, sign, dist
    if (abs(xDist) < abs(yDist))
    {
        distInfo[0] = 0;
        distInfo[1] = (0 < xDist) - (0 > xDist);
        distInfo[2] = abs(xDist);
    }
    else
    {
        distInfo[0] = 1;
        distInfo[1] = (0 < yDist) - (0 > yDist);
        distInfo[2] = abs(yDist);
    }

    return distInfo;
}

std::vector<int> shortestPathToXBoundary(int v, int L)   
{
    std::vector<int> distInfo = distanceToClosestXBoundary(v, L);
    int &dir = distInfo[0];
    int &sign = distInfo[1];
    int &dist = distInfo[2];
    vint path;
    coord cd = indexToCoord(v,L);

    vint moveDir1 = {0,0}; //dir, sign
    vint moveDir2 = {0,0}; 

    if (dir == 0)
    {
        if (cd.xi[3] == 1)
        {
            if ((cd.xi[0] + cd.xi[1] + cd.xi[2]) % 2 == 0)
            {
                if (sign == 1) moveDir1 = {xyz,1};
                else moveDir1 = {xz,-1};
            }
            else 
            {
                if (sign == 1) moveDir1 = {xz,1};
                else moveDir1 = {xyz,-1};
            }
            path.push_back(edgeIndex(v, moveDir1[0], moveDir[1], L));
            v = neigh(v, moveDir1[0], moveDir1[1], L);
            cd = indexToCoord(v, L);
            dist -= 1;
        }

        int zigzag = 0;
        if (cd.xi[2] ==0 && sign == 1) zigzag = 1;
        else if (cd.xi[2] == L-4 && sign == -1) zigzag = 1;
        while (dist > 0)
        {
            if (zigzag == 0)
            {
                if (sign == 1)
                {
                    moveDir1 = {yz,-1};
                    moveDir2 = {xy,1};
                }
                else 
                {
                    moveDir1 = {xy,-1};
                    moveDir2 = {yz,1};
                }
            }
            else 
            {
                if (sign == 1)
                {
                    moveDir1 = {xz,1};
                    moveDir2 = {xyz,1};
                }
                else 
                {
                    moveDir1 = {xyz,-1};
                    moveDir2 = {xz,-1};
                }
            }

            path.push_back(edgeIndex(v, moveDir1[0], moveDir1[1], L));
            v = neigh(v, moveDir1[0], moveDir1[1], L);
            path.push_back(edgeIndex(v, moveDir2[0], moveDir2[1], L));
            v = neigh(v, moveDir2[0], moveDir[1], L);
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
                if (cd.xi[2] == 0) moveDir1 = {xyz,1};
                else moveDir1 = {xy,1};
            }
            else
            {
                if (cd.xi[2] == 0) moveDir1 = {xy,-1};
                else moveDir1 = {xyz,-1};
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
                    moveDir1 = {xyz,1};
                    moveDir2 = {xz,-1};
                }
                else
                {
                    moveDir1 = {xy,-1};
                    moveDir2 = {yz,-1};
                }
            }
            else 
            {
                if (sign == 1)
                {
                    moveDir1 = {yz,1};
                    moveDir2 = {xy,1};
                }
                else 
                {
                    moveDir1 = {xz,1};
                    moveDir2 = {xyz,-1};
                }
            }

            path.push_back(edgeIndex(v, moveDir1[0], moveDir1[1], L));
            v = neigh(v, moveDir1[0], moveDir1[1], L);
            path.push_back(edgeIndex(v, moveDir2[0], moveDir2[1], L));
            v = neigh(v, moveDir2[0], moveDir2[1], L);
            cd = indexToCoord(v, L);
            dist -= 2;
        }
    }
    
    return path;
}

vint distanceToClosestZBoundary(int cell, int L)
{
    coord cd = indexToCoord(cell, L);
    int dist;
    
    if (cd.xi[2] > (L-3)/2) dist = (L-3) - cd.xi[2];
    else dist = -cd.xi[2];

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
        if (cd.xi[1] == 0)
        {
            path.push_back(cellToFaces[cell][6]);
            cell = cell + L + L*L;
            dist -= 1;
        }
        else if (cd.xi[1] == L-3)
        {
            path.push_back(cellToFaces[cell][7]);
            cell = cell - L + L*L;
            dist -= 1;
        }

        int zigzag = 0;
        if (cd.xi[0] == L-4) zigzag = 1;
        while (dist > 0)
        {
            if (zigzag = 0) 
            {
                path.push_back(cellToFaces[cell][4]);
                cell = cell + 1 + L*L;
            }
            else
            {
                path.push_back(cellToFaces[cell][5]);
                cell = cell - 1 + L*L;
            }
            zigzag = (zigzag + 1) % 2;
            dist -= 1;
        }
    }

    else
    {
        if (cd.xi[1] == 0)
        {
            path.push_back(cellToFaces[cell][10]);
            cell = cell + L - L*L;
            dist -= 1;
        }
        else if (cd.xi[1] == L-3)
        {
            path.push_back(cellToFaces[cell][11]);
            cell = cell - L - L*L;
            dist -= 1;
        }

        zigzag = 0;
        if (cd.xi[0] == L-4) zigzag = 1;
        while (dist > 0)
        {
            if (zigzag = 0)
            {
                path.push_back(cellToFaces[cell][8]);
                cell = cell + 1 - L*L;
            }
            else 
            {
                path.push_back(cellToFaces[cell][9]);
                cell = cell - 1 - L*L;
            }
            zigzag = (zigzag + 1) % 2;
            dist -= 1;
        }
    }

    return path;
}

//!!!--------------------!!!
//||LOOP DECODER GOES HERE||
//!!!--------------------!!!


//From here is technically the same code in both rhombic1 and 2
//but they use different path to boundary functions as defined above
//could merge the two from this point by adding an argument for rhombic1 or 2
//and then functions from rhombic1 or 2 namespace as needed
//but I'm not sure it's worth it as all the extra if statements would slow things down

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
        if (dual == 0) weights.push_back(distanceToClosestXBoundary(defects[i], L)[1]);
        else weights.push_back(distanceToClosestZBoundary(defects[i], L)[1]);
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
    std::vector<int> path;
    //If matched to boundary
    if (v2 == -1) path = shortestPathToXBoundary(v1, L);
    else path = shortestPath(v1, v2, syndIndices, vertexToEdges, L, 2);
    for (int i : path) syndrome[i] = (syndrome[i] + 1) % 2;
}

void joinDualPair(int cell1, int cell2, vint &outerQubitIndices, vint &innerQubitIndices, vvint &cellToFaces, vint &qubits, int L)
{
    vint path;
    if (cell2 == -1) path = shortestPathToZBoundary(cell1, cellToFaces, L);
    else path = shortestDualPath(cell1, cell2, outerQubitIndices, 
                                    innerQubitIndices, cellToFaces, L);
    for (int i : path) qubits[i] = (qubits[i] + 1) % 2;
}

void zErrorDecoder(vint &qubits, vint &outerQubitIndices, vint &innerQubitIndices, vvint &cellToFaces, vint &xStabs, int L)
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
        joinDualPair(pair.first, pair.second, outerQubitIndices, 
                        innerQubitIndices, cellToFaces, qubits, L);
    }
}

void measErrorDecoder(vint &syndromeZ, vint &zSyndIndices, vvint &vertexToEdges, vint &defects, int L)
{
    vpint defectPairs = mwpm(defects, L, 0);
    for (auto& pair : defectPairs)
    {
        joinPair(pair.first, pair.second, zSyndIndices, vertexToEdges, syndromeZ, L);
    }
}

}
