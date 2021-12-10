#include "decoderRhombic2.h"

namespace rhombic {

namespace r2 {

coord w1ToW0(coord cd, int L)
{
    //Moves from w1 vertex to w0, either in xyz or -xyz 
    //depending on which is possible
    int v = coordToIndex(cd, L);
    if ((cd.xi[0] + cd.xi[1] + cd.xi[2]) % 2 == 0) cd = indexToCoord(neigh(v,3,1,L),L);
    else cd = indexToCoord(neigh(v,3,-1,L),L);
    return cd;
}

int shortestPathLength(int v1, int v2, int L, int twoD)
{
    coord c1 = indexToCoord(v1, L);
    coord c2 = indexToCoord(v2, L);
    std::vector<float> diff = differenceVector(c1,c2);
    int dist = 0;
    if (c1.xi[3] == 1)
    {
        c1 = w1ToW0(c1,L);
        std::vector<float> newDiff = differenceVector(c1,c2);
        if (magnitude(newDiff) > magnitude(diff)) dist -= 1;
        else if (magnitude(newDiff) < magnitude(diff)) dist += 1;
        diff = newDiff;
    }
    if (c2.xi[3] == 1)
    {
        c2 = w1ToW0(c2,L);
        std::vector<float> newDiff = differenceVector(c1,c2);
        if (magnitude(newDiff) > magnitude(diff)) dist -= 1;
        else if (magnitude(newDiff) < magnitude(diff)) dist += 1;
        diff = newDiff;
    }
        
    for (float &i : diff) i = abs(i);
    std::sort(diff.begin(), diff.end());
    if ((diff[0] + diff[1]) > diff[2]) dist += 2*(diff[0] + diff[1]);
    else dist += 2*diff[2];
    if (twoD == 1) dist = dist/2;
    return dist; 
}

std::vector<int> distanceToClosestXBoundary(int v, int L, int twoD)
{
    //strings of edges can terminate at w=0 vertices at +/- x and w=1 at +/- y
    coord cd = indexToCoord(v, L);
    int xDist, yDist;

    if (cd.xi[3] == 0)
    {
        if (cd.xi[0] < (L-4)/2) xDist = -2*cd.xi[0];
        else xDist = 2*((L-4) - cd.xi[0]);

        if (cd.xi[1] <= (L-4)/2) yDist = -2*cd.xi[1] + 1;
        else yDist = 2*((L-4) - cd.xi[1]) + 1;
    }

    else 
    {
        if (cd.xi[0] < (L-4)/2) xDist = -2*cd.xi[0] - 1;
        else xDist = 2*((L-4) - cd.xi[0]) - 1;

        if (cd.xi[1] < (L-4)/2) yDist = -2*cd.xi[1];
        else yDist = 2*((L-4) - cd.xi[1]);
    }

    vint distInfo = {0,0,0}; //dir, sign, dist
    
    if (twoD == 1 || abs(xDist) >= abs(yDist))
    {
        distInfo[0] = 1;
        distInfo[1] = (0 < yDist) - (0 > yDist);
        if (twoD == 1) distInfo[2] = (abs(yDist) - 1)/2 + 1;
        else distInfo[2] = abs(yDist);
    }
    else 
    {
        distInfo[0] = 0;
        distInfo[1] = (0 < xDist) - (0 > xDist);
        distInfo[2] = abs(xDist);
    }
    return distInfo;
}

std::vector<int> shortestPathToXBoundary(int v, int L, int twoD)   
{
    std::vector<int> distInfo = distanceToClosestXBoundary(v, L, twoD);
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
            path.push_back(edgeIndex(v, moveDir1[0], moveDir1[1], L));
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
                if (cd.xi[2] == 0) moveDir1 = {xyz,1};
                else moveDir1 = {xy,1};
            }
            else
            {
                if (cd.xi[2] == 0) moveDir1 = {xz,1};
                else moveDir1 = {yz,-1};
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
                    moveDir1 = {xz,-1};
                    moveDir2 = {xyz,1};
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
    vint distInfo = {0,0}; //sign, dist
    
    if (cd.xi[2] > (L-3)/2)
    {
        distInfo[0] = 1;
        distInfo[1] = (L-3) - cd.xi[2];
    }
    else
    {
        distInfo[0] = -1;
        distInfo[1] = cd.xi[2];
    }
    return distInfo;
}

vint shortestPathToZBoundary(int cell, vvint &cellToFaces, int L)
{
    vint distInfo = distanceToClosestZBoundary(cell, L);
    int &sign = distInfo[0];
    int &dist = distInfo[1];
    coord cd = indexToCoord(cell, L);
    vint path;
    
    int zigzag = 0;
    if (sign == 1)
    {
        if (cd.xi[1] == L-3) zigzag = 1;
        while (dist > 0)
        {
            if (zigzag == 0)
            {
                path.push_back(cellToFaces[cell][6]);
                cell = cell + L + L*L;
            }
            else 
            {
                path.push_back(cellToFaces[cell][7]);
                cell = cell - L + L*L;
            }
            zigzag = (zigzag + 1) % 2;
            dist -= 1;
        }
    }
    else 
    {
        if (cd.xi[1] == L-3) zigzag = 1;
        while (dist > 0)
        {
            if (zigzag == 0)
            {
                path.push_back(cellToFaces[cell][10]);
                cell = cell + L - L*L;
            }
            else 
            {
                path.push_back(cellToFaces[cell][11]);
                cell = cell - L - L*L;
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

vpint mwpm(vint &defects, int L, int dual, int twoD)
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
            if (dual == 0) weights.push_back(shortestPathLength(defects[i], defects[j], L, twoD));
            else weights.push_back(shortestPathLength(defects[i], defects[j], L, 0)/2);
            // Add boundary node edges with wt = 0 so they can be matched for no cost
            edges.push_back(nodeNum + i);
            edges.push_back(nodeNum + j);
            weights.push_back(0);
        }
        // Add edge to boundary node
        edges.push_back(i);
        edges.push_back(nodeNum + i);
        if (dual == 0) weights.push_back(distanceToClosestXBoundary(defects[i], L, twoD)[1]);
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

void joinPair(int v1, int v2, Lattice &lattice)
{
    vint path;
    //If matched to boundary
    if (v2 == -1) path = shortestPathToXBoundary(v1, lattice.L, 0);
    else path = shortestPath(v1, v2, lattice);
    for (int i : path) lattice.syndromeZ[i] = (lattice.syndromeZ[i] + 1) % 2;
}

void joinDualPair(int cell1, int cell2, Lattice &lattice, int useOuter, int useInner)
{
    vint path;
    if (cell2 == -1) path = shortestPathToZBoundary(cell1, lattice.cellToFaces, lattice.L);
    else path = shortestDualPath(cell1, cell2, lattice, useOuter, useInner);
    for (int i : path) lattice.qubitsZ[i] = (lattice.qubitsZ[i] + 1) % 2;
}

void zErrorDecoder(Lattice &lattice, int useOuter, int useInner)
{
    vint violatedXStabs;
    vpint xStabPairs;
    for (int j = 0; j < lattice.syndromeX.size(); j++)
    {
        if (lattice.syndromeX[j] == 1) violatedXStabs.push_back(j);
    }
    xStabPairs = mwpm(violatedXStabs, lattice.L, 1, 0);
    for (auto &pair : xStabPairs)
    {
        joinDualPair(pair.first, pair.second, lattice, useOuter, useInner);
    }
}

void xErrorDecoder2D(Lattice &lattice, vint &syndromeVertices)
{
    //This is a similar idea to the cubic 2D X decoder so check that for more explanation
    //main difference here is edge -> face conversion method after finding path
    vint x0Vertices;
    for (int v : syndromeVertices)
    {
        coord cd = indexToCoord(v, lattice.L);
        if (cd.xi[0] == 0 && cd.xi[3] == 0) x0Vertices.push_back(v);
    }
    vpint defectPairs = mwpm(x0Vertices, lattice.L, 0, 1);
    for (auto &pair : defectPairs)
    {
        vint path;
        if (pair.second == -1)
        {
            path = shortestPathToXBoundary(pair.first, lattice.L, 1);
            int edgeAtBoundary = path.back();
            vint faces = lattice.edgeToFaces[edgeAtBoundary];
            for (int f : faces)
            {
                if (std::find(lattice.outerQubitIndices.begin(),
                              lattice.outerQubitIndices.end(), f)
                    != lattice.outerQubitIndices.end()) 
                {
                    lattice.qubitsX[f] = (lattice.qubitsX[f] + 1) % 2;
                }
            }
            path.pop_back();
        }
        else path = shortestPath(pair.first, pair.second, lattice);
        for (int i = 0; i < path.size(); i+=2)
        {
            pint vs1 = lattice.edgeToVertices[path[i]];
            pint vs2 = lattice.edgeToVertices[path[i+1]];
            int f = findFace({vs1.first,vs2.first}, lattice.vertexToFaces, lattice.faceToVertices);
            lattice.qubitsX[f] = (lattice.qubitsX[f] + 1) % 2;
        }
    }
}

void measErrorDecoder(Lattice &lattice)
{
    vpint defectPairs = mwpm(lattice.defects, lattice.L, 0, 0);
    for (auto& pair : defectPairs)
    {
        joinPair(pair.first, pair.second, lattice);
    }
}

}

}
