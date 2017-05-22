//
// Created by Luciano Laratelli on 17/05/2017.
//

#include "compressibility.h"

double get_simulation_compressibility(double temperature, double pressure, double volume)
{
    double num = 512.0/AVOGADRO;
    return pressure*volume/(num*GAS_CONSTANT*temperature);
}

