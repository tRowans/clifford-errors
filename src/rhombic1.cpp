#include "rhombic1.h"

namespace rhombic1 {

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

void buildFaces(vvint &faceToVertices, vvint &faceToEdges, vpint &faceToCells, vvint &vertexToFaces, vvint &edgeToFaces, int L)
{
    faceToVertices.assign(3 * L * L * L, {});
    faceToEdges.assign(3 * L * L * L, {});
    faceToCells.assign(3 * L * L * L, {});
    vertexToFaces.assign(2 * L * L * L, {});
    edgeToFaces.assign(8 * L * L * L, {});
    int f = 0;
    // Loop through all w=0 vertices coordinates
    for (int v = 0; v < L * L * L; ++v) 
    {
        coord c = indexToCoord(v, L);
        // Only half of w=0 coordinates have vertices
        if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 1) continue;
        vint signs = {1, 1, 1, 1};
        vint signs2 = {1, 1};
        // Each face containing w=0 vertex is in two such vertices, so we add 6/12 per vertex to avoid double counting
        addFace(v, f, {xyz, yz, yz, xyz}, {1, 2}, signs, signs2,
                 faceToVertices, faceToEdges, faceToCells, vertexToFaces, edgeToFaces, L);
        ++f;
        addFace(v, f, {xyz, xz, xz, xyz}, {0, 2}, signs, signs2,
                 faceToVertices, faceToEdges, faceToCells, vertexToFaces, edgeToFaces, L);
        ++f;
        addFace(v, f, {xyz, xy, xy, xyz}, {0, 1}, signs, signs2,
                 faceToVertices, faceToEdges, faceToCells, vertexToFaces, edgeToFaces, L);
        ++f;
        signs = {1, -1, -1, 1};
        signs2 = {1, -1};
        addFace(v, f, {xy, xz, xz, xy}, {1, 2}, signs, signs2,
                 faceToVertices, faceToEdges, faceToCells, vertexToFaces, edgeToFaces, L);
        ++f;
        addFace(v, f, {xy, yz, yz, xy}, {0, 2}, signs, signs2,
                 faceToVertices, faceToEdges, faceToCells, vertexToFaces, edgeToFaces, L);
        ++f;
        addFace(v, f, {xz, yz, yz, xz}, {0, 1}, signs, signs2,
                 faceToVertices, faceToEdges, faceToCells, vertexToFaces, edgeToFaces, L);
        ++f;
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

int faceToBaseVertex(int face, int L)
{
    //Each w=0 vertex has six associated faces but the indexes don't always match 
    //This takes a face and returns the index of its associated vertex
    int v = face / 3;
    int dir = face % 6;
    coord cd = indexToCoord(v, L);
    if ((cd.xi[0] + cd.xi[1] + cd.xi[2]) % 2 == 1)
    {
        if (dir < 3) v += 1;
        else v -= 1;
    }
    return v;
}

vvint buildVertexToEdges(int L)
{
    vvint vertexToEdges;
    vertexToEdges.assign(2 * L * L * L, {});
    for (int v = 0; v < 2 * L * L * L; ++v)
    {
        vint edges;
        coord c = indexToCoord(v, L);
        if (c.xi[3] == 0)
        {
            if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 0)
            {
                // w=0 vertex (8 valent)
                edges.push_back(edgeIndex(v, xy, 1, L));
                edges.push_back(edgeIndex(v, xz, 1, L));
                edges.push_back(edgeIndex(v, yz, 1, L));
                edges.push_back(edgeIndex(v, xyz, 1, L));
                edges.push_back(edgeIndex(v, xy, -1, L));
                edges.push_back(edgeIndex(v, xz, -1, L));
                edges.push_back(edgeIndex(v, yz, -1, L));
                edges.push_back(edgeIndex(v, xyz, -1, L));
            }
            // Other w=0 vertices aren't present in lattice so we leave their edges as {}
        }
        else
        {
        if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 0)
        {
            // w=1 vertex (4 valent) type 1
            edges.push_back(edgeIndex(v, xy, 1, L));
            edges.push_back(edgeIndex(v, xz, 1, L));
            edges.push_back(edgeIndex(v, yz, 1, L));
            edges.push_back(edgeIndex(v, xyz, -1, L));
        }   
        else
        {
            // w=1 vertex (4 valent) type 2
            edges.push_back(edgeIndex(v, xyz, 1, L));
            edges.push_back(edgeIndex(v, xy, -1, L));
            edges.push_back(edgeIndex(v, xz, -1, L));
            edges.push_back(edgeIndex(v, yz, -1, L));
        }
        }
        std::sort(edges.begin(), edges.end());
        vertexToEdges[v] = edges;
    }
    return vertexToEdges;
}

