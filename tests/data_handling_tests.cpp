//
// Created by luciano on 8/11/2017.
//

#include "gtest/gtest.h"
#include "../src/compressibility.h"
#include "tests.h"

TEST( test_unit_conversion, unit_correctness)
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

    EXPECT_EQ(all_runs[0][0].pressure_bar, 0.99999972975 );
    EXPECT_EQ(all_runs[0][0].pressure_pa, 101324.9726169187);
    EXPECT_EQ(all_runs[0][0].simulation_V, 1 );

}

