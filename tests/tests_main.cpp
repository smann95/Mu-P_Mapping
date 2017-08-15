//
// Created by luciano on 8/11/2017.
//

#define CATCH_CONFIG_MAIN
#include "catch/catch.hpp"

#include "../src/compressibility.h"
#include "../src/data_handling.cpp"
#include "../src/peng_robinson.cpp"
#include "../src/Reference_Data.cpp"

/*
 * BEGIN DATA_HANDLING.cpp TESTS
 */
TEST_CASE("Units converted properly", "[unit conversion]")
{
    std::vector<std::vector<run>> all_runs;
    std::vector<general_run_data> general_run;

    general_run_data baby_general_run;

    baby_general_run.num_runs = 1;
    baby_general_run.species = "AR";

    general_run.push_back(baby_general_run);

    run baby_run;
    std::vector<run> medium_run;

    baby_run.atom_type = "AR";
    baby_run.moles = MOLES;
    baby_run.pressure_atm = 0.986923;
    baby_run.temperature = 293;
    baby_run.simulation_V = 1e30;

    get_peng_robinson_constants(baby_run);
    medium_run.push_back(baby_run);

    all_runs.push_back(medium_run);

    convert_data_to_other_units(all_runs, general_run);

    REQUIRE(all_runs[0][0].pressure_bar == 1 );
    REQUIRE(all_runs[0][0].pressure_pa == 100000);
    REQUIRE(all_runs[0][0].simulation_V == 1 );
}

TEST_CASE("Compressibility is 1 when expected", "[compressibility")
{
    double temperature = 297,
           pressure_pa = 328042000,
           V = 8e-28;
    REQUIRE(get_compressibility(temperature, pressure_pa, V) == Approx(1.0).epsilon(0.01));
}

TEST_CASE("Case-insensitive string comparison works", "[get_species_mass]")
{
    REQUIRE(get_species_mass("Co2") == 44.0095);
    REQUIRE(get_species_mass("cO2") == 44.0095);
    REQUIRE(get_species_mass("CO2") == 44.0095);
}

/*
 * END DATA_HANDLING.cpp TESTS
 */

