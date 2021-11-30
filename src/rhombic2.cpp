#include "rhombic2.h"

namespace rhombic {

namespace r2 {

void buildFaces(Lattice &lattice, int L)
{
    lattice.faceToVertices.assign(3 * L * L * L, {});
    lattice.faceToEdges.assign(3 * L * L * L, {});
    lattice.faceToCells.assign(3 * L * L * L, {});
    lattice.vertexToFaces.assign(2 * L * L * L, {});
    lattice.edgeToFaces.assign(8 * L * L * L, {});
    int f = 0;
    // Loop through all w=0 vertices coordinates
    for (int v = 0; v < L * L * L; ++v) 
    {
        coord c = indexToCoord(v, L);
        // Only half of w=0 coordinates have vertices
        if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 0) continue;
        vint signs = {1, 1, 1, 1};
        vint signs2 = {1, 1};
        // Each face containing w=0 vertex is in two such vertices, so we add 6/12 per vertex to avoid double counting
        addFace(v, f, {xyz, yz, yz, xyz}, {1, 2}, signs, signs2, lattice, L);
        ++f;
        addFace(v, f, {xyz, xz, xz, xyz}, {0, 2}, signs, signs2, lattice, L);
        ++f;
        addFace(v, f, {xyz, xy, xy, xyz}, {0, 1}, signs, signs2, lattice, L);
        ++f;
        signs = {1, -1, -1, 1};
        signs2 = {1, -1};
        addFace(v, f, {xy, xz, xz, xy}, {1, 2}, signs, signs2, lattice, L);
        ++f;
        addFace(v, f, {xy, yz, yz, xy}, {0, 2}, signs, signs2, lattice, L);
        ++f;
        addFace(v, f, {xz, yz, yz, xz}, {0, 1}, signs, signs2, lattice, L);
        ++f;
    }
}

int faceToBaseVertex(int face, int L)
{
    //Each w=0 vertex has six associated faces but the indexes don't always match 
    //This takes a face and returns the index of its associated vertex
    int v = face / 3;
    int dir = face % 6;
    coord cd = indexToCoord(v, L);
    if ((cd.xi[0] + cd.xi[1] + cd.xi[2]) % 2 == 0)
    {
        if (dir < 3) v += 1;
        else v -= 1;
    }
    return v;
}

void buildVertexToEdges(vvint &vertexToEdges, int L)
{
    vvint vertexToEdges;
    vertexToEdges.assign(2 * L * L * L, {});
    for (int v = 0; v < 2 * L * L * L; ++v)
    {
        vint edges;
        coord c = indexToCoord(v, L);
        if (c.xi[3] == 0)
        {
            if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 1)
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
        if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 1)
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

void buildEdgeToVertices(vpint &edgeToVertices, int L)
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
            if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 0) continue;
        }
        else
        {
            if ((c.xi[0] + c.xi[1] + c.xi[2]) % 2 == 1)
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
    //One cell for each even vertex in the w=0 lattice
    //I do these separately and not as part of buildFaces because I want them to be in a specific order
    //this is useful later when making paths through faces on dual lattice for Z error decoding

    cellToFaces.assign(L * L * L, {});
    for (int v = 0; v < L * L * L; v++)
    {
        coord c = indexToCoord(v, L);
        if ((cd.xi[0] + cd.xi[1] + cd.xi[2]) % 2 == 1) continue;
        vint faces;

        int xPlus = neighXYZ(v,0,1,L);
        int xMinus = neighXYZ(v,0,-1,L);
        int yPlus = neighXYZ(v,1,1,L);
        int yMinus = neighXYZ(v,1,-1,L);
        int zPlus = neighXYZ(v,2,1,L);
        int zMinus = neighXYZ(v,2,-1,L);
        
        faces.push_back(findFace({xPlus,yPlus}, vertexToFaces, faceToVertices));   //0
        faces.push_back(findFace({xPlus,yMinus}, vertexToFaces, faceToVertices));  //1
        faces.push_back(findFace({xMinus,yPlus}, vertexToFaces, faceToVertices));  //2
        faces.push_back(findFace({xMinus,yMinus}, vertexToFaces, faceToVertices)); //3 
        faces.push_back(findFace({xPlus,zPlus}, vertexToFaces, faceToVertices));   //4
        faces.push_back(findFace({xMinus,zPlus}, vertexToFaces, faceToVertices));  //5
        faces.push_back(findFace({yPlus,zPlus}, vertexToFaces, faceToVertices));   //6
        faces.push_back(findFace({yMinus,zPlus}, vertexToFaces, faceToVertices));  //7
        faces.push_back(findFace({xPlus,zMinus}, vertexToFaces, faceToVertices));  //8
        faces.push_back(findFace({xMinus,zMinus}, vertexToFaces, faceToVertices)); //9
        faces.push_back(findFace({yPlus,zMinus}, vertexToFaces, faceToVertices));  //10
        faces.push_back(findFace({yMinus,zMinus}, vertexToFaces, faceToVertices)); //11

        cellToFaces[v] = faces;
    }
}

