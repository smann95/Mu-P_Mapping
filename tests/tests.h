//
// Created by luciano on 8/11/2017.
//

#ifndef COMPRESSIBILITY_TESTS_H
#define COMPRESSIBILITY_TESTS_H

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <cmath>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <cstring>
#include <iterator>


#define GAS_CONSTANT 8.3144598
#define BAR_TO_ATM 0.986923
#define BAR_TO_PASCAL 100000.0
#define AVOGADRO 6.0022141E23
#define CUBIC_A_TO_CUBIC_M 1.0E-30
#define G_IN_KG 1000.0
#define MOLES 64.0/AVOGADRO


std::map<std::string, std::map<std::string, std::vector<isotherm_reference_data>>> read_reference_data();
//FUNCTIONS CALLED BY READ_REFERENCE_DATA:
void get_species_temperatures(std::vector<std::string> &this_species_temps, std::string species);
//END FUNCTIONS CALLED BY READ_REFERENCE_DATA

void calculate_reference_fugacities(std::map<std::string, std::map<std::string, std::vector<isotherm_reference_data>>> NIST_data);
//FUNCTIONS CALLED BY GET_REFERENCE_FUGACITY:
double integrate_compressibility_for_fugacity(double pressure_atm, std::map<std::string, std::map<std::string, std::vector<isotherm_reference_data>>> & NIST_data);
//END FUNCTIONS CALLED BY GET_REFERENCE_FUGACITY

std::vector<general_run_data> set_up_general_runs(int argc, char ** argv);
std::vector<std::vector<run>> set_up_simulation_structs(std::vector<general_run_data> general_runs);
//FUNCTIONS CALLED BY SET_UP_SIMULATION_STRUCTS:
double get_species_mass(std::string atom_type);
void get_peng_robinson_constants(run &current);
// END

void read_simulation_data(int argc, char ** argv, std::vector<std::vector<run>> &all_runs);
void convert_data_to_other_units(std::vector<std::vector<run>> &all_runs, std::vector<general_run_data> general_runs);
void calculate_data(std::vector<std::vector<run>> &all_runs);
//FUNCTIONS CALLED BY CALCULATE_DATA:
double get_compressibility(double temperature, double pressure, double volume);
double solve_peng_robinson_for_compressibility(double temperature, double pressure, run some_run );
double solve_peng_robinson_for_fugacity(double temperature, double pressure, run some_run );
//END

void file_output(std::vector<std::vector<run>> all_runs,
                 std::vector<general_run_data> general_runs,
                 std::map<std::string, std::map<std::string, std::vector<isotherm_reference_data>>> NIST_data,
                 char ** argv);
//FUNCTIONS CALLED BY FILE_OUTPUT:
double get_reference_compressibility(std::string atom_type,
                                     double pressure_atm,
                                     double this_temperature,
                                     std::map<std::string, std::map<std::string, std::vector<isotherm_reference_data>>> NIST_data);
double get_reference_fugacity(std::string atom_type,
                                     double pressure_atm,
                                     double this_temperature,
                                     std::map<std::string, std::map<std::string, std::vector<isotherm_reference_data>>> NIST_data);
//END

#endif //COMPRESSIBILITY_TESTS_H
