//
// Created by Luciano Laratelli on 17/05/2017.
//

#include "compressibility.h"

double get_simulation_compressibility(double temperature, double pressure, double volume)
{
    double num = 512.0/AVOGADRO;
    return pressure*volume/(num*GAS_CONSTANT*temperature);
}

double get_simulation_fugacity(double Z, double pressure)
{
    double goes_in_exp = (Z - 1.0) * log(pressure);
    double simulation_fugacity = pressure * exp(goes_in_exp);
    return simulation_fugacity;
}