vpint buildEdgeToVertices(int L)
{
    vpint edgeToVertices;
    edgeToVertices.assign(8 * L * L * L, {});
    for (int e = 0; e < 8 * L * L * L; ++e)
    {
        int v1 = e / 4;
        int dir = e % 4;
        coord c = indexToCoord(v1, L);
        if (c.xi[3] == 0)
        {
            // No edges from some vertices
            if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 1) continue;
        }
        else
        {
            if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 0)
            {
                if (dir == xyz) continue;
            }
            else
            {
                if (dir == xy || dir == xz || dir == yz) continue;
            }
        }
        int v2 = neigh(v1, dir, 1, L);
        if (v1 < v2)
        {
            edgeToVertices[e] = {v1, v2};
        }
        else
        {
            edgeToVertices[e] = {v2, v1};
        }
    }
    return edgeToVertices;
}


void buildCellToFaces(vvint &cellToFaces, vvint &vertexToFaces, vvint &faceToVertices, int L)
{
    //One cell for each odd vertex in the w=0 lattice
    //I do these separately and not as part of buildFaces because I want them to be in a specific order
    //this is useful later when making paths through faces on dual lattice for Z error decoding
    cellToFaces.assign(L * L * L, {});
    for (int v = 0; v < L * L * L; v++)
    {
        coord cd = indexToCoord(v, L);
        if ((cd.xi[0] + cd.xi[1] + cd.xi[2]) % 2 == 0) continue;
        vint faces;    
       
        int xPlus = neighXYZ(v,0,1,L);
        int xMinus = neighXYZ(v,0,-1,L);
        int yPlus = neighXYZ(v,1,1,L);
        int yMinus = neighXYZ(v,1,-1,L);
        int zPlus = neighXYZ(v,2,1,L);
        int zMinus = neighXYZ(v,2,-1,L);

        faces.push_back(findFace({xPlus,yPlus}, vertexToFaces, faceToVertices));
        faces.push_back(findFace({xPlus,yMinus}, vertexToFaces, faceToVertices));
        faces.push_back(findFace({xMinus,yPlus}, vertexToFaces, faceToVertices));
        faces.push_back(findFace({xMinus,yMinus}, vertexToFaces, faceToVertices));
        faces.push_back(findFace({xPlus,zPlus}, vertexToFaces, faceToVertices));
        faces.push_back(findFace({xMinus,zPlus}, vertexToFaces, faceToVertices));
        faces.push_back(findFace({yPlus,zPlus}, vertexToFaces, faceToVertices));
        faces.push_back(findFace({yMinus,zPlus}, vertexToFaces, faceToVertices));
        faces.push_back(findFace({xPlus,zMinus}, vertexToFaces, faceToVertices));
        faces.push_back(findFace({xMinus,zMinus}, vertexToFaces, faceToVertices));
        faces.push_back(findFace({yPlus,zMinus}, vertexToFaces, faceToVertices));
        faces.push_back(findFace({yMinus,zMinus}, vertexToFaces, faceToVertices));

        cellToFaces[v] = faces;
    }
    return cellToFaces;
}

