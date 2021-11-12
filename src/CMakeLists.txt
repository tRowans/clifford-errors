add_executable(czErrors main.cpp czErrorGen.cpp decoderCubic.cpp decoderRhombic1.cpp decoderRhombic2.cpp cubic.cpp rhombic1.cpp rhombic2.cpp rhombicGeneric.cpp lattice.cpp)

add_library(cubic_lib decoderCubic.cpp cubic.cpp lattice.cpp)
add_library(rhombic1_lib decoderRhombic1.cpp rhombic1.cpp rhombicGeneric.cpp lattice.cpp)
add_library(rhombic2_lib decoderRhombic2.cpp rhombic2.cpp rhombicGeneric.cpp lattice.cpp)
add_library(cz_lib czErrorGen.cpp rhombicGeneric.cpp lattice.cpp)

include_directories(../lib/blossom5)

target_link_libraries(czErrors PUBLIC PerfectMatching)
