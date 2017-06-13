#!/bin/bash

#SBATCH -N 1
#SBATCH -p RM-shared
#SBATCH --ntasks-per-node 1
#SBATCH -t 48:00:00
#SBATCH --mem=4G
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=laratelli@mail.usf.edu # send-to address
#SBATCH --job-name="CH4_9_site_nonpolar_P00.1_T115.0"

mpmc *.inp > CH4_9_site_nonpolar_P00.1_T115.0.log
