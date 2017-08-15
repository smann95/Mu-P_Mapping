import csv
from math import exp
from scipy import integrate
import numpy as np
import os


def compressibility(p, polynomial):
    return (polynomial(p) - 1) / p


def fugacity(file_name, temperature, pressure):
    print file_name, temperature, pressure
    with open(file_name) as f:
        reader = csv.reader(f, delimiter=" ")
        pressure_strings, volume_strings = zip(*reader)

    pressures = []
    volumes = []
    compressibilities = []

    for p_str, v_str in zip(pressure_strings, volume_strings):
        pressures.append(float(p_str))
        volumes.append(float(v_str))
    for p, v in zip(pressures, volumes):
        this_z = (p * v) / (0.082057388 * temperature)
        compressibilities.append(this_z)

    x = np.array(pressures)
    z = np.array(compressibilities)

    for i in range(len(z)):
        print x[i], z[i]

    coefficients = np.polynomial.legendre.legfit(x, z, 10)

    polynomial = np.polynomial.legendre.Legendre(coefficients)
    return pressure * exp(integrate.quad(compressibility, 0.1, 30, args=polynomial)[0])


for species in "AR", "CH4", "CO2", "H2", "HE", "KR", "N2", "NE", "XE":
    file_name = "../misc/ISOTHERM_REFERENCE_DATA/"
    file_name += species
    file_name += "/"
    species_temperatures = os.listdir(file_name)

    for temperature in species_temperatures:
        file_name = "../misc/ISOTHERM_REFERENCE_DATA/"
        file_name += species
        file_name += "/"
        file_name += temperature
        out_file = open(file_name + "OUT", 'w')
        in_file = open(file_name, 'r')
        for line in in_file:
            fugacity(file_name, float(temperature), float(line.split(" ")[0]))
            out_file.write(line.rstrip('\n') + " " + str(fugacity(file_name, float(temperature), float(line.split(" ")[0]))) + '\n')


