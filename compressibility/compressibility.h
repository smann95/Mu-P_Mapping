//
// Created by Luciano Laratelli on 17/05/2017.
//

#ifndef COMPRESSIBILITY_COMPRESSIBILITY_H
#define COMPRESSIBILITY_COMPRESSIBILITY_H

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

struct general_run_data
{
    std::string species;
    int num_runs;
};



double get_state_compressibility(double temperature, double pressure);
double get_simulation_compressiblity(double temperature, double pressure);
#endif //COMPRESSIBILITY_COMPRESSIBILITY_H
