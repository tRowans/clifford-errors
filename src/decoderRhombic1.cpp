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

//got to here
    
std::vector<int> shortestPathToXBoundary(int v, vint &syndIndices, int L)   
{
    std::vector<int> distInfo = distanceToClosestXBoundary(v, L);
    int &sign = distInfo[0];
    int &dist = distInfo[1];
    std::vector<int> path;
    coord c = indexToCoord(v, L);

    if (c.xi[3] == 0)
    {
        //if upper defect
        if (std::find(syndIndices.begin(), syndIndices.end(), 4*v + xyz) == syndIndices.end())
        {
            if (sign == 1)
            {
                v = neigh(v, xy, -1, L);
                path.push_back(4*v + xy);
            }
            else
            {
                v = neigh(v, yz, -1, L);
                path.push_back(4*v + yz);
            }
        }
        //if lower defect
        else
        {
            if (sign == 1)
            {
                path.push_back(4*v + yz);
                v = neigh(v, yz, 1, L);
            }
            else 
            {
                path.push_back(4*v + xy);
                v = neigh(v, xy, 1, L);
            }
        }
        dist -= 1;
        c = indexToCoord(v, L);
    }

    if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 0 && sign == 1)
    {
        while (dist > 0)
        {
            path.push_back(4*v + yz);
            v = neigh(v, yz, 1, L);
            v = neigh(v, xy, -1, L);
            path.push_back(4*v + xy);
            dist -= 2;
        }
    }
    else if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 0 && sign == -1)
    {
        while (dist > 0)
        {
            path.push_back(4*v + xy);
            v = neigh(v, xy, 1, L);
            v = neigh(v, yz, -1, L);
            path.push_back(4*v + yz);
            dist -= 2;
        }
    }
    else if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 1 && sign == 1)
    {
        while (dist > 0)
        {
            v = neigh(v, xy, -1, L);
            path.push_back(4*v + xy);
            path.push_back(4*v + yz);
            v = neigh(v, yz, 1, L);
            dist -= 2;
        }
    }
    else if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 1 && sign == -1)
    {
        while (dist > 0)
        {
            v = neigh(v, yz, -1, L);
            path.push_back(4*v + yz);
            path.push_back(4*v + xy);
            v = neigh(v, xy, 1, L);
            dist -= 2;
        }
    }

    return path;
}

vint distanceToClosestZBoundary(int cell, int L)
{
    coord cd = cellToCoord(cell, L);
    vint distInfo;
    int dPlus;  //distance to +y boundary
    int dMinus;  //distance to -y boundary
   
    //-z boundary +x edge 
    if ((cd.xi[0] + cd.xi[1] + cd.xi[2] == (2*L-5) + (3*L+2)) && cd.xi[2] == 1)
    {
        if (cd.xi[1] == (L-3)) dPlus = 1;
        else dPlus = (L-1) - cd.xi[1];
    }
    //+x boundary
    else if (cd.xi[0] + cd.xi[1] + cd.xi[2] == (2*L-5) + (3*L+4))
    {
        if (cd.xi[2] == 2)
        {
            if (cd.xi[1] == (L-3)) dPlus = 2;
            else dPlus = L - cd.xi[1];
        }
        else dPlus = (L-1) - cd.xi[1];
    }
    else dPlus = (L-2) - cd.xi[1];
    //+z boundary -x edge
    if ((cd.xi[0] + cd.xi[1] + cd.xi[2] == (2*L-5) + 2) && cd.xi[2] == L-2)
    {
        if (cd.xi[1] == 2) dMinus = 1;
        else dMinus = cd.xi[1];
    }
    //-x boundary
    else if (cd.xi[0] + cd.xi[1] + cd.xi[2] == 2*L-5)
    {
        if (cd.xi[2] == L-3)
        {
            if (cd.xi[1] == 2) dMinus = 2;
            else dMinus = cd.xi[1] + 1;
        }
        else dMinus = cd.xi[1];
    }
    else dMinus = cd.xi[1] - 1;

    if (dPlus > dMinus) distInfo = {-1, dMinus};
    //goes to +y boundary if distances are equal
    //but this choice is not important
    else distInfo = {1, dPlus};

    return distInfo;
}

