// Created by Luciano Laratelli on 17/05/2017.
//

#include "compressibility.h"

using namespace std;

/*
 * set_up_general_runs reads the first two lines of the input file(s) to determine the
 * species of each group of simulations and how many state points are in each
 * group of simulations
 */

vector <general_run_data> set_up_general_runs(int argc, char ** argv)
{
    vector<general_run_data> general_runs;
    string file_name,
           line;
    for(auto i = 1; i < argc; i++)//argv[0] is the program name, don't forget!
    {
        file_name = argv[i];
        ifstream input(file_name);
        if(input.is_open())
        {
            general_run_data this_species;
            getline(input,line);//first line contains species string
            this_species.species = line;
            getline(input,line);//second line contains number of state points for current species
            stringstream convert(line);//get num as an int
            convert >> this_species.num_runs;
            general_runs.push_back(this_species);
        }
        else//check the execution directory to make sure your data is there
        {
            cerr << "Error in opening file " << file_name
                 << " in set_up_general_runs() " << endl;
            exit(EXIT_FAILURE);
        }
    }
    return general_runs;
}


/*
 * Now that we know how many groups of simulation runs we have, we
 * create the necessary vectors of vectors
 * the inner vectors (of type run) each
 */
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
            current.mass = get_species_mass(current.atom_type);
            this_run.push_back(current);
        }
        all_runs.push_back(this_run);
        general_beg++;
    }
    return all_runs;
}
double get_species_mass(string atom_type)
{
    if(strcasecmp((atom_type).c_str(), "co2") == 0)
        return 44.0095;
    else if(strcasecmp((atom_type).c_str(), "n2") == 0)
        return 28.0134;
}

void give_structs_simulation_data(int argc, char ** argv, vector<vector<run>> &all_runs)
{
    string file_name;
    string line;
    int j = 0;
    for(int i = 1;i < argc;i++)
    {
        file_name = argv[i];
        ifstream input;
        input.open(file_name);
        input.ignore('\n');//first two lines are stuff we already got
        input.ignore('\n');
        if(input.is_open())
        {
            //then get the other input and read it in
            while(getline(input,line))
            {
                vector<string> this_line;
                istringstream iss(line);
                copy(
                        istream_iterator<string>(iss),
                        istream_iterator<string>(),
                        back_inserter(this_line)
                );
                if(!strncasecmp(this_line[0].c_str(), "#",1))
                    continue;//lines that start with # are comments
                else
                {
                        (all_runs[i-1])[j].temperature = atof(this_line[0].c_str());
                        (all_runs[i-1])[j].pressure_bar = atof(this_line[1].c_str());
                        (all_runs[i-1])[j].simulation_V = atof(this_line[2].c_str());
                }
                j++;
            }

        }
        else
        {
            cerr << "Error in opening file " << file_name
                 << " in give_structs_simulation_data() " << endl;
        }
    }
}


void convert_data_to_other_units(vector<vector<run>> &all_runs, vector<general_run_data> general_runs)
{
    for(int i = 0;i<general_runs.size();i++)
    {
       for(int j = 0;j<general_runs[i].num_runs;j++)
       {
           (all_runs[i])[j].pressure_atm = (all_runs[i])[j].pressure_bar * BAR_TO_ATM;
           (all_runs[i])[j].pressure_pa = (all_runs[i])[j].pressure_bar * BAR_TO_PASCAL;
           (all_runs[i])[j].mass /=AVOGADRO;
           (all_runs[i])[j].mass /=G_IN_KG;
           (all_runs[i])[j].simulation_V *= CUBIC_A_TO_CUBIC_M;
       }
    }
}

void calculate_data(vector<vector<run>> &all_runs, vector<general_run_data> general_runs)
{
    for(int i = 0;i<all_runs.size();i++)
    {
        for(int j = 0;j<general_runs[i].num_runs;j++)
        {
            auto &ref = (all_runs[i])[j];
            ref.simulation_Z = get_simulation_compressibility(ref.temperature, ref.pressure_pa, ref.simulation_V);
            if(ref.atom_type == "co2")
            {
                ref.EOS_Z = get_co2_state_compressibility(ref.temperature, ref.pressure_atm);
                ref.EOS_fugacity = get_co2_state_fugacity(ref.temperature, ref.pressure_atm);
                ref.simulation_fugacity = get_simulation_fugacity(ref.simulation_Z,ref.pressure_atm, ref.temperature, "co2");
            }
            else if(ref.atom_type == "n2")
            {
                ref.EOS_Z = get_n2_state_compressibility(ref.temperature, ref.pressure_atm);
                ref.EOS_fugacity = get_n2_fugacity(ref.temperature, ref.pressure_atm);
                ref.simulation_fugacity = get_simulation_fugacity(ref.simulation_Z,ref.pressure_atm, ref.temperature, "n2");
            }
        }
    }
}

void file_output(vector<vector<run>> all_runs, vector<general_run_data> general_runs, int argc, char ** argv)
{
    for(int i = 1;i < argc;i++)
    {
        string input_name = argv[i];
        string file_name = input_name + ".OUT";
        ofstream output_file(file_name);
        cout << "FILE NAME FOR OUTPUT : "
             << file_name
             << endl;
        output_file << "#TEMP  #PRES   #SIM_Z      #EOS_Z       #SIM_FUG    #EOS_FUG"
                    << endl;
        for(auto j = 0;j<all_runs.size();j++)
        {
           for(int k = 0;k<general_runs[j].num_runs;k++)
           {
               auto ref = (all_runs[j])[k];
               output_file << ref.temperature << ",     "
                           << ref.pressure_bar << ",    "
                           << ref.simulation_Z << ",  "
                           << ref.EOS_Z << ",  "
                           << ref.simulation_fugacity << ", "
                           << ref.EOS_fugacity
                           << endl;
           }
        }
        output_file.close();
    }
}

/* this is only here to play nice with the MPMC functions for
 * n2 fugacity that I'm using here
 */
void output(string msg)
{
    cout << msg << endl;
}
