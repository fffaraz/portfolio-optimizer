add_executable(OhlcListTest OhlcListTest.cpp)
target_link_libraries(OhlcListTest portopt gtest_main)
gtest_discover_tests(OhlcListTest)

add_executable(PortfolioTest PortfolioTest.cpp)
target_link_libraries(PortfolioTest portopt gtest_main)
gtest_discover_tests(PortfolioTest)

add_executable(UtilsTest UtilsTest.cpp)
target_link_libraries(UtilsTest portopt gtest_main)
gtest_discover_tests(UtilsTest)
