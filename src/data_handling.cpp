// Created by Luciano Laratelli on 17/05/2017.
//

#include "compressibility.h"

using namespace std;

/*
 * set_up_general_runs reads the first two lines of the input file(s) to determine the
 * species of each group of simulations and how many state points are in each
 * group of simulations
 */



vector<general_run_data> set_up_general_runs(int argc, char **argv)
{
    vector<general_run_data> general_runs;
    string file_name,
            line;
    for (auto i = 1; i < argc; i++)//argv[0] is the program name, don't forget!
    {
        file_name = argv[i];
        ifstream input(file_name);
        if (input.is_open())
        {
            general_run_data this_species;
            getline(input, line);//this line contains species string
            this_species.species = line;
            getline(input, line);
            getline(input, line);//this line contains number of state points for current species
            stringstream convert(line);//get num as an int
            convert >> this_species.num_runs;
            general_runs.push_back(this_species);
        } else//check the execution directory to make sure your small_dataset is there
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

    for (gen_beg; gen_beg != gen_end; gen_beg++)
    {
        vector<run> this_run;
        for (auto i = 0; i < gen_beg->num_runs; i++)
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
    if (boost::iequals(atom_type, "co2"))//case-insensitive string comparison
        mass = 44.0095;
    else if (boost::iequals(atom_type, "n2"))
        mass = 28.0134;
    else if (boost::iequals(atom_type, "h2"))
        mass = 2.016;
    else if (boost::iequals(atom_type, "ch4"))
        mass = 16.04260;
    else if (boost::iequals(atom_type, "he"))
        mass = 4.002602;
    else if (boost::iequals(atom_type, "ne"))
        mass = 20.1797;
    else if (boost::iequals(atom_type, "xe"))
        mass = 131.293;
    else if (boost::iequals(atom_type, "kr"))
        mass = 83.798;
    else if (boost::iequals(atom_type, "ar"))
        mass = 39.948;
    return mass;
}

/*
 * Now that we've set up our vectors, we read the simulation small_dataset
 * from our input file into the vectors
 */
void read_simulation_data(int argc, char **argv, vector<vector<run>> &all_runs)
{
    string file_name,
            a_line;
    for (int i = 1; i < argc; i++)
    {
        int j = 0;
        file_name = argv[i];
        ifstream input(file_name);
        getline(input, a_line);
        getline(input, a_line);
        getline(input, a_line);
        int num_runs;
        stringstream convert(a_line);
        if (!(convert >> num_runs))
        {
            cerr << "Error in converting number of runs to integer in data_handling.cpp"
                 << "function read_simulation_data()"
                 << endl;
            exit(1);//error handling
        }
        if (all_runs[i - 1][j].atom_type == "H2" || all_runs[i - 1][j].atom_type == "HE")
        {
            getline(input, a_line);//helium and H2 have an extra line in their input file
        }
        getline(input, a_line);
        if (input.is_open())
        {
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
                if (!this_line.empty())
                {
                    auto &ref = (all_runs[i - 1])[j];//make current run a ref to clean up the code a bit
                    ref.temperature = atof(this_line[0].c_str());
                    ref.pressure_atm = atof(this_line[1].c_str());
                    ref.simulation_V = atof(this_line[2].c_str());
                    ref.density = atof(this_line[3].c_str());
                    if (j == (num_runs - 1))
                    {
                        break;
                    } else
                    {
                        j++;
                    }
                } else
                {
                    cout << "Data line in read_simulation_data() is empty--check your input file"
                         << endl;
                }
            }
        } else
        {
            cerr << "Error in opening file " << file_name
                 << " in read_simulation_data() " << endl;
        }
        input.close();
    }
}

void convert_data_to_other_units(vector<vector<run>> &all_runs, vector<general_run_data> general_runs)
{
    for (int i = 0; i < general_runs.size(); i++)
    {
        for (int j = 0; j < general_runs[i].num_runs; j++)
        {
            auto &ref = (all_runs[i])[j];
            ref.pressure_bar = ref.pressure_atm / BAR_TO_ATM;
            ref.pressure_pa = ref.pressure_bar * BAR_TO_PASCAL;
            ref.mass /= AVOGADRO;
            ref.mass /= G_IN_KG;
            ref.simulation_V *= CUBIC_A_TO_CUBIC_M;
        }
    }
}


void calculate_data(vector<vector<run>> &all_runs)
{
    for (auto &all_run : all_runs)
    {
        for (auto mini_beg = all_run.begin(); mini_beg != all_run.end(); mini_beg++)
        {
            mini_beg->simulation_Z = get_compressibility(mini_beg->temperature, mini_beg->pressure_pa, mini_beg->simulation_V);
            mini_beg->EOS_Z = solve_peng_robinson_for_compressibility(mini_beg->temperature, mini_beg->pressure_atm, *mini_beg);
            mini_beg->EOS_fugacity = solve_peng_robinson_for_fugacity(mini_beg->temperature, mini_beg->pressure_atm, *mini_beg);
        }
    }
}

//takes pressure in Pa and volume in m^3
double get_compressibility(double temperature, double pressure, double volume)
{
    double num = 64.0 / AVOGADRO;//n is in moles
    return (pressure * volume) / (num * GAS_CONSTANT * temperature);
}

void get_simulation_fugacities(int argc, char **argv)
{
    for (int i = 1; i < argc; i++)
    {
        string input_name = argv[i];
        auto file_name = input_name + ".OUT";
        auto command = "python ../scripts/integrate_simulation_data.py " + file_name;
        system(command.c_str());
    }
}
