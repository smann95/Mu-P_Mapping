#!/bin/bash
         #ID    #T_high #T_low #temperature_increment
CH4_data=( C74828 415 115 030 )
CO2_data=( C124389 395 235 020 )
H2_data=( C1333740 420 020 040 )
NE_data=( C7440019 330 030 030)
AR_data=( C7440371 390 090 030)
KR_data=( C7439909 420 120 030)
XE_data=( C7440633 370 170 020 )

P_LOW=0.1
P_HIGH=30.0
P_INC=0.01

rm -r ISOTHERM_REFERENCE_DATA
mkdir -p ISOTHERM_REFERENCE_DATA
cd ISOTHERM_REFERENCE_DATA
for species in CH4 CO2 H2 NE AR KR XE; do
    mkdir ${species}
    cd ${species}
    pwd
    tmp=${species}"_data[1]"
    T_HIGH=${!tmp}
    tmp=${species}"_data[2]"
    T_LOW=${!tmp}
    tmp=${species}"_data[3]"
    INC=${!tmp}
    for temperature in `seq ${T_LOW} ${INC} ${T_HIGH}`; do
        tmp=${species}"_data[0]"
        species_ID=${!tmp}
        link="http://webbook.nist.gov/cgi/fluid.cgi?Action=Data&Wide=on&ID="${species_ID}"&Type=IsoTherm&Digits=5&PLow=${P_LOW}&PHigh=${P_HIGH}&PInc=${P_INC}&T=${temperature}&RefState=DEF&TUnit=K&PUnit=atm&DUnit=mol%2Fl&HUnit=kJ%2Fmol&WUnit=m%2Fs&VisUnit=uPa*s&STUnit=N%2Fm"
        python ../../tables.py ${link} | awk '{print $2,$4}' | tail -n +2 >> ${temperature}
        sed -i '/^\s*$/d' ${temperature}
    done
    cd ..
done
cd ..

#these need special loops because their increments are not sane
N2_data_0=( C7727379 290 110 30 )
N2_data_1=( C7727379 410 330 40 )

P_LOW=0.1
P_HIGH=30.0
P_INC=0.05


cd ISOTHERM_REFERENCE_DATA
mkdir -p N2
cd N2
    pwd
#seq doesn't like 0 increments so just get the one for this temperature
link="http://webbook.nist.gov/cgi/fluid.cgi?Action=Data&Wide=on&ID=C7727379&Type=IsoTherm&Digits=5&PLow=${P_LOW}&PHigh=${P_HIGH}&PInc=${P_INC}&T=77&RefState=DEF&TUnit=K&PUnit=atm&DUnit=mol%2Fl&HUnit=kJ%2Fmol&WUnit=m%2Fs&VisUnit=uPa*s&STUnit=N%2Fm"
python ../../tables.py ${link} | awk '{print $2,$4}' | tail -n +2 >> "077"
sed -i '/^\s*$/d' "077"
cd ..
cd ..

cd ISOTHERM_REFERENCE_DATA
for species in N2; do
  mkdir -p ${species}
  cd ${species}
    pwd
  for special in 0 1; do
    tmp=${species}"_data_$special[1]"
    T_HIGH=${!tmp}
    tmp=${species}"_data_$special[2]"
    T_LOW=${!tmp}
    tmp=${species}"_data_$special[3]"
    INC=${!tmp}
    for temperature in `seq ${T_LOW} ${INC} ${T_HIGH}`; do
        tmp=${species}"_data_$special[0]"
        species_ID=${!tmp}
        link="http://webbook.nist.gov/cgi/fluid.cgi?Action=Data&Wide=on&ID="${species_ID}"&Type=IsoTherm&Digits=5&PLow=${P_LOW}&PHigh=${P_HIGH}&PInc=${P_INC}&T=${temperature}&RefState=DEF&TUnit=K&PUnit=atm&DUnit=mol%2Fl&HUnit=kJ%2Fmol&WUnit=m%2Fs&VisUnit=uPa*s&STUnit=N%2Fm"
        python ../../tables.py ${link} | awk '{print $2,$4}' | tail -n +2 >> ${temperature}
        sed -i '/^\s*$/d' ${temperature}
    done
  done
  cd ..
done
cd ..


HE_data_0=( C7440597 65 5 30 )
HE_data_1=( C7440597 310 100 30 )


cd ISOTHERM_REFERENCE_DATA
for species in HE; do
    mkdir ${species}
    cd ${species}
    pwd
  for special in 0 1; do
    tmp=${species}"_data_$special[1]"
    T_HIGH=${!tmp}
    tmp=${species}"_data_$special[2]"
    T_LOW=${!tmp}
    tmp=${species}"_data_$special[3]"
    INC=${!tmp}
    for temperature in `seq ${T_LOW} ${INC} ${T_HIGH}`; do
        tmp=${species}"_data_$special[0]"
        species_ID=${!tmp}
        link="http://webbook.nist.gov/cgi/fluid.cgi?Action=Data&Wide=on&ID="${species_ID}"&Type=IsoTherm&Digits=5&PLow=${P_LOW}&PHigh=${P_HIGH}&PInc=${P_INC}&T=${temperature}&RefState=DEF&TUnit=K&PUnit=atm&DUnit=mol%2Fl&HUnit=kJ%2Fmol&WUnit=m%2Fs&VisUnit=uPa*s&STUnit=N%2Fm"
        python ../../tables.py ${link} | awk '{print $2,$4}' | tail -n +2 >> ${temperature}
        sed -i '/^\s*$/d' ${temperature}
    done
  done
  cd ..
done
cd ..

