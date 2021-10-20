#include "latticeGeneric.h"

int findFace(vint vertices, vvint &vertexToFaces, vvint &faceToVertices)
{
    //Finds faces in the rhombic lattices
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
    return -1; 
}

void buildOverlappingFaces(std::map<pint,std::pair<pint,pint>> &overlappingFaces, vvint &faceToEdgesT, vpint &edgeToVerticesT, vvint &vertexToFacesR1, vvint &faceToVerticesR1, vvint &vertexToFacesR2, vvint &faceToVerticesR2, int L) 
{
    int xPlus, yPlus, zPlus;
    vint fxy, fxz, fyz, fR1, fR2;
    vvint fTrio;
    int parity, fIndexT, fIndexR1, fIndexR2;
    
    //For all xy-planes of vertices in cube except for top one we add all faces in plane and also
    //all faces which stick up out of the plane in the +z direction
    for (int v = 2*L*L*L; v < 3*L*L*L; v++)
    {
        xPlus = ((v + 1) % L) - (v % L);            //+1 unless (v+1)%L=0, and then -(L-1)
        yPlus = (((v/L + 1) % L) - ((v/L) % L))*L;  //Same but for +1 in y direction
        zPlus = L*L;                                //Cube doesnt touch top Z boundary in toric

        //These are faces in the toric lattice
        fxy = {v, v + xPlus, v + yPlus, v + xPlus + yPlus};
        fxz = {v, v + xPlus, v + zPlus, v + xPlus + zPlus};
        fyz = {v, v + yPlus, v + zPlus, v + yPlus + zPlus};
        fTrio = {fxy, fxz, fyz};
        parity = ((v % L) + ((v/L) % L) + ((v/(L*L)) % (5*L))) % 2;
        for (int i = 0; i < 3; i++)
        {
            if (v >= (3*L*L*L - L*L) && i != 0) continue;   //don't add z direction faces in top layer
            fIndexT = 3*v + i;
            if (parity == 0)
            {
                fR1 = {latticeSwitchVertex(fTrio[i][0], 0, 1, L),
                        latticeSwitchVertex(fTrio[i][3], 0, 1, L)};
                fR2 = {latticeSwitchVertex(fTrio[i][1], 0, 2, L),
                        latticeSwitchVertex(fTrio[i][2], 0, 2, L)};
            }
            else
            {   
                fR1 = {latticeSwitchVertex(fTrio[i][1], 0, 1, L), 
                        latticeSwitchVertex(fTrio[i][2], 0, 1, L)};
                fR2 = {latticeSwitchVertex(fTrio[i][0], 0, 2, L), 
                        latticeSwitchVertex(fTrio[i][3], 0, 2, L)};
            }
            fIndexR1 = findFace(fR1, vertexToFacesR1, faceToVerticesR1);
            fIndexR2 = findFace(fR2, vertexToFacesR2, faceToVerticesR2);
            if (fIndexR1 == -1 || fIndexR2 == -1) continue; 
            overlappingFaces[{0,fIndexT}] = {{1,fIndexR1}, {2,fIndexR2}};
            overlappingFaces[{1,fIndexR1}] = {{0,fIndexT}, {2,fIndexR2}};
            overlappingFaces[{2,fIndexR2}] = {{0,fIndexT}, {1,fIndexR1}};
        }
    }
}

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

