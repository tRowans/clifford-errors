#include "cubic.h"

namespace cubic {

int coordToIndex(coord c, int L)
{
    if (0 <= c.xi[0] && c.xi[0] < L &&  //could actually make this L-2 but maybe not worth it
        0 <= c.xi[1] && c.xi[1] < L && 
        0 <= c.xi[2] && c.xi[2] < L)
    {
        return (c.xi[0] + c.xi[1] * L + c.xi[2] * L * L);
    }
    else
    {
        throw std::invalid_argument("Coord out of range");
    }
}

coord indexToCoord(int i, int L)
{
    if (0 <= i && i < L*L*L)
    {
        coord c;
        c.xi[0] = i % L;
        c.xi[1] = (int)floor(i / L) % L;
        c.xi[2] = (int)floor(i / (L * L)) % L;
        return c;
    }
    else
    {
        throw std::invalid_argument("Index out of range");
    }
}

int neigh(int v, int dir, int sign, int L)
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
    if (dir == x)
        return 3 * v;
    else if (dir == y)
        return 3 * v + 1;
    else if (dir == z)
        return 3 * v + 2;
    else
        throw std::invalid_argument("Invalid direction");
}

void buildFaceToEdges(vvint &faceToEdges, int L)
{
    for (int f = 0; f < 3 * L * L * L; ++f)
    {
        int v = f / 3;
        int dir = f % 3;
        if (dir == 0)
        {
            faceToEdges.push_back({edgeIndex(v, x, 1, L), edgeIndex(v, y, 1, L), edgeIndex(neigh(v, x, 1, L), y, 1, L), edgeIndex(neigh(v, y, 1, L), x, 1, L)});
        }
        else if (dir == 1)
        {
            faceToEdges.push_back({edgeIndex(v, x, 1, L), edgeIndex(v, z, 1, L), edgeIndex(neigh(v, x, 1, L), z, 1, L), edgeIndex(neigh(v, z, 1, L), x, 1, L)});
        }
        else if (dir == 2)
        {
            faceToEdges.push_back({edgeIndex(v, y, 1, L), edgeIndex(v, z, 1, L), edgeIndex(neigh(v, y, 1, L), z, 1, L), edgeIndex(neigh(v, z, 1, L), y, 1, L)});
        }
    }
}

void buildEdgeToFaces(vvint &edgeToFaces, int L)
{
    for (int e = 0; e < 3 * L * L * L; ++e)
    {
        vint faces;
        int v = e / 3;
        int dir = e % 3;

        if (dir == x)
        {
            faces.push_back(3 * v); // xy
            faces.push_back(3 * v + 1); // xz
            faces.push_back(3 * neigh(v, y, -1, L)); // x,-y
            faces.push_back(3 * neigh(v, z, -1, L) + 1); // x,-z
        }
        else if (dir == y)
        {
            faces.push_back(3 * v); // xy
            faces.push_back(3 * v + 2); // yz
            faces.push_back(3 * neigh(v, x, -1, L)); // -x,y
            faces.push_back(3 * neigh(v, z, -1, L) + 2); // y,-z
        }
        else if (dir == z)
        {
            faces.push_back(3 * v + 2); // yz
            faces.push_back(3 * v + 1); // xz
            faces.push_back(3 * neigh(v, y, -1, L) + 2); // -y,z
            faces.push_back(3 * neigh(v, x, -1, L) + 1); // -x,z
        }

        edgeToFaces.push_back(faces);
    }
}

void buildVertexToEdges(vvint &vertexToEdges, int L)
{
    for (int v = 0; v < L * L * L; ++v)
    {
        vint edges;
        edges.push_back(edgeIndex(v, x, 1, L));
        edges.push_back(edgeIndex(v, y, 1, L));
        edges.push_back(edgeIndex(v, z, 1, L));
        edges.push_back(edgeIndex(v, x, -1, L));
        edges.push_back(edgeIndex(v, y, -1, L));
        edges.push_back(edgeIndex(v, z, -1, L));
        vertexToEdges.push_back(edges);
    }
}

void buildEdgeToVertices(vpint &edgeToVertices, int L)
{
    for (int e = 0; e < 3 * L * L * L; ++e)
    {
        int v1 = e / 3;
        int dir = e % 3;
        int v2 = neigh(v1, dir, 1, L);
        edgeToVertices.push_back({v1, v2});
    }
}

void buildCellToFaces(vvint &cellToFaces, int L)
{
    for (int v = 0; v < L * L * L; v++)
    {
        cellToFaces.push_back({3*v, 
                               3*v+1, 
                               3*v+2, 
                               3*neigh(v,z,1,L),
                               3*neigh(v,y,1,L)+1,
                               3*neigh(v,x,1,L)+2});
    }
}

