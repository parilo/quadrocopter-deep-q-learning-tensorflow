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
git clone --recurse-submodules https://github.com/tensorflow/tensorflow Tensorflow
cd Tensorflow
git checkout tags/v0.8.0
git submodule update --init
TF_UNOFFICIAL_SETTING=1 ./configure
##bazel build -c opt --config=cuda //tensorflow/cc:tutorials_example_trainer
bazel build -c opt --config=cuda --spawn_strategy=standalone --genrule_strategy=standalone //tensorflow/tools/pip_package:build_pip_package
cp -r bazel-bin/tensorflow/tools/pip_package/build_pip_package.runfiles/__main__/* bazel-bin/tensorflow/tools/pip_package/build_pip_package.runfiles/
bazel-bin/tensorflow/tools/pip_package/build_pip_package /tmp/tensorflow_pkg
sudo pip install /tmp/tensorflow_pkg/tensorflow-0.8.0-cp27-none-linux_x86_64.whl
ln -s ../scripts/protoc-recursively.bash
source ./protoc-recursively.bash
cd ${BASEDIR}
