#!/bin/bash

BASEDIR=`pwd`
INSTALLDIR=${BASEDIR}/installdir
BUILDDIR=${BASEDIR}/builddir

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${INSTALLDIR}/lib"
export PATH="${PATH}:${INSTALLDIR}/bin"

wget -c  http://parilo.ru/.../cudnn-7.5-linux-x64-v5.0-ga.tgz
tar -xzf cudnn-7.5-linux-x64-v5.0-ga.tgz
mkdir cudnn
tar -xzf cudnn-7.5-linux-x64-v5.0-ga.tgz -C cudnn/
sudo cp cudnn/cuda/lib64/libcudnn* /usr/local/cuda/lib64
sudo cp cudnn/cuda/include/cudnn.h /usr/local/cuda/include/