vint shortestPathToZB(int cell, vvint &cellToFaces, int L)
{
    vint distInfo = distanceToClosestZBoundary(cell, L);
    int &sign = distInfo[0];
    int &dist = distInfo[1];
    coord cd = cellToCoord(cell, L);
    vint path;
    
    if (sign == 1)
    {
        //+x boundary
        if (cd.xi[0] + cd.xi[1] + cd.xi[2] == (2*L-5) + (3*L+4))
        {
            path.push_back(cellToFaces[cell][9]);
            cd.xi[0] -= 1;
            cd.xi[2] -= 1;
            cell = coordToIndex(cd, L)/2;
            dist -= 1;
        }
        
        //-z boundary 
        if (cd.xi[2] == 1)
        {
            //+x edge (except +y corner)
            if ((cd.xi[0] + cd.xi[1] + cd.xi[2] == (2*L-5) + (3*L+2)) && cd.xi[1] != (L-3))
            {
                path.push_back(cellToFaces[cell][5]);
                cd.xi[0] -= 1;
                cd.xi[2] += 1;
                cell = coordToIndex(cd, L)/2;
                dist -= 1;
            }
            else
            {
                path.push_back(cellToFaces[cell][6]);
                cd.xi[1] += 1;
                cd.xi[2] += 1;
                cell = coordToIndex(cd, L)/2;
                dist -= 1;
            }
        }

        //-x boundary
        //This would not work for -z edge of this boundary 
        //but these have already been addressed by the previous if statement
        if (cd.xi[0] + cd.xi[1] + cd.xi[2] == 2*L-5)
        {
            path.push_back(cellToFaces[cell][0]);
            cd.xi[0] += 1;
            cd.xi[1] += 1;
            cell = coordToIndex(cd, L)/2;
            dist -= 1;
        }

        //+z boundary
        //This would not work for +z edge of +x boundary
        //but this boundary has been dealt with by the first if statement
        else if (cd.xi[2] == L-2)
        {
            path.push_back(cellToFaces[cell][10]);
            cd.xi[1] += 1;
            cd.xi[2] -= 1;
            cell = coordToIndex(cd, L)/2;
            dist -= 1;
        }

        while (dist > 0)
        {
            path.push_back(cellToFaces[cell][2]);
            cd.xi[0] -= 1;
            cd.xi[1] += 1;
            cell = coordToIndex(cd, L)/2;
            dist -= 1;
        }
    }
    else
    {
        //-x boundary
        if (cd.xi[0] + cd.xi[1] + cd.xi[2] == 2*L-5)
        {
            path.push_back(cellToFaces[cell][4]);
            cd.xi[0] += 1;
            cd.xi[2] += 1;
            cell = coordToIndex(cd, L)/2;
            dist -= 1;
        }
    
        //+z boundary 
        if (cd.xi[2] == L-2)
        {
            //-x edge (except -y corner)
            if ((cd.xi[0] + cd.xi[1] + cd.xi[2] == (2*L-5) + 2) && cd.xi[1] != 2)
            {
                path.push_back(cellToFaces[cell][8]);
                cd.xi[0] += 1;
                cd.xi[2] -= 1;
                cell = coordToIndex(cd, L)/2;
                dist -= 1;
            }
            else
            {
                path.push_back(cellToFaces[cell][11]);
                cd.xi[1] -= 1;
                cd.xi[2] -= 1;
                cell = coordToIndex(cd, L)/2;
                dist -= 1;
            }
        }

        //+x boundary
        ////This would not work for +z edge of this boundary 
        //but these have already been addressed by the previous if statement
        if (cd.xi[0] + cd.xi[1] + cd.xi[2] == (2*L-5) + (3*L+4))
        {
            path.push_back(cellToFaces[cell][3]);
            cd.xi[0] -= 1;
            cd.xi[1] -= 1;
            cell = coordToIndex(cd, L)/2;
            dist -= 1;
        }
    
        //-z boundary
        //This would not work for -z edge of -x boundary
        //but this boundary has been dealt with by the first if statement
        if (cd.xi[2] == 1)
        {
           path.push_back(cellToFaces[cell][7]);
           cd.xi[1] -= 1;
           cd.xi[2] += 1;
           cell = coordToIndex(cd, L)/2;
           dist -= 1;
        }
        
        while (dist > 0)
        {
            path.push_back(cellToFaces[cell][1]);
            cd.xi[0] += 1;
            cd.xi[1] -= 1;
            cell = coordToIndex(cd, L)/2;
            dist -= 1;
        }
    }
    return path;
}

