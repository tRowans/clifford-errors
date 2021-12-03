add_executable(RunTests cubic-test.cpp decoderCubic-test.cpp rhombicGeneric-test.cpp rhombic1-test.cpp decoderRhombic1-test.cpp rhombic2-test.cpp decoderRhombic2-test.cpp czErrorGen-test.cpp)

include_directories(../src ../lib/blossom5)

target_link_libraries(RunTests PUBLIC cz_lib gtest_main PerfectMatching) #Non-generic libs are just for building index vectors in these tests
