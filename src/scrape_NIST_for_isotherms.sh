#!/bin/bash

CH4_data=( C74828 415 115 030 )
CO2_data=( C124389 395 235 020 )
H2_data=( C1333740 420 020 040 )
NE_data=( C7440019 330 030 030)
AR_data=( C7440371 390 090 030)
KR_data=( C7439909 420 120 030)
XE_data=( C7440633 370 170 020 )

#two datasets for N2 and HE because their increments are not sane
N2_data=( C7727379 290 110 30 )
HE_data=( C7440597 65 5 30 )
N22_data=( C7727379 410 330 40 )
HE2_data=( C7440597 310 100 30 )

function get_data_for()
{
    local -n species_array=$1
    P_LOW=$2
    P_HIGH=$3
    P_INC=0.01
    for species in "${species_array[@]}"; do
        mkdir -p ${species}
        cd ${species}
        tmp=${species}"_data[0]"
        species_ID=${!tmp}
        tmp=${species}"_data[1]"
        T_HIGH=${!tmp}
        tmp=${species}"_data[2]"
        T_LOW=${!tmp}
        tmp=${species}"_data[3]"
        INC=${!tmp}
        for temperature in `seq ${T_LOW} ${INC} ${T_HIGH}`; do
            link="http://webbook.nist.gov/cgi/fluid.cgi?Action=Data&Wide=on&ID="${species_ID}"&Type=IsoTherm&Digits=5&PLow=${P_LOW}&PHigh=${P_HIGH}&PInc=${P_INC}&T=${temperature}&RefState=DEF&TUnit=K&PUnit=atm&DUnit=mol%2Fl&HUnit=kJ%2Fmol&WUnit=m%2Fs&VisUnit=uPa*s&STUnit=N%2Fm"
            python ../../tables.py ${link} | awk '{print $2,$4}' | tail -n +2 >> ${temperature}
            sed -i '/^\s*$/d' ${temperature}
        done
        cd ..
    done
}

echo "Working..."

rm -rf ISOTHERM_REFERENCE_DATA
mkdir ISOTHERM_REFERENCE_DATA
cd ISOTHERM_REFERENCE_DATA

first_array=("CH4" "CO2" "H2" "NE" "AR" "KR" "XE")
second_array=("N2" "HE")
third_array=("N22" "HE2")

pres_low=0.1
pres_max=5
while [ "${pres_max}" -le 30 ]
do
    get_data_for first_array ${pres_low} ${pres_max}
    get_data_for second_array ${pres_low} ${pres_max}
    get_data_for third_array ${pres_low} ${pres_max}
    pres_low=$( echo "${pres_low} + 5" | bc)
    pres_max=$(( $pres_max + 5 ))
done

cp -R N22/. N2/
cp -R HE2/. HE/

rm -r N22 HE2

cd ..

echo "Done!"