vpint mwpm(vint &defects, std::map<std::pair<int, int>, int> &defectPairDistances, int L, int dual)
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
            if (defectPairDistances.count({defects[i],defects[j]}))
            {
                weights.push_back(defectPairDistances[{defects[i],defects[j]}]);
            }
            else
            {
                if (dual == 0) weights.push_back(shortestPathLength(defects[i], defects[j], L));
                else 
                {
                    //this is so dumb, should have just kept cell and vertex indices the same
                    int v1 = coordToIndex(cellToCoord(defects[i], L), L);
                    int v2 = coordToIndex(cellToCoord(defects[j], L), L);
                    weights.push_back(shortestPathLength(v1, v2, L)/2);
                }
            }
            // Add boundary node edges with wt = 0 so they can be matched for no cost
            edges.push_back(nodeNum + i);
            edges.push_back(nodeNum + j);
            weights.push_back(0);
        }
        // Add edge to boundary node
        edges.push_back(i);
        edges.push_back(nodeNum + i);
        if (defectPairDistances.count({defects[i], -1}))
        {
            weights.push_back(defectPairDistances[{defects[i], -1}]);
        }
        else
        {
            if (dual == 0) weights.push_back(distanceToClosestXBoundary(defects[i], L)[1]);
            else weights.push_back(distanceToClosestZBoundary(defects[i], L)[1]);
        }
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

void updatePairing(std::map<std::pair<int, int>, int> &defectPairDistances, std::vector<int> &defects, int L)
{
    std::sort(defects.begin(), defects.end()); //need a standard ordering for pairs
    std::vector<std::pair<int, int>> defectPairs = mwpm(defects, defectPairDistances, L, 0); 
    std::vector<std::pair<int, int>> removeThese;
    for (auto &p : defectPairDistances)
    {
        if (std::find(defectPairs.begin(), defectPairs.end(), p.first) == defectPairs.end())
        {
            removeThese.push_back(p.first);
        }
    }
    for (std::pair<int, int> p : removeThese) defectPairDistances.erase(p);
    
    int distance;

    for (std::pair<int, int> &q : defectPairs)
    {
        if (!defectPairDistances.count(q))
        {
            if (q.second == -1)
            {
                distance = distanceToClosestXBoundary(q.first, L)[1];
            }
            else
            {
                distance = shortestPathLength(q.first, q.second, L);
            }
            defectPairDistances[q] = distance; 
        }
    }
}

void joinPair(int v1, int v2, vint &syndIndices, vvint &vertexToEdges, vint &syndrome, int L)
{
    vint path;
    //If matched to boundary
    if (v2 == -1) path = shortestPathToXB(v1, syndIndices, L);
    else path = shortestPath(v1, v2, syndIndices, vertexToEdges, L);
    for (int i : path) syndrome[i] = (syndrome[i] + 1) % 2;
}

void joinDualPair(int cell1, int cell2, vint &qubitIndices, vvint &cellToFaces, vint &qubits, int L)
{
    vint path;
    if (cell2 == -1) path = shortestPathToZB(cell1, cellToFaces, L);
    else path = shortestDualPath(cell1, cell2, qubitIndices, cellToFaces, L);
    for (int i : path) qubits[i] = (qubits[i] + 1) % 2;
}

