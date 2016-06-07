#!/bin/bash

BASEDIR=`pwd`
INSTALLDIR=${BASEDIR}/installdir
BUILDDIR=${BASEDIR}/builddir
#mkdir ${INSTALLDIR}
#mkdir ${INSTALLDIR}/bin
#mkdir ${BUILDDIR}

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${INSTALLDIR}/lib"
export PATH="${PATH}:${INSTALLDIR}/bin"

#echo "installing Box2D"
#git clone https://github.com/erincatto/Box2D.git Box2D
#cd Box2D
#git checkout tags/v2.3.1
#cd ..
#BOX2DDIR=${BUILDDIR}/build-box2d
#mkdir ${BOX2DDIR}
#cd ${BOX2DDIR}
#cmake -DBOX2D_INSTALL=ON -DBOX2D_BUILD_SHARED=ON -DBOX2D_BUILD_EXAMPLES=OFF -DCMAKE_INSTALL_PREFIX=${INSTALLDIR} ${BASEDIR}/Box2D/Box2D
#make
#make install
#cd ${BASEDIR}


#cuda
#cd ${BUILDDIR}
#wget http://developer.download.nvidia.com/compute/cuda/7.5/Prod/local_installers/cuda_7.5.18_linux.run
#chmod +x cuda_7.5.18_linux.run
#./cuda_7.5.18_linux.run -extract=`pwd`/nvidia_installers
#cd nvidia_installers
#sudo ./NVIDIA-Linux-x86_64-352.39.run
#sudo modprobe nvidia
#sudo ./cuda-linux64-rel-7.5.18-19867135.run
#cd ..

##wget http://parilo.ru/asdk2jn2kjbuvysbvs/cudnn-6.5-linux-x64-v2.tgz
##tar -xzf cudnn-6.5-linux-x64-v2.tgz
##sudo cp cudnn-6.5-linux-x64-v2/libcudnn* /usr/local/cuda/lib64
##sudo cp cudnn-6.5-linux-x64-v2/cudnn.h /usr/local/cuda/include/

#wget -c  http://parilo.ru/asdk2jn2kjbuvysbvs/cudnn-7.5-linux-x64-v5.0-ga.tgz
#tar -xzf cudnn-7.5-linux-x64-v5.0-ga.tgz
#mkdir cudnn
#tar -xzf cudnn-7.5-linux-x64-v5.0-ga.tgz -C cudnn/
#sudo cp cudnn/cuda/lib64/libcudnn* /usr/local/cuda/lib64
#sudo cp cudnn/cuda/include/cudnn.h /usr/local/cuda/include/


#bazel
#BAZELDIR=${BUILDDIR}/bazel
#git clone https://github.com/bazelbuild/bazel.git ${BAZELDIR}
#cd ${BAZELDIR}
#git checkout tags/0.2.3
#cd ..
#cd ${BAZELDIR}
#./compile.sh
#cp output/bazel ${INSTALLDIR}/bin
#cd ${BASEDIR}


#tensorflow with cuda
#cd ${BASEDIR}
#export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:/usr/local/cuda/lib64"
#export CUDA_HOME=/usr/local/cuda
#git clone --recurse-submodules https://github.com/tensorflow/tensorflow Tensorflow
#cd Tensorflow
#git checkout tags/v0.8.0
#git submodule update --init
#TF_UNOFFICIAL_SETTING=1 ./configure
##bazel build -c opt --config=cuda //tensorflow/cc:tutorials_example_trainer
#bazel build -c opt --config=cuda //tensorflow/tools/pip_package:build_pip_package
#cp -r bazel-bin/tensorflow/tools/pip_package/build_pip_package.runfiles/__main__/* bazel-bin/tensorflow/tools/pip_package/build_pip_package.runfiles/
#bazel-bin/tensorflow/tools/pip_package/build_pip_package /tmp/tensorflow_pkg
#sudo pip install /tmp/tensorflow_pkg/tensorflow-0.8.0-cp27-none-linux_x86_64.whl
#ln -s ../scripts/protoc-recursively.bash
#source ./protoc-recursively.bash
#cd ${BASEDIR}


#mkdir ${BASEDIR}/Tensorflow/tensorflow/libtensorflow
#echo 'cc_binary(
#     name = "libtensorflow.so",
#     linkshared = 1,
#     deps = [
#         "//tensorflow/core:tensorflow",
#     ]
# )' > ${BASEDIR}/Tensorflow/tensorflow/libtensorflow/BUILD
#cd ${BASEDIR}/Tensorflow/tensorflow/libtensorflow
#bazel build -c opt --config=cuda :libtensorflow.so
#cd ${BASEDIR}
#ln -s ${BASEDIR}/Tensorflow/bazel-bin/tensorflow/libtensorflow/libtensorflow.so ${INSTALLDIR}/lib


echo "installing Quadrocopter Simulator"
BUILDDIR=${BUILDDIR}/build-simulator
mkdir ${BUILDDIR}
cd ${BUILDDIR}
cmake -DCMAKE_INSTALL_PREFIX=${INSTALLDIR} -DTF_GRAPH_DIR=${BASEDIR}/TensorflowGraph/models/ -DTF_GRAPH_SAVE_DIR=${BASEDIR}/TensorflowGraph/states/ ${BASEDIR}
#-DCMAKE_VERBOSE_MAKEFILE=ON ${BASEDIR}
make
make install
cd ${BASEDIR}
