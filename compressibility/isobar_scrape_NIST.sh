#!/bin/bash
         #ID    #T_high #T_low #temperature_increment
CH4_data=( C74828 415 115 30 )
CO2_data=( C124389 395 235 20 )
H2_data=( C1333740 420 20 40 )
NE_data=( C7440019 330 30 30)
AR_data=( C7440371 390 90 30)
KR_data=( C7439909 420 120 30)
XE_data=( C7440633 370 170 20 )

rm -r ISOBAR_REFERENCE_DATA
mkdir -p ISOBAR_REFERENCE_DATA
cd ISOBAR_REFERENCE_DATA
for species in CH4 CO2 H2 NE AR KR XE; do
    mkdir ${species}
    cd ${species}
    pwd
    for pressure in 0.1 1.0 5.0 10.0 20.0 30.0; do
        tmp=${species}"_data[1]"
        T_HIGH=${!tmp}
        tmp=${species}"_data[2]"
        T_LOW=${!tmp}
        tmp=${species}"_data[3]"
        INC=${!tmp}
        tmp=${species}"_data[0]"
        species_ID=${!tmp}
        link="http://webbook.nist.gov/cgi/fluid.cgi?Action=Data&Wide=on&ID="${species_ID}"&Type=IsoBar&Digits=5&P=${pressure}&THigh=${!T_HIGH}&TLow=${!T_LOW}&TInc=${!INC}&RefState=DEF&TUnit=K&PUnit=atm&DUnit=mol%2Fl&HUnit=kJ%2Fmol&WUnit=m%2Fs&VisUnit=uPa*s&STUnit=N%2Fm"
        python ../tables.py ${link} | awk '{print $1,$4}' | tail -n +2 >> ${species}${pressure}
        sed -i '/^\s*$/d' ${species}${pressure}
    done
    cd ..
done
cd ..

#these need special loops because their increments are not sane
N2_data_0=( C7727379 77 77 0 )
N2_data_1=( C7727379 290 110 30 )
N2_data_2=( C7727379 410 330 40 )

cd ISOBAR_REFERENCE_DATA
for species in N2; do
    mkdir ${species}
    cd ${species}
    for pressure in 0.1 1.0 5.0 10.0 20.0 30.0; do
        for special in 0 1 2; do
            tmp=${species}"_data_$special[0]"
            species_ID=${!tmp}
            tmp=${species}"_data_$special[1]"
            T_HIGH=${!tmp}
            tmp=${species}"_data_$special[2]"
            T_LOW=${!tmp}
            tmp=${species}"_data_$special[3]"
            INC=${!tmp}
            link="http://webbook.nist.gov/cgi/fluid.cgi?Action=Data&Wide=on&ID="${species_ID}"&Type=IsoBar&Digits=5&P="${pressure}"&THigh="${T_HIGH}"&TLow="${T_LOW}"&TInc="${INC}"&RefState=DEF&TUnit=K&PUnit=atm&DUnit=mol%2Fl&HUnit=kJ%2Fmol&WUnit=m%2Fs&VisUnit=uPa*s&STUnit=N%2Fm"
            python ../tables.py ${link} | awk '{print $1,$4}' | tail -n +2 >> ${species}${pressure}
            sed -i '/^\s*$/d' ${species}${pressure}
        done
    done
    cd ..
done
cd ..

HE_data_0=( C7440597 65 5 30 )
HE_data_1=( C7440597 310 100 30 )

cd ISOBAR_REFERENCE_DATA
for species in HE; do
    mkdir ${species}
    cd ${species}
    pwd
    for pressure in 0.1 1.0 5.0 10.0 20.0 30.0; do
        for special in 0 1; do
            tmp=${species}"_data_$special[0]"
            species_ID=${!tmp}
            tmp=${species}"_data_$special[1]"
            T_HIGH=${!tmp}
            tmp=${species}"_data_$special[2]"
            T_LOW=${!tmp}
            tmp=${species}"_data_$special[3]"
            INC=${!tmp}
            link="http://webbook.nist.gov/cgi/fluid.cgi?Action=Data&Wide=on&ID="${species_ID}"&Type=IsoBar&Digits=5&P="${pressure}"&THigh="${T_HIGH}"&TLow="${T_LOW}"&TInc="${INC}"&RefState=DEF&TUnit=K&PUnit=atm&DUnit=mol%2Fl&HUnit=kJ%2Fmol&WUnit=m%2Fs&VisUnit=uPa*s&STUnit=N%2Fm"
            python ../tables.py ${link} | awk '{print $1,$4}' | tail -n +2 >> ${species}${pressure}
            sed -i '/^\s*$/d' ${species}${pressure}
        done
    done
    cd ..
done
cd ..
