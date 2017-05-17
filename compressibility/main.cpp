#include <iostream>
#include <vector>
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
    string line,
           file_name;
    for(int i = 0;i < argc;i++)
    {
        file_name = argv[i];
    }
    return 0;
}