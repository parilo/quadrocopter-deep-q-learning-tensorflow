#!/bin/bash +

BASEDIR=`pwd`
INSTALLDIR=${BASEDIR}/installdir
BUILDDIR=${BASEDIR}/builddir
PATH=${PATH}:${INSTALLDIR}/bin
#mkdir ${INSTALLDIR}
#mkdir ${BUILDDIR}

sudo pip install --upgrade /tmp/tensorflow_pkg/tensorflow-0.8.0-py2-none-any.whl
