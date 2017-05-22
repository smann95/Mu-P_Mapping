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
    give_structs_simulation_data(argc, argv, all_runs, general_runs);

    for(auto i = 0;i<all_runs.size();i++)
    {
        for(int j = 0;j<general_runs[i].num_runs;j++)
        {
            cout << (all_runs[i])[j].temperature << " " << (all_runs[i])[j].pressure_bar << " "
                 << (all_runs[i])[j].simulation_V << " " << endl;
        }
    }

    return 0;
}