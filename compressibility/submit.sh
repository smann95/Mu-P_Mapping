#!/bin/bash

#SBATCH -N 1
#SBATCH -p RM-shared
#SBATCH --ntasks-per-node 1
#SBATCH -t 48:00:00
#SBATCH --mail-type=FAIL
#SBATCH --mail-user=laratelli@mail.usf.edu # send-to address
#SBATCH --job-name="XXXJOBNAMEXXX"

mpmc *.inp > XXXJOBNAMEXXX.log
