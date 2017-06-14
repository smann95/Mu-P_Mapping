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
#include <boost/algorithm/string.hpp>
#include <string.h>


#define GAS_CONSTANT 8.3144598
#define BAR_TO_ATM 0.986923
#define BAR_TO_PASCAL 100000.0
#define AVOGADRO 6.0022141E23
#define CUBIC_A_TO_CUBIC_M 1.0E-30
#define G_IN_KG 1000.0
#define MOLES 64.0/AVOGADRO

//BEGIN MPMC DEFINES
#define MAXLINE         512
#define NA		    	6.0221415e23
#define KB		     	1.3806503e-23
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
           simulation_fugacity,
           density;
    double EOS_fugacity,
           EOS_Z;//compressibility
    double Tc,
           Pc,
           w;
    double moles;
};

struct reference_data
{
    double temperature,
           volume_l_mol,
           volume_m3,
           compressibility;
};


std::vector<general_run_data> set_up_general_runs(int argc, char ** argv);
std::vector<std::vector<run>> set_up_simulation_structs(std::vector<general_run_data> general_runs);
void read_simulation_data(int argc, char ** argv, std::vector<std::vector<run>> &all_runs);
double get_species_mass(std::string atom_type);
void convert_data_to_other_units(std::vector<std::vector<run>> &all_runs, std::vector<general_run_data> general_runs);
void calculate_data(std::vector<std::vector<run>> &all_runs);
void file_output(std::vector<std::vector<run>> all_runs,
                 std::vector<general_run_data> general_runs,
                 std::vector<std::vector<std::vector<reference_data>>> NIST_data,
                 char ** argv);
double get_compressibility(double temperature, double pressure, double volume);
//double get_simulation_fugacity(double Z, double pressure,double temperature, std::string species);

double get_co2_state_compressibility(double temperature, double pressure);
double get_co2_state_fugacity(double temperature, double pressure);

double get_n2_state_compressibility(double temperature, double pressure);

void get_peng_robinson_constants(run &current);

void output(std::string msg);

std::vector<std::vector<std::vector<reference_data>>>
read_reference_data(std::vector<std::string> species,
                    std::vector<std::string> pressures);

double get_reference_data_for_output(std::string atom_type,
                                     double pressure_atm,
                                     double this_temperature,
                                     std::vector<std::vector<std::vector<reference_data>>> NIST_data);
//MPMC FUNCTIONS
double solve_peng_robinson_for_compressibility(double temperature, double pressure, run some_run );
double get_n2_fugacity(double temperature, double pressure);
double n2_fugacity_back(double temperature, double pressure);
double n2_comp_back(double temperature, double pressure);
double n2_fugacity_PR(double temperature, double pressure);
double n2_fugacity_zhou(double temperature, double pressure);

double get_h2_fugacity(double temperature, double pressure);
double h2_fugacity_back(double temperature, double pressure);
double h2_comp_back(double temperature, double pressure);
double h2_fugacity_shaw(double temperature, double pressure);
double h2_fugacity_zhou(double temperature, double pressure);

double get_ch4_fugacity(double temperature, double pressure);
double ch4_fugacity_back(double temperature, double pressure);
double ch4_comp_back(double temperature, double pressure);
double ch4_fugacity_PR(double temperature, double pressure);


//END MPMC FUNCTIONS

#endif //COMPRESSIBILITY_COMPRESSIBILITY_H
