import csv
from math import exp
from scipy import integrate


def z(p):
    return (Z_of.get(p, Z_of[min(Z_of.keys(), key=lambda k: abs(k - p))]) + 1) / p


file_name = "../misc/ISOTHERM_REFERENCE_DATA/AR/300"

with open(file_name) as f:
    reader = csv.reader(f, delimiter=" ")
    pressure_strings, volume_strings = zip(*reader)

pressures = []
volumes = []

for p_str, v_str in zip(pressure_strings, volume_strings):
    pressures.append(float(p_str))
    volumes.append(float(v_str))

my_Zs = []

for p, v in zip(pressures, volumes):
    this_z = (p * v) / (0.082057388 * 300)
    my_Zs.append(this_z)

Z_of = {}

for i, p in enumerate(pressures):
    Z_of[p] = my_Zs[i]

result, error = integrate.quad(z, 0.1, 30.0)
answer = 30.0 * exp(result)

print answer
