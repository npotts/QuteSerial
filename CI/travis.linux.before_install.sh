#!/bin/bash
#shamelessly stolen from http://github.com/Mudlet/Mudlet
set -ev
sudo apt-add-repository ppa:ubuntu-sdk-team/ppa -y
# newer GCC version, 4.7
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
#sudo add-apt-repository ppa:boost-latest/ppa -y
sudo apt-get update
sudo apt-cache search qt5
