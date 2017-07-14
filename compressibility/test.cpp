#include <iostream>
#include <experimental/filesystem>
#include <vector>
#include <map>
#include <fstream>

typedef struct isotherm_reference_data
{
	double temperature,
	       pressure,
	       volume_l_mol,
	       volume_m3,
	       compressiblity,
	       fugacity;
} isotherm_ref;

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
                    isotherm_ref this_point;
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

int main()
{
	map<string, map<double, vector<isotherm_reference_data>>> my_data = read_isotherm_reference_data();
	return 0;
}
