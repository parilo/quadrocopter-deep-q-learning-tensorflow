cmake_minimum_required (VERSION 2.8.1)
set(CMAKE_CXX_STANDARD 11)
SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
cmake_policy(SET CMP0015 NEW) #for relative paths in link_directories

#add_library(Box2D STATIC IMPORTED)
#set_property(TARGET Box2D PROPERTY IMPORTED_LOCATION "/Users/anton/devel/unity/QuadrocopterHabr2D/console-box2d/box2d-install/lib/libBox2D.a")

link_directories (../../installdir/lib)

#add_subdirectory("../../../Box2D/Box2D/Box2D" "${CMAKE_CURRENT_BINARY_DIR}/Box2D")

#/Users/anton/devel/build/tensorflow /Users/anton/devel/build/tensorflow/google/protobuf/src /Users/anton/devel/build/tensorflow/protobuf-generated /Users/anton/devel/build/tensorflow/bazel-tensorflow /Users/anton/devel/build/tensorflow/third_party/eigen3 /Users/anton/devel/build/tensorflow/bazel-tensorflow/external/eigen_archive/eigen-eigen-d861b41b1996

project (Quadrocopter2DBrain)

include_directories(
../../installdir/include
../../Tensorflow
../../Tensorflow/google/protobuf/src
../../Tensorflow/protobuf-generated
../../Tensorflow/bazel-Tensorflow
../../Tensorflow/bazel-Tensorflow/third_party/eigen3
../../Tensorflow/bazel-Tensorflow/external/eigen_archive
../../Tensorflow/bazel-Tensorflow/external/eigen_archive/eigen-eigen-50812b426b7c
../../Tensorflow/bazel-Tensorflow/external/eigen_archive/eigen-eigen-3f653ace7d28
)

#add_executable(QuadrocopterBox2DSimulator Quadrocopter1D.cpp Simulation.cpp World.cpp main.cpp)
add_library(Quadrocopter2DBrain SHARED 
BrainContinousDeepQ.cpp
BrainDiscreteDeepQ.cpp
ExpLambdaFilter.cpp
ExperienceBank.cpp
ExperienceFilter.cpp
ExperienceItem.cpp
Lib.cpp
Observation.cpp
ObservationSeqLimited.cpp
Quadrocopter2DBrain.cpp
QuadrocopterBrain.cpp
Tensors.cpp
)

target_link_libraries(Quadrocopter2DBrain tensorflow)

install(TARGETS Quadrocopter2DBrain
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)
