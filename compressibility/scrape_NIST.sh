#!/bin/bash
         #ID    #T_high #T_low #temperature_increment
CH4_data=( C74828 415 115 30 )
CO2_data=( C124389 395 235 20 )
H2_data=( C1333740 420 20 40 )
NE_data=( C7440019 330 30 30)
AR_data=( C7440371 390 90 30)
KR_data=( C7439909 420 120 30)
XE_data=( C7440633 370 170 20 )

rm -r data
mkdir -p data
cd data
for species in CH4 CO2 H2 NE AR KR XE; do
    for pressure in 00.1 001 005 010 020 030; do
        species_ID="${species}_data[0]}"
        T_HIGH="${species}_data[1]}"
        T_LOW="${species}_data[2]}"
        INC="${species}_data[3]}"
        link="http://webbook.nist.gov/cgi/fluid.cgi?Action=Data&Wide=on&ID=${!species_ID}&Type=IsoBar&Digits=5&P=${pressure}&THigh=${!T_HIGH}&TLow=${!T_LOW}&TInc=${!INC}&RefState=DEF&TUnit=K&PUnit=atm&DUnit=mol%2Fl&HUnit=kJ%2Fmol&WUnit=m%2Fs&VisUnit=uPa*s&STUnit=N%2Fm"
        final_link="${link}"
        python ../tables.py ${final_link} | awk '{print $1,$4}' | tail -n +2 >> ${species}${pressure}
        sed -i '/^\s*$/d' ${species}${pressure}
    done
done
cd ..

#these need special loops because their increments are not sane
N2_data_0=( C7727379 77 77 0 )
N2_data_1=( C7727379 290 110 30 )
N2_data_2=( C7727379 410 330 40 )
HE_data_0=( C7440597 65 5 30 )
HE_data_1=( C7440597 310 100 30 )

cd data
for species in N2; do
    for pressure in 00.1 001 005 010 020 030; do
        for special in 0 1 2; do
            species_ID="${species}_data_${special}[0]}"
            T_HIGH="${species}_data_${special}[1]}"
            T_LOW="${species}_data_${special}[2]}"
            INC="${species}_data_${special}[3]}"
            link="http://webbook.nist.gov/cgi/fluid.cgi?Action=Data&Wide=on&ID=${!species_ID}&Type=IsoBar&Digits=5&P=${pressure}&THigh=${!T_HIGH}&TLow=${!T_LOW}&TInc=${!INC}&RefState=DEF&TUnit=K&PUnit=atm&DUnit=mol%2Fl&HUnit=kJ%2Fmol&WUnit=m%2Fs&VisUnit=uPa*s&STUnit=N%2Fm"
            final_link="${link}"
            python ../tables.py ${final_link} | awk '{print $1,$4}' | tail -n +2 >> ${species}${pressure}
            sed -i '/^\s*$/d' ${species}${pressure}
        done
    done
done
cd ..

cd data
for species in HE; do
    for pressure in 00.1 001 005 010 020 030; do
        for special in 0 1; do
            species_ID="${species}_data_${special}[0]}"
            T_HIGH="${species}_data_${special}[1]}"
            T_LOW="${species}_data_${special}[2]}"
            INC="${species}_data_${special}[3]}"
            link="http://webbook.nist.gov/cgi/fluid.cgi?Action=Data&Wide=on&ID=${!species_ID}&Type=IsoBar&Digits=5&P=${pressure}&THigh=${!T_HIGH}&TLow=${!T_LOW}&TInc=${!INC}&RefState=DEF&TUnit=K&PUnit=atm&DUnit=mol%2Fl&HUnit=kJ%2Fmol&WUnit=m%2Fs&VisUnit=uPa*s&STUnit=N%2Fm"
            final_link="${link}"
            python ../tables.py ${final_link} | awk '{print $1,$4}' | tail -n +2 >> ${species}${pressure}
            sed -i '/^\s*$/d' ${species}${pressure}
        done
    done
done
cd ..
