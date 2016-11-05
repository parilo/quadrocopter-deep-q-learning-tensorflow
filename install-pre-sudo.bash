#!/bin/bash +

#sudo apt-get update
#sudo apt-get upgrade -y

##sudo add-apt-repository ppa:openjdk-r/ppa
##sudo apt-get update
##sudo apt-get install openjdk-8-jdk
##echo "change java to 8"

sudo apt-get install -y build-essential python-pip python-dev git python-numpy swig python-dev default-jdk zip zlib1g-dev cmake

## Blacklist Noveau which has some kind of conflict with the nvidia driver
##echo -e "blacklist nouveau\nblacklist lbm-nouveau\noptions nouveau modeset=0\nalias nouveau off\nalias lbm-nouveau off\n" | sudo tee /etc/modprobe.d/blacklist-nouveau.conf
##echo options nouveau modeset=0 | sudo tee -a /etc/modprobe.d/nouveau-kms.conf
##sudo update-initramfs -u
##sudo reboot # Reboot (annoying you have to do this in 2015!)

sudo apt-get install -y linux-image-extra-virtual

# Install latest Linux headers
sudo apt-get install -y linux-source linux-headers-`uname -r` 

sudo apt-get install libpng-dev libjpeg-dev libxxf86vm1 libxxf86vm-dev libxi-dev libxrandr-dev mesa-common-dev mesa-utils-extra libgl1-mesa-dev libglapi-mesa libglfw-dev -y


##bazel
sudo add-apt-repository ppa:webupd8team/java
sudo apt-get update
sudo apt-get install oracle-java8-installer -y
##sudo apt-get install software-properties-common -y
##echo "deb http://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list
##curl https://storage.googleapis.com/bazel-apt/doc/apt-key.pub.gpg | sudo apt-key add -
##sudo apt-get update && sudo apt-get install bazel -y

sudo update-alternatives --config java
sudo update-alternatives --config javac

sudo apt-get install libglew-dev libglfw3-dev libsqlite3-dev libfontconfig1-dev libgtk-3-dev libcurl4-openssl-dev libtiff-dev libwebp-dev
