#include "fugacity.h"

int main(int argc, char ** argv)
{
  int num_of_runs = atoi(argv[1]);
      
  run runs[num_of_runs];//make a struct for every run
  give_structs_species_data(runs, num_of_runs);
  read_simulation_input(runs);
  convert_to_proper_units(runs,num_of_runs);
  get_state_fugacity(runs,num_of_runs);
  get_state_excess_mu(runs,num_of_runs);
  get_ideal_mu(runs,num_of_runs);
  get_simulation_mu(runs,num_of_runs);
  get_simulation_fugacity(runs,num_of_runs);
  /*
  printf("SIMULATION V/N                =    %.12e\n",SIM_V/SIM_N);
  printf("IDEAL RT/P                    =    %.12e\n",((GAS_CONSTANT*T)/(p*BAR_TO_PASCAL))/AVOGADRO);
  */
  return 0;
}
