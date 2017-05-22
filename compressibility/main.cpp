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

    vector<general_run_data> general_runs = set_up_general_runs(argc, argv);
    vector<vector<run>> all_runs = set_up_simulation_structs(general_runs);
    give_structs_simulation_data(argc, argv, all_runs);
    get_species_mass(all_runs, general_runs);
    convert_data_to_other_units(all_runs, general_runs);

    for(int i = 0;i<all_runs.size();i++)
    {
        for(int j = 0;j<general_runs[i].num_runs;j++)
        {
            auto &ref = (all_runs[i])[j];
            ref.simulation_Z = get_simulation_compressibility(ref.temperature, ref.pressure_pa, ref.simulation_V);
            ref.EOS_Z = get_state_compressibility(ref.temperature, ref.pressure_atm);
        }
    }


    for(auto i = 0;i<all_runs.size();i++)
    {
        for(int j = 0;j<general_runs[i].num_runs;j++)
        {
            auto ref = (all_runs[i])[j];
            cout << ref.temperature << " "
                 << ref.pressure_bar << " "
                 << ref.EOS_Z << " "
                 << ref.simulation_Z << " "
                 << endl;
        }
    }

    return 0;
}