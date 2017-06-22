#!/usr/bin/env bash

system=$(uname -s)
if [ "$system" = "Linux" ]; then
    echo "Installing dependencies for Linux"
    sudo apt install libboost-all-dev cmake
    sudo -H pip install matplotlib numpy pandas
elif [ "$system" = "Darwin" ]; then
    echo "Installing dependencies for Mac"
    brew install boost cmake
    sudo -H pip install matplotlib numpy pandas
else
    echo "Christ, is this windows? Get boost, cmake, and these python things: matplotlib, numpy, and pandas"
fi


