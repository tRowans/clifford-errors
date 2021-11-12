add_executable(TestCubic ... )
add_executable(TestRhombic1 ... )
add_executable(TestRhombic2 ... )
add_executable(TestCZ ... )

include_directories(../src ../lib/blossom5)

target_link_libraries(TestCubic PUBLIC cubic_lib gtest_main PerfectMatching)
target_link_libraries(TestRhombic1 PUBLIC rhombic1_lib gtest_main PerfectMatching)
target_link_libraries(TestRhombic2 PUBLIC rhombic2_lib gtest_main PerfectMatching)
target_link_libraries(TestCZ PUBLIC cz_lib gtest_main) 
