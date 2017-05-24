#!/usr/bin/bash
scp laratelli@itn.rc.usf.edu:~/muP_mapping/co2_05_24_2017.dat .
scp laratelli@itn.rc.usf.edu:~/muP_mapping/n2_05_24_2017.dat .

cmake-build-debug/compressibility co2_05_24_2017.dat n2_05_24_2017.dat

mkdir -p co2 n2

split -l 11 co2_05_24_2017.dat.OUT DATA
cd co2/
rm DATAa*
cd ..
mv DATAa* co2/

split -l 12 n2_05_24_2017.dat.OUT DATA
cd n2/
rm DATAa*
cd ..
mv DATAa* n2/

cd co2/
python graphout.py DATAaa
python graphout.py DATAab
python graphout.py DATAac
python graphout.py DATAad
python graphout.py DATAae
python graphout.py DATAaf
cd ..

cd n2/
python graphout.py DATAaa
python graphout.py DATAab
python graphout.py DATAac
python graphout.py DATAad
python graphout.py DATAae
python graphout.py DATAaf
cd ..
