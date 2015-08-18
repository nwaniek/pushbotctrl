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
	src/PushbotConnection.cpp
	src/SensorsProcessor.cpp
	src/RobotControl.cpp
	src/gui/Dialog.cpp
	src/gui/DVSEventWidget.cpp
	src/gui/NavigationWidget.cpp
	src/gui/MainWindow.cpp
	src/gui/RobotControlWidget.cpp
)

set(HEADERS
	src/Datatypes.hpp
	src/Commands.hpp
	src/BytestreamParser.hpp
	src/PushbotConnection.hpp
	src/SensorsProcessor.hpp
	src/RobotControl.hpp
	src/gui/Dialog.hpp
	src/gui/DVSEventWidget.hpp
	src/gui/NavigationWidget.hpp
	src/gui/MainWindow.hpp
	src/gui/RobotControlWidget.hpp
)

include_directories(src)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS_MOC} ${HEADERS})
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Network m)
