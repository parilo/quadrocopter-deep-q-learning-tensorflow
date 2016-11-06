cmake_minimum_required (VERSION 2.8.1)
set(CMAKE_CXX_STANDARD 11)
SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
cmake_policy(SET CMP0015 NEW) #for relative paths in link_directories

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

set(LINUX_LIBS "")
if(LINUX)
    set(LINUX_LIBS pthread)
endif()


#add_library(Box2D STATIC IMPORTED)
#set_property(TARGET Box2D PROPERTY IMPORTED_LOCATION "/Users/anton/devel/unity/QuadrocopterHabr2D/console-box2d/box2d-install/lib/libBox2D.a")

link_directories (../../installdir/lib)

#add_subdirectory("../../../Box2D/Box2D/Box2D" "${CMAKE_CURRENT_BINARY_DIR}/Box2D")

project (QuadrocopterSimulator)
include_directories(
../../installdir/include
../../Quadrocopter-Box2D/Quadrocopter-Box2D/Quadrocopter-Box2D
../../Quadrocopter2DBrain/Quadrocopter2DBrain
../../Quadrocopter-Visualizer/Classes
)

add_executable(QuadrocopterSimalator
#add_library(
ConsumerProducerSyncronizer.cpp
Lib.cpp
ObstacleDiscrete1D.cpp
ObstacleDiscrete2D.cpp
PID.cpp
Quadrocopter2DContActionCtrl.cpp
Quadrocopter2DContActionPIDCtrl.cpp
Quadrocopter2DContActionPIDLSTMCtrl.cpp
Quadrocopter2DContActionLSTMCtrl.cpp
Quadrocopter2DContActionMLPSeqCtrl.cpp
Quadrocopter2DCtrl.cpp
QuadrocopterContActionCtrl.cpp
QuadrocopterCtrl.cpp
QuadrocopterDiscrete1D.cpp
QuadrocopterDiscrete2D.cpp
QuadrocopterDiscrete2DCtrl.cpp
QuadrocopterDiscreteCtrl.cpp
QuadrocopterSimulator.cpp
WorldDiscrete.cpp
main.cpp
)

target_link_libraries(QuadrocopterSimalator QuadrocopterBox2DSimulator Quadrocopter2DBrain ${LINUX_LIBS})

install(TARGETS QuadrocopterSimalator
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
