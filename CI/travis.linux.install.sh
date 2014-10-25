#!/bin/bash
#shamelessly stolen from http://github.com/Mudlet/Mudlet
set -ev
sudo apt-get install \
  build-essential \
  qt5-default qtmultimedia5-dev qttools5-dev libqt5widgets5 libqt5serialport5\
  gcc-4.7 \
  g++-4.7
sudo update-alternatives --remove gcc /usr/bin/gcc-4.6
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.7 60 --slave /usr/bin/g++ g++ /usr/bin/g++-4.7
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.6 40 --slave /usr/bin/g++ g++ /usr/bin/g++-4.6