vint buildQubitIndices(vvint &vertexToFaces, vvint &faceToVertices, int L)
{
    vint qubitIndices;
    for (int f = 0; f < 3 * L * L * L; f++)
    {
        int v = faceToBaseVertex(f, L);
        int dir = f % 6;
        coord cd = indexToCoord(v, L);

        if (cd.xi[0] < (L-3) && cd.xi[1] < (L-2) && cd.xi[2] < (L-2))
        {
            if (dir == 0 && cd.xi[1] < (L-3) && cd.xi[2] < (L-3)) qubitInidces.push_back(f);
            else if (dir == 1 && cd.xi[0] < (L-4)
                              && cd.xi[1] > 0 && cd.xi[1] < (L-3)
                              && cd.xi[2] < (L-3)) qubitIndices.push_back(f);
            else if (dir == 2 && cd.xi[0] < (L-4)
                              && cd.xi[1] < (L-3)
                              && cd.xi[2] > 0 && cd.xi[2] < (L-3)) qubitIndices.push_back(f);
            else if (dir == 3 && cd.xi[1] < (L-3) && cd.xi[2] > 0) qubitIndices.push_back(f);
            else if (dir == 4 && cd.xi[0] < (L-4)
                              && cd.xi[1] > 0 && cd.xi[1] < (L-3)
                              && cd.xi[2] > 0) qubitIndices.push_back(f);
            else if (dir == 5 && cd.xi[0] < (L-4)
                              && cd.xi[1] > 0
                              && cd.xi[2] > 0 && cd.xi[2] < (L-3)) qubitIndices.push_back(f);
        }
    }
    return qubitIndices;
}

//X boundary stabilisers on +z and -z boundaries

vint buildXSyndIndices(int L)
{
    vint xSyndIndices;
    for (int v = 0; v < L * L * L; v++)
    {
        coord cd = indexToCoord(v, L);
        if ((cd.xi[0] + cd.xi[1] + cd.xi[2]) % 2 == 1) 
        {
            if (cd.xi[0] < (L-3)
                && cd.xi[1] > 0 && cd.xi[1] < (L-3)
                && cd.xi[2] < (L-2)) xSyndIndices.push_back(v); 
        }
    }
    return xSyndIndices;
}

//Z boundary stabilisers on +y and -y boundaries

vint buildZSyndIndices(int L)
{
    vint zSyndIndices;
    for (int v = 0; v < L * L * L; v++)
    {
        coord cd = indexToCoord(v, L);
        if ((cd.xi[0] + cd.xi[1] + cd.xi[2]) % 2 == 1) continue;
        if (cd.xi[0] > (L-4) || cd.xi[1] > (L-3) || cd.xi[2] == 0 || cd.xi[2] > (L-4)) continue;
        vint dirs;
        dirs.assign(8, 1); //xy, xz, yz, xyz, -xy, -xz, -yz, -xyz
        //set the ones we don't want to 0 for each boundary
        //this is nice because it automatically gets the boundary intersections too
        if (cd.xi[0] == 0)
        {
            dirs[2] = 0;
            dirs[4] = 0;
            dirs[5] = 0;
            dirs[7] = 0;
        }
        else if (cd.xi[0] == (L-4))
        {
            dirs[0] = 0;
            dirs[1] = 0;
            dirs[3] = 0;
            dirs[6] = 0;
        }
        if (cd.xi[1] == 0)
        {
            dirs[1] = 0;
            dirs[4] = 0;
            dirs[6] = 0;
            dirs[7] = 0;
        }
        else if (cd.xi[1] == (L-3))
        {
            dirs[0] = 0;
            dirs[2] = 0;
            dirs[3] = 0;
            dirs[5] = 0;
        }
        for (i = 0; i < 4; i++)
        {
            if (dirs[i] == 1) zSyndIndices.push_back(edgeIndex(v,i,1,L));
            if (dirs[i+4] == 1) zSyndIndices.push_back(edgeIndex(v,i+4,-1,L));
        }
    }
    std::sort(zSyndIndices.begin(), zSyndIndices.end());
    return zSyndIndices;
}

vint buildLogicals(vint &xLogical, vint &zLogical, vint &qubitIndices, int L)
{
    //Do these together because the loops are the same 
    for (int q : qubitIndices)
    {
        vint vertices = faceToVertices[q];
        for (int v : vertices)
        {
            coord cd = indexToCoord(v, L);
            if (cd.xi[1] == 0 && cd.xi[3] == 0) xLogical.push_back(q);
            if (cd.xi[0] == 0 && cd.xi[2] == 0 && cd.xi[3] == 0) zLogical.push_back(q);
        }
    }
}

