#!/bin/bash

BASEDIR=`pwd`
INSTALLDIR=${BASEDIR}/installdir
BUILDDIR=${BASEDIR}/builddir
PATH=${PATH}:${INSTALLDIR}/bin
#mkdir ${INSTALLDIR}
#mkdir ${BUILDDIR}

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${INSTALLDIR}/lib"
export PATH="${PATH}:${INSTALLDIR}/bin"

#echo "installing Box2D"
#git clone https://github.com/erincatto/Box2D.git Box2D
#BOX2DDIR=${BUILDDIR}/build-box2d
#mkdir ${BOX2DDIR}
#cd ${BOX2DDIR}
#cmake -DBOX2D_INSTALL=ON -DBOX2D_BUILD_SHARED=ON -DCMAKE_INSTALL_PREFIX=${INSTALLDIR} ${BASEDIR}/Box2D/Box2D
#make
#make install
#cd ${BASEDIR}

#echo "installing tensorflow"
#BAZELDIR=${BUILDDIR}/bazel
#git clone https://github.com/bazelbuild/bazel.git ${BAZELDIR}
#cd ${BAZELDIR}
#./compile.sh
#cp output/bazel ${INSTALLDIR}/bin
#cd ${BASEDIR}


#git clone https://github.com/tensorflow/tensorflow Tensorflow
#git checkout tags/v0.10.0

#tensorflow
#cd ${BASEDIR}
#git clone --recurse-submodules https://github.com/tensorflow/tensorflow Tensorflow
#cd Tensorflow
#./configure
#bazel build -c opt //tensorflow/tools/pip_package:build_pip_package
#bazel-bin/tensorflow/tools/pip_package/build_pip_package /tmp/tensorflow_pkg
#sudo pip install --upgrade /tmp/tensorflow_pkg/tensorflow-0.8.0-py2-none-any.whl
#mkdir ${BASEDIR}/Tensorflow/tensorflow/libtensorflow
#echo 'cc_binary(
#     name = "libtensorflow.so",
#     linkshared = 1,
#     deps = [
#         "//tensorflow/core:tensorflow",
#     ]
# )' > ${BASEDIR}/Tensorflow/tensorflow/libtensorflow/BUILD
#cd ${BASEDIR}/Tensorflow/tensorflow/libtensorflow
#bazel build :libtensorflow.so
#cd ${BASEDIR}
#ln -s ${BASEDIR}/Tensorflow/bazel-bin/tensorflow/libtensorflow/libtensorflow.so ${INSTALLDIR}/lib

#echo "installing Quadrocopter Simulator"
#BUILDDIR=${BUILDDIR}/build-simulator
#mkdir ${BUILDDIR}
#cd ${BUILDDIR}
#cmake -DCMAKE_INSTALL_PREFIX=${INSTALLDIR} -DTF_GRAPH_DIR=${BASEDIR}/TensorflowGraph/models/graph.pb ${BASEDIR}
#make
#make install
#cd ${BASEDIR}
