#!/bin/bash

#RUN THIS PROGRAM ON THE COMPUTER YOU WILL BE DOING DATA ANALYSIS ON
#for me, this is my laptop/work computer

trap "[ -z "$ctl" ] || ssh -S $ctl -O exit $user@$host" EXIT # closes conn, deletes fifo

read -p "Host to connect to? " host
read -p "User to login with? " user

BOLD="\n$(tput bold)"; NORMAL="$(tput sgr0)"
echo -e "${BOLD}Create authenticated persistent control master connection:${NORMAL}"
sshfifos=~/.ssh/controlmasters
[ -d ${sshfifos} ] || mkdir -p $sshfifos; chmod 755 ${sshfifos}
ctl=${sshfifos}/${user}@${host}:22 # ssh stores named socket for open ctrl conn here

ssh -fNMS $ctl $user@$host  # Control Master: Prompts password then persists in background

if [ "$host" = "bridges.psc.edu" ]; then
    scp_path="/pylon5/ch3benp/luciano2/MuP_Mapping/compressibility/"
fi
if [ "$host" = "itn.rc.usf.edu" ]; then
    scp_path="~/repos/MuP_Mapping/compressibility/"
fi
date_stamp=$(date +'_%m_%d_%Y')
mkdir -p GRAPHS ; cd GRAPHS ; rm *.png ; cd ..

CH4_models=( 9_site_nonpolar 9_site_polar trappe )
CO2_models=( PHAST TRAPPE PHAST_STAR )
H2_models=( BSS BSSP BUCH DL POLAR_ADJQ)
N2_models=( TRAPPE PHAST_STAR_LB PHAST_LB)
HE_models=( HE )
NE_models=( NE )
AR_models=( AR )
KR_models=( KR )
XE_models=( XE )

for species in CH4 CO2 NE AR KR XE; do
    array="${species}_models[@]"
    mkdir -p ${species}
    cd ${species}
    for model in "${!array}"; do
        mkdir -p ${model}
        cd .. #out of species into the main directory
        scp -o ControlPath=${ctl} ${user}@${host}:${scp_path}${species}${model}${date_stamp}".dat" .
        cmake-build-debug/compressibility ${species}${model}${date_stamp}".dat"
        split -l 11 ${species}${model}${date_stamp}".dat.OUT" DATA
        cd ${species}/${model} #into the innermost directory (species + model)
            rm DATAa*
        cd ../../ #to the outermost directory (the main working directory)
        mv DATAa* ${species}/${model}
        cd ${species}/${model} #into the innermost directory (species + model)
            echo "Starting $species($model) graphs now..."
            python ../../graph_most.py DATAaa ${species} ${model}
            python ../../graph_most.py DATAab ${species} ${model}
            python ../../graph_most.py DATAac ${species} ${model}
            python ../../graph_most.py DATAad ${species} ${model}
            python ../../graph_most.py DATAae ${species} ${model}
            python ../../graph_most.py DATAaf ${species} ${model}
            mv *.png ../../GRAPHS
        cd ../ #back to species
    done
    cd ..
done

for species in H2 HE; do
    mkdir -p ${species}
    model_array="${species}_models[@]"
    temp_array="${species}_temperatures[@]"
    cd ${species}
        for corrections in FH_ON FH_OFF; do
            mkdir -p ${corrections}
            cd ${corrections}
                for model in "${!model_array}"; do
                    mkdir -p ${model}
                    cd ../.. #out of species into the main directory
                    scp -o ControlPath=${ctl} ${user}@${host}:${scp_path}${species}${corrections}${model}${date_stamp}".dat" .
                    cmake-build-debug/compressibility ${species}${corrections}${model}${date_stamp}".dat"
                    split -l 11 ${species}${corrections}${model}${date_stamp}".dat.OUT" DATA
                    cd ${species}/${corrections}/${model} #into the innermost directory
                        rm DATAa*
                    cd ../../../ #to the outermost directory (the main working directory)
                    mv DATAa* ${species}/${corrections}/${model}/
                    cd ${species}/${corrections}/${model} #into the innermost directory (species + model)
                        python ../../graph_H2He.py DATAaa ${species} ${corrections} ${model}
                        python ../../graph_H2He.py DATAab ${species} ${corrections} ${model}
                        python ../../graph_H2He.py DATAac ${species} ${corrections} ${model}
                        python ../../graph_H2He.py DATAad ${species} ${corrections} ${model}
                        python ../../graph_H2He.py DATAae ${species} ${corrections} ${model}
                        python ../../graph_H2He.py DATAaf ${species} ${corrections} ${model}
                        mv *.png ../../GRAPHS
                    cd .. #back to species
                done
            cd ../
        done
    cd ..
done
