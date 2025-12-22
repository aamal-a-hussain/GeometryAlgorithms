file(GLOB TEST_FILES "*.cpp")
add_executable(
    CppGeometry_Tests
    ${TEST_FILES}
)

target_link_libraries(
    CppGeometry_Tests
    PRIVATE
        CppGeometry
        GTest::gtest_main
)


gtest_discover_tests(CppGeometry_Tests)
