#!/bin/bash

BASEDIR=`pwd`
INSTALLDIR=${BASEDIR}/installdir
BUILDDIR=${BASEDIR}/builddir

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${INSTALLDIR}/lib"
export PATH="${PATH}:${INSTALLDIR}/bin"


mkdir ${BASEDIR}/Tensorflow/tensorflow/libtensorflow
echo 'cc_binary(
     name = "libtensorflow.so",
     linkshared = 1,
     deps = [
         "//tensorflow/core:tensorflow",
     ]
)' > ${BASEDIR}/Tensorflow/tensorflow/libtensorflow/BUILD
cd ${BASEDIR}/Tensorflow/tensorflow/libtensorflow
bazel build -c opt --config=cuda :libtensorflow.so
cd ${BASEDIR}
ln -s ${BASEDIR}/Tensorflow/bazel-bin/tensorflow/libtensorflow/libtensorflow.so ${INSTALLDIR}/lib
