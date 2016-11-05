#!/bin/bash

BASEDIR=`pwd`
INSTALLDIR=${BASEDIR}/installdir
BUILDDIR=${BASEDIR}/builddir

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${INSTALLDIR}/lib"
export PATH="${PATH}:${INSTALLDIR}/bin"

#tensorflow with cuda
cd ${BASEDIR}
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/cuda/lib64"
export CUDA_HOME=/usr/local/cuda
#git clone https://github.com/tensorflow/tensorflow Tensorflow
cd Tensorflow
#git checkout tags/v0.11.0rc2
#./configure
#bazel build -c opt --config=cuda //tensorflow/tools/pip_package:build_pip_package
#bazel-bin/tensorflow/tools/pip_package/build_pip_package /tmp/tensorflow_pkg
#sudo pip install /tmp/tensorflow_pkg/tensorflow-0.11.0rc2-py2-none-any.whl
#ln -s ../scripts/protoc-recursively.bash
#source ./protoc-recursively.bash

bazel build -c opt --config=cuda //tensorflow:libtensorflow.so
ln -s ${BASEDIR}/Tensorflow/bazel-bin/tensorflow/libtensorflow.so ${INSTALLDIR}/lib

cd ${BASEDIR}
