#!/bin/bash
           #ID    #TH  #TL #INC
data_CH4=( C74824 415 115 30 )
data_CO2=( C124389 395 195 20 )
data_H2=( C1333740 420 20 40 )
data_NE=( C7440019 330 30 30)
data_AR=( C7440371 390 90 30)
data_KR=( C7439909 420 120 30)
data_XE=( C7440633 370 170 20 )

mkdir -p data
cd data
#for species in CH4 CO2 H2 Ne AR KR XE; do
for species in CH4
    for pressure in 00.1 001 005 010 020 030; do
        #array="data_${species}[@]"
        #echo ${!array[1]}
        #link='http://webbook.nist.gov/cgi/fluid.cgi?Action=Data&Wide=on&ID='"${!array[0]}"'&Type=IsoBar&Digits=5&P='${pressure}'&THigh='${!array[1]}'&TLow='${!array[2]}'&TInc='${!array[3]}'&RefState=DEF&TUnit=K&PUnit=atm&DUnit=mol%2Fl&HUnit=kJ%2Fmol&WUnit=m%2Fs&VisUnit=uPa*s&STUnit=N%2Fm'
        this_id=${CH4_ID}
        link='http://webbook.nist.gov/cgi/fluid.cgi?Action=Data&Wide=on&ID=C74828&Type=IsoBar&Digits=5&P='${pressure}'&THigh='${T_HIGH}'&TLow='${T_LOW}'&TInc='${T_INC}'&RefState=DEF&TUnit=K&PUnit=atm&DUnit=mol%2Fl&HUnit=kJ%2Fmol&WUnit=m%2Fs&VisUnit=uPa*s&STUnit=N%2Fm'
        python ../tables.py ${link} | awk '{print $1,$4}' | tail -n +2 >> ${species}${pressure}
    done
done
cd ..

#these need special loops because their increments are not sane
data_N2=(  C7727379)
data_HE_1=( C7440597 )
data_HE_2=( C7440597 )
