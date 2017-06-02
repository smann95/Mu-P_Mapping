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
    cd ${species}
    for model in "${!array}"; do
        cd .. #out of species into the main directory
        scp -o ControlPath=${ctl} ${user}@${host}:~/muP_mapping/${species}${model}${date_stamp}".dat" .
        cmake-build-debug/compressibility ${species}${model}${date_stamp}".dat"
        split -l 11 ${species}${model}${date_stamp}".dat.OUT" DATA
        cd ${species}/${model} #into the innermost directory (species + model)
            rm DATAa*
        cd ../../ #to the outermost directory (the main working directory)
        mv DATAa* ${species}/${model}
        cd ${species}/${model} #into the innermost directory (species + model)
            echo "Starting $species($model) graphs now..."
            python graphout.py DATAaa
            python graphout.py DATAab
            python graphout.py DATAac
            python graphout.py DATAad
            python graphout.py DATAae
            python graphout.py DATAaf
            mv *.png ../../GRAPHS
        cd ../ #back to species
    done
    cd ..
done

for species in H2 HE; do
    model_array="${species}_models[@]"
    temp_array="${species}_temperatures[@]"
    cd ${species}
        for corrections in FH_ON FH_OFF; do
            cd ${corrections}
                for model in "${!model_array}"; do
                    cd .. #out of species into the main directory
                    scp -o ControlPath=${ctl} ${user}@${host}:~/muP_mapping/${species}${model}${date_stamp}".dat" .
                    cmake-build-debug/compressibility ${species}${model}${date_stamp}".dat"
                    split -l 11 ${species}${model}${date_stamp}".dat.OUT" DATA
                    cd ${species}/${model} #into the innermost directory (species + model)
                        rm DATAa*
                    cd ../../ #to the outermost directory (the main working directory)
                    mv DATAa* ${species}/${model}
                    cd ${species}/${model} #into the innermost directory (species + model)
                        echo "Starting $species($model) graphs now..."
                        python graphout.py DATAaa
                        python graphout.py DATAab
                        python graphout.py DATAac
                        python graphout.py DATAad
                        python graphout.py DATAae
                        python graphout.py DATAaf
                        mv *.png ../../GRAPHS
                    cd .. #back to species
                done
            cd ../
        done
    cd ..
done
