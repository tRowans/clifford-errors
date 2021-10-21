#include "toricCodeSlice.h"

namespace toric {

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

vvint buildFaceToEdges(int L)
{
    vvint faceToEdges;
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
    return faceToEdges;
}

vvint buildEdgeToFaces(int L)
{
    vvint edgeToFaces;
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
    return edgeToFaces;
}

vvint buildVertexToEdges(int L)
{
    vvint vertexToEdges;
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
    return vertexToEdges;
}

vpint buildEdgeToVertices(int L)
{
    vpint edgeToVertices;
    for (int e = 0; e < 3 * L * L * L; ++e)
    {
        int v1 = e / 3;
        int dir = e % 3;
        int v2 = neigh(v1, dir, 1, L);
        edgeToVertices.push_back({v1, v2});
    }
    return edgeToVertices;
}

vvint buildCellToFaces(int L)
{
    vvint cellToFaces;
    for (int v = 0; v < L * L * L; v++)
    {
        cellToFaces.push_back({3*v, 
                               3*v+1, 
                               3*v+2, 
                               3*neigh(v,x,1,L)+2, 
                               3*neigh(v,y,1,L)+1, 
                               3*neigh(v,z,1,L)});
    }
    return cellToFaces;
}

vpint buildFaceToCells(int L)
{
    vpint faceToCells;
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
    return faceToCells;
}

vvint buildFaceToVertices(int L)
{
    vvint faceToVertices;
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
    return faceToVertices;
}

vint buildQubitIndices(int L)
{
    vint qubitIndices;
    for (int f = 0; f < 3 * L * L * L; f++)
    {
        int v = f / 3;
        int dir = f % 3;
        coord cd = indexToCoord(v, L);
        if (cd.xi[0] < (L-3) && cd.xi[1] < (L-3) && cd.xi[2] < (L-3))
        {
            if (dir == 0 && cd.xi[0] < (L-4) && cd.xi[2] > 0) qubitIndices.push_back(f);
            else if (dir == 1 && cd.xi[0] < (L-4) && cd.xi[1] > 0) qubitIndices.push_back(f);
            else if (dir == 2) qubitIndices.push_back(f);
        }
    }
    return qubitIndices;
}

vint buildXSyndIndices(int L)
{
    vint xSyndIndices;
    for (int v = 0; v < L * L * L; v++)
    {
        coord cd = indexToCoord(v, L);
        if (cd.xi[0] < (L-4) && cd.xi[1] < (L-3) && cd.xi[2] < (L-3))
        {
            xSyndIndices.push_back(v);
        }
    }
    return xSyndIndices;
}

vint buildZSyndIndices(int L)
{
    vint zSyndIndices;
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
    return zSyndIndices;
}

vint buildXLogical(int L)
{
    vint xLogical;
    for (int z; z < (L-3); z++)
    {
        for (int y; y < (L-3); y++)
        {
            int f = 3*(y*L + z*L*L) + 2;
            xLogical.push_back(f);
        }
    }
    return xLogical;
}

vint buildZLogical(int L)
{
    vint zLogical;
    for (int x; x < (L-3); x++)
    {
        f = 3*x + 2;
        zLogical.push_back(f);
    }
    return zLogical;
}

}
