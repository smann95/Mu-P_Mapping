// Handles file in/out
// Created by Luciano Laratelli on 17/05/2017.
//

#include "compressibility.h"

using namespace std;

/* sets up struct that contains general run data (species type and number
 * of runs per species
 */

vector <general_run_data> set_up_general_runs(int argc, char ** argv)
{
    vector<general_run_data> general_runs;
    string file_name;
    string line;
    for(int i = 1;i < argc;i++)
    {
        file_name = argv[i];
        ifstream input;
        input.open(file_name);
        if(input.is_open())
        {
            general_run_data this_run;
            getline(input,line);
            this_run.species = line;
            getline(input,line);
            stringstream convert (line);
            convert >> this_run.num_runs;
            general_runs.push_back(this_run);
        }
        else
        {
            cerr << "Error in opening file " << file_name
                 << " in main() " << endl;
        }
    }
    return general_runs;
}
