#!/bin/bash

#this runs on the supercomputer you did your work on
#re-runs only those jobs that are not currently in squeue


current=$(pwd)
ensemble=npt

CH4_models=( 9_site_nonpolar 9_site_polar trappe )
CO2_models=( PHAST TRAPPE PHAST_STAR )
H2_models=( BSS BSSP BUCH DL POLAR_ADJQ)
N2_models=( TRAPPE PHAST_STAR_LB PHAST_LB)
HE_models=( HE )
NE_models=( NE )
AR_models=( AR )
KR_models=( KR )
Xe_models=( XE )

CH4_temperatures=( 115.0 145.0 175.0 205.0 235.0 265.0 295.0 325.0 355.0 385.0 415.0 )
CO2_temperatures=( 195.0 215.0 235.0 255.0 275.0 295.0 315.0 335.0 355.0 375.0 395.0 )
H2_temperatures=( 020.0 060.0 100.0 140.0 180.0 220.0 260.0 300.0 340.0 380.0 420.0 )
N2_temperatures=( 077.0 110.0 140.0 170.0 200.0 230.0 260.0 290.0 330.0 370.0 410.0 )
HE_temperatures=( 005.0 035.0 065.0 100.0 130.0 160.0 190.0 220.0 250.0 280.0 310.0 )
NE_temperatures=( 030.0 060.0 090.0 120.0 150.0 180.0 210.0 240.0 270.0 300.0 330.0 )
AR_temperatures=( 090.0 120.0 150.0 180.0 210.0 240.0 270.0 300.0 330.0 360.0 390.0 )
KR_temperatures=( 120.0 150.0 180.0 210.0 240.0 270.0 300.0 330.0 360.0 390.0 420.0 )
Xe_temperatures=( 170.0 190.0 210.0 230.0 250.0 270.0 290.0 310.0 330.0 350.0 370.0 )

for species in CH4 CO2 N2 NE AR KR XE; do
    model_array="${species}_models[@]"
    temp_array="${species}_temperatures[@]"
    mkdir -p ${species}
    cd ${species}
        for model in "${!model_array}"; do
            mkdir -p ${model}
            cd ${model}
                for pres in 00.1 01.0 05.0 10.0 20.0 30.0; do
                    mkdir -p ${pres}
                    cd ${pres}
                        for temperature in "${!temp_array}";do
                            mkdir -p ${temperature}
                            cd ${temperature}
                                job_number=$(ls *.out | sed 's/^[^-]*-\([^.]*\).*/\1/')
                                submitted=$(squeue -u $USER | grep -q "${job_number}")
                                if [ ${submitted} -eq 0 ]; then
                                    mkdir -p saved_data
                                    mv input.pqr saved_data/old_input.pqr
                                    if [ -s *restar*.pqr ]; then
                                        cp *restar*.pqr saved_data/restart.pqr
                                        cp *restar*.pqr input.pqr
                                    else #if restart.pqr is empty, use restart.pqr.last
                                        cp *restar*.pqr.last saved_data/restart.pqr.last
                                        cp *restar*.pqr.last input.pqr
                                    fi
                                    cp *.log saved_data/old_runlog.log
                                    sbatch submit.sh
                                fi
                            cd .. #out of temperature
                        done
                    cd .. #out of pressure
                done
            cd .. #out of model
        done
    cd .. #out of species
done

for species in H2 HE; do
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
                        for pres in 00.1 01.0 05.0 10.0 20.0 30.0; do
                            mkdir -p ${pres}
                            cd ${pres}
                                for temperature in "${!temp_array}";do
                                    mkdir -p ${temperature}
                                    cd ${temperature}
                                        job_number=$(ls *.out | sed 's/^[^-]*-\([^.]*\).*/\1/')
                                        submitted=$(squeue -u $USER | grep -q "${job_number}")
                                        if [ ${submitted} -eq 0 ]; then
                                            mkdir -p saved_data
                                            mv input.pqr saved_data/old_input.pqr
                                            if [ -s *restar*.pqr ]; then
                                                cp *restar*.pqr saved_data/restart.pqr
                                                cp *restar*.pqr input.pqr
                                            else #if restart.pqr is empty, use restart.pqr.last
                                                cp *restar*.pqr.last saved_data/restart.pqr.last
                                                cp *restar*.pqr.last input.pqr
                                            fi
                                            cp *.log saved_data/old_runlog.log
                                            sbatch submit.sh
                                        fi
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
