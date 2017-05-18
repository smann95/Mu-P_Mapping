#include "fugacity.h"

int main(int argc, char ** argv)
{
  if(argc < 2)
  {
    printf("Wrong number of args. Takes input filenames only, with at least one.\n");
    exit(EXIT_FAILURE);
  }

  int num_of_runs = 0;
  char * line = (char*)malloc(sizeof(char)*256),
       * file_name =  (char*)malloc(sizeof(char)*256);
  char ** total_species = malloc(sizeof(char*)*(argc-1));
  /*************************************************************
   * ONLY WORKS FOR ONE ATOM TYPE RIGHT NOW
   * This should probably be split up into its own function
   * eventually
   * ***********************************************************/
  for(int i = 0;i<argc;i++)
  {
    strcpy(file_name,argv[i]);

    FILE * input;
    input = fopen(file_name,"r");
    if(input == NULL)
    {
      printf("Failed to open input file %d in main().\nTry again.",i+1);
      return 1;
    }
    fgets(line,256,input);
    sscanf(line,"%d",&num_of_runs);
    fclose(input);
  }
  run * runs = malloc(sizeof(run)*num_of_runs); //make a struct for every run
  double ** output_array = malloc(num_of_runs*sizeof(double*));
  for(int i = 0;i<num_of_runs;i++)
  {
    output_array[i] = malloc(sizeof(double)*OUTPUTS);
  }

  give_structs_species_data(runs, num_of_runs);
  read_simulation_input(runs, file_name);
  convert_to_proper_units(runs,num_of_runs);

  get_state_fugacity(runs,num_of_runs);
  get_state_excess_mu(runs,num_of_runs);

  get_ideal_mu(runs,num_of_runs);

  get_simulation_mu(runs,num_of_runs);
  get_simulation_fugacity(runs,num_of_runs);

  populate_output_array(output_array,runs,num_of_runs);
  output(output_array,file_name,num_of_runs);

  for(int i = 0;i<num_of_runs;i++)
  {
    free(output_array[i]);
  }
  free(output_array);
  free(file_name);
  free(runs);
  free(line);
  free(total_species);
  return 0;
}
