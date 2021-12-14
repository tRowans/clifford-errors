#include "rhombicGeneric.h"

namespace rhombic {

int coordToIndex(coord c, int L)
{
    if (0 <= c.xi[0] && c.xi[0] < L &&
        0 <= c.xi[1] && c.xi[1] < L &&
        0 <= c.xi[2] && c.xi[2] < L &&
        0 <= c.xi[3] && c.xi[3] < 2)
    {
        return (c.xi[0] + c.xi[1] * L + c.xi[2] * L * L + c.xi[3] * L * L * L);
    }
    else 
    {
        throw std::invalid_argument("Coord out of range");
    }
}

coord indexToCoord(int i, int L)
{
    if (0 <= i && i < 2*L*L*L)
    {
        coord c;
        c.xi[0] = i % L;
        c.xi[1] = (int)floor(i / L) % L;
        c.xi[2] = (int)floor(i / (L * L)) % L;
        c.xi[3] = (int)floor(i / (L * L * L)) % L; //not sure this needs a %L but why not
        return c;
    }
    else
    {
        throw std::invalid_argument("Index out of range");
    }
}

int neigh(int v, int dir, int sign, int L)
{
    if (0 <= dir && dir < 4 && (sign == -1 || sign == 1))
    {
        coord c = indexToCoord(v, L);
        if (c.xi[3] == 1)
        {
            if (dir == xy)
            {
                c.xi[0] = (c.xi[0] + (sign > 0)) % L;
                c.xi[1] = (c.xi[1] + (sign > 0)) % L;
                c.xi[2] = (c.xi[2] + (sign < 0)) % L;
                c.xi[3] = 0;
            }
            if (dir == xz)
            {
                c.xi[0] = (c.xi[0] + (sign > 0)) % L;
                c.xi[2] = (c.xi[2] + (sign > 0)) % L;
                c.xi[1] = (c.xi[1] + (sign < 0)) % L;
                c.xi[3] = 0;
            }
            if (dir == yz)
            {
                c.xi[1] = (c.xi[1] + (sign > 0)) % L;
                c.xi[2] = (c.xi[2] + (sign > 0)) % L;
                c.xi[0] = (c.xi[0] + (sign < 0)) % L;
                c.xi[3] = 0;
            }
            if (dir == xyz)
            {
                c.xi[0] = (c.xi[0] + (sign > 0)) % L;
                c.xi[1] = (c.xi[1] + (sign > 0)) % L;
                c.xi[2] = (c.xi[2] + (sign > 0)) % L;
                c.xi[3] = 0;
            }
        }
        else
        {
            if (dir == xy)
            {
                // Mod doesn't work as you expect for -ve numbers in C++
                c.xi[0] = (c.xi[0] - (sign < 0) + L) % L;
                c.xi[1] = (c.xi[1] - (sign < 0) + L) % L;
                c.xi[2] = (c.xi[2] - (sign > 0) + L) % L;
                c.xi[3] = 1;
            }
            if (dir == xz)
            {
                c.xi[0] = (c.xi[0] - (sign < 0) + L) % L;
                c.xi[2] = (c.xi[2] - (sign < 0) + L) % L;
                c.xi[1] = (c.xi[1] - (sign > 0) + L) % L;
                c.xi[3] = 1;
            }
            if (dir == yz)
            {
                c.xi[1] = (c.xi[1] - (sign < 0) + L) % L;
                c.xi[2] = (c.xi[2] - (sign < 0) + L) % L;
                c.xi[0] = (c.xi[0] - (sign > 0) + L) % L;
                c.xi[3] = 1;
            }
            if (dir == xyz)
            {
                c.xi[0] = (c.xi[0] - (sign < 0) + L) % L;
                c.xi[1] = (c.xi[1] - (sign < 0) + L) % L;
                c.xi[2] = (c.xi[2] - (sign < 0) + L) % L;
                c.xi[3] = 1;
            }
        }
        return coordToIndex(c, L);
    }
    else 
    {
        std::string errorMsg = "Invalid argument passed to neigh (arguments were " 
                                + std::to_string(v) + ',' + std::to_string(dir) + ',' 
                                + std::to_string(sign) + ',' + std::to_string(L) + ")\n";
    throw std::invalid_argument(errorMsg);
    }
}

int neighXYZ(int v, int dir, int sign, int L)
{
    coord c = indexToCoord(v, L);
    if (sign > 0)
    {
        if (dir == 0) c.xi[0] = (c.xi[0] + 1) % L;
        else if (dir == 1) c.xi[1] = (c.xi[1] + 1) % L;
        else if (dir == 2) c.xi[2] = (c.xi[2] + 1) % L;
    }
    else if (sign < 0)
    {
        if (dir == 0) c.xi[0] = (c.xi[0] - 1 + L) % L;
        else if (dir == 1) c.xi[1] = (c.xi[1] - 1 + L) % L;
        else if (dir == 2) c.xi[2] = (c.xi[2] - 1 + L) % L;
    }
    return coordToIndex(c, L);
}

int edgeIndex(int v, int dir, int sign, int L)
{
    if (sign < 0)
    {
        v = neigh(v, dir, sign, L);
    }
    return 4 * v + dir;
}

// This is different to cubic lattice, 
// I am not using a clever indexing system and just manually adding each face
void addFace(int v, int f, const vint &dirs, const vint &dirs2, 
                 const vint &signs, const vint &signs2, Lattice &lattice)
{
    // Construct edges and vertices
    vint vertices;
    vint edges;
    pint cells;
    int neighV = neigh(v, dirs[0], signs[0], lattice.L);
    vertices = {v, neighV,
                neigh(v, dirs[1], signs[1], lattice.L),
                neigh(neighV, dirs[2], signs[2], lattice.L)};
    edges = {edgeIndex(v, dirs[0], signs[0], lattice.L),
             edgeIndex(v, dirs[1], signs[1], lattice.L),
             edgeIndex(neighV, dirs[2], signs[2], lattice.L),
             edgeIndex(vertices[2], dirs[3], signs[3], lattice.L)};
    if (neighXYZ(v, dirs2[0], signs2[0], lattice.L) < neighXYZ(v, dirs2[1], signs2[1], lattice.L))
    {
        cells = {neighXYZ(v, dirs2[0], signs2[0], lattice.L), neighXYZ(v, dirs2[1], signs2[1], lattice.L)};
    }
    else cells = {neighXYZ(v, dirs2[1], signs2[1], lattice.L), neighXYZ(v, dirs2[0], signs2[0], lattice.L)};
    std::sort(vertices.begin(), vertices.end());
    std::sort(edges.begin(), edges.end());
    // Populate vvints
    lattice.faceToVertices[f] = vertices;
    lattice.faceToEdges[f] = edges;
    lattice.faceToCells[f] = cells;
    for (const auto vertex :  vertices)
    {
        lattice.vertexToFaces[vertex].push_back(f);
    }
    for (const auto edge : edges)
    {
        lattice.edgeToFaces[edge].push_back(f);
    }
}

int findFace(vint vertices, vvint &vertexToFaces, vvint &faceToVertices)
{
    std::sort(vertices.begin(), vertices.end());
    auto v0Faces = vertexToFaces[vertices[0]];
    for (const auto &face : v0Faces)
    {
        //just the two w=0 vertices is enough to specify a face
        if (faceToVertices[face][1] == vertices[1])
        {
            return face;
        }
    }
    return -1; //no face contains both of these vertices
}

int scalarProduct(std::vector<float> vec, int dir, int sign)
{
    if (sign != -1 && sign != 1) throw std::invalid_argument("Invalid sign");
    if (dir == 0) return sign*vec[0] + sign*vec[1] - sign*vec[2];   //{1,1,-1}
    else if (dir == 1) return sign*vec[0] - sign*vec[1] + sign*vec[2];  //{1,-1,1}
    else if (dir == 2) return -1*sign*vec[0] + sign*vec[1] + sign*vec[2];   //{-1,1,1}
    else if (dir == 3) return sign*vec[0] + sign*vec[1] + sign*vec[2];  //{1,1,1}
    else throw std::invalid_argument("Invalid direction");
}

std::vector<float> differenceVector(coord c1, coord c2)
{
    std::vector<float> diff = {c2.xi[0] - c1.xi[0] + 
                              (static_cast<float>(c2.xi[3] - c1.xi[3])/2),
                              c2.xi[1] - c1.xi[1] + 
                              (static_cast<float>(c2.xi[3] - c1.xi[3])/2),
                              c2.xi[2] - c1.xi[2] + 
                              (static_cast<float>(c2.xi[3] - c1.xi[3])/2)};
    return diff;
}

float magnitude(std::vector<float> diff)
{
    float mag = sqrt(diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2]);
    return mag;
}

