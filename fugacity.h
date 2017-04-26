#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M_PI 3.14159265358979323846264338327950288
#define GAS_CONSTANT 8.3144598 
#define BAR_TO_ATM 0.986923
#define BAR_TO_PASCAL 100000.0
#define BOLTZMANN_J_PER_K 1.3806485279E-23
#define BOLTZMANN_KJ_MOLAR 0.008314462175 //KJ/(mol*K)
#define PLANCK 6.62607004E-34 //J*S
#define AVOGADRO 6.0022141E23
#define CUBIC_A_TO_CUBIC_M 1E-30
#define G_IN_KG 1000.0
#define J_TO_KJ 1000.0

#ifndef FUGACITY_H
#define FUGACITY_H 
typedef struct _run
{
  double temperature,
         pressure_bar,
         pressure_atm,
         pressure_pa,
         mass;
  double simulation_N,
         simulation_volume,
         simulation_mu,
         simulation_fugacity;
  double state_excess_mu,
         state_fugacity;//fug
  double ideal_gas_mu;
}run;

void read_simulation_input(run * runs);

void give_structs_species_data(run * runs, int num_of_runs);
void convert_to_proper_units(run * runs, int num_of_runs);

double co2_pengr_fugacity(double temperature, double pressure);


void get_ideal_mu(run * runs, int num_of_runs);

void get_state_excess_mu(run * runs, int num_of_runs);
void get_state_fugacity(run * runs, int num_of_runs);

void get_simulation_mu(run * runs, int num_of_runs);
void get_simulation_fugacity(run * runs, int num_of_runs);

void populate_output_array(double * output_array, run * runs, int num_of_runs);
void output(double * output_array, char * file_name, int num_of_runs);


#endif