void buildQubitIndices(Lattice &lattice, int L)
{
    for (int f = 0; f < 3 * L * L * L; f++)
    {
        int v = faceToBaseVertex(f, L);
        int dir = f % 6;
        coord cd = indexToCoord(v, L);

        if (cd.xi[0] < (L-3) && cd.xi[1] < (L-2) && cd.xi[2] < (L-2))
        {
            if (dir == 0 && cd.xi[1] < (L-3) && cd.xi[2] < (L-3)) 
            {
                if (cd.xi[0] == 0) lattice.outerQubitIndices.push_back(f);
                else lattice.innerQubitIndices.push_back(f);
            }
            else if (dir == 1 && cd.xi[0] < (L-4)
                              && cd.xi[1] > 0 && cd.xi[1] < (L-3)
                              && cd.xi[2] < (L-3)) lattice.innerQubitIndices.push_back(f);
            else if (dir == 2 && cd.xi[0] < (L-4)
                              && cd.xi[1] < (L-3) && cd.xi[2] > 0 
                              && cd.xi[2] < (L-3)) lattice.innerQubitIndices.push_back(f);
            else if (dir == 3 && cd.xi[1] < (L-3) && cd.xi[2] > 0) 
            {
                if (cd.xi[0] == 0) lattice.outerQubitIndices.push_back(f);
                else lattice.innerQubitIndices.push_back(f);
            }
            else if (dir == 4 && cd.xi[0] < (L-4)
                              && cd.xi[1] > 0 && cd.xi[1] < (L-3)
                              && cd.xi[2] > 0) lattice.innerQubitIndices.push_back(f);
            else if (dir == 5 && cd.xi[0] < (L-4)
                              && cd.xi[1] > 0 && cd.xi[2] > 0 
                              && cd.xi[2] < (L-3)) lattice.innerQubitIndices.push_back(f);
        }
    }
}

// X boundary stabilisers on +y and -y boundaries

void buildXSyndIndices(vint &xSyndIndices, int L)
{
    vint xSyndIndices;
    for (int v = 0; v < L * L * L; v++)
    {
        coord cd = indexToCoord(v, L);
        if ((cd.xi[0] + cd.xi[1] + cd.xi[2]) % 2 == 0)
        {
            if (cd.xi[0] < (L-3)
                && cd.xi[1] < (L-2)
                && cd.xi[2] >  0 && cd.xi < (L-3)) xSyndIndices.push_back(v);
        }
    }
    return xSyndIndices;
}

//Z boundary stabilisers on +z and -z boundaries

void buildZSyndIndices(vint &zSyndIndices, int L)
{
    vint zSyndIndices;
    for (int v = 0; v < L * L * L; v++)
    {
        coord cd = indexToCoord(v, L);
        if ((cd.xi[0] + cd.xi[1] + cd.xi[2]) % 2 == 0) continue;
        if (cd.xi[0] > (L-4) || cd.xi[1] == 0 || cd.xi[1] > (L-4) || cd.xi[2] > (L-3)) continue;
        vint dirs;
        dirs.assign(8,1); //xy, xz, yz, xyz, -xy, -xz, -yz, -xyz
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
        if (cd.xi[2] == 0)
        {
            dirs[0] = 0;
            dirs[5] = 0;
            dirs[6] = 0;
            dirs[7] = 0;
        }
        else if (cd.xi[2] == (L-3))
        {
            dirs[1] = 0;
            dirs[2] = 0;
            dirs[3] = 0;
            dirs[4] = 0;
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

void buildLogicals(Lattice &lattice, int L)
{
    //Do these together because the loops are the same
    for (vint &qubitIndices : {lattice.outerQubitIndices, lattice.innerQubitIndices})
    {
        for (int q : qubitIndices)
        {
            int v = faceToBaseVertex(q, L);
            coord cd = indexToCoord(v, L);
            if (cd.xi[2] == 0 && cd.xi[3] == 0) lattice.xLogical.push_back(q);
            if (cd.xi[0] == 0 && cd.xi[1] == 0 && cd.xi[3] == 0)lattice.zLogical.push_back(q);
        }
    }
}

void buildLattice(Lattice &lattice, int L)
{
    buildFaces(lattice, L);
    buildVertexToEdges(lattice.vertexToEdges, L);
    buildEdgeToVertices(lattice.edgeToVertices, L);
    buildCellToFaces(lattice.cellToFaces, lattice.vertexToFaces, lattice.faceToVertices, L);
    buildQubitIndices(lattice, L);
    buildXSyndIndices(lattice.xSyndIndices, L);
    buildZSyndIndices(lattice.zSyndIndices, L);
    buildLogicals(lattice, L);
}

}

}
