#include <vector>
#include <map>
#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>


struct isotherm_reference_data
{
	double pressure,
	       volume_l_mol,
	       volume_m3,
	       compressiblity;
};
using namespace std;
using namespace boost::filesystem;
void get_species_temperatures(vector<string> & this_species_temps)
{
    vector<string> species = {"AR","CH4","CO2","H2","HE","KR","N2","NE","XE"};
    for(string s : species)
    {
        path p("ISOTHERM_REFERENCE_DATA/" + s);
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
}

map<string, map<string, vector<isotherm_reference_data>>> read_isotherm_reference_data()
{
    map<string, map<string, vector<isotherm_reference_data>>> NIST_data;
    vector<string> species = {"AR","CH4","CO2","H2","HE","KR","N2","NE","XE"};

    for(string s : species)
    {
        vector<string> this_species_temps = {};
        get_species_temperatures(this_species_temps);
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


int main()
{
    auto NIST_data =read_isotherm_reference_data();
    vector<string> species = {"AR","CH4","CO2","H2","HE","KR","N2","NE","XE"};

    for(string s : species)
    {
        vector<string> this_species_temps = {};
        get_species_temperatures(this_species_temps);
        auto temperature_beg = this_species_temps.begin(),
	     temperature_end = this_species_temps.end();
	while(temperature_beg != temperature_end)
	{
		auto current_temp = *temperature_beg;
		auto this_point_beg = NIST_data[s][current_temp].begin(),
		     this_point_end = NIST_data[s][current_temp].end();
		while(this_point_beg != this_point_end)
		{
			cout << s
			     << this_point_beg->volume_l_mol 
			     << endl;
			this_point_beg++;
		}
		temperature_beg++;
	}
    }

	return 0;
}
