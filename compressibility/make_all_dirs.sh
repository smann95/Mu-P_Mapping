#!/bin/bash
#this file makes all of the directories of interest for my simulations.
#this is not a neat script but it gets the job done

#TODO: make new input files and change that part in the innermost body of the for loops

current=$(pwd)

for species in H2; do
    echo "Making directories for $species..."
    mkdir -p ${species}
    cd ${species}
        touch these_numbers_are_pressures_in_ATM
        for pres in 0.1 1.0 5.0 10.0 20.0 30.0; do
            mkdir -p ${pres}
            cd ${pres}
            touch these_are_temperatures_in_kelvin
            for temp in 020.0 060.0 100.0 140.0 180.0 220.0 260.0 300.0 340.0 380.0 420.0 ; do
                mkdir -p ${temp}
                cd ${temp};
                for ensemble in npt; do
                    mkdir -p ${ensemble}
                    cd ${ensemble}
                    for model in BSS BUCH DL POLAR_ADJQ; do
                        mkdir -p ${model}
                        cd ${model}
                        awk -v pres="$pres" -v temp="$temp" -v ensemble="$ensemble" -v species="$species" '{
                        gsub(/XXXENSEMBLEXXX/, ensemble);
                        gsub(/XXXTEMPXXX/, temp);
                        gsub(/XXXPRESXXX/, pres);
                        gsub(/XXXSPECIESXXX/, species);
                        print;
                        }' ${current}/inputfiles/${ensemble}"H2input.inp" > ${species}".inp"
                        cp ${current}/modelfiles/FOURBYFOUR/${species}_${model}".pqr" input.pqr
                        cd .. #out of model
                    done
                    cd .. #out of ensemble
                done
                cd .. #out of temperature
            done
            cd .. #out of pressure
        done
    cd .. #out of species
done

for species in CO2; do
    echo "Making directories for $species..."
    mkdir -p ${species}
    cd ${species}
        touch these_numbers_are_pressures_in_ATM
        for pres in 0.1 1.0 5.0 10.0 20.0 30.0; do
            mkdir -p ${pres}
            cd ${pres}
            touch these_are_temperatures_in_kelvin
            for temp in 195.0 215.0 235.0 255.0 275.0 295.0 315.0 335.0 355.0 375.0 395.0; do
                mkdir -p ${temp}
                cd ${temp};
                for ensemble in npt; do
                    mkdir -p ${ensemble}
                    cd ${ensemble}
                    for model in PHAST TRAPPE; do
                        mkdir -p ${model}
                        cd ${model}
                        awk -v pres="$pres" -v temp="$temp" -v ensemble="$ensemble" -v species="$species" '{
                        gsub(/XXXENSEMBLEXXX/, ensemble);
                        gsub(/XXXTEMPXXX/, temp);
                        gsub(/XXXPRESXXX/, pres);
                        gsub(/XXXSPECIESXXX/, species);
                        print;
                        }' ${current}/inputfiles/${ensemble}"H2input.inp" > ${species}".inp"
                        cp ${current}/modelfiles/FOURBYFOUR/${species}_${model}".pqr" input.pqr
                        cd .. #out of model
                    done
                    cd .. #out of ensemble
                done
                cd .. #out of temperature
            done
            cd .. #out of pressure
        done
    cd .. #out of species
done

for species in CH4; do
    echo "Making directories for $species..."
    mkdir -p ${species}
    cd ${species}
        touch these_numbers_are_pressures_in_ATM
        for pres in 0.1 1.0 5.0 10.0 20.0 30.0; do
            mkdir -p ${pres}
            cd ${pres}
            touch these_are_temperatures_in_kelvin
            for temp in 115.0 145.0 175.0 205.0 235.0 265.0 295.0 325.0 355.0 385.0 415.0; do
                mkdir -p ${temp}
                cd ${temp};
                for ensemble in npt; do
                    mkdir -p ${ensemble}
                    cd ${ensemble}
                    for model in 9_site_nonpolar 9_site_polar _trappe; do
                        mkdir -p ${model}
                        cd ${model}
                        awk -v pres="$pres" -v temp="$temp" -v ensemble="$ensemble" -v species="$species" '{
                        gsub(/XXXENSEMBLEXXX/, ensemble);
                        gsub(/XXXTEMPXXX/, temp);
                        gsub(/XXXPRESXXX/, pres);
                        gsub(/XXXSPECIESXXX/, species);
                        print;
                        }' ${current}/inputfiles/${ensemble}"H2input.inp" > ${species}".inp"
                        cp ${current}/modelfiles/FOURBYFOUR/${species}_${model}".pqr" input.pqr
                        cd .. #out of model
                    done
                    cd .. #out of ensemble
                done
                cd .. #out of temperature
            done
            cd .. #out of pressure
        done
    cd .. #out of species
done
