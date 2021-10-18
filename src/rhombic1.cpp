#include "rhombic1.h"

namespace rhombic1 {

int coordToIndex(coord c, int L)
{
    if (0 <= c.xi[0] && c.xi[0] < 5*L &&
        0 <= c.xi[1] && c.xi[1] < L &&
        0 <= c.xi[2] && c.xi[2] < L &&
        0 <= c.xi[3] && c.xi[3] < 2)
    {
        return (c.xi[0] + c.xi[1] * 5 * L + c.xi[2] * 5 * L * L + c.xi[3] * 5 * L * L * L);
    }
    else 
    {
        throw std::invalid_argument("Coord out of range");
    }
}

coord indexToCoord(int i, int L)
{
    if (0 <= i && i < 10*L*L*L)
    {
        coord c;
        c.xi[0] = i % (5 * L);
        c.xi[1] = (int)floor(i / (5 * L)) % L;
        c.xi[2] = (int)floor(i / (5 * L * L)) % L;
        c.xi[3] = (int)floor(i / (5 * L * L * L)) % L; //not sure this needs a %L but I'll leave it
        return c;
    }
    else
    {
        throw std::invalid_argument("Index out of range");
    }
}

coord cellToCoord(int cell, int L)
{
    coord cd = indexToCoord(2*cell, L);
    if ((cd.xi[0] + cd.xi[1] + cd.xi[2]) % 2 == 0) cd = indexToCoord(2*cell + 1, L);
    return cd;
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
                c.xi[0] = (c.xi[0] + (sign > 0)) % (5 * L);
                c.xi[1] = (c.xi[1] + (sign > 0)) % L;
                c.xi[2] = (c.xi[2] + (sign < 0)) % L;
                c.xi[3] = 0;
            }
            if (dir == xz)
            {
                c.xi[0] = (c.xi[0] + (sign > 0)) % (5 * L);
                c.xi[2] = (c.xi[2] + (sign > 0)) % L;
                c.xi[1] = (c.xi[1] + (sign < 0)) % L;
                c.xi[3] = 0;
            }
            if (dir == yz)
            {
                c.xi[1] = (c.xi[1] + (sign > 0)) % L;
                c.xi[2] = (c.xi[2] + (sign > 0)) % L;
                c.xi[0] = (c.xi[0] + (sign < 0)) % (5 * L);
                c.xi[3] = 0;
            }
            if (dir == xyz)
            {
                c.xi[0] = (c.xi[0] + (sign > 0)) % (5 * L);
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
                c.xi[0] = (c.xi[0] - (sign < 0) + (5 * L)) % (5 * L);
                c.xi[1] = (c.xi[1] - (sign < 0) + L) % L;
                c.xi[2] = (c.xi[2] - (sign > 0) + L) % L;
                c.xi[3] = 1;
            }
            if (dir == xz)
            {
                c.xi[0] = (c.xi[0] - (sign < 0) + (5 * L)) % (5 * L);
                c.xi[2] = (c.xi[2] - (sign < 0) + L) % L;
                c.xi[1] = (c.xi[1] - (sign > 0) + L) % L;
                c.xi[3] = 1;
            }
            if (dir == yz)
            {
                c.xi[1] = (c.xi[1] - (sign < 0) + L) % L;
                c.xi[2] = (c.xi[2] - (sign < 0) + L) % L;
                c.xi[0] = (c.xi[0] - (sign > 0) + (5 * L)) % (5 * L);
                c.xi[3] = 1;
            }
            if (dir == xyz)
            {
                c.xi[0] = (c.xi[0] - (sign < 0) + (5 * L)) % (5 * L);
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
        if (dir == 0) c.xi[0] = (c.xi[0] + 1) % (5*L);
        else if (dir == 1) c.xi[1] = (c.xi[1] + 1) % L;
        else if (dir == 2) c.xi[2] = (c.xi[2] + 1) % L;
    }
    else if (sign < 0)
    {
        if (dir == 0) c.xi[0] = (c.xi[0] - 1 + 5*L) % (5*L);
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
        cells = {neighXYZ(v, dirs2[0], signs2[0], L)/2, neighXYZ(v, dirs2[1], signs2[1], L)/2};
    }
    else cells = {neighXYZ(v, dirs2[1], signs2[1], L)/2, neighXYZ(v, dirs2[0], signs2[0], L)/2};
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
    faceToVertices.assign(3 * 5 * L * L * L, {});
    faceToEdges.assign(3 * 5 * L * L * L, {});
    faceToCells.assign(3 * 5 * L * L * L, {});
    vertexToFaces.assign(2 * 5 * L * L * L, {});
    edgeToFaces.assign(8 * 5 * L * L * L, {});
    int f = 0;
    // Loop through all w=0 vertices coordinates
    for (int v = 0; v < 5 * L * L * L; ++v) 
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

//Haven't done L -> 5L change here but I don't think I need this function
/*
vvint listToBin(const vvint& in, int L)
{
    vvint bin;
    for (auto const& vec : in)
    {   
        vint row(3 * L * L * L, 0);
        for (auto const& elem : vec)
        {
            row[elem] = 1;
        }
        bin.push_back(row);
    }
    return bin;
}
*/

vvint buildVertexToEdges(int L)
{
    vvint vertexToEdges;
    vertexToEdges.assign(2 * 5 * L * L * L, {});
    for (int v = 0; v < 2 * 5 * L * L * L; ++v)
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
    edgeToVertices.assign(8 * 5 * L * L * L, {});
    for (int e = 0; e < 8 * 5 * L * L * L; ++e)
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


vvint buildCellToFaces(vvint &vertexToFaces, vvint &faceToVertices, int L)
{
    //One cell for each odd vertex in the w=0 lattice
    //I do these separately and not as part of buildFaces because I want them to be in a specific order
    //this is useful later when making paths through faces on dual lattice for Z error decoding
    vvint cellToFaces;
    for (int v = 0; v < 5 * L * L * L; v++)
    {
        coord c = indexToCoord(v, L);
        if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 1)
        {
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

            cellToFaces.push_back(faces);
        }
    }
    return cellToFaces;
}

/*
vpint buildFaceToCells(int L)
{
    vpint faceToCells;
    for (int f = 0; f < 3 * 5 * L * L * L; f++)
    {
        std::pair<int,int> cells;
        int v = f / 6;
        int dir = f % 6;
        int xPlus = (v+1)%5*L;
        int xMinus = (v-1+5*L)%5*L;
        int yPlus = (v+5*L)%(5*L*L);
        int yMinus = (v-5*L+5*L*L)%(5*L*L);
        int zPlus = (v+5*L*L)%(5*L*L*L);
        int zMinus = (v-5*L*L+5*L*L*L)%(5*L*L*L);

        if (dir == 0) cells = {yPlus/2,zPlus/2};
        else if (dir == 1) cells = {xPlus/2, zPlus/2};
        else if (dir == 2) cells = {xPlus/2, yPlus/2};
        else if (dir == 3) cells = {yPlus/2, zMinus/2};
        else if (dir == 4) cells = {xPlus/2, zMinus/2};
        else if (dir == 5) cells = {xPlus/2, yMinus/2};

        faceToCells.push_back(cells);
    }
    return faceToCells;
}

// String-like logicals 
// Not sure if there L -> 5L changes here are correct but I don't think I need this function anyway

vvint buildLogicals(int L, vvint &vertexToFaces, vvint &faceToVertices)
{
    vvint logicals;
    vint logical1, logical2, logical3;
    for (int i = 0; i < 5 * L; i += 2)
    {
        int v = coordToIndex({i, 0, 0, 0}, L);
        int neighV = neigh(v, xz, -1, L);
        vint faceVertices = {v, neighV, 
                            neigh(v, xyz, -1, L), 
                            neigh(neighV, xyz, -1, L)};
        std::sort(faceVertices.begin(), faceVertices.end());
        logical1.push_back(findFace(faceVertices, vertexToFaces, faceToVertices));
        neighV = neigh(v, xy, 1, L);
        faceVertices = {v, neighV,
                        neigh(v, yz, -1, L),
                        neigh(neighV, yz, -1, L)};
        std::sort(faceVertices.begin(), faceVertices.end());
        logical1.push_back(findFace(faceVertices, vertexToFaces, faceToVertices));
    }
    logicals.push_back(logical1);
    for (int i = 0; i < L; i += 2)
    {
        int v = coordToIndex({0, i, 0, 0}, L);
        int neighV = neigh(v, yz, -1, L);
        vint faceVertices = {v,neighV,
                            neigh(v, xyz, -1, L),
                            neigh(neighV, xyz, -1, L)};
        std::sort(faceVertices.begin(), faceVertices.end());
        logical2.push_back(findFace(faceVertices, vertexToFaces, faceToVertices));
        neighV = neigh(v, xy, 1, L);
        faceVertices = {v, neighV,
                        neigh(v, xz, -1, L),
                        neigh(neighV, xz, -1, L)};
        std::sort(faceVertices.begin(), faceVertices.end());
        logical2.push_back(findFace(faceVertices, vertexToFaces, faceToVertices));
    }
    logicals.push_back(logical2);
    for (int i = 0; i < L; i += 2)
    {
        int v = coordToIndex({0, 0, i, 0}, L);
        int neighV = neigh(v, xz, -1, L);
        vint faceVertices = {v, neighV,
                            neigh(v, xyz, -1, L),
                            neigh(neighV, xyz, -1, L)};
        std::sort(faceVertices.begin(), faceVertices.end());
        logical3.push_back(findFace(faceVertices, vertexToFaces, faceToVertices));
        neighV = neigh(v, yz, 1, L);
        faceVertices = {v, neighV,
                        neigh(v, xy, -1, L),
                        neigh(neighV, xy, -1, L)};
        std::sort(faceVertices.begin(), faceVertices.end());
        logical3.push_back(findFace(faceVertices, vertexToFaces, faceToVertices));
    }
    logicals.push_back(logical3);
    return logicals;
}
*/

void buildSlice(vint &lowerQubitIndices, vint &middleQubitIndices, vint &upperQubitIndices, vint &syndIndicesZ, vint &syndIndicesXLayer, vint &defectIndices, vint &bulkSweepVertices, vint &middleBoundarySweepVertices, vint &upperBoundarySweepVertices1, vint &upperBoundarySweepVertices2, vint &zLogical, vint &xLogicalFragment1, vint &xLogicalFragment2, vvint &vertexToFaces, vvint &faceToVertices, vvint &faceToEdges, vpint &edgeToVertices, int L)
{
    vint generatorVertices = {coordToIndex({1,L-3,L-3,1},L)};
    vint vertices; //this is for findFace function
    while (!generatorVertices.empty())
    {
        int v = generatorVertices[0];
        //Adding faces (qubits)
        vertices = {v, neigh(v, xy, -1, L), neigh(v, xz, -1, L), v - 1};
        lowerQubitIndices.push_back(findFace(vertices, vertexToFaces, faceToVertices));
        vertices = {v, neigh(v, yz, -1, L), neigh(v, xyz, 1, L), v + 1}; 
        middleQubitIndices.push_back(findFace(vertices, vertexToFaces, faceToVertices));

        coord c = indexToCoord(v, L);
        if (c.xi[1] > 1)
        {
            vertices = {v, neigh(v, xy, -1, L), neigh(v, yz, -1, L), v - 5*L};
            lowerQubitIndices.push_back(findFace(vertices, vertexToFaces, faceToVertices));
            c.xi[0] += 1;
            c.xi[1] -= 1;
            //can't use sets because they get automatically sorted and don't preserve ordering
            if (std::find(generatorVertices.begin(), generatorVertices.end(), coordToIndex(c,L)) 
                    == generatorVertices.end()) generatorVertices.push_back(coordToIndex(c,L));
            c = indexToCoord(v, L);
        }
        if (c.xi[2] > 1)
        {
            vertices = {v, neigh(v, xz, -1, L), neigh(v, yz, -1, L), v - 5*L*L};
            lowerQubitIndices.push_back(findFace(vertices, vertexToFaces, faceToVertices));
            c.xi[0] += 1;
            c.xi[2] -= 1;
            if (std::find(generatorVertices.begin(), generatorVertices.end(), coordToIndex(c,L)) 
                    == generatorVertices.end()) generatorVertices.push_back(coordToIndex(c,L));
            c = indexToCoord(v, L);
        }
        if (c.xi[2] < (L-3))
        {
            vertices = {v, neigh(v, xy, -1, L), neigh(v, xyz, 1, L), v + 5*L*L};
            middleQubitIndices.push_back(findFace(vertices, vertexToFaces, faceToVertices));
        }
        if (c.xi[1] < (L-3))
        {
            vertices = {v, neigh(v, xz, -1, L), neigh(v, xyz, 1, L), v + 5*L};
            middleQubitIndices.push_back(findFace(vertices, vertexToFaces, faceToVertices));
        }

        //Logical Z 
        if (c.xi[2] == 1)
        {
            vertices = {v, neigh(v, xy, -1, L), neigh(v, xz, -1, L), v - 1};
            zLogical.push_back(findFace(vertices, vertexToFaces, faceToVertices));
            if (c.xi[1] != 1)
            {
                vertices = {v, neigh(v, xy, -1, L), neigh(v, yz, -1, L), v - 5*L};
                zLogical.push_back(findFace(vertices, vertexToFaces, faceToVertices));
            }
        }

        //Logical X 
        if (c.xi[1] == 1)
        {
            vertices = {v, neigh(v, xy, -1, L), neigh(v, xz, -1, L), v - 1};
            xLogicalFragment1.push_back(findFace(vertices, vertexToFaces, faceToVertices));
            vertices = {v, neigh(v, yz, -1, L), neigh(v, xyz, 1, L), v + 1};
            xLogicalFragment2.push_back(findFace(vertices, vertexToFaces, faceToVertices));
            if (c.xi[2] != 1)
            {
                vertices = {v, neigh(v, xz, -1, L), neigh(v, yz, -1, L), v - 5*L*L};
                xLogicalFragment1.push_back(findFace(vertices, vertexToFaces, faceToVertices));
            }
            if (c.xi[2] != (L-3))
            {
                vertices = {v, neigh(v, xy, -1, L), neigh(v, xyz, 1, L), v + 5*L*L};
                xLogicalFragment2.push_back(findFace(vertices, vertexToFaces, faceToVertices));
            }
        }

        //Adding edges (Z stabilisers)
        //These are just boundary stabilisers
        //Some boundary stabilisers are supported only on lower layer qubits so don't add these
        if (c.xi[1] == (L-3))
        {
           if (c.xi[2] != (L-3))
           {
              syndIndicesZ.push_back(4*v + xyz);
              syndIndicesZ.push_back(4*(v+1) + yz);
              syndIndicesZ.push_back(4*(v+5*L*L) + xy);
              syndIndicesZ.push_back(4*neigh(v,xyz,1,L) + xz);
              //There are relations for these vertices 
              //but they won't show up in the defect adding step
              //because we have ignored one of the stabilisers
              if (c.xi[2] != 1) defectIndices.push_back(v);
           }
        }
        if (c.xi[1] == 1)
        {
            if (c.xi[2] != 1)
            {
                syndIndicesZ.push_back(4*neigh(v,yz,-1,L) + xy);
                syndIndicesZ.push_back(4*neigh(v,yz,-1,L) + yz);
                syndIndicesZ.push_back(4*neigh(v,yz,-1,L) + xyz);
                defectIndices.push_back(neigh(v,yz,-1,L));
            }
            if (c.xi[2] != (L-3)) syndIndicesZ.push_back(4*(v+1) + xz);
        }

        //X stabilisers
        if (c.xi[1] != 1)
        {
            syndIndicesXLayer.push_back((neigh(v,yz,-1,L) + 5*L*L)/2);
            if (c.xi[2] == 1) syndIndicesXLayer.push_back((neigh(v,yz,-1,L) + 1)/2);
        }

        generatorVertices.erase(generatorVertices.begin());
    }
    
    upperQubitIndices = moveFaces(lowerQubitIndices, vertexToFaces, faceToVertices);

    //We actually want Z logical on top face
    zLogical = moveFaces(zLogical, vertexToFaces, faceToVertices);
    
    //These are the lower and middle bulk Z stabilisers
    std::map<int, int> edgeCount;
    std::set<int> upperVertices;
    for (vint qubits : {lowerQubitIndices, middleQubitIndices})
    {
        for (int f : qubits)
        {
            for (int e : faceToEdges[f])
            {
                edgeCount[e] += 1;
            }
        }
    }
    for (int f : upperQubitIndices)
    {
        for (int e : faceToEdges[f])
        {
            edgeCount[e] += 1;
        }
        for (int v : faceToVertices[f])
        {
            upperVertices.insert(v);
        }
    }
    for (std::pair<int, int> p : edgeCount)
    {
        if (p.second == 3) syndIndicesZ.push_back(p.first);
    }
    
    //Adding defect sites
    std::map<int, int> vertexCount;
    for (int e : syndIndicesZ)
    {
        vertexCount[edgeToVertices[e].first] += 1;
        vertexCount[edgeToVertices[e].second] += 1;
    }
    for (std::pair<int, int> p : vertexCount)
    {
        if (p.second == 4 &&
           (indexToCoord(p.first, L).xi[3] == 1 || 
            indexToCoord(p.first, L).xi[1] == L-2 ||
            std::find(upperVertices.begin(),upperVertices.end(),p.first) == upperVertices.end()))
        {
            defectIndices.push_back(p.first);
        }
    }

    //We only need to sweep from some vertices in the decoding step
    //So having vectors of these saves a lot of time later
    std::set<int> bulkSweepVerticesSet;
    std::set<int> middleBoundarySweepVerticesSet;
    std::set<int> upperBoundarySweepVertices1Set;
    std::set<int> upperBoundarySweepVertices2Set;
    for (int face : lowerQubitIndices)
    {
        vertices = faceToVertices[face];
        for (int vertex : vertices)
        {
            coord c = indexToCoord(vertex, L);
            if (c.xi[3] == 0) 
            {
                bulkSweepVerticesSet.insert(vertex);
            }
            else if (((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 1) && c.xi[2] == 1)
            {
                middleBoundarySweepVerticesSet.insert(vertex);
            }
        }
    }
    for (int face : upperQubitIndices)
    {
        vertices = faceToVertices[face];
        for (int vertex : vertices)
        {
            coord c = indexToCoord(vertex, L);
            if (c.xi[3] == 1 && (c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 0)
            {
                bulkSweepVerticesSet.insert(vertex);
                if (c.xi[2] == 1) upperBoundarySweepVertices2Set.insert(vertex);
                else if (c.xi[2] == (L-3)) upperBoundarySweepVertices2Set.insert(vertex);
            }
            else if (c.xi[3] == 0 && c.xi[1] == (L-2))
            {
                upperBoundarySweepVertices1Set.insert(vertex);
            }
        }
    }
    for (int vertex : bulkSweepVerticesSet) bulkSweepVertices.push_back(vertex);
    for (int vertex : middleBoundarySweepVerticesSet) middleBoundarySweepVertices.push_back(vertex);
    for (int vertex : upperBoundarySweepVertices1Set) upperBoundarySweepVertices1.push_back(vertex);
    for (int vertex : upperBoundarySweepVertices2Set) upperBoundarySweepVertices2.push_back(vertex);
}

vint moveFaces(vint &indexVector, vvint &vertexToFaces, vvint &faceToVertices)
{
    vint vertices = {};
    vint newIndexVector = {};
    for (int face : indexVector)
    {
        vertices = faceToVertices[face];
        for (int i = 0; i < vertices.size(); i++) vertices[i] += 2;
        face = findFace(vertices, vertexToFaces, faceToVertices);
        newIndexVector.push_back(face);
    }

    return newIndexVector;
}

void moveIndices(vint &lowerQubitIndices, vint &middleQubitIndices, vint &upperQubitIndices, vint &syndIndices, vint &defectIndices, vint &bulkSweepVertices, vint &middleBoundarySweepVertices, vint &upperBoundarySweepVertices1, vint &upperBoundarySweepVertices2, vint &zLogical, vvint &vertexToFaces, vvint &faceToVertices, int L)
{
    lowerQubitIndices = moveFaces(lowerQubitIndices, vertexToFaces, faceToVertices);
    middleQubitIndices = moveFaces(middleQubitIndices, vertexToFaces, faceToVertices);
    upperQubitIndices = moveFaces(upperQubitIndices, vertexToFaces, faceToVertices);
    for (int &i : syndIndices) i += 8;
    for (int &i : defectIndices) i += 2;
    for (int &i : bulkSweepVertices) i += 2;
    for (int &i : middleBoundarySweepVertices) i += 2;
    for (int &i : upperBoundarySweepVertices1) i += 2;
    for (int &i : upperBoundarySweepVertices2) i += 2;
    zLogical = moveFaces(zLogical, vertexToFaces, faceToVertices);
}

}

// ToDo: Tests!
/*
void testCoordToIndex()
{
    int L = 10;
    coord c = {1, 2, 3, 1};
    assert (coordToIndex(c, L) == 1321);
}

void testIndexToCoord()
{
    int L = 10;
    int v = 752;
    assert (indexToCoord(v, L).xi[0] == 2);
    assert (indexToCoord(v, L).xi[1] == 5);
    assert (indexToCoord(v, L).xi[2] == 7);
    assert (indexToCoord(v, L).xi[3] == 0);
}

void testNeigh()
{
    int v = 0;
    int L = 10;
    assert (neigh(v, xyz, 1, L) == 1000);
    assert (neigh(v, xyz, -1, L) == 1999);
    assert (neigh(v, xy, 1, L) == 1900);
    assert (neigh(v, xy, -1, L) == 1099);
    assert (neigh(v, xz, 1, L) == 1090);
    assert (neigh(v, xz, -1, L) == 1909);
    assert (neigh(v, yz, 1, L) == 1009);
    assert (neigh(v, yz, -1, L) == 1990);
    v = 1000;
    assert (neigh(v, xyz, 1, L) == 111);
    assert (neigh(v, xyz, -1, L) == 0);
    assert (neigh(v, xy, 1, L) == 11);
    assert (neigh(v, xy, -1, L) == 100);
    assert (neigh(v, xz, 1, L) == 101);
    assert (neigh(v, xz, -1, L) == 10);
    assert (neigh(v, yz, 1, L) == 110);
    assert (neigh(v, yz, -1, L) == 1);
}

void testEdgeIndex()
{
    int L = 10;
    int v = 0;
    assert (edgeIndex(v, xy, 1, L) == 0);
    assert (edgeIndex(v, xz, 1, L) == 1);
    assert (edgeIndex(v, yz, 1, L) == 2);
    assert (edgeIndex(v, xyz, 1, L) == 3);
    assert (edgeIndex(v, xy, -1, L) == 4396);
    assert (edgeIndex(v, xz, -1, L) == 7637);
    assert (edgeIndex(v, yz, -1, L) == 7962);
    assert (edgeIndex(v, xyz, -1, L) == 7999);
}

void testAddFace()
{
    int L = 10;
    int v = 111;
    int f = 69;
    vvint faceToVertices;
    vvint faceToEdges;
    vvint vertexToFaces;
    vvint edgeToFaces;
    faceToVertices.assign(6 * L * L * L, {});
    faceToEdges.assign(6 * L * L * L, {});
    vertexToFaces.assign(2 * L * L * L, {});
    edgeToFaces.assign(8 * L * L * L, {});
    addFace(v, f, {xyz, xy, xy, xyz}, {1, 1, 1, 1}, faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, L);
    assert (faceToVertices[f][0] == v);
    assert (faceToVertices[f][1] == 122);
    assert (faceToVertices[f][2] == 1011);
    assert (faceToVertices[f][3] == 1111);
    assert (faceToEdges[f][0] == 444);
    assert (faceToEdges[f][1] == 447);
    assert (faceToEdges[f][2] == 4047);
    assert (faceToEdges[f][3] == 4444);
    assert (vertexToFaces[v][0] == f);
    assert (vertexToFaces[122][0] == f);
    assert (vertexToFaces[1011][0] == f);
    assert (vertexToFaces[1111][0] == f);
    assert (edgeToFaces[444][0] == f);
    assert (edgeToFaces[447][0] == f);
    assert (edgeToFaces[4047][0] == f);
    assert (edgeToFaces[4444][0] == f);
}

void testBuildFaces()
{
    int L = 10;
    vvint faceToVertices;
    vvint faceToEdges;
    vvint vertexToFaces;
    vvint edgeToFaces;
    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, L);
    int f = 3; // (xy, -xz, -xz, xy) face of vertex (0,0,0,0) should be fourth face to be added
    assert (faceToVertices[f][0] == 0); 
    assert (faceToVertices[f][1] == 910);
    assert (faceToVertices[f][2] == 1900);
    assert (faceToVertices[f][3] == 1909);
    assert (faceToEdges[f][0] == 0);
    assert (faceToEdges[f][1] == 3641);
    assert (faceToEdges[f][2] == 7636);
    assert (faceToEdges[f][3] == 7637);
    assert (vertexToFaces[0][3] == f);
    assert (vertexToFaces[910][0] == f);
    assert (vertexToFaces[1900][1] == f);
    assert (vertexToFaces[1909][0] == f);
    assert (edgeToFaces[0][1] == f);
    assert (edgeToFaces[3641][0] == f);
    assert (edgeToFaces[7636][0] == f);
    assert (edgeToFaces[7637][0] == f);
}

void testBuildVertexToEdges()
{
    int L = 10;
    vvint vertexToEdges = buildVertexToEdges(L);
    int v = 345; // (5,4,3,0) should have all 8 edges
    assert (vertexToEdges[v][0] == 1380);
    assert (vertexToEdges[v][1] == 1381);
    assert (vertexToEdges[v][2] == 1382);
    assert (vertexToEdges[v][3] == 1383);
    assert (vertexToEdges[v][4] == 4939);
    assert (vertexToEdges[v][5] == 4942);
    assert (vertexToEdges[v][6] == 4977);
    assert (vertexToEdges[v][7] == 5336);
    assert (vertexToEdges[v].size() == 8);
    v = 1222; // (2,2,2,1) should have 4 edges (type 1)
    assert (vertexToEdges[v][0] == 891);
    assert (vertexToEdges[v][1] == 4888);
    assert (vertexToEdges[v][2] == 4889);
    assert (vertexToEdges[v][3] == 4890);
    assert (vertexToEdges[v].size() == 4);
    v = 1111; // (1,1,1,1) should have 4 edges (type 2)
    assert (vertexToEdges[v][0] == 450);
    assert (vertexToEdges[v][1] == 485);
    assert (vertexToEdges[v][2] == 844);
    assert (vertexToEdges[v][3] == 4447);
    assert (vertexToEdges[v].size() == 4);
    v = 788; // (8,8,7,0) should have no edges
    assert (vertexToEdges[v].size() == 0);
}

void testBuildEdgeToVertices()
{
    int L = 10;
    vpint edgeToVertices = buildEdgeToVertices(L);
    int e = 888 + 3; // xyz edge of (2,2,2,0)
    assert (edgeToVertices[e].first == 222);
    assert (edgeToVertices[e].second == 1222);
    e = 884; // xy edge of (1,2,2,0) [shouldn't exist]
    assert (edgeToVertices[e].first == 0); // Empty pair get initalized to {0,0}
    assert (edgeToVertices[e].second == 0);
    e = 4447; // xyz edge of (1,1,1,1) 
    assert (edgeToVertices[e].first == 222);
    assert (edgeToVertices[e].second == 1111);
    e = 4445; // xz edge of (1,1,1,1) [shouldn't exist]
    assert (edgeToVertices[e].first == 0);
    assert (edgeToVertices[e].second == 0);
    e = 4933; // xz edge of (3,3,2,1)
    assert (edgeToVertices[e].first == 334);
    assert (edgeToVertices[e].second == 1233);
    e = 4935; // xz edge of (3,3,2,1) [shouldn't exist]
    assert (edgeToVertices[e].first == 0);
    assert (edgeToVertices[e].second == 0);
}

void testFindFace()
{
    int L = 10;
    vvint faceToVertices;
    vvint faceToEdges;
    vvint vertexToFaces;
    vvint edgeToFaces;
    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, L);
    vint vertices = {0, 910, 1900, 1909}; // (xy, -xz, -xz, xy) face of vertex (0,0,0,0) should be fourth face to be added
    assert (findFace(vertices, vertexToFaces, faceToVertices) == 3);
}

void testBuildLogicals()
{
    int L = 4;
    vvint faceToVertices;
    vvint faceToEdges;
    vvint vertexToFaces;
    vvint edgeToFaces;
    buildFaces(faceToVertices, faceToEdges, vertexToFaces, edgeToFaces, L);
    auto logicals = buildLogicals(L, vertexToFaces, faceToVertices);
    vint expectedZ1 = {151, 4, 145, 10};
    assert (logicals.at(0) == expectedZ1);
    vint expectedZ2 = {180, 3, 156, 27};
    // std::cout << logicals[1][0] << " ";
    // std::cout << logicals[1][1] << " ";
    // std::cout << logicals[1][2] << " ";
    // std::cout << logicals[1][3] << " " << std::endl;
    assert (logicals.at(1) == expectedZ2);
    vint expectedZ3 = {151, 58, 55, 154};
    // std::cout << logicals[2][0] << " ";
    // std::cout << logicals[2][1] << " ";
    // std::cout << logicals[2][2] << " ";
    // std::cout << logicals[2][3] << " " << std::endl;
    assert (logicals.at(2) == expectedZ3);
}



int main()
{
    testCoordToIndex();
    testIndexToCoord();
    testNeigh();
    testEdgeIndex();
    testAddFace();
    testBuildFaces();
    testBuildVertexToEdges();
    testBuildEdgeToVertices();
    testFindFace();
    testBuildLogicals();
}
*/
