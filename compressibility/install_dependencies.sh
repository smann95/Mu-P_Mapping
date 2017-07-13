#!/usr/bin/env bash

system=$(uname -s)
if [ "$system" = "Linux" ]; then
    echo "Installing dependencies for Linux"
    sudo apt install libboost-all-dev cmake pip
    sudo -H pip install matplotlib numpy pandas bs4
elif [ "$system" = "Darwin" ]; then
    echo "Installing dependencies for Mac"
    brew install boost cmake pip 
    sudo -H pip install matplotlib numpy pandas bs4
else
    echo "Christ, is this windows? Get boost, cmake, and these python things: matplotlib, numpy, and pandas"
fi


