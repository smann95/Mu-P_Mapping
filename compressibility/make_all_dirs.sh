#!/bin/bash
#this file makes all of the directories of interest for my simulations.
#this is not a neat script but it gets the job done

#TODO: make new input files and change that part in the innermost body of the for loops

current=$(pwd)
ensemble=$(npt)

CH4_models=( 9_site_nonpolar 9_site_polar trappe )
CO2_models=( PHAST TRAPPE PHAST_STAR )
H2_models=( BSS BSSP BUCH DL POLAR_ADJQ)
N2_models=( TRAPPE PHAST_STAR_LB )

CH4_temperatures=( 115.0 145.0 175.0 205.0 235.0 265.0 295.0 325.0 355.0 385.0 415.0 )
CO2_temperatures=( 195.0 215.0 235.0 255.0 275.0 295.0 315.0 335.0 355.0 375.0 395.0 )
H2_temperatures=( 020.0 060.0 100.0 140.0 180.0 220.0 260.0 300.0 340.0 380.0 420.0 )
N2_temperatures=( 077.0 110.0 140.0 170.0 200.0 230.0 260.0 290.0 330.0 370.0 410.0 )

for species in CH4 CO2 H2 N2; do
    model_array="${species}_models[@]"
    temp_array="${species}_temperatures[@]"
    mkdir -p ${species}
    cd ${species}
        for model in "${!model_array}"; do
            mkdir -p ${model}
            cd ${model}
                touch these_numbers_are_pressures_in_ATM
                for pres in 00.1 01.0 05.0 10.0 20.0 30.0; do
                    mkdir -p ${pres}
                    cd ${pres}
                        touch these_are_temperatures_in_kelvin
                        for temperature in "${!temp_array}";do
                            mkdir -p ${temperature}
                            cd ${temperature}
                                awk -v pres="$pres" -v temp="$temp" -v ensemble="$ensemble" -v species="$species" '{
                                gsub(/XXXENSEMBLEXXX/, ensemble);
                                gsub(/XXXTEMPXXX/, temp);
                                gsub(/XXXPRESXXX/, pres);
                                gsub(/XXXSPECIESXXX/, species);
                                print;
                                }' ${current}/inputfiles/${species}_${model}"input.inp" > ${species}".inp"
                                cp ${current}/modelfiles/FOURBYFOUR/${species}_${model}".pqr" input.pqr
                            cd .. #out of temperature
                        done
                    cd .. #out of pressure
                done
            cd .. #out of model
        done
    cd .. #out of species
done