vint pathToTop(int v, int L)
{
    vint path;
    if (v == -1)
    {
        //no deferral for boundaries
        return path;
    }
    coord c = indexToCoord(v, L);
    if (c.xi[3] == 0)
    {
        if (c.xi[1] == L-2)
        {
            v = neigh(v, yz, -1, L);
            path.push_back(4*v + yz);
        }
        else
        {
            path.push_back(4*v + xy);
            v = neigh(v, xy, 1, L);
        }
        c = indexToCoord(v, L);
    }
    if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 1)
    {
        path.push_back(4*v + xyz);
        v = neigh(v, xyz, 1, L);
        v = neigh(v, yz, -1, L);
        path.push_back(4*v + yz);
        c = indexToCoord(v, L);
    }
    if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 0)
    {
        path.push_back(4*v + xz);
    }
    return path;
}

void syndromeRepair(vint &syndrome, vint &syndIndices, vvint &vertexToEdges, vpint &edgeToVertices, std::map<std::pair<int, int>, int> &defectPairDistances, int L, int cutoff, int reduction)
{
    std::map<std::pair<int, int>, int> newDefectPairDistances;
    for (std::pair<const std::pair<int, int>, int> &p : defectPairDistances)
    {
        std::pair<int, int> newPair;
        vint path1 = pathToTop(p.first.first, L);
        vint path2 = pathToTop(p.first.second, L);
        int joinRange;
        if (cutoff == 0) joinRange = (path1.size() + path2.size());
        else joinRange = cutoff;
        int reduceBy;
        if (reduction == 0) reduceBy = (path1.size() + path2.size());
        else reduceBy = reduction;

        if (p.second <= joinRange)
        {
            joinPair(p.first.first, p.first.second, syndIndices, vertexToEdges, syndrome, L);
        }
        else
        {
            for (int i : path1) syndrome[i] = (syndrome[i] + 1) % 2;
            //smaller index is first in edgeToVertices pair so will be w=0 vertex
            //w=0 vertex of final edge in path is where we end
            newPair.first = edgeToVertices[path1.back()].first;
            if (p.first.second != -1)
            {
                for (int i : path2) syndrome[i] = (syndrome[i] + 1) % 2;
                newPair.second = edgeToVertices[path2.back()].first;
            }
            else newPair.second = -1;
            newDefectPairDistances[newPair] = p.second - reduceBy;
        }
    }
    defectPairDistances = newDefectPairDistances;
}

void zErrorDecoder(vint &qubits, vint &qubitIndices, vvint &cellToFaces, vint &xStabs, int L)
{
    std::map<std::pair<int,int>,int> empty;
    vint violatedXStabs;
    vpint xStabPairs;
    for (int j = 0; j < xStabs.size(); j++)
    {
        if (xStabs[j] == 1) violatedXStabs.push_back(j);
    }
    xStabPairs = mwpm(violatedXStabs, empty, L, 1);
    for (auto &pair : xStabPairs)
    {
        joinDualPair(pair.first, pair.second, qubitIndices, cellToFaces, qubits, L);
    }
}

