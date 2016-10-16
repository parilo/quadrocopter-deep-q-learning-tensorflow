cmake_minimum_required (VERSION 2.8.1)
set(CMAKE_CXX_STANDARD 11)
SET_PROPERTY(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
cmake_policy(SET CMP0015 NEW) #for relative paths in link_directories

#add_library(Box2D STATIC IMPORTED)
#set_property(TARGET Box2D PROPERTY IMPORTED_LOCATION "/Users/anton/devel/unity/QuadrocopterHabr2D/console-box2d/box2d-install/lib/libBox2D.a")

link_directories (../../installdir/lib)

#add_subdirectory("../../../Box2D/Box2D/Box2D" "${CMAKE_CURRENT_BINARY_DIR}/Box2D")

project (Quadrocopter2DBrain)

include_directories(
../../Tensorflow
../../Tensorflow/bazel-Tensorflow/external/protobuf/src
../../Tensorflow/protobuf-generated
../../Tensorflow/bazel-Tensorflow
../../Tensorflow/bazel-Tensorflow/external/eigen_archive
)

#add_executable(QuadrocopterBox2DSimulator Quadrocopter1D.cpp Simulation.cpp World.cpp main.cpp)
add_library(Quadrocopter2DBrain SHARED 
BrainAlgorithm.cpp
BrainDiscreteDeepQ.cpp
DDPG.cpp
DDPG_LSTM.cpp
ExpLambdaFilter.cpp
ExperienceBank.cpp
ExperienceBankRemovable.cpp
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
