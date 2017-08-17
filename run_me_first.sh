#!/usr/bin/env bash


echo "Hi, this file will install dependencies for this project and will attempt to compile the executable."
while true; do
    read -p "Do you wish to proceed?" yn
    case $yn in
        [Yy]* ) bash scripts/install_dependencies.sh; bash scripts/compile_project.sh; break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no.";;
    esac
done
