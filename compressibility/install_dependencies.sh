#!/usr/bin/env bash

system=$(uname -s)
if [[ system -eq "Linux" ]]; then
    echo "Installing dependencies for Linux"
    sudo apt install libboost-all-dev cmake
elif [[ system -eq "Darwin" ]]; then
    echo "Installing dependencies for Mac"
    brew install boost cmake
fi

sudo -H pip install matplotlib numpy pandas

