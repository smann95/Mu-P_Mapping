//
// Created by Luciano Laratelli on 17/05/2017.
//

#ifndef COMPRESSIBILITY_COMPRESSIBILITY_H
#define COMPRESSIBILITY_COMPRESSIBILITY_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>

#define GAS_CONSTANT 8.3144598
#define BAR_TO_ATM 0.986923
#define BAR_TO_PASCAL 100000.0
#define AVOGADRO 6.0022141E23
#define CUBIC_A_TO_CUBIC_M 1.0E-30
#define G_IN_KG 1000.0
//BEGIN MPMC DEFINES
#define MAXLINE                                 512
#define NA			6.0221415e23
#define KB			1.3806503e-23
#define ATM2PASCALS		101325.0
#define ATM2PSI			14.6959488
//END MPMC DEFINES

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
           simulation_Z,
           simulation_fugacity;
    double EOS_fugacity,
           EOS_Z;//compressibility
};

std::vector<general_run_data> set_up_general_runs(int argc, char ** argv);
std::vector<std::vector<run>> set_up_simulation_structs(std::vector<general_run_data> general_runs);
void give_structs_simulation_data(int argc, char ** argv, std::vector<std::vector<run>> &all_runs);
void get_species_mass(std::vector<std::vector<run>> &all_runs, std::vector<general_run_data> general_runs);
void convert_data_to_other_units(std::vector<std::vector<run>> &all_runs, std::vector<general_run_data> general_runs);

double get_simulation_compressibility(double temperature, double pressure, double volume);
double get_simulation_fugacity(double Z, double pressure,double temperature, std::string species);

double get_co2_state_compressibility(double temperature, double pressure);
double get_co2_state_fugacity(double temperature, double pressure);

double get_n2_state_compressibility(double temperature, double pressure);

//MPMC FUNCTIONS
void output(std::string msg);
double get_n2_fugacity(double temperature, double pressure);
double n2_fugacity_back(double temperature, double pressure);
double n2_comp_back(double temperature, double pressure);
double n2_fugacity_PR(double temperature, double pressure);
double n2_fugacity_zhou(double temperature, double pressure);

//END MPMC FUNCTIONS

#endif //COMPRESSIBILITY_COMPRESSIBILITY_H
