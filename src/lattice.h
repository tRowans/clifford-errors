#ifndef LATTICE_H
#define LATTICE_H

#include <vector>
#include <string>
#include <set>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <map>

typedef std::vector<int> vint;
typedef std::vector<std::vector<int>> vvint;
typedef std::pair<int,int> pint;
typedef std::vector<std::pair<int, int>> vpint;
typedef std::pair<std::pair<int,int>,std::pair<int,int>> ppint;

class Lattice
{
    public:
        vvint faceToVertices;   
        vvint vertexToFaces;    //This one not used in cubic lattice
        vvint faceToEdges;
        vvint edgeToFaces;
        vpint faceToCells;
        vvint cellToFaces;
        vvint vertexToEdges;
        vpint edgeToVertices;

        vint outerQubitIndices; //2D code
        vint innerQubitIndices; //3D code minus 2D code
        vint xSyndIndices;
        vint zSyndIndices;
        vint defectIndices;
        vint xLogical;
        vint zLogical; 

        vint qubitsX(3*L*L*L, 0);
        vint qubitsZ(3*L*L*L, 0);
        vint syndromeX(L*L*L, 0);   //X stabiliser syndromes
        vint syndromeZ(8*L*L*L, 0); //This is bigger than cubic needs but doesn't matter
        vint defects = {};

        void depolarisingError(double p, std::mt19937& engine, 
                                std::uniform_real_distribution<double>& dist);
        void biasedError(double p, std::mt19937& engine, 
                                std::uniform_real_distribution<double>& dist, 
                                char pauli, int innerOnly);
        void measError(double q, std::mt19937& engine, 
                                std::uniform_real_distribution<double>& dist, char pauli);
        void applyZStab(int edge);
        void zStabPattern(std::mt19937& engine, 
                            std::uniform_real_distribution<double>& dist)
        void calcSynd(char pauli, int useOuter, int useInner);
        void findDefects();
        void checkInBounds();
        void checkInCodespace(char pauli, int useOuter, int useInner);
        void checkJumpCorrection();
        int checkLogicalError(char pauli);
        void wipe(); //clears error and syndrome info
};

#endif
