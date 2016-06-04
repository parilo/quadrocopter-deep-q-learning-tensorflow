cmake_minimum_required (VERSION 2.8.1)
set(CMAKE_CXX_STANDARD 11)
SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
cmake_policy(SET CMP0015 NEW) #for relative paths in link_directories

#add_library(Box2D STATIC IMPORTED)
#set_property(TARGET Box2D PROPERTY IMPORTED_LOCATION "/Users/anton/devel/unity/QuadrocopterHabr2D/console-box2d/box2d-install/lib/libBox2D.a")

#link_directories (../../installdir/lib)

#add_subdirectory("../../../Box2D/Box2D/Box2D" "${CMAKE_CURRENT_BINARY_DIR}/Box2D")

add_subdirectory(Quadrocopter-Box2D/Quadrocopter-Box2D/Quadrocopter-Box2D)
add_subdirectory(Quadrocopter2DBrain/Quadrocopter2DBrain)
add_subdirectory(Quadrocopter-Visualizer/Classes)

include_directories(
Quadrocopter-Box2D/Quadrocopter-Box2D/Quadrocopter-Box2D
Quadrocopter2DBrain/Quadrocopter2DBrain
Quadrocopter-Visualizer/Classes
)

#project (QuadrocopterSimulator)
#include_directories(../../installdir/include)
#add_executable(
#add_library(
#QuadrocopterSimalator SHARED 
#AppDelegate.cpp
#HelloWorldScene.cpp
#Lib.cpp
#QuadrocopterCtrl.cpp
#)

#target_link_libraries(Quadrocopter2DBrain tensorflow)
