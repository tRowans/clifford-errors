add_executable(TestToric toricCodeSlice-test.cpp decoderToric-test.cpp)
add_executable(TestRhombic1 rhombic1-test.cpp decoderRhombic1-test.cpp)
add_executable(TestRhombic2 rhombic2-test.cpp decoderRhombic2-test.cpp)
add_executable(TestGeneric latticeGeneric-test.cpp decoderGeneric-test.cpp)

include_directories(../src ../lib/blossom5)

target_link_libraries(TestToric PUBLIC JITdecoderToric_lib gtest_main PerfectMatching)
target_link_libraries(TestRhombic1 PUBLIC JITdecoderRhombic1_lib gtest_main PerfectMatching)
target_link_libraries(TestRhombic2 PUBLIC JITdecoderRhombic2_lib gtest_main PerfectMatching)
target_link_libraries(TestGeneric PUBLIC JITdecoderGeneric_lib JITdecoderToric_lib JITdecoderRhombic1_lib JITdecoderRhombic2_lib gtest_main) #Non-generic libs are just for building index vectors in these tests
