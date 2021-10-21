#include "latticeGeneric.h"

int findFace(vint &vertices, vvint &vertexToFaces, vvint &faceToVertices)
{
    //finds face in rhombic lattice
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

std::map<pint,std::pair<pint,pint>> buildOverlappingFaces(Lattice &cubic, Lattice &rhombic1, Lattice &rhombic2, int L)
{
    std::map<pint,std::pair<pint,pint>> overlappingFaces;
    int fC, fR1, fR2
    for (int v = 0; v < L*L*L; v++)
    {
        for (int i = 0; i < 3; i++)
        fC = 3*v + i;
        vint verts = cubic.faceToVertices[fC];
        fR1 = findFace({verts[0],verts[3]}, rhombic1.vertexToFaces, rhombic1.faceToVertices);
        if (fR1 == -1)
        {
            fR1 = findFace({verts[1],verts[2]}, rhombic1.vertexToFaces, rhombic1.faceToVertices);
            fR2 = findFace({verts[0],verts[3]}, rhombic2.vertexToFaces, rhombic2.faceToVertices);
        }
        else fR2 = findFace({verts[1],verts[2]}, rhombic2.vertexToFaces, rhombic2.faceToVertices);
        overlappingFaces[{0,fC}] = {{1,fR1}, {2,fR2}};
        overlappingFaces[{1,fR1}] = {{0,fC}, {2,fR2}};
        overlappingFaces[{2,fR2}] = {{0,fC}, {1,fR1}};
    }
    return overlappingFaces;
}

//Do I need this? See how it goes. 
void randomiseStabs(vvint &xStabs, std::mt19937 &engine, std::uniform_real_distribution<double> &dist)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < xStabs[i].size(); j++)
        {
            if (dist(engine) < 0.5)
            {
                xStabs[i][j] = 1;
            }
        }
    }
}

