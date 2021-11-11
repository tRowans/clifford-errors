#include "rhombicGeneric.h"

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
        c.xi[3] = (int)floor(i / (L * L * L)) % L; //not sure this needs a %L but I'll leave it
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
        std::string errorMsg = "Invalid argument passed to neigh (arguments were " + std::to_string(v) + ',' + std::to_string(dir) + ',' + std::to_string(sign) + ',' + std::to_string(L) + ")\n";
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

// This is different to cubic lattice, I am not using a clever indexing system and just manually adding each face
void addFace(int v, int f, const vint &dirs, const vint &dirs2, const vint &signs, const vint &signs2, vvint &faceToVertices, vvint &faceToEdges, vpint &faceToCells, vvint &vertexToFaces, vvint &edgeToFaces, int L)
{
    // Construct edges and vertices
    vint vertices;
    vint edges;
    pint cells;
    int neighV = neigh(v, dirs[0], signs[0], L);
    vertices = {v, neighV,
                neigh(v, dirs[1], signs[1], L),
                neigh(neighV, dirs[2], signs[2], L)};
    edges = {edgeIndex(v, dirs[0], signs[0], L),
             edgeIndex(v, dirs[1], signs[1], L),
             edgeIndex(neighV, dirs[2], signs[2], L),
             edgeIndex(vertices[2], dirs[3], signs[3], L)};
    if (neighXYZ(v, dirs2[0], signs2[0], L) < neighXYZ(v, dirs2[1], signs2[1], L))
    {
        cells = {neighXYZ(v, dirs2[0], signs2[0], L), neighXYZ(v, dirs2[1], signs2[1], L)};
    }
    else cells = {neighXYZ(v, dirs2[1], signs2[1], L), neighXYZ(v, dirs2[0], signs2[0], L)};
    std::sort(vertices.begin(), vertices.end());
    std::sort(edges.begin(), edges.end());
    // Populate vvints
    faceToVertices[f] = vertices;
    faceToEdges[f] = edges;
    faceToCells[f] = cells;
    for (const auto vertex :  vertices)
    {
        vertexToFaces[vertex].push_back(f);
    }
    for (const auto edge : edges)
    {
        edgeToFaces[edge].push_back(f);
    }
}

int findFace(vint &vertices, vvint &vertexToFaces, vvint &faceToVertices)
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

int scalarProduct(std::vector<float> vec, int dir)
{
    //scalar product of a vector with one of the edge directions
    //but these directions can be negative
    int sign = 1;
    if (dir < 0) 
    {
        dir = -1*dir;
        sign = -1;
    }
    if (dir == 1) return sign*vec[0] + sign*vec[1] - sign*vec[2];   //{1,1,-1}
    else if (dir == 2) return sign*vec[0] - sign*vec[1] + sign*vec[2];  //{1,-1,1}
    else if (dir == 3) return -1*sign*vec[0] + sign*vec[1] + sign*vec[2];   //{-1,1,1}
    else if (dir == 4) return sign*vec[0] + sign*vec[1] + sign*vec[2];  //{1,1,1}
    else throw std::invalid_argument("Invalid direction");
}

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

vint shortestPath(int v1, int v2, vint &syndIndices, vvint &vertexToEdges, int L, int r)
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
            if (r == 1)
            {
                if ((c1.xi[0] + c1.xi[1] + c1.xi[2]) % 2 == 0) dirs = dirs2;
                else dirs = dirs3;
            }
            else 
            {
                if ((c1.xi[0] + c1.xi[1] + c1.xi[2]) % 2 == 1) dirs = dirs2;
                else dirs = dirs3;
            }
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

vint shortestDualPath(int cell1, int cell2, vint &outerQubitIndices, vint &innerQubitIndices, vvint &cellToFaces, int L)
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
            int q = cellToFaces[cell1][i];
            if (std::find(outerQubitIndices.begin(), outerQubitIndices.end(), q) 
                    != outerQubitIndices.end() ||
                std::find(innerQubitIndices.begin(), innerQubitIndices.end(), q) 
                    != innerQubitIndices.end())
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

