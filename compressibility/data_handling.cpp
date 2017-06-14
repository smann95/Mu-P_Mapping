// Created by Luciano Laratelli on 17/05/2017.
//

#include <strings.h>
#include <cstring>
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
            getline(input,line);
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
            current.moles = MOLES;
            get_peng_robinson_constants(current);
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
           a_line;
    for(int i = 1;i < argc;i++)
    {
        int j = 0;
        file_name = argv[i];
        ifstream input(file_name);
        getline(input,a_line);
        getline(input,a_line);
        getline(input,a_line);
        int num_runs = atoi(a_line.c_str());
        getline(input,a_line);
        if(input.is_open())
        {
            while(getline(input,a_line))
            {
                vector<string> this_line;
                istringstream iss(a_line);
                //copy the numbers of interest from the line into the vector this_line (thanks Doug / SO !)
                copy(
                        istream_iterator<string>(iss),
                        istream_iterator<string>(),
                        back_inserter(this_line)
                );
                if(this_line.size())
                {
                    auto &ref = (all_runs[i - 1])[j];//make current run a ref to clean up the code a bit
                    ref.temperature = atof(this_line[0].c_str());
                    ref.pressure_atm = atof(this_line[1].c_str());
                    ref.simulation_V = atof(this_line[2].c_str());
                    ref.density = atof(this_line[3].c_str());
                    if(j == (num_runs - 1))
                        break;
                    else
                        j++;
                }
                else
                {
                    cout << "Data line in read_simulation_data() is empty--check your input file"
                         << endl;
                }
            }
        }
        else
        {
            cerr << "Error in opening file " << file_name
                 << " in read_simulation_data() " << endl;
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
            mini_beg->simulation_Z = get_compressibility(mini_beg->temperature, mini_beg->pressure_pa, mini_beg->simulation_V);
            mini_beg->EOS_Z = solve_peng_robinson_for_compressibility(mini_beg->temperature, mini_beg->pressure_atm, *mini_beg);
            /*
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
            else if(mini_beg->atom_type == "ch4")
            {
                mini_beg->EOS_Z = ch4_comp_back(mini_beg->temperature, mini_beg->pressure_atm);
                mini_beg->EOS_fugacity = get_ch4_fugacity(mini_beg->temperature, mini_beg->pressure_atm);
                //mini_beg->simulation_fugacity = get_simulation_fugacity(mini_beg->simulation_Z,mini_beg->pressure_atm, mini_beg->temperature, "h2");
            }
            */
        }
    }
}

void file_output(vector<vector<run>> all_runs,
                 vector<general_run_data> general_runs,
                 vector<vector<vector<reference_data>>> NIST_data,
                 char ** argv)
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
           double reference_Z =get_reference_data_for_output(general_runs[i-1].species,
                                                             mini_beg->pressure_atm,
                                                             mini_beg->temperature,
                                                             NIST_data);

           output_file << mini_beg->temperature << ",     "
                       << mini_beg->pressure_atm << ",    "
                       << mini_beg->simulation_Z << ",  "
                       << mini_beg->EOS_Z << ", "
                       << reference_Z*100000.0
                       << endl;
        }
        output_file.close();
        i++;
    }
}

double get_reference_data_for_output(string atom_type,
                                     double pressure_atm,
                                     double this_temperature,
                                     vector<vector<vector<reference_data>>> NIST_data)
{
                            /*  0    1     2     3    4    5    6    7    8 */
    vector<string> species = {"AR","CH4","CO2","H2","HE","KR","N2","NE","XE"};
                            /*    0      1       2      3      4      5*/
    vector<double> pressures = {00.1, 001, 005, 010, 020, 030};

    int current_species_ind = 0,
        current_pressure_ind = 0;
    double reference_Z = 0.0;
    for(int species_ind = 0;species_ind < species.size();species_ind++)
    {
        if(atom_type == species[species_ind])
        {
            current_species_ind = species_ind;
            break;
        }
    }
    for(int pressure_ind = 0;pressure_ind < pressures.size();pressure_ind++)
    {
        if(pressure_atm == pressures[pressure_ind])
        {
            current_pressure_ind = pressure_ind;
            break;
        }
    }
    vector<reference_data> current_point = NIST_data[current_species_ind][current_pressure_ind];
    for(int temperature_ind = 0;temperature_ind < current_point.size(); temperature_ind++)
    {
        if (this_temperature == current_point[temperature_ind].temperature)
        {
            reference_Z = current_point[temperature_ind].compressibility;
            break;
        }
    }
    return reference_Z;
}


/*
 * this is only here to play nice with the
 * MPMC functions for n2 fugacity that I'm using
 */
void output(string msg)
{
    cout << msg << endl;
}

/*
 * This next function is a no-good struct inside of  a vector
 * inside of a vector inside of a vector (did I get that right?)
 * I am so sorry that you are here.
 * Without further ado, here is an explanation for my crimes:
 * Each SPECIES from the reference data has six pressures associated with it, these
 * are hardcoded in pressure_floats. Each of these pressures can have between 11
 * and 13 temperatures associated with it. We use the temperature as the final
 * identifier for the state point, which is the innermost struct.
 */
vector<vector<vector<reference_data>>>
read_reference_data(vector<string> species,
                    vector<string> pressure_strings)
{
    vector<double> pressure_floats = {00.1, 001, 005, 010, 020, 030};
   // ALL->SPECIES->PRES->STATE POINTS
    vector<vector<vector<reference_data>>> NIST_data;
    for(unsigned long species_ind = 0;species_ind<species.size();species_ind++)
    {
        //SPECIES->PRES->STATE POINTS
        vector<vector<reference_data>> this_species;
        string a_line;
        for(unsigned long pressure_ind= 0; pressure_ind < pressure_strings.size(); pressure_ind++)
        {
            string file_name = "data/";
            file_name += species[species_ind];
            file_name += pressure_strings[pressure_ind];
            ifstream input(file_name);
            if(input.is_open())
            {
                //PRES->STATE POINTS
                vector<reference_data> this_pressure;
                while (getline(input, a_line))
                {
                    vector<string> this_line;
                    istringstream iss(a_line);
                    //copy the numbers of interest from the line into the vector this_line (thanks Doug / SO !)
                    copy(
                            istream_iterator<string>(iss),
                            istream_iterator<string>(),
                            back_inserter(this_line)
                    );
                    if(this_line.size())
                    {
                        //STATE_POINT (we made it!)
                        reference_data this_point;
                        this_point.temperature = atof(this_line[0].c_str());
                        this_point.volume_l_mol = atof(this_line[1].c_str());
                        double liters = this_point.volume_l_mol*MOLES;
                        this_point.volume_m3 = liters / 1000.0;
                        this_point.compressibility = get_compressibility(this_point.temperature,
                                                                         pressure_floats[pressure_ind],
                                                                         this_point.volume_m3);
                        this_pressure.push_back(this_point);//add current temperature to the pressure it corresponds to
                    }
                    else
                    {
                        cerr << "Data line in read_reference_data() is empty--check your input file"
                             << endl;
                    }
                }
                this_species.push_back(this_pressure);//add current pressure to species it corresponds to
            }
            else
            {
                cerr << "Error in opening file " << file_name
                     << " in read_reference_data()" << endl;
            }
            input.close();
        }
        NIST_data.push_back(this_species);//add current species to all data
    }
    return NIST_data;
}