vint shortestPath(int v1, int v2, Lattice &lattice)
{
    int originalVertex = v1;
    coord c1 = indexToCoord(v1, lattice.L);
    coord c2 = indexToCoord(v2, lattice.L);
    std::vector<float> diff = {c2.xi[0] - c1.xi[0] + 
                                    (static_cast<float>(c2.xi[3] - c1.xi[3])/2), 
                               c2.xi[1] - c1.xi[1] + 
                                    (static_cast<float>(c2.xi[3] - c1.xi[3])/2),
                               c2.xi[2] - c1.xi[2] + 
                                    (static_cast<float>(c2.xi[3] - c1.xi[3])/2)};
    vint path = {};
    int turnAround = 0;
    while (abs(diff[0]) + abs(diff[1]) + abs(diff[2]) > 0)
    {
        int bestEdge;
        vpint products;
        vint &edges = lattice.vertexToEdges[v1];
        for (int edge : edges)
        {
            int dir = edge % 4;
            int sign;
            if (edge/4 == v1) sign = 1;
            else sign = -1;
            if (std::find(lattice.zSyndIndices.begin(), lattice.zSyndIndices.end(), edge) 
                    != lattice.zSyndIndices.end())
            {
                products.push_back({scalarProduct(diff, dir, sign), edge});
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
        if (std::find(path.begin(), path.end(), bestEdge) != path.end())
        {
            //Sometimes gets stuck at a dead end and needs to turn around
            //we go back to the previous vertex and rerun then choose the second best edge 
            if (path.back() == bestEdge) turnAround = 1;
            //If we have gone in a larger loop just abort
            //but I don't think this should happen
            else
            {
                std::string errorMsg = ("Loop occurred while building path between vertices " 
                                        + std::to_string(v1) + " and " 
                                        + std::to_string(v2) + '\n'
                                        + " (vertices passed to function were " 
                                        + std::to_string(originalVertex) + " and " 
                                        + std::to_string(v2) + ")\n");
                throw std::runtime_error(errorMsg);
            }
        }
        if (turnAround == 0) path.push_back(bestEdge);
        else path.pop_back();
        pint verts = lattice.edgeToVertices[bestEdge];
        if (verts.first == v1) v1 = verts.second;
        else v1 = verts.first;
        c1 = indexToCoord(v1, lattice.L);
        diff = {c2.xi[0] - c1.xi[0] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2), 
                c2.xi[1] - c1.xi[1] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2),
                c2.xi[2] - c1.xi[2] + (static_cast<float>(c2.xi[3] - c1.xi[3])/2)};
    }

    return path;
}

