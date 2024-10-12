cmake_minimum_required(VERSION 3.12)
project(BattleBugs VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ======== BattleBugs ========
add_subdirectory(dependencies/BattleBugs)

# ======== Client Application  ========
add_executable(client
	main.cpp)
target_link_libraries(client PUBLIC clientlib)