#!/bin/bash

BASEDIR=`pwd`
INSTALLDIR=${BASEDIR}/installdir
BUILDDIR=${BASEDIR}/builddir

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${INSTALLDIR}/lib"
export PATH="${PATH}:${INSTALLDIR}/bin"

#bazel
BAZELDIR=${BUILDDIR}/bazel
git clone https://github.com/bazelbuild/bazel.git ${BAZELDIR}
cd ${BAZELDIR}
git checkout tags/0.3.2
cd ..
cd ${BAZELDIR}
./compile.sh
cp output/bazel ${INSTALLDIR}/bin
cd ${BASEDIR}
