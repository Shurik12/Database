# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.20)

# Set the project name
project(db VERSION 0.1 DESCRIPTION "Database with 3 structures of search")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++17")

# Create a sources variable with a link to all cpp files to compile
set(CLIENT_SOURCES
	src/client.cpp
	src/list.cpp
	src/list_node.cpp
	src/record.cpp
	src/command.cpp
)

set(SERVER_SOURCES 
        src/server.cpp
        src/list.cpp
        src/list_node.cpp
        src/record.cpp
        src/command.cpp
)

# Add an executable with the above sources
add_executable(server ${SERVER_SOURCES})
add_executable(client ${CLIENT_SOURCES})

# Set the directories that should be included in the build command for this target
# when running g++ these will be included as -I/directory/path/
target_include_directories(server
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(client
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)
