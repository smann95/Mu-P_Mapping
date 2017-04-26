#include "fugacity.h"

void read_simulation_input(run * runs)
{
  FILE * input;
  input = fopen("co2.dat","r");
  char * line = (char*)malloc(sizeof(char)*256);
  int i = 0;
  while(fgets(line,256,input))
  {
    sscanf(line,"%lf %lf %lf %lf",&(runs+i)->temperature,&(runs+i)->pressure_bar,&(runs+i)->simulation_N,&(runs+i)->simulation_volume);
    i++;
  }
  free(line);
  fclose(input);
}


void give_structs_species_data(run * runs, int num_of_runs)
{
  for(int i = 0;i<num_of_runs;i++)
  {
    (runs+i)->mass = 44.01;
  }
}
void convert_to_proper_units(run * runs, int num_of_runs)
{
 //convert mass from g/mol to kg/molecule
  for(int i = 0;i<num_of_runs;i++)
  {
    (runs+i)->mass /= AVOGADRO;//grams per molecule
    (runs+i)->mass /= G_IN_KG;//kilograms per molecule
  }
 //convert volume to meters^3
  for(int i = 0;i<num_of_runs;i++)
  {
    (runs+i)->simulation_volume *= CUBIC_A_TO_CUBIC_M;
  }
 //convert pressure
  for(int i = 0;i<num_of_runs;i++)
  {
    (runs+i)->pressure_atm = (runs+i)->pressure_bar*BAR_TO_ATM;
    (runs+i)->pressure_pa = (runs+i)->pressure_bar*BAR_TO_PASCAL;
  }
}

void get_state_fugacity(run * runs,int num_of_runs)
{
  for(int i = 0;i<num_of_runs;i++)
  {
    (runs+i)->state_fugacity = co2_pengr_fugacity((runs+i)->temperature,(runs+i)->pressure_bar);
  }
}
 
void get_state_excess_mu(run * runs, int num_of_runs)
{
  for(int i = 0;i<num_of_runs;i++)
  {
    (runs+i)->state_excess_mu = log((runs+i)->state_fugacity/((runs+i)->pressure_atm))*GAS_CONSTANT*(runs+i)->temperature;
    (runs+i)->state_excess_mu /= J_TO_KJ;
  }
}


