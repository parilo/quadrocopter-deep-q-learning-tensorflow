#!/bin/bash

BASEDIR=`pwd`
INSTALLDIR=${BASEDIR}/installdir
BUILDDIR=${BASEDIR}/builddir

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${INSTALLDIR}/lib"
export PATH="${PATH}:${INSTALLDIR}/bin"

#cuda
cd ${BUILDDIR}
wget http://developer.download.nvidia.com/compute/cuda/7.5/Prod/local_installers/cuda_7.5.18_linux.run
chmod +x cuda_7.5.18_linux.run
./cuda_7.5.18_linux.run -extract=`pwd`/nvidia_installers
cd nvidia_installers
sudo ./NVIDIA-Linux-x86_64-352.39.run
sudo modprobe nvidia
sudo ./cuda-linux64-rel-7.5.18-19867135.run
cd ..
