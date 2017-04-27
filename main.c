#include "fugacity.h"

int main(int argc, char ** argv)
{
  if(argc != 2)
  {
    printf("Wrong number of args. Takes input filename as only input.\n");
    exit(EXIT_FAILURE);
  }

  int num_of_runs;
  char * first_line = (char*)malloc(sizeof(char)*256),
       * file_name =  (char*)malloc(sizeof(char)*256);
  strcpy(file_name,argv[1]);

  FILE * input;
  input = fopen(file_name,"r");
  if(input == NULL)
  {
    printf("Failed to open input file in main().\nTry again.");
    return 1;
  }
  fgets(first_line,256,input);
  sscanf(first_line,"%d\n",&num_of_runs);

  run runs[num_of_runs];//make a struct for every run
  double ** output_array = (double**)malloc(num_of_runs*sizeof(double*));
  for(int i = 0;i<num_of_runs;i++)
  {
    output_array[i] = (double*)malloc(sizeof(double)*outputs);
  }

  give_structs_species_data(runs, num_of_runs);
  read_simulation_input(runs, file_name);
  convert_to_proper_units(runs,num_of_runs);

  get_state_fugacity(runs,num_of_runs);
  get_state_excess_mu(runs,num_of_runs);

  get_ideal_mu(runs,num_of_runs);

  get_simulation_mu(runs,num_of_runs);                                         
  get_simulation_fugacity(runs,num_of_runs);

  populate_output_array(*output_array,runs,num_of_runs);
  output(*output_array,file_name,num_of_runs);
  /*
  printf("SIMULATION V/N                =    %.12e\n",SIM_V/SIM_N);
  printf("IDEAL RT/P                    =    %.12e\n",((GAS_CONSTANT*T)/(p*BAR_TO_PASCAL))/AVOGADRO);
  */
  fclose(input);
  free(first_line);
  free(file_name);
  for(int i = 0;i<num_of_runs;i++)
  {
    free(output_array[i]);
  }
  return 0;
}
