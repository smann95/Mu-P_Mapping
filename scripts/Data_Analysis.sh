#!/bin/bash

date_stamp="_08_14_2017"
#RUN THIS PROGRAM AFTER CLUSTER_SCRAPE

CH4_models=( 9_site_nonpolar 9_site_polar trappe )
CO2_models=( PHAST TRAPPE PHAST_STAR )
H2_models=( BSS BSSP BUCH DL POLAR_ADJQ)
N2_models=( TRAPPE PHAST_STAR_LB PHAST_LB)
HE_models=( HE )
NE_models=( NE )
AR_models=( AR )
KR_models=( KR )
XE_models=( XE )

rm -rf GRAPHS
mkdir -p ../GRAPHS/

for species in CH4 CO2 N2 NE AR KR XE; do
    array="${species}_models[@]"
    mkdir -p ../GRAPHS/${species}
    echo ${species}
    cd ../GRAPHS/${species}
    for model in "${!array}"; do
        mkdir -p ${model}
        cd .. #out of species into the main directory
        ../cmake-build-debug/mapMu-P ../data/${species}${model}${date_stamp}".dat"
        split -l 11 ../data/${species}${model}${date_stamp}".dat.OUT.FINAL" DATA
        cd ${species}/${model} #into the innermost directory (species + model)
            rm DATAa* 2> /dev/null
        cd ../../ #to the outermost directory (the main working directory)
        mv DATAa* ${species}/${model}
        cd ${species}/${model} #into the innermost directory (species + model)
            echo "Starting $species($model) graphs now..."
            python ../../../scripts/graph_most_fugacities.py DATAaa ${species} ${model}
            python ../../../scripts/graph_most_fugacities.py DATAab ${species} ${model}
            python ../../../scripts/graph_most_fugacities.py DATAac ${species} ${model}
            python ../../../scripts/graph_most_fugacities.py DATAad ${species} ${model}
            python ../../../scripts/graph_most_fugacities.py DATAae ${species} ${model}
            python ../../../scripts/graph_most_fugacities.py DATAaf ${species} ${model}
            mv *.png ../../../GRAPHS/
        cd ../ #back to species
    done
    cd ..
done

cd ~/Dropbox/Mu-P_Mapping/scripts/

for species in H2 HE; do
    mkdir -p ../GRAPHS/${species}
    model_array="${species}_models[@]"
    temp_array="${species}_temperatures[@]"
    echo ${species}
    cd ../GRAPHS/${species}
        for corrections in FH_ON FH_OFF; do
            mkdir -p ${corrections}
            cd ${corrections}
                for model in "${!model_array}"; do
                    mkdir -p ${model}
                    cd ../../ #out of species into the main directory
                    ../cmake-build-debug/mapMu-P ../data/${species}${corrections}${model}${date_stamp}".dat"
                    split -l 11 ../data/${species}${corrections}${model}${date_stamp}".dat.OUT.FINAL" DATA
                    cd ${species}/${corrections}/${model} #into the innermost directory
                        rm DATAa* 2> /dev/null
                    cd ../../../ #to the outermost directory (the main working directory)
                    mv DATAa* ${species}/${corrections}/${model}/
                    cd ${species}/${corrections}/${model} #into the innermost directory (species + model)
                        echo "Starting $species($model[$corrections]) graphs now..."
                        python ../../../../scripts/graph_H2He_fugacities.py DATAaa ${species} ${corrections} ${model}
                        python ../../../../scripts/graph_H2He_fugacities.py DATAab ${species} ${corrections} ${model}
                        python ../../../../scripts/graph_H2He_fugacities.py DATAac ${species} ${corrections} ${model}
                        python ../../../../scripts/graph_H2He_fugacities.py DATAad ${species} ${corrections} ${model}
                        python ../../../../scripts/graph_H2He_fugacities.py DATAae ${species} ${corrections} ${model}
                        python ../../../../scripts/graph_H2He_fugacities.py DATAaf ${species} ${corrections} ${model}
                        mv *.png ../../../
                    cd .. #back to species
                done
            cd ../
        done
    cd ..
done
