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

    for(const auto &s : species)
    {
        vector<string> this_species_temps = {};
        get_species_temperatures(this_species_temps, s);
        path p("../misc/ISOTHERM_REFERENCE_DATA/" + s);
        for(const auto &t : this_species_temps)
        {
            string my_file_name = "../misc/ISOTHERM_REFERENCE_DATA/" + s + "/" + t;
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
    for (auto &all_run : all_runs)
    {
        string input_name = argv[i];
        auto file_name = input_name + ".OUT";
        std::ofstream output_file(file_name);
        cout << "FILE NAME FOR OUTPUT : " << file_name << endl;
        for (auto &mini_beg : all_run)
        {
            double reference_Z = get_reference_compressibility(general_runs[i-1].species,
                                                               mini_beg.pressure_atm,
                                                               mini_beg.temperature,
                                                               NIST_data);
            double reference_f = get_reference_fugacity(general_runs[i-1].species,
                                                               mini_beg.pressure_atm,
                                                               mini_beg.temperature,
                                                               NIST_data);

            output_file << mini_beg.temperature << ",     "
                        << mini_beg.pressure_atm << ",    "
                        << mini_beg.simulation_Z << ",  "
                        << mini_beg.EOS_Z << ", "
                        << reference_Z << ", "
                        << mini_beg.EOS_fugacity << ", "
                        << reference_f
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

double get_reference_fugacity(string atom_type,
                              double pressure_atm,
                              double this_temperature,
                              map<string, map<string, vector<isotherm_reference_data>>> NIST_data)
{
    ostringstream temperature_string;
    temperature_string << this_temperature;
    auto beg = NIST_data[atom_type][temperature_string.str()].begin(),
            end = NIST_data[atom_type][temperature_string.str()].end();
    double reference_f = 0.0;
    while(beg != end)
    {
        if(beg->pressure == pressure_atm)
        {
            reference_f = beg->fugacity;
        }
        beg++;
    }
    return reference_f;
}
void get_species_temperatures(vector<string> & this_species_temps, string species)
{
    path p("../misc/ISOTHERM_REFERENCE_DATA/" + species);
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

void calculate_reference_fugacities(std::map<std::string, std::map<std::string, std::vector<isotherm_reference_data>>> NIST_data)
{
    vector<string> species = {"AR", "CH4", "CO2", "H2", "HE", "KR", "N2", "NE", "XE"};

    for (const auto &s : species)
    {
        vector<string> this_species_temps = {};
        get_species_temperatures(this_species_temps, s);
        for(const auto &t : this_species_temps)
        {
            auto beg = NIST_data[s][t].begin(),
                    end = NIST_data[s][t].end();
            while(beg != end)
            {
                double integral_result = integrate_compressibility_for_fugacity(beg->pressure, NIST_data);
                beg->fugacity = beg->pressure * exp(integral_result);
                beg++;
            }
        }
    }
}

/*this function uses the trapezoid rule to integrate for fugacity*/

double integrate_compressibility_for_fugacity(double pressure_atm, map<string, map<string,vector<isotherm_reference_data>>> & NIST_data)
{
    vector<string> species = {"AR", "CH4", "CO2", "H2", "HE", "KR", "N2", "NE", "XE"};
    double fugacity = 0.0;
    for (const auto &s : species)
    {
      vector<string> this_species_temps;
      get_species_temperatures(this_species_temps, s);
      for(const auto &t : this_species_temps)
      {
        auto beg = NIST_data[s][t].begin(),
             end = NIST_data[s][t].end();
        fugacity += (beg->compressibility + 1.0) / pressure_atm;
        end--;//need to get last term for trapezoid rule
        fugacity += (end->compressibility + 1.0) / pressure_atm;
        beg ++;//already got the first term, advance pointer
        end ++;//put pointer back where it should be

        auto diff = abs(beg->pressure - pressure_atm);
        while(beg != end)
        {
            if(diff < 10e-2) {
                break;
            }
            double this_term = (beg->compressibility + 1.0) / pressure_atm;
            fugacity += 2.0 * this_term;
            beg++;
            diff = abs(beg->pressure - pressure_atm);
        }
      }
    }
    auto num_intervals = (pressure_atm - .1) / .01,
         outer_term = (pressure_atm - .1) / num_intervals;
    fugacity *= outer_term;
    return fugacity;
}
