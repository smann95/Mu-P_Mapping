#!/bin/bash

datestamp=$(date +'_%m_%d_%Y')

for species in co2 n2; do
    scp laratelli@itn.rc.usf.edu:~/muP_mapping/${species}${datestamp}".dat" .
    cmake-build-debug/compressibility ${species}${datestamp}".dat"
    mkdir -p ${species} GRAPHS
    cd GRAPHS
    rm *.png
    cd ..
done

split -l 11 "co2"${datestamp}".dat.OUT" DATA
cd co2/
rm DATAa*
cd ..
mv DATAa* co2/

split -l 12 "n2"${datestamp}".dat.OUT" DATA
cd n2/
rm DATAa*
cd ..
mv DATAa* n2/

for species in co2 n2; do
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