vint shortestDualPath(int cell1, int cell2, Lattice &lattice, 
                                int useOuter, int useInner)
{
    int originalCell = cell1;
    coord cd1 = indexToCoord(cell1, lattice.L);
    coord cd2 = indexToCoord(cell2, lattice.L);
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
            int q = lattice.cellToFaces[cell1][i];
            int allowedFace = 0;
            if (useOuter == 1)
            {
                if (std::find(lattice.outerQubitIndices.begin(), 
                              lattice.outerQubitIndices.end(), q) 
                              != lattice.outerQubitIndices.end()) allowedFace = 1;
            }
            if (useInner == 1 && allowedFace == 0)
            {
                if (std::find(lattice.innerQubitIndices.begin(), 
                              lattice.innerQubitIndices.end(), q) 
                              != lattice.innerQubitIndices.end()) allowedFace = 1;
            }
            if (allowedFace == 1)
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
        bestFace = lattice.cellToFaces[cell1][bestDir];
        //Once again, probably don't need this
        //but no harm in leaving it in just in case 
        if (std::find(path.begin(), path.end(), bestFace) != path.end())
        {
            if (path.back() == bestFace) turnAround = 1;
            else
            {
                std::string errorMsg = ("Loop occurred while building path between cells " 
                                        + std::to_string(cell1) + " and " 
                                        + std::to_string(cell2) + '\n'
                                        + " (cells passed to function were " 
                                        + std::to_string(originalCell) + " and " 
                                        + std::to_string(cell2) + ")\n");
                throw std::runtime_error(errorMsg);
            }
        }
        if (turnAround == 0) path.push_back(bestFace);
        else path.pop_back();
        cd1.xi[0] += dirs[bestDir][0];
        cd1.xi[1] += dirs[bestDir][1];
        cd1.xi[2] += dirs[bestDir][2];
        cell1 = coordToIndex(cd1, lattice.L);
        diff = {cd2.xi[0] - cd1.xi[0], 
                cd2.xi[1] - cd1.xi[1], 
                cd2.xi[2] - cd1.xi[2]};
    }
    return path;
}

