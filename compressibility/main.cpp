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
    vector<general_run_data> general_runs = set_up_general_runs(argc, argv);
    auto beg = general_runs.begin(),
         end = general_runs.end();

    return 0;
}