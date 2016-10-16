#!/bin/bash

BASEDIR=`pwd`
INSTALLDIR=${BASEDIR}/installdir
BUILDDIR=${BASEDIR}/builddir
PATH=${PATH}:${INSTALLDIR}/bin
#mkdir ${INSTALLDIR}
#mkdir ${BUILDDIR}

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${INSTALLDIR}/lib"
export PATH="${PATH}:${INSTALLDIR}/bin"


#tensorflow
cd ${BASEDIR}
git clone https://github.com/tensorflow/tensorflow Tensorflow
git checkout tags/v0.10.0
cd Tensorflow
./configure
bazel build -c opt //tensorflow/tools/pip_package:build_pip_package
bazel-bin/tensorflow/tools/pip_package/build_pip_package /tmp/tensorflow_pkg
cd ${BASEDIR}/Tensorflow/tensorflow
bazel build :libtensorflow.so
cd ${BASEDIR}
ln -s ${BASEDIR}/Tensorflow/bazel-bin/tensorflow/libtensorflow.so ${INSTALLDIR}/lib
ln -s ../scripts/protoc-recursively.bash
source ./protoc-recursively.bash
cd ${BASEDIR}
