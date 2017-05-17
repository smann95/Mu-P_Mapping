#include "compressibility.h"

using namespace std;

int main(int argc, char ** argv)
{
    if(argc < 2)
    {
        cerr << "Wrong number of args. Takes at least one input filename.\n"
             << endl;
        return 1;
    }
    int num_of_runs = 0;
    string file_name,
           line;
    vector<general_run_data> general_runs;
    for(int i = 1;i < argc;i++)
    {
        file_name = argv[i];
        ifstream input;
        input.open("file_name");
        if(input.is_open())
        {
            general_run_data this_run;
            getline(input,line);
            this_run.species = line;
            getline(input,line);
            stringstream convert (line);
            convert >> this_run.num_runs;
            general_runs.push_back(this_run);
        }
        else
        {
            cerr << "Error in opening file " << file_name
                 << " in main() " << endl;
        }
    }
    auto beg = general_runs.begin(),
         end = general_runs.end();
    while(beg != end)
    {
        cout << "species" << beg->species
             << "num of runs"  << beg->num_runs
             << endl;
        beg++;
    }

    return 0;
}