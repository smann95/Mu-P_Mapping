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
                 << " in set_up_general_runs() " << endl;
        }
    }
    return general_runs;
}


vector<vector<run>> set_up_simulation_structs(vector<general_run_data> general_runs)
{
    vector<vector<run>> all_runs;
    auto general_beg = general_runs.begin(),
         general_end = general_runs.end();

    while(general_beg != general_end)
    {
        vector<run> this_run;
        for(auto i = 0;i < general_beg->num_runs;i++)
        {
            run current;
            current.atom_type = general_beg->species;
            this_run.push_back(current);
        }
        all_runs.push_back(this_run);
        general_beg++;
    }
    return all_runs;
}

void give_structs_simulation_data(int argc, char ** argv, vector<vector<run>> all_runs)
{
    auto beg = all_runs.begin(),
         end = all_runs.end();

    string file_name;
    string line;
    for(int i = 1;i < argc;i++)
    {
        file_name = argv[i];
        ifstream input;
        input.open(file_name);
        if(input.is_open())
        {
            
        }
        else
        {
            cerr << "Error in opening file " << file_name
                 << " in give_structs_simulation_data() " << endl;
        }
    }
}
