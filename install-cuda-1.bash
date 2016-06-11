#!/bin/bash

BASEDIR=`pwd`
INSTALLDIR=${BASEDIR}/installdir
BUILDDIR=${BASEDIR}/builddir
mkdir ${INSTALLDIR}
mkdir ${INSTALLDIR}/bin
mkdir ${BUILDDIR}

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${INSTALLDIR}/lib"
export PATH="${PATH}:${INSTALLDIR}/bin"

echo "installing Box2D"
git clone https://github.com/erincatto/Box2D.git Box2D
cd Box2D
git checkout tags/v2.3.1
cd ..
BOX2DDIR=${BUILDDIR}/build-box2d
mkdir ${BOX2DDIR}
cd ${BOX2DDIR}
cmake -DBOX2D_INSTALL=ON -DBOX2D_BUILD_SHARED=ON -DBOX2D_BUILD_EXAMPLES=OFF -DCMAKE_INSTALL_PREFIX=${INSTALLDIR} ${BASEDIR}/Box2D/Box2D
make
make install
cd ${BASEDIR}
