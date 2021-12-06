add_executable(LatticeTests lattice-test.cpp)
add_executable(CubicTests cubic-test.cpp decoderCubic-test.cpp)
add_executable(RhombicTests rhombicGeneric-test.cpp rhombic1-test.cpp rhombic2-test.cpp decoderRhombic1-test.cpp decoderRhombic2-test.cpp)
add_executable(CZTests czErrorGen-test.cpp)

include_directories(../src ../lib/blossom5)

target_link_libraries(LatticeTests PUBLIC lattice_lib gtest_main PerfectMatching)
target_link_libraries(CubicTests PUBLIC cubic_lib gtest_main PerfectMatching)
target_link_libraries(RhombicTests PUBLIC rhombic_lib gtest_main PerfectMatching)
target_link_libraries(CZTests PUBLIC cz_lib gtest_main PerfectMatching) 