//Currently unused 
/*
//No references on lattice index vectors because we want copies of these
//so that the positions are reset at the start of each run
int oneRun(int L, double p, double q, vvint &faceToEdges, vvint &edgeToFaces, vvint &faceToVertices, vvint &vertexToFaces, vvint &vertexToEdges, vpint &edgeToVertices, vint lowerQubitIndices, vint middleQubitIndices, vint upperQubitIndices, vint syndIndices, vint defectIndices, vint bulkSweepVertices, vint middleBoundarySweepVertices, vint upperBoundarySweepVertices1, vint upperBoundarySweepVertices2, vint zLogical, int run, int cutoff, int reduction)
{
    std::vector<int> qubits(3*5*L*L*L, 0);
    std::vector<int> syndrome(8*5*L*L*L, 0);
    std::vector<int> defects = {};

    std::map<std::pair<int, int>, int> defectPairDistances;
    
    std::random_device rd{};
    std::mt19937 engine{rd()};
    std::uniform_real_distribution<double> dist(0,1);
    
    for (int i=0; i < (3*L)/2; i++)
    {
        calcSynd(syndrome, qubits, lowerQubitIndices, middleQubitIndices, upperQubitIndices, faceToEdges, syndIndices);
        measError(syndrome, q, engine, dist, syndIndices);
        findDefects(syndrome, vertexToEdges, edgeToVertices, defects, defectIndices);
        updatePairing(defectPairDistances, defects, L);
        syndromeRepair(syndrome, syndIndices, vertexToEdges, edgeToVertices, 
                        defectPairDistances, L, cutoff, reduction);
        findDefects(syndrome, vertexToEdges, edgeToVertices, defects, defectIndices);
        if (defects.size() != 0)
        {
            std::cout << "syndrome repair failed on run " << run << '\n';
            return 2;
        }
        middleSweepRule(qubits, middleQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                            faceToEdges, faceToVertices, vertexToFaces, L);
        middleBoundarySweepRule(qubits, middleQubitIndices, syndrome, syndIndices, 
                                    middleBoundarySweepVertices, faceToEdges, faceToVertices, 
                                    vertexToFaces, L, engine, dist);
        upperSweepRule(qubits, upperQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                            faceToEdges, faceToVertices, vertexToFaces, L);
        upperBoundarySweepRule1(qubits, upperQubitIndices, syndrome, syndIndices, 
                                    upperBoundarySweepVertices1, faceToEdges, faceToVertices, 
                                    vertexToFaces, L);
        upperBoundarySweepRule2(qubits, upperQubitIndices, syndrome, syndIndices, 
                                    upperBoundarySweepVertices2, faceToEdges, faceToVertices, 
                                    vertexToFaces, L, engine, dist);
        dataError(qubits, p, engine, dist, upperQubitIndices);
        moveIndices(lowerQubitIndices, middleQubitIndices, upperQubitIndices, syndIndices, defectIndices, bulkSweepVertices, middleBoundarySweepVertices, upperBoundarySweepVertices1, upperBoundarySweepVertices2, zLogical, vertexToFaces, faceToVertices, L);
    }

    calcSynd(syndrome, qubits, lowerQubitIndices, middleQubitIndices, upperQubitIndices, faceToEdges, syndIndices);
    findDefects(syndrome, vertexToEdges, edgeToVertices, defects, defectIndices);
    updatePairing(defectPairDistances, defects, L);
    for (auto &p : defectPairDistances)
    {
        joinPair(p.first.first, p.first.second, syndIndices, vertexToEdges, syndrome, L);
    }
    findDefects(syndrome, vertexToEdges, edgeToVertices, defects, defectIndices);
    middleSweepRule(qubits, middleQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                            faceToEdges, faceToVertices, vertexToFaces, L);
    middleBoundarySweepRule(qubits, middleQubitIndices, syndrome, syndIndices, 
                                middleBoundarySweepVertices, faceToEdges, faceToVertices, 
                                vertexToFaces, L, engine, dist);
    upperSweepRule(qubits, upperQubitIndices, syndrome, syndIndices, bulkSweepVertices, 
                        faceToEdges, faceToVertices, vertexToFaces, L);
    upperBoundarySweepRule1(qubits, upperQubitIndices, syndrome, syndIndices, 
                                upperBoundarySweepVertices1, faceToEdges, faceToVertices, 
                                vertexToFaces, L);
    upperBoundarySweepRule2(qubits, upperQubitIndices, syndrome, syndIndices, 
                                upperBoundarySweepVertices2, faceToEdges, faceToVertices, 
                                vertexToFaces, L, engine, dist);

    //Check return to codespace
    *//*
    std::set<int> upperStabs = {};
    for (int f : upperQubitIndices)
    {
        vint edges = faceToEdges[f];
        for (int e : edges)
        {
            if (std::find(syndIndices.begin(), syndIndices.end(), e) != syndIndices.end())
            {
                upperStabs.insert(e);
            }
        }
    }
    calcSynd(syndrome, qubits, lowerQubitIndices, middleQubitIndices, upperQubitIndices, faceToEdges, syndIndices);
    int notEmpty = 0;
    for (int stab : upperStabs)
    {
        if (syndrome[stab] == 1) notEmpty += 1;
    }
    if (notEmpty != 0) std::cout << "Return to codespace failed" << '\n';
    *//*
    if (checkCorrection(qubits, zLogical))
    {
       return 1;
    }
    else
    {
       return 0;
    } 
}
*/

}
