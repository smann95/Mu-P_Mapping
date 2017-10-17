//
// Created by laratelli on 7/13/17.
//

#include "compressibility.h"

using namespace std;
using namespace boost::filesystem;

map<string, map<string, vector<isotherm_reference_data>>> read_reference_data()
{
    map<string, map<string, vector<isotherm_reference_data>>> NIST_data;
    vector<string> species = {"AR", "CH4", "CO2", "H2", "HE", "KR", "N2", "NE", "XE"};

    for(const auto &s : species)
    {
        vector<string> this_species_temps = {};
        get_species_temperatures(this_species_temps, s);
        path p("../misc/ISOTHERM_REFERENCE_DATA/" + s);
        for(const auto &t : this_species_temps)
        {
            string my_file_name = "/home/luciano/Dropbox/Mu-P_Mapping/misc/ISOTHERM_REFERENCE_DATA/";
            my_file_name.append(s);
            my_file_name.append("/");
            my_file_name.append(t);


            std::fstream file(my_file_name);
            if(file.is_open())
            {
                double pressure = 0.0,
                        volume = 0.0,
                        fugacity = 0.0;
                while(file >> pressure >> volume >> fugacity)
                {
                    isotherm_reference_data this_point;
                    this_point.pressure = pressure;
                    this_point.volume_l_mol = volume;
                    double liters = this_point.volume_l_mol * MOLES;
                    this_point.volume_m3 = liters / 1000.0;
                    this_point.compressibility = get_compressibility(stod(t),
                                                                     pressure * 101325,
                                                                     this_point.volume_m3);
                    this_point.fugacity = fugacity;
                    NIST_data[s][t].push_back(this_point);
                }
            } else
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
                 char **argv)
{
    int i = 1;
    for(auto &all_run : all_runs)
    {
        string input_name = argv[i];
        auto file_name = input_name + ".OUT";
        std::ofstream output_file(file_name);
        cout << "FILE NAME FOR OUTPUT : " << file_name << endl;
        for(auto &mini_beg : all_run)
        {
            double reference_Z = get_reference_compressibility(general_runs[i - 1].species,
                                                               mini_beg.pressure_atm,
                                                               mini_beg.temperature,
                                                               NIST_data);
            double reference_f = get_reference_fugacity(general_runs[i - 1].species,
                                                        mini_beg.pressure_atm,
                                                        mini_beg.temperature,
                                                        NIST_data);

            output_file << mini_beg.temperature << " "
                        << mini_beg.pressure_atm << " "
                        << mini_beg.simulation_Z << " "
                        << mini_beg.EOS_Z << " "
                        << reference_Z << " "
                        << mini_beg.EOS_fugacity << " "
                        << reference_f << " "
                        << endl;
        }
        output_file.close();
        i++;
    }
}

double get_reference_compressibility(string atom_type,
                                     double pressure_atm,
                                     double this_temperature,
                                     map<string, map<string, vector<isotherm_reference_data>>> NIST_data)
{
    ostringstream temperature_string;
    temperature_string << this_temperature;
    auto beg = NIST_data[atom_type][temperature_string.str() + "OUT"].begin(),
            end = NIST_data[atom_type][temperature_string.str() + "OUT"].end();
    while(beg != end)
    {
        if(beg->pressure == pressure_atm)
        {
            return beg->compressibility;
        }
        beg++;
    }
}

double get_reference_fugacity(string atom_type,
                              double pressure_atm,
                              double this_temperature,
                              map<string, map<string, vector<isotherm_reference_data>>> NIST_data)
{
    ostringstream temperature_string;
    temperature_string << this_temperature;
    auto beg = NIST_data[atom_type][temperature_string.str() + "OUT"].begin(),
            end = NIST_data[atom_type][temperature_string.str() + "OUT"].end();
    double reference_f = 0.0;
    while(beg != end)
    {
        /*
         * this next line is a hack to get some state points that NIST can't get exactly; this was done to see how
         * reproducible the results of luo and lloyd are. Ideally, only the first conditional would be needed but
         * that is sadly not the case
         */
        if((beg->pressure == pressure_atm) || fabs(beg->pressure-pressure_atm) < .1)
        {
            return beg->fugacity;
        }
        beg++;
    }
    return reference_f;
}

void get_species_temperatures(vector<string> &this_species_temps, string species)
{
    string this_path = "/home/luciano/Dropbox/Mu-P_Mapping/misc/ISOTHERM_REFERENCE_DATA/";
    this_path.append(species);
    path p(this_path);
    for(auto i = directory_iterator(p); i != directory_iterator(); i++)
    {
        if(!is_directory(i->path()))
        {
            this_species_temps.push_back(i->path().filename().string());
        } else
        {
            continue;
        }
    }
}

