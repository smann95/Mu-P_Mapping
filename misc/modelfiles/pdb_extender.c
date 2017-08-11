/*  This program is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 3 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  This program is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *                  University of South Florida                           *
 *                  Adam Hogan 2013                                       */

/*

VERSION 0.1

compile: gcc pdb_extender.c -o pdb_extender

usage: ./pdb_extender filename_input filenane_output axis

axis = x,y,z

Replicates a pdb in the x, y and z axises

Column #  Description
2     Atom ID
3     Site Name
4     Molecule Name
5     Moveable
6     Molecule ID
7,8,9 X, Y, and Z position coordinates (Å)
10    Mass (a.m.u.)
11    Charge (e)
12    Static Polarizability (Å³)
13    Lennard-Jones Epsilon (K)
14    Lennard-Jones Sigma (Å)
15    Dipole Frequency (a.u.)
16    ????

*/

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_NUM_OF_ATOMS 50000

struct Atom
{
  int atom_id;
  char site_name[255];
  char molecule_name[255];
  char moveable[255];
  int molecule_id;
  double x;
  double y;
  double z;
  char mass[255];
  char charge[255];
  char static_polarizability[255];
  char lj_epsilon[255];
  char lj_sigma[255];
  char dipole_freq[255];
  char question_marks[255];
  char c6[255];
  char c8[255];
};

struct Atom atoms[MAX_NUM_OF_ATOMS];

