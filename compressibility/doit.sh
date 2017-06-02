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

for species in co2 n2 h2; do
    scp -o ControlPath=${ctl} ${user}@${host}:~/muP_mapping/${species}${date_stamp}".dat" .
    cmake-build-debug/compressibility ${species}${date_stamp}".dat"
    mkdir -p ${species} GRAPHS
    cd GRAPHS
    rm *.png
    cd ..
done

for species in co2 h2;do
    split -l 11 ${species}${date_stamp}".dat.OUT" DATA
    cd ${species}
    rm DATAa*
    cd ..
    mv DATAa* ${species}
done

split -l 12 "n2"${date_stamp}".dat.OUT" DATA
cd n2/
rm DATAa*
cd ..
mv DATAa* n2/

for species in co2 n2 h2; do
    cd ${species}
        echo "Starting $species graphs now..."
        python graphout.py DATAaa
        python graphout.py DATAab
        python graphout.py DATAac
        python graphout.py DATAad
        python graphout.py DATAae
        python graphout.py DATAaf
        mv *.png ../GRAPHS
    cd ..
done
