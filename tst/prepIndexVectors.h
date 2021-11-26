#ifndef PREPINDEXVECTORSALL_H
#define PREPINDEXVECTORSALL_H

inline vvint faceToEdgesT; 
inline vvint edgeToFacesT;
inline vvint vertexToEdgesT;
inline vpint edgeToVerticesT;
inline vvint cellToFacesT;
inline vpint faceToCellsT;

inline vint lowerQubitIndicesT;
inline vint middleQubitIndicesT;
inline vint upperQubitIndicesT;
inline vint syndIndicesZT;
inline vint syndIndicesXT;
inline vint syndIndicesXLayerT;
inline vint defectIndicesT;
inline vint zLogicalT;
inline vint xLogicalFragmentT;
inline vint sweepVerticesT;

inline vvint vertexToEdgesR1; 
inline vpint edgeToVerticesR1;
inline vvint faceToVerticesR1;
inline vvint faceToEdgesR1;
inline vvint vertexToFacesR1;
inline vvint edgeToFacesR1;
inline vvint cellToFacesR1; 
inline vpint faceToCellsR1;

inline vint lowerQubitIndicesR1;
inline vint middleQubitIndicesR1;
inline vint upperQubitIndicesR1;
inline vint syndIndicesZR1;
inline vint syndIndicesXR1;
inline vint syndIndicesXLayerR1;
inline vint defectIndicesR1;
inline vint bulkSweepVerticesR1;
inline vint middleBoundarySweepVerticesR1;
inline vint upperBoundarySweepVertices1R1;
inline vint upperBoundarySweepVertices2R1;
inline vint zLogicalR1;
inline vint xLogicalFragment1R1;
inline vint xLogicalFragment2R1;

inline vvint vertexToEdgesR2; 
inline vpint edgeToVerticesR2;
inline vvint faceToVerticesR2;
inline vvint faceToEdgesR2;
inline vvint vertexToFacesR2;
inline vvint edgeToFacesR2;
inline vvint cellToFacesR2; 
inline vpint faceToCellsR2;

inline vint lowerQubitIndicesR2;
inline vint middleQubitIndicesR2;
inline vint upperQubitIndicesR2;
inline vint syndIndicesZR2;
inline vint syndIndicesXR2;
inline vint syndIndicesXLayerR2;
inline vint defectIndicesR2;
inline vint bulkSweepVerticesR2;
inline vint middleBoundarySweepVerticesR2;
inline vint zLogicalR2;
inline vint xLogicalFragment1R2;
inline vint xLogicalFragment2R2;

inline std::map<pint,std::pair<pint,pint>> overlappingFaces;

#endif
