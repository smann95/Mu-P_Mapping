#include "compressibility.h"

using namespace std;

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cerr << "Wrong number of args. Takes at least one filename as input.\n"
             << endl;
        return 1;
    }

    cout << "BEGIN MU-P MAPPING FOR " << argv[1] << endl;

    //read in and store reference_data
    auto NIST_data = read_reference_data();

    /* these three functions:
     * 1. figure out how many vectors and vectors-of-vectors we're going to need
     * 2. set up the needed data structures
     * 3. read in the simulation data from the input file to all_runs*/
    auto general_runs = set_up_general_runs(argc, argv);
    auto all_runs = set_up_simulation_structs(general_runs);
    read_simulation_data(argc, argv, all_runs);

    convert_data_to_other_units(all_runs, general_runs);

    /* We get our answers...*/
    calculate_data(all_runs);

    /* And write them to a file*/
    file_output(all_runs, general_runs, NIST_data, argv);


    /*
     *
     *After outputting most of the data we need, the magic happens via python because doing quadrature is a pain in
     *the ass in C++. This is probably the dumbest way to do this but it works
     *
     */

    cout << "BEGIN FUGACITY INTEGRATOR" << endl;
    get_simulation_fugacities(argc, argv);
    cout << "DONE WITH FUGACITY" << endl;

    cout << "END MU-P MAPPING FOR " << argv[1] << endl;
    return 0;
}
