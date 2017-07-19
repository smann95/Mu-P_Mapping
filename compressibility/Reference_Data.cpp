//
// Created by laratelli on 7/13/17.
//

#include "compressibility.h"

using namespace std;
using namespace boost::filesystem;

map<string, map<string, vector<isotherm_reference_data>>> read_reference_data()
{
    map<string, map<string, vector<isotherm_reference_data>>> NIST_data;
    vector<string> species = {"AR","CH4","CO2","H2","HE","KR","N2","NE","XE"};

    for(string s : species)
    {
        vector<string> this_species_temps = {};
        get_species_temperatures(this_species_temps, s);
        path p("ISOTHERM_REFERENCE_DATA/" + s);
        for(auto t : this_species_temps)
        {
            string my_file_name = "ISOTHERM_REFERENCE_DATA/" + s + "/" + t;
            std::fstream file(my_file_name);
            if(file.is_open())
            {
                double pressure = 0.0,
                        volume = 0.0;
                while(file >> pressure >> volume)
                {
                    isotherm_reference_data this_point;
                    this_point.pressure = pressure;
                    this_point.volume_l_mol = volume;
                    double liters = this_point.volume_l_mol * MOLES;
                    this_point.volume_m3 = liters / 1000.0;
                    this_point.compressibility = get_compressibility(stod(t),
                                                                     pressure*101325,
                                                                     this_point.volume_m3);
                    NIST_data[s][t].push_back(this_point);
                }
            }
            else
            {
                cout << "Error in opening file " << my_file_name << " in read_isotherm_reference_data()" << endl;
            }
        }
    }
    return NIST_data;
}


void file_output(vector<vector<run>> all_runs,
                 vector<general_run_data> general_runs,
                 map<string, map<string, vector<isotherm_reference_data>>> NIST_data,
                 char ** argv)
{
    int i = 1;
    for(auto all_beg = all_runs.begin();all_beg != all_runs.end();all_beg++)
    {
        string input_name = argv[i];
        auto file_name = input_name + ".OUT";
        std::ofstream output_file(file_name);
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
                                     map<string, map<string, vector<isotherm_reference_data>>> NIST_data) {
    ostringstream temperature_string;
    temperature_string << this_temperature;
    auto beg = NIST_data[atom_type][temperature_string.str()].begin(),
         end = NIST_data[atom_type][temperature_string.str()].end();
    double reference_Z = 0.0;
    while(beg != end)
    {
        if(beg->pressure == pressure_atm)
        {
            reference_Z = beg->compressibility;
        }
        beg++;
    }
    return reference_Z;
}

void get_species_temperatures(vector<string> & this_species_temps, string species)
{
    path p("ISOTHERM_REFERENCE_DATA/" + species);
    for (auto i = directory_iterator(p); i != directory_iterator(); i++)
    {
        if (!is_directory(i->path()))
        {
            this_species_temps.push_back(i->path().filename().string());
        }
        else
        {
            continue;
        }
    }
}

double get_reference_fugacity(map<string, map<string, vector<isotherm_reference_data>>> NIST_data)
{

    vector<string> species = {"AR", "CH4", "CO2", "H2", "HE", "KR", "N2", "NE", "XE"};

    for (auto s : species)
    {
        vector<string> this_species_temps = {};
        get_species_temperatures(this_species_temps, s);
        for(auto t : this_species_temps)
        {
            auto beg = NIST_data[s][t].begin(),
                 end = NIST_data[s][t].end();
            while(beg != end)
            {

                beg++;
            }
        }
    }


}


