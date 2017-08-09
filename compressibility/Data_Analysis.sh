#!/bin/bash

date_stamp=$(date +'_%m_%d_%Y')
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
mkdir -p GRAPHS/compressibilities

for species in CH4 CO2 N2 NE AR KR XE; do
    array="${species}_models[@]"
    mkdir -p ${species}
    echo ${species}
    cd ${species}
    for model in "${!array}"; do
        mkdir -p ${model}
        cd .. #out of species into the main directory
        ./compressibility ${species}${model}${date_stamp}".dat"
        split -l 11 ${species}${model}${date_stamp}".dat.OUT" DATA
        cd ${species}/${model} #into the innermost directory (species + model)
            rm DATAa* 2> /dev/null
        cd ../../ #to the outermost directory (the main working directory)
        mv DATAa* ${species}/${model}
        cd ${species}/${model} #into the innermost directory (species + model)
            echo "Starting $species($model) graphs now..."
            python ../../graph_most_compressibilities.py DATAaa ${species} ${model}
            python ../../graph_most_compressibilities.py DATAab ${species} ${model}
            python ../../graph_most_compressibilities.py DATAac ${species} ${model}
            python ../../graph_most_compressibilities.py DATAad ${species} ${model}
            python ../../graph_most_compressibilities.py DATAae ${species} ${model}
            python ../../graph_most_compressibilities.py DATAaf ${species} ${model}
            mv *.png ../../GRAPHS/compressibilities
        cd ../ #back to species
    done
    cd ..
done

for species in H2 HE; do
    mkdir -p ${species}
    model_array="${species}_models[@]"
    temp_array="${species}_temperatures[@]"
    echo ${species}
    cd ${species}
        for corrections in FH_ON FH_OFF; do
            mkdir -p ${corrections}
            cd ${corrections}
                for model in "${!model_array}"; do
                    mkdir -p ${model}
                    cd ../.. #out of species into the main directory
                    ./compressibility ${species}${corrections}${model}${date_stamp}".dat"
                    split -l 11 ${species}${corrections}${model}${date_stamp}".dat.OUT" DATA
                    cd ${species}/${corrections}/${model} #into the innermost directory
                        rm DATAa* 2> /dev/null
                    cd ../../../ #to the outermost directory (the main working directory)
                    mv DATAa* ${species}/${corrections}/${model}/
                    cd ${species}/${corrections}/${model} #into the innermost directory (species + model)
                        echo "Starting $species($model[$corrections]) graphs now..."
                        python ../../../graph_H2He_compressibilities.py DATAaa ${species} ${corrections} ${model}
                        python ../../../graph_H2He_compressibilities.py DATAab ${species} ${corrections} ${model}
                        python ../../../graph_H2He_compressibilities.py DATAac ${species} ${corrections} ${model}
                        python ../../../graph_H2He_compressibilities.py DATAad ${species} ${corrections} ${model}
                        python ../../../graph_H2He_compressibilities.py DATAae ${species} ${corrections} ${model}
                        python ../../../graph_H2He_compressibilities.py DATAaf ${species} ${corrections} ${model}
                        mv *.png ../../GRAPHS/compressibilities
                    cd .. #back to species
                done
            cd ../
        done
    cd ..
done
