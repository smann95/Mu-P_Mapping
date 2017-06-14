#!/bin/bash

#RUN THIS PROGRAM ON THE COMPUTER YOU WILL BE DOING DATA ANALYSIS ON
#RUN IT BEFORE DATA_ANALYSIS
#for me, this is my laptop/work computer

trap "[ -z "$ctl" ] || ssh -S $ctl -O exit $user@$host" EXIT # closes conn, deletes fifo

read -p "Host to connect to? " host
read -p "User to login with? " user

BOLD="\n$(tput bold)"; NORMAL="$(tput sgr0)"
echo -e "${BOLD}Create authenticated persistent control master connection:${NORMAL}"
sshfifos=~/.ssh/controlmasters
[ -d ${sshfifos} ] || mkdir -p ${sshfifos}; chmod 755 ${sshfifos}
ctl=${sshfifos}/${user}@${host}:22 # ssh stores named socket for open ctrl conn here

ssh -fNMS ${ctl} ${user}@${host}  # Control Master: Prompts password then persists in background

if [ "$host" = "bridges.psc.edu" ]; then
    scp_path="/pylon5/ch3benp/luciano2/Mu-P_Mapping/compressibility/"
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
    for model in "${!array}"; do
        scp -o ControlPath=${ctl} ${user}@${host}:${scp_path}${species}${model}${date_stamp}".dat" .
    done
done

for species in H2 HE; do
    model_array="${species}_models[@]"
        for corrections in FH_ON FH_OFF; do
                for model in "${!model_array}"; do
                    scp -o ControlPath=${ctl} ${user}@${host}:${scp_path}${species}${corrections}${model}${date_stamp}".dat" .
                done
        done
done
