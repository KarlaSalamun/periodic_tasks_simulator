cmake_minimum_required(VERSION 3.10)
project(periodic_simulator LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra")
include_directories(include)

#TODO: set compiler flags

add_library(periodic_simulator STATIC
	src/Scheduler.cpp
		include/Simulator.tpp
	src/Task.cpp
	src/TaskCreator.cpp
	src/UunifastCreator.cpp include/StaticCreator.h src/StaticCreator.cpp)

add_executable(periodic_test tests/main.cpp)
target_link_libraries(periodic_test periodic_simulator)
