//
// Created by laratelli on 7/13/17.
//

#include "compressibility.h"

using namespace std;

map<string, map<string, vector<isobar_reference_data>>> read_isobar_reference_data()
{
    map<string, map<string, vector<isobar_reference_data>>> NIST_data;
    vector<string> species = {"AR","CH4","CO2","H2","HE","KR","N2","NE","XE"};
    map<string, double> pressures = { {"0.1", 00.1},
                                      {"1.0", 01.0},
                                      {"5.0", 05.0},
                                      {"10.0", 10.0},
                                      {"20.0", 20.0},
                                      {"30.0", 30.0}
    };
    for(string s : species)
    {
        for(auto p : pressures)
        {
            string fileName = "ISOBAR_REFERENCE_DATA/" + s + "/" + p.first;
            fstream file(fileName);
            if(file.is_open())
            {
                double temperature = 0.0,
                        volume = 0.0;
                while(file >> temperature >> volume)
                {
                    isobar_reference_data this_point;
                    this_point.temperature = temperature;
                    this_point.volume_l_mol = volume;
                    double liters = this_point.volume_l_mol * MOLES;
                    this_point.volume_m3 = liters / 1000.0;
                    this_point.compressibility = get_compressibility(this_point.temperature,
                                                                     p.second*101325,
                                                                     this_point.volume_m3);
                    NIST_data[s][p.first].push_back(this_point);
                }
            }
            file.close();
        }
    }
    return NIST_data;
}

void file_output(vector<vector<run>> all_runs,
                 vector<general_run_data> general_runs,
                 map<string, map<string, vector<isobar_reference_data>>> NIST_data,
                 char ** argv)
{
    int i = 1;
    for(auto all_beg = all_runs.begin();all_beg != all_runs.end();all_beg++)
    {
        string input_name = argv[i];
        auto file_name = input_name + ".OUT";
        ofstream output_file(file_name);
        cout << "FILE NAME FOR OUTPUT : " << file_name << endl;
        for(auto mini_beg = all_beg->begin();mini_beg != all_beg->end();mini_beg++)
        {
            double reference_Z = get_reference_data_for_output(general_runs[i-1].species,
                                                               mini_beg->pressure_atm,
                                                               mini_beg->temperature,
                                                               NIST_data);

            output_file << mini_beg->temperature << ",     "
                        << mini_beg->pressure_atm << ",    "
                        << mini_beg->simulation_Z << ",  "
                        << mini_beg->EOS_Z << ", "
                        << reference_Z << ", "
                        << mini_beg->EOS_fugacity
                        << endl;
        }
        output_file.close();
        i++;
    }
}

double get_reference_data_for_output(string atom_type,
                                     double pressure_atm,
                                     double this_temperature,
                                     map<string, map<string, vector<isobar_reference_data>>> NIST_data)
{
    vector<string> species = {"AR","CH4","CO2","H2","HE","KR","N2","NE","XE"};
    map<string, double> pressures = { {"0.1", 00.1},
                                      {"1.0", 01.0},
                                      {"5.0", 05.0},
                                      {"10.0", 10.0},
                                      {"20.0", 20.0},
                                      {"30.0", 30.0}
    };
    string this_pressure;
    for(auto p : pressures)
    {
        if(p.second == pressure_atm)
        this_pressure = p.first;
    }
    double reference_Z;
    auto nist_begin = NIST_data[atom_type][this_pressure].begin(),
        nist_end = NIST_data[atom_type][this_pressure].end();
    while(nist_begin != nist_end)
    {
        /*
         * the (int) cast in the next line is horrific but necessary due to the limitations of
         * the NIST fluids reference page; if you request more than a certain number of state
         * points for a specific species, it adjusts the interval between these state points
         * for the total number of points to be equal to the cap. I chose to just use the data
         * for the first temperature closest to the temperature of interest than try to fight NIST,
         * as one is less work than the other
         */
        if (this_temperature == (int)nist_begin->temperature)
        {
            reference_Z = nist_begin->compressibility;
            break;
        }
        nist_begin++;
    }
    return reference_Z;
}



map<string, map<double, vector<isotherm_reference_data>>> read_isotherm_reference_data()
{
    map<string, map<double, vector<isotherm_reference_data>>> NIST_data;
    vector<string> species = {"AR","CH4","CO2","H2","HE","KR","N2","NE","XE"};

    namespace fs = experimental::filesystem;

    for(string s : species)
    {
        vector<string> this_species_temps = {};
        string path = "ISOTHERM_REFERENCE_DATA";
        for(auto p : fs::directory_iterator(path))
        {
            this_species_temps.emplace_back(p);
        }
        string my_file_name = path + "/" + s + "/";
        for(auto t : this_species_temps)
        {
            my_file_name += t;
            fstream file(my_file_name);
            if(file.is_open())
            {
                double pressure = 0.0,
                        volume = 0.0;
                while(file >> pressure >> volume)
                {
                    isotherm_reference_data this_point;
                    this_point.temperature = stod(t);
                    this_point.pressure = pressure;
                    this_point.volume_l_mol = volume;
                    double liters = this_point.volume_l_mol * MOLES;
                    this_point.volume_m3 = liters / 1000.0;
                    this_point.compressibility = get_compressibility(this_point.temperature,
                                                                     pressure*101325,
                                                                     this_point.volume_m3);
                    NIST_data[s][this_point.temperature].push_back(this_point);
                }
            }
        }
    }
    return NIST_data;
}
