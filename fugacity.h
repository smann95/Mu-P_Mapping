#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M_PI 3.14159265358979323846264338327950288
#define GAS_CONSTANT 8.3144598 
#define BAR_TO_ATM 1.01325
#define BAR_TO_PASCAL 100000.0
#define BOLTZMANN_J_PER_K 0.000000000000000000000013806485279 
#define BOLTZMANN_KJ_MOLAR 0.008314462175 //KJ/(mol*K)
#define PLANCK 0.000000000000000000000000000000000662607 //J*S
#define AVOGADRO 602214100000000000000000

#ifndef FUGACITY_H
#define FUGACITY_H

double co2_pengr_fugacity(double temperature, double pressure);

void read_simulation_input();

void get_mass_in_kilograms();

void get_ideal_mu();

void get_state_excess_mu();
void get_state_fugacity();

void get_simulation_fugacity();
void get_simulation_mu();




#endif
