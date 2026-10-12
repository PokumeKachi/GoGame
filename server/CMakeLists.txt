cmake_minimum_required(VERSION 3.16)
project(GoGameServer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 6.2 REQUIRED COMPONENTS Core Network WebSockets)

qt_add_executable(appserver main.cpp)
target_link_libraries(appserver PRIVATE Qt6::Core Qt6::Network Qt6::WebSockets)

include(GNUInstallDirs)
install(TARGETS appserver RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CTest)
if(BUILD_TESTING)
    qt_add_executable(serverIntegrationTest tests/Integration.cpp
        ../src/Network/NetworkClient.cpp ../src/Network/NetworkClient.h)
    set_target_properties(serverIntegrationTest PROPERTIES AUTOMOC ON)
    target_link_libraries(serverIntegrationTest PRIVATE Qt6::Core Qt6::Network Qt6::WebSockets)
    add_test(NAME serverIntegration COMMAND serverIntegrationTest $<TARGET_FILE:appserver>)
    set_tests_properties(serverIntegration PROPERTIES TIMEOUT 30)
endif()
