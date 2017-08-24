#!/usr/bin/env bash

mkdir -p RESULTS
cd RESULTS
    current=$(pwd)
cd ..

ensemble=npt

H2_models=( luo_lloyd )

H2_temperatures=( 077.0 100.0 200.0 )

for species in H2; do
    model_array="${species}_models[@]"
    temp_array="${species}_temperatures[@]"
    cd ${species}
        for corrections in FH_ON FH_OFF; do
            cd ${corrections}
                for model in "${!model_array}"; do
                    sum=0
                    cd ${model}
                        for pres in 9.86923 19.7385 29.6077 39.4769 49.3462 59.2154 69.0846 78.9538 88.8231 98.6923 108.562 118.431 128.3 138.169 148.038 157.908 167.777 177.646 187.515 197.385 ; do
                            cd ${pres}
                                for temperature in "${!temp_array}";do
                                    cd ${temperature}
                                        volume=$(tail -20 *.log | grep "OUTPUT: volume "|sed -nr '/[0-9]/{s/^[^0-9]*([0-9]+\.?[0-9]*).*$/\1/p;q}')
                                        density=$(tail -20 *.log | grep "OUTPUT: density"|sed -nr '/[0-9]/{s/^[^0-9]*([0-9]+\.?[0-9]*).*$/\1/p;q}')
                                        ((sum++))
                                        echo "$temperature $pres $volume $density" >> ${current}/TEMP
                                    cd .. #out of temperature
                                done
                            cd .. #out of pressure
                        done
                        echo ${species}                >> ${current}/${species}${corrections}${model}${datestamp}".dat"
                        echo ${model}                  >> ${current}/${species}${corrections}${model}${datestamp}".dat"
                        echo ${sum}                    >> ${current}/${species}${corrections}${model}${datestamp}".dat"
                        echo ${corrections}            >> ${current}/${species}${corrections}${model}${datestamp}".dat"
                        echo "#TEMP #PRES #V #DENSITY" >> ${current}/${species}${corrections}${model}${datestamp}".dat"
                        cat ${current}/TEMP            >> ${current}/${species}${corrections}${model}${datestamp}".dat"
                        rm ${current}/TEMP
                    cd .. #out of model
                done
            cd .. #out of corrections
        done;
    cd .. #out of species
done