int main ( int argc, char * argv[] )
{
  char line[1000];
  if (argc!=4)
  {
    printf("Usage: %s filename_input filenane_output axis\n",argv[0]);
    printf("axis = x,y,z\n");
    return 1;
  }

  int axis = 0;

  // which axis are we doubling?
  if (argv[3][0]=='x')
  {
    axis = 0;
  }
  else if (argv[3][0]=='y')
  {
    axis = 1;
  }
  else if (argv[3][0]=='z')
  {
    axis = 2;
  }
  else
  {
    printf("axis = x,y,z\n");
    return 1;
  }

  FILE *file;
  file = fopen(argv[1], "r");
  if (file==NULL)
  {
    printf("Unable to open file %s\n",argv[1]);
    return 1;
  }

  FILE *output;
  output = fopen(argv[2], "w");
  if (output==NULL)
  {
    printf("Unable to open file %s\n",argv[2]);
    return 1;
  }

  double x_basis,y_basis,z_basis;
  x_basis = y_basis = z_basis = 100000000000.0;

  int num_of_atoms = 0;
  int num_of_molecules = 0;

  //read small_dataset and populate atoms
  char *first_part,*second_part;
  while (fgets(line,1000,file) != NULL && num_of_atoms < MAX_NUM_OF_ATOMS)
  {
    first_part = strtok(line," \t");
    if (strcmp(first_part,"ATOM")==0)
    {
      char string[255];
      atoms[num_of_atoms].atom_id = num_of_atoms+1;
      (char*)strtok(NULL," \t");
      strcpy(atoms[num_of_atoms].site_name, (char*)strtok(NULL," \t"));
      strcpy(atoms[num_of_atoms].molecule_name, (char*)strtok(NULL," \t"));
      strcpy(atoms[num_of_atoms].moveable, (char*)strtok(NULL," \t"));
      atoms[num_of_atoms].molecule_id = atoi((char*)strtok(NULL," \t"));
      atoms[num_of_atoms].x = atof((char*)strtok(NULL," \t"));
      atoms[num_of_atoms].y = atof((char*)strtok(NULL," \t"));
      atoms[num_of_atoms].z = atof((char*)strtok(NULL," \t"));
      strcpy(atoms[num_of_atoms].mass, (char*)strtok(NULL," \t"));
      strcpy(atoms[num_of_atoms].charge, (char*)strtok(NULL," \t"));
      strcpy(atoms[num_of_atoms].static_polarizability, (char*)strtok(NULL," \t"));
      strcpy(atoms[num_of_atoms].lj_epsilon, (char*)strtok(NULL," \t"));
      strcpy(atoms[num_of_atoms].lj_sigma, (char*)strtok(NULL," \t"));
      strcpy(atoms[num_of_atoms].dipole_freq, (char*)strtok(NULL," \t"));
      strcpy(atoms[num_of_atoms].question_marks, (char*)strtok(NULL," \n"));
      char *tokens;
      tokens = (char*)strtok(NULL," \n");
      if (tokens!=NULL) strcpy(atoms[num_of_atoms].c6, tokens);
      else strcpy(atoms[num_of_atoms].c6, "0.000");
      tokens = (char*)strtok(NULL," \n");
      if (tokens!=NULL) strcpy(atoms[num_of_atoms].c8, tokens);
      else strcpy(atoms[num_of_atoms].c8, "0.000");
      num_of_atoms++;
    }
    // read basis vectors
    else if (strcmp(first_part,"REMARK")==0)
    {
      if (strcmp(strtok(NULL," \t"),"BOX")==0)
      {
        second_part = strtok(NULL," \t");
        if (strcmp(second_part,"BASIS[0]")==0)
        {
          strtok(NULL," \t");
          x_basis = atof(strtok(NULL," \t"));
        }
        if (strcmp(second_part,"BASIS[1]")==0)
        {
          strtok(NULL," \t");
          strtok(NULL," \t");
          y_basis = atof(strtok(NULL," \t"));
        }
        if (strcmp(second_part,"BASIS[2]")==0)
        {
          strtok(NULL," \t");
          strtok(NULL," \t");
          strtok(NULL," \t");
          z_basis = atof(strtok(NULL," \t"));
        }
      }
    }
  }

  // the basis vectors need to be set for this program to work properly
  if (x_basis==100000000000.0||y_basis==100000000000.0||z_basis==100000000000.0)
  {
    printf("Warning: box basis vectors not specified in .pdb file\n");
    return 1;
  }

  int i;
  for(i=0;i<num_of_atoms;i++)
  {
    // find num of molecules
    if (atoms[i].molecule_id>num_of_molecules) num_of_molecules = atoms[i].molecule_id;
  }

  // shift all atoms left
  for(i=0;i<num_of_atoms;i++)
  {
    if(axis==0)
    {
      atoms[i].x -= x_basis/2.0;
    }
    if(axis==1)
    {
      atoms[i].y -= y_basis/2.0;
    }
    if(axis==2)
    {
      atoms[i].z -= z_basis/2.0;
    }
  }

  int old_num_of_atoms = num_of_atoms;
  // create new atoms
  for(i=0;i<old_num_of_atoms;i++)
  {
    atoms[old_num_of_atoms+i].atom_id = num_of_atoms+1;
    strcpy(atoms[old_num_of_atoms+i].site_name, atoms[i].site_name);
    strcpy(atoms[old_num_of_atoms+i].molecule_name, atoms[i].molecule_name);
    strcpy(atoms[old_num_of_atoms+i].moveable, atoms[i].moveable);
    // give new atoms a new molecule id
    atoms[old_num_of_atoms+i].molecule_id = atoms[i].molecule_id+num_of_molecules;
    // add new atoms to the right
    if (axis==0) atoms[old_num_of_atoms+i].x = atoms[i].x+x_basis;
    else atoms[old_num_of_atoms+i].x = atoms[i].x;
    if (axis==1) atoms[old_num_of_atoms+i].y = atoms[i].y+y_basis;
    else atoms[old_num_of_atoms+i].y = atoms[i].y;
    if (axis==2) atoms[old_num_of_atoms+i].z = atoms[i].z+z_basis;
    else atoms[old_num_of_atoms+i].z = atoms[i].z;
    strcpy(atoms[old_num_of_atoms+i].mass, atoms[i].mass);
    strcpy(atoms[old_num_of_atoms+i].charge, atoms[i].charge);
    strcpy(atoms[old_num_of_atoms+i].static_polarizability, atoms[i].static_polarizability);
    strcpy(atoms[old_num_of_atoms+i].lj_epsilon, atoms[i].lj_epsilon);
    strcpy(atoms[old_num_of_atoms+i].lj_sigma, atoms[i].lj_sigma);
    strcpy(atoms[old_num_of_atoms+i].dipole_freq, atoms[i].dipole_freq);
    strcpy(atoms[old_num_of_atoms+i].question_marks, atoms[i].question_marks);
    strcpy(atoms[old_num_of_atoms+i].c6, atoms[i].c6);
    strcpy(atoms[old_num_of_atoms+i].c8, atoms[i].c8);
    num_of_atoms++;
  }

  // double the basis vector we just doubled
  if(axis==0)
  {
    x_basis *= 2;
  }
  if(axis==1)
  {
    y_basis *= 2;
  }
  if(axis==2)
  {
    z_basis *= 2;
  }

  // output new pdb
  for(i=0;i<num_of_atoms;i++)
  {
    fprintf(output,"ATOM %6i %4s %2s %1s %4i %10.3f %7.3f %7.3f %8s %8s %8s %8s %8s %8s %8s %8s %8s\n",atoms[i].atom_id,atoms[i].site_name,atoms[i].molecule_name,atoms[i].moveable,atoms[i].molecule_id,atoms[i].x,atoms[i].y,atoms[i].z,atoms[i].mass,atoms[i].charge,atoms[i].static_polarizability,atoms[i].lj_epsilon,atoms[i].lj_sigma,atoms[i].dipole_freq,atoms[i].question_marks,atoms[i].c6,atoms[i].c8);
  }

  fprintf(output,"REMARK BOX BASIS[0] = %20.14f %20.14f %20.14f\n",x_basis,0.0,0.0);
  fprintf(output,"REMARK BOX BASIS[1] = %20.14f %20.14f %20.14f\n",0.0,y_basis,0.0);
  fprintf(output,"REMARK BOX BASIS[2] = %20.14f %20.14f %20.14f\n",0.0,0.0,z_basis);
  fprintf(output,"END");

  fclose(file);

  return 0;
}
