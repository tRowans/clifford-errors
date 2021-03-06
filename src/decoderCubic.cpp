#include "decoderCubic.h"

namespace cubic {

vint taxi(int v1, int v2, int L)
{
    coord c1 = indexToCoord(v1, L);
    coord c2 = indexToCoord(v2, L);
    int dx, dy, dz;
    dx = c2.xi[0] - c1.xi[0];
    dy = c2.xi[1] - c1.xi[1];
    dz = c2.xi[2] - c1.xi[2];
    vint dists = {dx, dy, dz};
    return dists;
}

vint shortestPath(int v1, int v2, int L)
{
    //Pretty sure I don't need to check its using valid edges in here 
    //because I don't think there's a defect configuration where it would want to use invalid ones
    //but should probably have some test or check at some point
    vint dists = taxi(v1, v2, L);
    vint path;
    for (int i = 0; i < 3; i++)
    {
        int sign = (0 < dists[i]) - (0 > dists[i]);
        int dist = abs(dists[i]);
        while (dist > 0)
        {
            path.push_back(edgeIndex(v1, i, sign, L));
            v1 = neigh(v1, i, sign, L);
            dist -= 1;
        }
    }
    return path;
}    

vint shortestDualPath(int c1, int c2, int L)
{
    //Again don't think I need a check for valid faces here
    vint dists = taxi(c1, c2, L);
    vint path;
    for (int i = 0; i < 3; i++)
    {
        int sign = (0 < dists[i]) - (0 > dists[i]);
        int dist = abs(dists[i]);
        if (sign == -1)
        {
            while (dist > 0)
            {
                path.push_back(3*c1 + ((i+2)%4 - 2*(i%2))); //0<-->2, 1<-->1
                c1 = neigh(c1, i, sign, L);
                dist -= 1;
            }
        }
        else 
        {
            while (dist > 0)
            {
                c1 = neigh(c1, i, sign, L);
                path.push_back(3*c1 + ((i+2)%4 - 2*(i%2)));
                dist -= 1;
            }
        }
    }
    return path;
}

vint distanceToClosestXBoundary(int v, int L, int yOnly)
{
    coord cd = indexToCoord(v, L);
    vint dists = {0,0}; //y dist, z dist
    
    for (int i = 0; i < 2; i++)
    {
        if (cd.xi[i+1] <= (L-3)/2) dists[i] = -cd.xi[i+1];
        else dists[i] = (L-3) - cd.xi[i+1];
    }

    vint distInfo = {0,0,0}; //dir, sign, dist

    if (yOnly == 1) distInfo[0] = y;    //For matching in 2D code
    else if (abs(dists[0]) < abs(dists[1])) distInfo[0] = y;
    else distInfo[0] = z;
    distInfo[1] = (0 < dists[distInfo[0]-1]) - (0 > dists[distInfo[0]-1]);
    distInfo[2] = abs(dists[distInfo[0]-1]);

    return distInfo;
}

vint shortestPathToXBoundary(int v, int L, int yOnly)   
{
    vint distInfo = distanceToClosestXBoundary(v, L, yOnly);
    vint path;

    while (distInfo[2] > 0)
    {
        path.push_back(edgeIndex(v, distInfo[0], distInfo[1], L));
        v = neigh(v, distInfo[0], distInfo[1], L);
        distInfo[2] -= 1;
    }

    return path;
}

vint distanceToClosestZBoundary(int cell, int L)
{
    coord cd = indexToCoord(cell,L);
    vint distInfo = {0,0}; //sign, dist
    if (cd.xi[0] < (L-4)/2)
    {
        distInfo[0] = -1;
        distInfo[1] = cd.xi[0] + 1;
    }
    else 
    {
        distInfo[0] = 1;
        distInfo[1] = (L-4) - cd.xi[0];
    }
    return distInfo;
} 

vint shortestPathToZBoundary(int cell, int L)
{
    vint distInfo = distanceToClosestZBoundary(cell, L);
    vint path;

    if (distInfo[0] == -1)
    {
        while (distInfo[1] > 0)
        {
            path.push_back(3*cell+2);
            cell = neigh(cell, x, -1, L);
            distInfo[1] -= 1;
        }
    }
    else 
    {
        while (distInfo[1] > 0)
        {
            cell = neigh(cell, x, 1, L);
            path.push_back(3*cell+2);
            distInfo[1] -= 1;
        }
    }

    return path;
}

char* convertSyndrome(vint &in, vint &syndromeIndices)
{
    // I am assuming syndromeIndices is sorted (I checked a few cases)
    int size = syndromeIndices.size();
    char *out = new char[size];
    for (int i = 0; i < size; ++i)
    {
        out[i] = in[syndromeIndices[i]];
    }
    return out;
}

void xErrorDecoder(bp_osd &decoderHz, mod2sparse *hz, Lattice &lattice)
{
    int m = mod2sparse_rows(hz);
    int n = mod2sparse_cols(hz);
    char *synd;
    char *corr;

    synd = convertSyndrome(lattice.syndromeZ, lattice.zSyndIndices); // Convert syndrome into BP-OSD format
    corr = decoderHz.bp_osd_decode(synd);
    for (int i = 0; i < n; ++i)
    {
        if (corr[i] == 1)
        {
            lattice.qubitsX[lattice.allQubitIndices[i]] = (lattice.qubitsX[lattice.allQubitIndices[i]] + 1) % 2;
        }
    }

    // Cleanup
    delete[] synd;
}

vpint mwpm(vint &defects, int L, int dual, int yOnly)
{
    vint edges;
    vint weights;
    int nodeNum = defects.size();
    for (int i = 0; i < nodeNum; ++i)
    {
        for (int j = i + 1; j < nodeNum; ++j)
        {
            edges.push_back(i);
            edges.push_back(j);
            vint dists = taxi(defects[i], defects[j], L); 
            weights.push_back(abs(dists[0]) + abs(dists[1]) + abs(dists[2]));
            // Add boundary node edges with wt = 0 so they can be matched for no cost
            edges.push_back(nodeNum + i);
            edges.push_back(nodeNum + j);
            weights.push_back(0);
        } // Add edge to boundary node 
        edges.push_back(i);
        edges.push_back(nodeNum + i);
        if (dual == 0) weights.push_back(distanceToClosestXBoundary(defects[i], L, yOnly)[2]);
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
    
    vpint defectPairs;
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
    else path = shortestPath(v1, v2, lattice.L);
    for (int i : path) lattice.syndromeZ[i] = (lattice.syndromeZ[i] + 1) % 2;
}

void joinDualPair(int cell1, int cell2, Lattice &lattice, int ignoreOuter)
{
    vint path;
    if (cell2 == -1) path = shortestPathToZBoundary(cell1, lattice.L);
    else path = shortestDualPath(cell1, cell2, lattice.L);
    for (int i : path)
    {
        if (ignoreOuter == 1 && std::find(lattice.outerQubitIndices.begin(),
                                          lattice.outerQubitIndices.end(), i)
                                != lattice.outerQubitIndices.end()) continue;
        lattice.qubitsZ[i] = (lattice.qubitsZ[i] + 1) % 2;
    }
}

void zErrorDecoder(Lattice &lattice, int ignoreOuter)
{
    vint violatedXStabs;
    vpint xStabPairs;
    for (int j = 0; j < lattice.L*lattice.L*lattice.L; j++)
    {
        if (lattice.syndromeX[j] == 1) violatedXStabs.push_back(j);
    }
    xStabPairs = mwpm(violatedXStabs,lattice.L, 1, 0);
    for (auto &pair : xStabPairs) 
    {
        joinDualPair(pair.first, pair.second, lattice, ignoreOuter);
    }
}

void xErrorDecoder2D(Lattice &lattice, vint &syndromeVertices)
{
    //Used to check for decoding of X errors in 2D code at end to check for success/failure.
    //Before running this function we find the syndrome using only the outer qubits 
    //and then find all the vertices of this syndrome. 
    //Then we take all the vertices of this syndrome with z=0 and interpret these as a syndrome for the 2D code
    //Then we can run MWPM to find a correction
    vint z0Vertices;
    for (int v : syndromeVertices)
    {
        if (indexToCoord(v, lattice.L).xi[2] == 0) z0Vertices.push_back(v);
    }
    vpint defectPairs = mwpm(z0Vertices, lattice.L, 0, 1);
    for (auto &pair : defectPairs)
    {
        vint path;
        if (pair.second == -1) path = shortestPathToXBoundary(pair.first, lattice.L, 1);
        else path = shortestPath(pair.first, pair.second, lattice.L);
        //This path is along the bottom edges of the faces we want to flip
        //So use that 3*v is the edge on the bottom of face 3*v + 1
        //and 3*v + 1 is the edge on the bottom of face 3*v + 2
        for (int i : path) lattice.qubitsX[i+1] = (lattice.qubitsX[i+1] + 1) % 2;
    }
}

void measErrorDecoder(Lattice &lattice)
{
    vpint defectPairs = mwpm(lattice.defects, lattice.L, 0, 0);
    for (auto &pair : defectPairs)
    {
        joinPair(pair.first, pair.second, lattice);
    }
}

void checkIn2DCodespace(Lattice &lattice)
{
    //We only need to check this for X errors/Z stabilisers
    //Because 2D Z stabilisers are also 3D Z stabilisers
    //So if there is no syndrome for 3D X stabilisers this means no 2D Z errors
    lattice.calcSynd('z', 1, 0);
    for (int i : lattice.zSyndIndices)
    {
        if (lattice.syndromeZ[i] == 1 && (i % 3) == 2)
        {
            throw std::runtime_error("Out of 2D codespace (bad X correction)");
        }
    }
}

void jumpCorrection(Lattice &lattice)
{
    for (int z = lattice.L-4; z > 0; z--)
    {
        for (int x = 0; x < lattice.L-3; x++)
        {
            for (int y = 0; y < lattice.L-3; y++)
            {
                int faceXZ = 3*(x + y*lattice.L + z*lattice.L*lattice.L) + 1;
                int faceYZ = 3*(x + y*lattice.L + z*lattice.L*lattice.L) + 2;
                for (const int &face : {faceXZ, faceYZ})
                {
                    if (lattice.qubitsZ[face] == 1) //run a bounds check before this func
                    {
                        //this just happens to work 
                        //because 3*v is the edge on the bottom of face 3*v + 1
                        //and 3*v + 1 is the edge on the bottom of face 3*v + 2
                        lattice.applyZStab(face-1);
                    }
                }
            }
        }
    }
}

}
