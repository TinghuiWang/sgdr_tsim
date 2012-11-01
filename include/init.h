#ifndef INIT_H
#define INIT_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "config.h"

typedef struct
{
	int opcode;
	int operand[3];
} inst_entry;

typedef struct
{
	char label[25];
	int address;
} sym_entry;

extern sym_entry *symbol_table;
extern long int *memory_location;


void get_memory_locations(FILE *infile);
inst_entry inst_fetch(int address, FILE *mem_file);

#endif