#!/bin/bash

trap "[ -z "$ctl" ] || ssh -S $ctl -O exit $user@$host" EXIT # closes conn, deletes fifo

read -p "Host to connect to? " host
read -p "User to login with? " user

BOLD="\n$(tput bold)"; NORMAL="$(tput sgr0)"
echo -e "${BOLD}Create authenticated persistent control master connection:${NORMAL}"
sshfifos=~/.ssh/controlmasters
[ -d ${sshfifos} ] || mkdir -p $sshfifos; chmod 755 ${sshfifos}
ctl=${sshfifos}/${user}@${host}:22 # ssh stores named socket for open ctrl conn here

ssh -fNMS $ctl $user@$host  # Control Master: Prompts password then persists in background


date_stamp=$(date +'_%m_%d_%Y')
mkdir -p GRAPHS
cd GRAPHS
rm *.png
cd ..

for species in H2; do
    mkdir -p ${species}
    cd ${species}
    for model in BSS BUCH DL POLAR_ADJQ BSSP; do
        mkdir -p ${model}
        scp -o ControlPath=${ctl} ${user}@${host}:~/muP_mapping/${species}${model}${date_stamp}".dat" .
        cmake-build-debug/compressibility ${species}${model}${date_stamp}".dat"
        split -l 11 ${species}${model}${date_stamp}".dat.OUT" DATA
        cd ${species}/${model}
        rm DATAa*
        cd ../../
        mv DATAa* ${species}/${model}
        cd ${species}/${model}
        echo "Starting $species($model) graphs now..."
        python graphout.py DATAaa
        python graphout.py DATAab
        python graphout.py DATAac
        python graphout.py DATAad
        python graphout.py DATAae
        python graphout.py DATAaf
        mv *.png ../../GRAPHS
    done;
    cd .. #out of species
done

for species in CO2; do
    mkdir -p ${species}
    cd ${species}
    for model in PHAST TRAPPE PHAST_STAR; do
        mkdir -p ${model}
        scp -o ControlPath=${ctl} ${user}@${host}:~/muP_mapping/${species}${model}${date_stamp}".dat" .
        cmake-build-debug/compressibility ${species}${model}${date_stamp}".dat"
        split -l 11 ${species}${model}${date_stamp}".dat.OUT" DATA
        cd ${species}/${model}
        rm DATAa*
        cd ../../
        mv DATAa* ${species}/${model}
        cd ${species}/${model}
        echo "Starting $species($model) graphs now..."
        python graphout.py DATAaa
        python graphout.py DATAab
        python graphout.py DATAac
        python graphout.py DATAad
        python graphout.py DATAae
        python graphout.py DATAaf
        mv *.png ../../GRAPHS
    done;
    cd .. #out of species
done

for species in CH4; do
    mkdir -p ${species}
    cd ${species}
    for model in 9_site_nonpolar 9_site_polar trappe; do
        mkdir -p ${model}
        scp -o ControlPath=${ctl} ${user}@${host}:~/muP_mapping/${species}${model}${date_stamp}".dat" .
        cmake-build-debug/compressibility ${species}${model}${date_stamp}".dat"
        split -l 11 ${species}${model}${date_stamp}".dat.OUT" DATA
        cd ${species}/${model}
        rm DATAa*
        cd ../../
        mv DATAa* ${species}/${model}
        cd ${species}/${model}
        echo "Starting $species($model) graphs now..."
        python graphout.py DATAaa
        python graphout.py DATAab
        python graphout.py DATAac
        python graphout.py DATAad
        python graphout.py DATAae
        python graphout.py DATAaf
        mv *.png ../../GRAPHS
    done;
    cd .. #out of species
done

for species in N2; do
    mkdir -p ${species}
    cd ${species}
    for model in TRAPPE PHAST_STAR_LB; do
        mkdir -p ${model}
        scp -o ControlPath=${ctl} ${user}@${host}:~/muP_mapping/${species}${model}${date_stamp}".dat" .
        cmake-build-debug/compressibility ${species}${model}${date_stamp}".dat"
        split -l 11 ${species}${model}${date_stamp}".dat.OUT" DATA
        cd ${species}/${model}
        rm DATAa*
        cd ../../
        mv DATAa* ${species}/${model}
        cd ${species}/${model}
        echo "Starting $species($model) graphs now..."
        python graphout.py DATAaa
        python graphout.py DATAab
        python graphout.py DATAac
        python graphout.py DATAad
        python graphout.py DATAae
        python graphout.py DATAaf
        mv *.png ../../GRAPHS
    done
    cd .. #out of species
done


N2_models=( TRAPPE PHAST_STAR_LB )

for species in N2; do
    mkdir -p ${species}
    cd ${species}
    for model in ${species}_models; do
        mkdir -p ${model}
        scp -o ControlPath=${ctl} ${user}@${host}:~/muP_mapping/${species}${model}${date_stamp}".dat" .
        cmake-build-debug/compressibility ${species}${model}${date_stamp}".dat"
        split -l 11 ${species}${model}${date_stamp}".dat.OUT" DATA
        cd ${species}/${model}
        rm DATAa*
        cd ../../
        mv DATAa* ${species}/${model}
        cd ${species}/${model}
        echo "Starting $species($model) graphs now..."
        python graphout.py DATAaa
        python graphout.py DATAab
        python graphout.py DATAac
        python graphout.py DATAad
        python graphout.py DATAae
        python graphout.py DATAaf
        mv *.png ../../GRAPHS
    done
    cd .. #out of species
done
