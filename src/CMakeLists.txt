add_executable(portfolio-info portfolio-info.cpp)
target_link_libraries(portfolio-info portopt)

add_executable(market-info market-info.cpp)
target_link_libraries(market-info portopt)

add_executable(two-asset-optimizer two-asset-optimizer.cpp)
target_link_libraries(two-asset-optimizer portopt)

add_executable(simple-risk-return simple-risk-return.cpp)
target_link_libraries(simple-risk-return portopt)

add_executable(risk-return-optimizer risk-return-optimizer.cpp)
target_link_libraries(risk-return-optimizer portopt)

add_executable(retirement-calculator retirement-calculator.cpp)
target_link_libraries(retirement-calculator portopt)
