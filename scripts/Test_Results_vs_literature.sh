#!/usr/bin/env bash

ensemble=npt


H2_models=( luo_lloyd )

H2_temperatures=( 020.0 060.0 077.0 100.0 140.0 180.0 200.0 220.0 260.0 293.0 300.0 340.0 )

#H2 and He have a separate loop because they require Feynman_Hibbs corrections
for species in H2; do
    model_array="${species}_models[@]"
    temp_array="${species}_temperatures[@]"
    mkdir -p ${species}
    cd ${species}
        for corrections in FH_ON FH_OFF; do
            mkdir -p ${corrections}
            cd ${corrections}
                for model in "${!model_array}"; do
                    mkdir -p ${model}
                    cd ${model}
                        touch these_numbers_are_pressures_in_ATM
                        for pres in 9.8692e-10 9.8692e-9 4.9346e-8 9.8692e-8 1.4804e-7 1.9738e-7; do
                            mkdir -p ${pres}
                            cd ${pres}
                                touch these_are_temperatures_in_kelvin
                                for temperature in "${!temp_array}";do mkdir -p ${temperature}
                                    cd ${temperature}
                                        awk -v pres="$pres" -v temp="$temperature" -v ensemble="$ensemble" -v species="$species" '{
                                        gsub(/XXXENSEMBLEXXX/, ensemble);
                                        gsub(/XXXTEMPXXX/, temp);
                                        gsub(/XXXPRESXXX/, pres);
                                        gsub(/XXXSPECIESXXX/, species);
                                        print;
                                        }' ~/Dropbox/Mu-P_Mapping/misc/inputfiles/${ensemble}${species}${corrections}"input.inp" > ${species}".inp"
                                        cp ~/Dropbox/Mu-P_Mapping/misc/modelfiles/FOURBYFOUR/${species}_${model}".pqr" input.pqr
                                    cd .. #out of temperature
                                done
                            cd .. #out of pressure
                        done
                    cd .. #out of model
                done
            cd .. #out of corrections
        done;
    cd .. #out of species
done


for species in H2; do
    model_array="${species}_models[@]"
    temp_array="${species}_temperatures[@]"
    cd ${species}
        for corrections in FH_ON FH_OFF; do
            cd ${corrections}
                for model in "${!model_array}"; do
                    cd ${model}
                        touch these_numbers_are_pressures_in_ATM
                        for pres in 9.8692e-10 9.8692e-9 4.9346e-8 9.8692e-8 1.4804e-7 1.9738e-7; do
                            cd ${pres}
                                touch these_are_temperatures_in_kelvin
                                for temperature in "${!temp_array}";do
                                    cd ${temperature}
                                        cp ~/Dropbox/Mu-P_Mapping/scripts/submit.sh .
                                        sed -i "s/XXXJOBNAMEXXX/${species}_${corrections}_${model}"_P"${pres}"_T"${temperature}/g" submit.sh
                                        #sbatch submit.sh
                                        pwd
                                    cd .. #out of temperature
                                done
                            cd .. #out of pressure
                        done
                    cd .. #out of model
                done
            cd .. #out of corrections
        done;
    cd .. #out of species
done
