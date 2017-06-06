// Created by Luciano Laratelli on 17/05/2017.
//

#include <strings.h>
#include <iterator>
#include "compressibility.h"
#include <boost/algorithm/string.hpp>

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
            getline(input,line);//this line contains species string
            this_species.species = line;
            getline(input,line);//this line contains number of state points for current species
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
 * The "outer" vector's members each represent a group of simulation runs
 * e.g. member one might be for CO2, member two for N2, etc.
 * The inner vector's members each represent a specific state point
 * i.e. at a specific temperature and pressure
 */
vector<vector<run>> set_up_simulation_structs(vector<general_run_data> general_runs)
{
    vector<vector<run>> all_runs;
    auto gen_beg = general_runs.begin(),
         gen_end = general_runs.end();

    for(gen_beg;gen_beg != gen_end;gen_beg++)
    {
        vector<run> this_run;
        for(auto i = 0;i < gen_beg->num_runs;i++)
        {
            run current;
            current.atom_type = gen_beg->species;
            current.mass = get_species_mass(current.atom_type);
            this_run.push_back(current);
        }
        all_runs.push_back(this_run);
    }
    return all_runs;
}
double get_species_mass(string atom_type)
{
    double mass = 0;
    if(boost::iequals(atom_type,"co2"))//case-insensitive string comparison
        mass = 44.0095;
    else if(boost::iequals(atom_type,"n2"))
        mass = 28.0134;
    else if(boost::iequals(atom_type,"h2"))
        mass = 2.016;
    else if(boost::iequals(atom_type,"ch4"))
        mass = 16.04260;
    else if(boost::iequals(atom_type,"he"))
        mass = 4.002602;
    else if(boost::iequals(atom_type,"ne"))
        mass = 20.1797;
    else if(boost::iequals(atom_type,"xe"))
        mass = 131.293;
    else if(boost::iequals(atom_type,"kr"))
        mass = 83.798;
    else if(boost::iequals(atom_type,"ar"))
        mass = 39.948;
    return mass;
}

/*
 * Now that we've set up our vectors, we read the simulation data
 * from our input file into the vectors
 */
void read_simulation_data(int argc, char ** argv, vector<vector<run>> &all_runs)
{
    string file_name,
           line;
    for(int i = 1;i < argc;i++)
    {
        int j = 0;
        file_name = argv[i];
        ifstream input(file_name);
        /*
        input.ignore('\n');//first two lines are stuff we already got
        input.ignore('\n');
        */
        getline(input,line);
        getline(input,line);
        if(input.is_open())
        {
            while(getline(input,line))
            {
                vector<string> this_line;
                istringstream iss(line);
                //I got these next three lines from doug; have no idea what they do but they do it well
                copy(
                        istream_iterator<string>(iss),
                        istream_iterator<string>(),
                        back_inserter(this_line)
                );
                if(!strncasecmp(this_line[0].c_str(), "#",1))
                    continue;//lines that start with # are comments
                else
                {
                    auto &ref = (all_runs[i-1])[j];//make current run a ref to clean up the code a bit
                    ref.temperature = atof(this_line[0].c_str());
                    ref.pressure_atm = atof(this_line[1].c_str());
                    ref.simulation_V = atof(this_line[2].c_str());
                }
                j++;
            }
        }
        else
        {
            cerr << "Error in opening file " << file_name
                 << " in give_structs_simulation_data() " << endl;
        }
        input.close();
    }
}


void convert_data_to_other_units(vector<vector<run>> &all_runs, vector<general_run_data> general_runs)
{
    for(int i = 0;i<general_runs.size();i++)
    {
       for(int j = 0;j<general_runs[i].num_runs;j++)
       {
           auto & ref = (all_runs[i])[j];
           ref.pressure_bar = ref.pressure_atm / BAR_TO_ATM;
           ref.pressure_pa = ref.pressure_bar * BAR_TO_PASCAL;
           ref.mass /=AVOGADRO;
           ref.mass /=G_IN_KG;
           ref.simulation_V *= CUBIC_A_TO_CUBIC_M;
       }
    }
}

void calculate_data(vector<vector<run>> &all_runs)
{
    for(auto all_beg = all_runs.begin();all_beg != all_runs.end();all_beg++)
    {
        for(auto mini_beg = all_beg->begin(); mini_beg != all_beg->end();mini_beg++)
        {
            mini_beg->simulation_Z = get_simulation_compressibility(mini_beg->temperature, mini_beg->pressure_pa, mini_beg->simulation_V);
            if(mini_beg->atom_type == "co2")
            {
                mini_beg->EOS_Z = get_co2_state_compressibility(mini_beg->temperature, mini_beg->pressure_atm);
                mini_beg->EOS_fugacity = get_co2_state_fugacity(mini_beg->temperature, mini_beg->pressure_atm);
                //mini_beg->simulation_fugacity = get_simulation_fugacity(mini_beg->simulation_Z,mini_beg->pressure_atm, mini_beg->temperature, "co2");
            }
            else if(mini_beg->atom_type == "n2")
            {
                mini_beg->EOS_Z = get_n2_state_compressibility(mini_beg->temperature, mini_beg->pressure_atm);
                mini_beg->EOS_fugacity = get_n2_fugacity(mini_beg->temperature, mini_beg->pressure_atm);
                //mini_beg->simulation_fugacity = get_simulation_fugacity(mini_beg->simulation_Z,mini_beg->pressure_atm, mini_beg->temperature, "n2");
            }
            else if(mini_beg->atom_type == "h2")
            {
                mini_beg->EOS_Z = h2_comp_back(mini_beg->temperature, mini_beg->pressure_atm);
                mini_beg->EOS_fugacity = get_h2_fugacity(mini_beg->temperature, mini_beg->pressure_atm);
                //mini_beg->simulation_fugacity = get_simulation_fugacity(mini_beg->simulation_Z,mini_beg->pressure_atm, mini_beg->temperature, "h2");
            }
        }
    }
}

void file_output(vector<vector<run>> all_runs, char ** argv)
{
    int i = 1;
    for(auto all_beg = all_runs.begin();all_beg != all_runs.end();all_beg++)
    {
        string input_name = argv[i];
        string file_name = input_name + ".OUT";
        ofstream output_file(file_name);
        cout << "FILE NAME FOR OUTPUT : " << file_name << endl;
        //output_file << "#TEMP  #PRES   #SIM_Z      #EOS_Z " << endl;
        for(auto mini_beg = all_beg->begin();mini_beg != all_beg->end();mini_beg++)
        {
           output_file << mini_beg->temperature << ",     "
                       << mini_beg->pressure_atm << ",    "
                       << mini_beg->simulation_Z << ",  "
                       << mini_beg->EOS_Z
                       << endl;
        }
        output_file.close();
        i++;
    }
}

/*
 * this is only here to play nice with the
 * MPMC functions for n2 fugacity that I'm using
 */
void output(string msg)
{
    cout << msg << endl;
}
