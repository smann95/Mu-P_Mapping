#include "fugacity.h"

 /************************************************************************
 * Reads in input from the .dat file created by my bash script and
 * stores it into the runs struct in fugacity.h
 *************************************************************************/
void read_simulation_input(run * runs, char * file_name)
{
  FILE * input;
  input = fopen(file_name,"r");
  if(input == NULL)
  {
    printf("Failed to open input file in read_simulation_input().\nTry again.");
    exit(EXIT_FAILURE);
  }
  char * line = malloc(sizeof(char)*256);
  int i = 0;

  fgets(line,256,input);//first three lines are trash
  fgets(line,256,input);
  fgets(line,256,input);

  while(fgets(line,256,input))
  {
    sscanf(line,"%lf %lf %lf %lf",&runs[i].temperature,&runs[i].pressure_bar,&runs[i].simulation_N,&runs[i].simulation_volume);
    i++;
  }
  free(line);
  fclose(input);
}


 /************************************************************************
 * TODO: add atom type checking here, this only works for CO2 at the moment
 *************************************************************************/
void give_structs_species_data(run * runs, int num_of_runs)
{
  for(int i = 0;i<num_of_runs;i++)
  {
    runs[i].mass = 44.01;
  }
}

 /************************************************************************
 * We need certain properties in certain units, so we convert them
 *************************************************************************/

void convert_to_proper_units(run * runs, int num_of_runs)
{
 //convert mass from g/mol to kg/molecule
  for(int i = 0;i<num_of_runs;i++)
  {
    runs[i].mass /= AVOGADRO;//grams per molecule
    runs[i].mass /= G_IN_KG;//kilograms per molecule
  }
 //convert volume to meters^3
  for(int i = 0;i<num_of_runs;i++)
  {
    runs[i].simulation_volume *= CUBIC_A_TO_CUBIC_M;
  }
 //convert pressure
  for(int i = 0;i<num_of_runs;i++)
  {
    runs[i].pressure_atm = runs[i].pressure_bar*BAR_TO_ATM;
    runs[i].pressure_pa = runs[i].pressure_bar*BAR_TO_PASCAL;
  }
}

 /************************************************************************
 * Gets fugacity using the Peng-Robinson equation of state, code provided
 * by the good folks at the Space lab and their MPMC code
 *************************************************************************/
void get_state_fugacity(run * runs,int num_of_runs)
{
  for(int i = 0;i<num_of_runs;i++)
  {
    runs[i].state_fugacity = co2_pengr_fugacity(runs[i].temperature,runs[i].pressure_atm);
  }
}

 /***********************************************************************
  * Gets the chemical potential of a gas with the fugacity given by the 
  * equation of state above
  ***********************************************************************/
void get_state_excess_mu(run * runs, int num_of_runs)
{
  for(int i = 0;i<num_of_runs;i++)
  {
    runs[i].state_excess_mu = log(runs[i].state_fugacity/(runs[i].pressure_atm))*GAS_CONSTANT*runs[i].temperature;
    runs[i].state_excess_mu /= J_TO_KJ;
  }
}


  /***********************************************************************
   * Chemical potential of an ideal gas, using stat mech and the ideal gas 
   * law 
   ***********************************************************************/
void get_ideal_mu(run * runs, int num_of_runs)
{
  double first_term,
         RT_over_p,
         log_exp,
         log_term;
  for(int i = 0;i<num_of_runs;i++)
  {
    first_term = -BOLTZMANN_KJ_MOLAR * runs[i].temperature;
    RT_over_p =(GAS_CONSTANT *runs[i].temperature)/(runs[i].pressure_pa)/AVOGADRO;
    log_exp = pow(((10.0*M_PI*runs[i].mass*BOLTZMANN_J_PER_K*runs[i].temperature)/(3.0*PLANCK*PLANCK)),(3.0/2.0)),
    log_term = RT_over_p * log_exp;
    runs[i].ideal_gas_mu = first_term * log(log_term);
  }
}
 /***********************************************************************
  * Get chemical potential for the simulation using statistical mechanics
  * and the average particle number / box volume size from the simulation
  ***********************************************************************/
void get_simulation_mu(run * runs, int num_of_runs)
{
  double first_term,
         sim_v_over_sim_n,
         log_exp,
         log_term;
  for(int i = 0;i<num_of_runs;i++)
  {
    first_term = -BOLTZMANN_KJ_MOLAR * runs[i].temperature,
    sim_v_over_sim_n = runs[i].simulation_volume / runs[i].simulation_N,
    log_exp = pow(((10.0*M_PI*runs[i].mass*BOLTZMANN_J_PER_K*runs[i].temperature)/(3.0*PLANCK*PLANCK)),(3.0/2.0)),
    log_term = sim_v_over_sim_n * log_exp;
    runs[i].simulation_mu = first_term * log(log_term);
  }
}

 /**********************************************************************
  * get fugacity of the simulation from the chemical potential and the
  * ideal gas chemical potential
  **********************************************************************/
void get_simulation_fugacity(run * runs, int num_of_runs)
{
  double exp_term,
         simulation_excess;
  for(int i = 0;i<num_of_runs;i++)
  {
    simulation_excess = runs[i].simulation_mu - runs[i].ideal_gas_mu;
    exp_term = -(simulation_excess/(BOLTZMANN_KJ_MOLAR*runs[i].temperature));
    runs[i].simulation_fugacity = exp(exp_term) * runs[i].pressure_atm;
  }
}


 /***********************************************************************
  * Ok, now we have our data and store it in an array
  ***********************************************************************/
void populate_output_array(double ** output_array, run * runs, int num_of_runs)
{
  for(int i = 0;i<num_of_runs;i++)
  {
    output_array[i][0] = runs[i].temperature;
    output_array[i][1] = runs[i].pressure_atm;
    output_array[i][2] = runs[i].simulation_fugacity;
    output_array[i][3] = runs[i].state_fugacity;
    output_array[i][4]  = runs[i].state_excess_mu;
    output_array[i][5] = runs[i].ideal_gas_mu;
    output_array[i][6] = runs[i].simulation_mu;
  }
}

 /***********************************************************************
  * And now we output
  ***********************************************************************/
void output(double ** output_array, char * file_name ,int num_of_runs)
{
  FILE * output;
  char * output_name = strtok(file_name,".");
  strcat(output_name,".csv");
  output = fopen(output_name,"w");

  fprintf(output,"#T     \t      #P_ATM      #SIMFUG     #STATEFUG   #STATE_EX_MU   #IDEAL_MU     #SIMULATION_MU\n");

  for(int i = 0;i<num_of_runs;i++)
  {
    for(int j = 0;j<OUTPUTS;j++)
    {
      fprintf(output,"%lf,",output_array[i][j]);
    }
    fprintf(output,"\n");
  }
  fclose(output);
}
