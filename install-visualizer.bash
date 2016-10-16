#!/bin/bash +

BASEDIR=`pwd`
INSTALLDIR=${BASEDIR}/installdir
BUILDDIR=${BASEDIR}/builddir

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${INSTALLDIR}/lib"
export PATH="${PATH}:${INSTALLDIR}/bin"


echo "installing Quadrocopter Visualizer"
BUILDDIR=${BUILDDIR}/build-vis
mkdir ${BUILDDIR}
cd ${BUILDDIR}
cmake -DCMAKE_INSTALL_PREFIX=${INSTALLDIR} ${BASEDIR}/Quadrocopter-Visualizer/Classes/visualizer/
make
make install
cd ${BASEDIR}
