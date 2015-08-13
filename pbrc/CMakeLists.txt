cmake_minimum_required(VERSION 2.8.11)
project(pbrc)

add_definitions(-std=c++11)
set(CMAKE_BUILD_TYPE RELEASE)
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SRC
	src/main.cpp
	src/BytestreamParser.cpp
)

set(HEADERS
	src/Datatypes.hpp
	src/BytestreamParser.hpp
)

find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS_MOC} ${HEADERS})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Network)
