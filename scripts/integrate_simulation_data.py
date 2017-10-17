from math import exp
from scipy import integrate
import numpy as np
import sys
from operator import itemgetter


def change_to_float(l, dtype=float):
    return map(dtype, l)


def compressibility(p, polynomial):
    return (polynomial(p) - 1) / p


def fugacity(a_list):
    pressures = []
    compressibilities = []

    for list_member in a_list:
        pressures.append(float(list_member[1]))
        compressibilities.append(float(list_member[2]))

    x = np.array(pressures)
    z = np.array(compressibilities)

    coefficients = np.polynomial.legendre.legfit(x, z, 4)

    polynomial = np.polynomial.legendre.Legendre(coefficients)

    for list_member in a_list:
        this_f = float(list_member[1])
        integral_term = integrate.quad(compressibility, 0.01, float(list_member[1]), args=polynomial)[0]
        this_f *= exp(integral_term)
        list_member.append(this_f)


def main():
    file_name = sys.argv[1]
    in_file = open(file_name, "r")
    file_out = sys.argv[1] + ".FINAL"
    out_file = open(file_out, "w")

    all_my_string_lists = []

    for input_line in in_file:
        out = [item for item in filter(None, input_line.strip().split(' '))]
        all_my_string_lists.append(out)

    all_my_lists = []
    for a_list in all_my_string_lists:
        all_my_lists.append(change_to_float(a_list))

    # sort by the temperature
    my_sorted_lists = sorted(all_my_lists, key=itemgetter(0))

    this_temperature = my_sorted_lists[0][0]
    current_list = []
    counter = 0
    for working_line in my_sorted_lists:
        current_temperature = working_line[0]
        if float(current_temperature) == float(my_sorted_lists[-1][0]):
            counter += 1
        if float(current_temperature) == float(this_temperature):
            current_list.append(working_line)
            this_temperature = current_temperature
        else:
            fugacity(current_list)
            current_list = [working_line]
            this_temperature = current_temperature
        if counter == 20:  # number of state points per temperature
            fugacity(current_list)

    # sort the results by pressure, which is the second column in each list
    final_list = sorted(my_sorted_lists, key=itemgetter(1))

    for output_line in final_list:
        out_file.write(','.join(map(str, output_line)) + '\n')


main()
