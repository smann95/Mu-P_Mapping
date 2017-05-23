#include "compressibility.h"

using namespace std;

int main(int argc, char ** argv)
{
    if(argc < 2)
    {
        cerr << "Wrong number of args. Takes at least one filename as input.\n"
             << endl;
        return 1;
    }

    /* these first three functions:
     * 1. figure out how many vectors and vectors-of-vectors we're going to need
     * 2. set up the needed data structures
     * 3. read in the simulation data from the input file to all_runs*/
    vector<general_run_data> general_runs = set_up_general_runs(argc, argv);
    vector<vector<run>> all_runs = set_up_simulation_structs(general_runs);
    give_structs_simulation_data(argc, argv, all_runs);

    /* These next two functions:
     * 1. assigns the mass to each individual <run> in all_runs based on atom type
     * 2. converts simulation data to friendlier units*/
    get_species_mass(all_runs, general_runs);
    convert_data_to_other_units(all_runs, general_runs);

    /* We get our answers...*/
    calculate_data(all_runs, general_runs);

    /* And write them to a new file*/
    file_output(all_runs, general_runs, argc, argv);

    return 0;
}