void checkIn2DCodespace(Lattice &lattice)
{
    //We only need to check this for X errors/Z stabilisers
    //Because 2D Z stabilisers are also 3D Z stabilisers
    //So if there is no syndrome for 3D X stabilisers this means no 2D Z errors
    lattice.calcSynd('z',1,0);
    for (int y = 0; y < lattice.L-1; y++)
    {
        for (int z = 0; z < lattice.L-1; z++)
        {
            coord cd = {0,y,z,0};
            int v = coordToIndex(cd,lattice.L);
            vint edges = lattice.vertexToEdges[v];
            int count = 0;
            for (int e : edges)
            {
                if (lattice.syndromeZ[e] == 1) count += 1;
            }
            if (count % 2 == 1) 
            {
                throw std::runtime_error("Out of 2D codespace (bad X correction)");
            }
        }
    }
}

//This assumes no out of bounds errors
void jumpCorrection(Lattice &lattice, std::mt19937& engine, 
                        std::uniform_real_distribution<double>& dist, int r)
{
    for (int x = lattice.L-4; x >= 0; x--)
    {
        for (int cycle = 0; cycle < 2; cycle++)
        {
            for (int z = 0; z < lattice.L-2; z++)
            {
                for (int y = 0; y < lattice.L-2; y++)
                {
                    if (r == 1 && (x + y + z) % 2 == 1) continue;
                    else if (r == 2 && (x + y + z) % 2 == 0) continue;
                    int v = x + y*lattice.L + z*lattice.L*lattice.L;
                    if (cycle == 0)
                    {
                        vint faces(4);
                        vvint edges(4);
                        faces[0] = findFace({v,neigh(neigh(v,xyz,1,lattice.L),xz,1,lattice.L)},
                                        lattice.vertexToFaces, lattice.faceToVertices);
                        faces[1] = findFace({v,neigh(neigh(v,xyz,1,lattice.L),xy,1,lattice.L)},
                                        lattice.vertexToFaces, lattice.faceToVertices);
                        faces[2] = findFace({v,neigh(neigh(v,xy,1,lattice.L),yz,-1,lattice.L)},
                                        lattice.vertexToFaces, lattice.faceToVertices);
                        faces[3] = findFace({v,neigh(neigh(v,yz,-1,lattice.L),xz,1,lattice.L)},
                                        lattice.vertexToFaces, lattice.faceToVertices);
                        edges[0] = {edgeIndex(v,xz,1,lattice.L), edgeIndex(v,xyz,1,lattice.L)};
                        edges[1] = {edgeIndex(v,xyz,1,lattice.L), edgeIndex(v,xy,1,lattice.L)};
                        edges[2] = {edgeIndex(v,xy,1,lattice.L), edgeIndex(v,yz,-1,lattice.L)};
                        edges[3] = {edgeIndex(v,yz,-1,lattice.L), edgeIndex(v,xz,1,lattice.L)};
                        for (int i = 0; i < 4; i++)
                        {
                            if (lattice.qubitsZ[faces[i]] == 1) //assumes bounds check
                            {
                                if (lattice.qubitsZ[faces[(i-1+4)%4]] == 1 
                                        && lattice.qubitsZ[faces[(i+1)%4]] == 0)
                                {
                                    lattice.applyZStab(edges[i][0]);
                                }
                                else if (lattice.qubitsZ[faces[(i-1+4)%4]] == 0 
                                            && lattice.qubitsZ[faces[(i+1)%4]] == 1)
                                {
                                    lattice.applyZStab(edges[i][1]);
                                }
                                else
                                {
                                    if (dist(engine) < 0.5)
                                    {
                                        if (std::find(lattice.zSyndIndices.begin(),
                                                      lattice.zSyndIndices.end(),edges[i][0]) 
                                                      != lattice.zSyndIndices.end())
                                        {
                                            lattice.applyZStab(edges[i][0]);
                                        }
                                        else lattice.applyZStab(edges[i][1]);
                                    }
                                    else 
                                    {
                                        if (std::find(lattice.zSyndIndices.begin(),
                                                      lattice.zSyndIndices.end(),edges[i][1]) 
                                                      != lattice.zSyndIndices.end())
                                        {
                                            lattice.applyZStab(edges[i][1]);
                                        }
                                        else lattice.applyZStab(edges[i][0]);
                                    }
                                }
                            }
                        }
                    }
                    else if (x != 0)
                    {
                        vint faces(2);
                        vvint edges(2);
                        faces[0] = findFace({v,neigh(neigh(v,xyz,1,lattice.L),yz,1,lattice.L)},
                                        lattice.vertexToFaces, lattice.faceToVertices);
                        faces[1] = findFace({v,neigh(neigh(v,xy,1,lattice.L),xz,-1,lattice.L)},
                                        lattice.vertexToFaces, lattice.faceToVertices);
                        edges[0] = {edgeIndex(v,yz,1,lattice.L), 
                                        edgeIndex(neigh(v,yz,1,lattice.L),xyz,1,lattice.L)};
                        edges[1] = {edgeIndex(v,xz,-1,lattice.L), 
                                        edgeIndex(neigh(v,xz,-1,lattice.L),xy,1,lattice.L)};
                        for (int i = 0; i < 2; i++)
                        {
                            if (lattice.qubitsZ[faces[i]] == 1) //assumes bounds check 
                            {
                                if (dist(engine) < 0.5)
                                {
                                    if (std::find(lattice.zSyndIndices.begin(), 
                                                  lattice.zSyndIndices.end(), 
                                                  edges[i][0]) != lattice.zSyndIndices.end())
                                    {
                                        lattice.applyZStab(edges[i][0]);
                                    }
                                    else lattice.applyZStab(edges[i][1]);
                                }
                                else
                                {
                                    if (std::find(lattice.zSyndIndices.begin(), 
                                                  lattice.zSyndIndices.end(), 
                                                  edges[i][1]) != lattice.zSyndIndices.end())
                                    {
                                        lattice.applyZStab(edges[i][1]);
                                    }
                                    else lattice.applyZStab(edges[i][0]);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

}