void buildFaceToCells(vpint &faceToCells, int L)
{
    for (int f = 0; f < 3 * L * L * L; f++)
    {
        std::pair<int,int> cells;
        int cell1 = f / 3;
        int cell2;
        int dir = f % 3;
        if (dir == 0) cell2 = neigh(cell1, z, -1, L);
        else if (dir == 1) cell2 = neigh(cell1, y, -1, L);
        else if (dir == 2) cell2 = neigh(cell1, x, -1, L);
        if (cell1 < cell2) cells = {cell1, cell2};
        else cells = {cell2, cell1};
        faceToCells.push_back(cells);
    }
}

void buildFaceToVertices(vvint &faceToVertices, int L)
{
    for (int f = 0; f < 3 * L * L * L; f++)
    {
        v = f / 3;
        dir = f % 3;
        vint vertices;
        if (dir == 0)
        {
            vertices = {v, neigh(v,x,1,L), neigh(v,y,1,L), neigh(neigh(v,x,1,L),y,1,L)};
        }
        else if (dir == 1)
        {
            vertices = {v, neigh(v,x,1,L), neigh(v,z,1,L), neigh(neigh(v,x,1,L),z,1,L)};
        }
        else 
        {
            vertices = {v, neigh(v,y,1,L), neigh(v,z,1,L), neigh(neigh(v,y,1,L),z,1,L)};
        }
    }
}

void buildQubitIndices(vint &outerQubitIndices, vint &innerQubitIndices, int L)
{
    for (int f = 0; f < 3 * L * L * L; f++)
    {
        int v = f / 3;
        int dir = f % 3;
        coord cd = indexToCoord(v, L);
        if (cd.xi[0] < (L-3) && cd.xi[1] < (L-3) && cd.xi[2] < (L-3))
        {
            if (dir == 0 && cd.xi[0] < (L-4) && cd.xi[2] > 0) innerQubitIndices.push_back(f);
            else if (dir == 1 && cd.xi[0] < (L-4) && cd.xi[1] > 0) 
            {
                if (cd.xi[2] == 0) outerQubitIndices.push_back(f);
                else innerQubitIndices.push_back(f);
            }
            else if (dir == 2)
            {
               if (cd.xi[2] == 0) outerQubitIndices.push_back(f);
               else innerQubitIndices.push_back(f);
            }
        }
    }
}

void buildXSyndIndices(vint &xSyndIndices, int L)
{
    for (int v = 0; v < L * L * L; v++)
    {
        coord cd = indexToCoord(v, L);
        if (cd.xi[0] < (L-4) && cd.xi[1] < (L-3) && cd.xi[2] < (L-3))
        {
            xSyndIndices.push_back(v);
        }
    }
}

void buildZSyndIndices(vint &zSyndIndices, int L)
{
    for (int e = 0; e < 3 * L * L * L; e++)
    {
        int v = e / 3;
        int dir = e % 3;
        coord cd = indexToCoord(v, L);

        if (cd.xi[0] < (L-3) && cd.xi[1] < (L-3) && cd.xi[2] < (L-3))
        {
            if (dir == 0 && cd.xi[0] < (L-4) 
                         && cd.xi[1] > 0
                         && cd.xi[2] > 0) zSyndIndices.push_back(e);
            else if (dir == 1 && cd.xi[2] > 0) zSyndIndices.push_back(e);
            else if (dir == 2 && cd.xi[1] > 0) zSyndIndices.push_back(e);
        }
    }
}

void buildXLogical(vint &xLogical, int L)
{
    for (int z; z < (L-3); z++)
    {
        for (int y; y < (L-3); y++)
        {
            int f = 3*(y*L + z*L*L) + 2;
            xLogical.push_back(f);
        }
    }
}

void buildZLogical(vint &zLogical, int L)
{
    for (int x; x < (L-3); x++)
    {
        f = 3*x + 2;
        zLogical.push_back(f);
    }
}

void buildLattice(Lattice &lattice, int L)
{
    buildFaceToEdges(lattice.faceToEdges, L);
    buildEdgeToFaces(lattice.edgeToFaces, L);
    buildVertexToEdges(lattice.vertexToEdges, L);
    buildEdgeToVertices(lattice.edgeToVertices, L);
    buildCellToFaces(lattice.cellToFaces, L);
    buildFaceToCells(lattice.faceToCells, L);
    buildFaceToVertices(lattice.faceToVertices, L);
    buildQubitIndices(lattice.outerQubitIndices, lattice.innerQubitIndices, L);
    buildXSyndIndices(lattice.xSyndIndices, L);
    buildZSyndIndices(lattice.zSyndIndices, L);
    buildXLogical(lattice.xLogical, L);
    buildZLogical(lattice.zLogical, L);
}
