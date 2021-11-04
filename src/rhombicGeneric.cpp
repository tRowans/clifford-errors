
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
