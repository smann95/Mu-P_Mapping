//
// Created by Luciano Laratelli on 17/05/2017.
//

#ifndef COMPRESSIBILITY_COMPRESSIBILITY_H
#define COMPRESSIBILITY_COMPRESSIBILITY_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#define M_PI 3.14159265358979323846264338327950288
#define GAS_CONSTANT 8.3144598
#define BAR_TO_ATM 0.986923
#define BAR_TO_PASCAL 100000.0
#define BOLTZMANN_J_PER_K 1.3806485279E-23
#define BOLTZMANN_KJ_MOLAR 0.008314462175 //KJ/(mol*K)
#define PLANCK 6.62607004E-34 //J*S
#define AVOGADRO 6.0022141E23
#define CUBIC_A_TO_CUBIC_M 1.0E-30
#define G_IN_KG 1000.0
#define J_TO_KJ 1000.0

struct general_run_data
{
    std::string species;
    int num_runs;
};

//a "run" is the data collected at one state point
struct run
{
    std::string atom_type;
    double temperature,
           pressure_bar,
           pressure_atm,
           pressure_pa,
           mass;
    double simulation_V,
           Z;//compressibility
};


std::vector<general_run_data> set_up_general_runs(int argc, char ** argv);
std::vector<std::vector<run>> set_up_simulation_structs(std::vector<general_run_data> general_runs);
void give_structs_simulation_data(int argc, char ** argv, std::vector<std::vector<run>> &all_runs, std::vector<general_run_data> general_runs);
void get_species_mass(std::vector<std::vector<run>> &all_runs, std::vector<general_run_data> general_runs);
void convert_data_to_other_units(std::vector<std::vector<run>> &all_runs, std::vector<general_run_data> general_runs);
double get_state_compressibility(double temperature, double pressure);
double get_simulation_compressiblity(double temperature, double pressure);
#endif //COMPRESSIBILITY_COMPRESSIBILITY_H
