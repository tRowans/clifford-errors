add_executable(czErrors main.cpp czErrorGen.cpp decoderCubic.cpp decoderRhombic1.cpp decoderRhombic2.cpp cubic.cpp rhombic1.cpp rhombic2.cpp rhombicGeneric.cpp vis.cpp lattice.cpp)

#These are just used for building the separate testing exes
add_library(lattice_lib lattice.cpp cubic.cpp)
add_library(cubic_lib decoderCubic.cpp cubic.cpp lattice.cpp)
add_library(rhombic_lib decoderRhombic1.cpp decoderRhombic2.cpp rhombic1.cpp rhombic2.cpp rhombicGeneric.cpp lattice.cpp)
#This is the main one for actually running the code
add_library(cz_lib czErrorGen.cpp decoderCubic.cpp decoderRhombic1.cpp decoderRhombic2.cpp cubic.cpp rhombic1.cpp rhombic2.cpp rhombicGeneric.cpp vis.cpp lattice.cpp)

include_directories(../lib/blossom5)
include_directories(../lib/bp_osd)
target_link_libraries(czErrors PUBLIC PerfectMatching)
target_link_libraries(czErrors PUBLIC bp_osd)
target_link_libraries(cz_lib PUBLIC bp_osd)
target_link_libraries(cubic_lib PUBLIC bp_osd)
target_link_libraries(rhombic_lib PUBLIC bp_osd)
