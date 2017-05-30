cmake_minimum_required (VERSION 2.8.1)
set(CMAKE_CXX_STANDARD 11)
SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
cmake_policy(SET CMP0015 NEW)

#add_library(Box2D STATIC IMPORTED)
#set_property(TARGET Box2D PROPERTY IMPORTED_LOCATION "/Users/anton/devel/unity/QuadrocopterHabr2D/console-box2d/box2d-install/lib/libBox2D.a")

link_directories (../../../installdir/lib)

#add_subdirectory("../../../Box2D/Box2D/Box2D" "${CMAKE_CURRENT_BINARY_DIR}/Box2D")

project (QuadrocopterBox2DSimulator)
include_directories(../../../installdir/include)
#add_executable(QuadrocopterBox2DSimulator Quadrocopter1D.cpp Simulation.cpp World.cpp main.cpp)
add_library(QuadrocopterBox2DSimulator SHARED
Obstacle1D.cpp
Obstacle2D.cpp
ObstacleModelIFace.cpp
Quadrocopter1D.cpp
Quadrocopter2D.cpp
QuadrocopterModelIFace.cpp
Simulation.cpp
World.cpp
WorldBase.cpp
WorldObjectInfo.cpp)

target_link_libraries(QuadrocopterBox2DSimulator Box2D)

install(TARGETS QuadrocopterBox2DSimulator
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
