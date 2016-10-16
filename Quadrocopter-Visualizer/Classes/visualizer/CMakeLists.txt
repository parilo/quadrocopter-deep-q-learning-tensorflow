cmake_minimum_required (VERSION 2.8.1)
set(CMAKE_CXX_STANDARD 11)
SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
cmake_policy(SET CMP0015 NEW) #for relative paths in link_directories

project (QuadrocopterVisualizer)

#add_library(Box2D STATIC IMPORTED)
#set_property(TARGET Box2D PROPERTY IMPORTED_LOCATION "/Users/anton/devel/unity/QuadrocopterHabr2D/console-box2d/box2d-install/lib/libBox2D.a")

set(PLATFORM_SPECIFIC_LIBS)
if(UNIX AND NOT(MACOSX OR APPLE))
    set(LINUX true)
    add_definitions(-DLINUX)
    set(PLATFORM_SPECIFIC_LIBS
	glfw
	pthread
	X11
	GL
	GLEW
	fontconfig
    )
#  foreach(_pkg OPENGL GLEW GLFW3 FMOD FONTCONFIG THREADS GTK3)
#    cocos_use_pkg(cocos2dInternal ${_pkg})
#  endforeach()
elseif(MACOSX OR APPLE)
#  cocos_use_pkg(cocos2dInternal GLFW3)

 INCLUDE_DIRECTORIES ( /System/Library/Frameworks )

 FIND_LIBRARY(COCOA_LIBRARY Cocoa)
 FIND_LIBRARY(OPENGL_LIBRARY OpenGL )
 FIND_LIBRARY(AUDIOTOOLBOX_LIBRARY AudioToolbox)
 FIND_LIBRARY(OPENAL_LIBRARY OpenAL )
 FIND_LIBRARY(QUARTZCORE_LIBRARY QuartzCore )
 FIND_LIBRARY(APPLICATIONSERVICES_LIBRARY ApplicationServices)
 FIND_LIBRARY(IOKIT_LIBRARY IOKit)
 FIND_LIBRARY(FOUNDATION_LIBRARY Foundation)
 FIND_LIBRARY(ICONV_LIBRARY iconv)

  set(PLATFORM_SPECIFIC_LIBS
    ${COCOA_LIBRARY}
    ${OPENGL_LIBRARY}
    ${OPENAL_LIBRARY}
    ${AUDIOTOOLBOX_LIBRARY}
    ${QUARTZCORE_LIBRARY}
    ${APPLICATIONSERVICES_LIBRARY}
    ${IOKIT_LIBRARY}
    ${FOUNDATION_LIBRARY}
    ${ICONV_LIBRARY}
    glfw3
    iconv
  )

else()
  message( FATAL_ERROR "Unsupported platform, CMake will exit" )
endif()


link_directories (
../../../installdir/lib
../../../builddir/cocos2d-x/builddir/lib
)

#add_subdirectory("../../../Box2D/Box2D/Box2D" "${CMAKE_CURRENT_BINARY_DIR}/Box2D")

include_directories(
../../../installdir/include
../../../Quadrocopter-Box2D/Quadrocopter-Box2D/Quadrocopter-Box2D
../../../Quadrocopter2DBrain/Quadrocopter2DBrain
../../../Quadrocopter-Visualizer/Classes
../../..//builddir/cocos2d-x/cocos
/usr/local/include/GLFW
/usr/include/GLFW
)

add_executable(QuadrocopterVisualizer
../AppDelegate.cpp
../ConsumerProducerSyncronizer.cpp
../Lib.cpp
../ObstacleDiscrete1D.cpp
../ObstacleDiscrete2D.cpp
../PID.cpp
../Quadrocopter2DContActionCtrl.cpp
../Quadrocopter2DContActionPIDCtrl.cpp
../Quadrocopter2DContActionPIDLSTMCtrl.cpp
../Quadrocopter2DCtrl.cpp
../QuadrocopterContActionCtrl.cpp
../QuadrocopterCtrl.cpp
../QuadrocopterDiscrete1D.cpp
../QuadrocopterDiscrete2D.cpp
../QuadrocopterDiscrete2DCtrl.cpp
../QuadrocopterDiscreteCtrl.cpp
../QuadrocopterSimulator.cpp
../Visualizer1DScene.cpp
../Visualizer2DScene.cpp
../WorldDiscrete.cpp
main.cpp
)

target_link_libraries(QuadrocopterVisualizer
QuadrocopterBox2DSimulator
Quadrocopter2DBrain
cocos2d
cocos2dInternal
bullet
flatbuffers
recast
tinyxml2
unzip
xxhash
freetype
tiff
Box2D
webp
z
png
jpeg
${PLATFORM_SPECIFIC_LIBS}
)

install(TARGETS QuadrocopterVisualizer
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
