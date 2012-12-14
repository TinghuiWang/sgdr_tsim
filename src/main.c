/*
 * main.c -- Main for sgdr_tsim
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.

 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "config.h"
#include "init.h"
#include "fu.h"
#include "rob.h"

#define IS_FLAG(flag)  \
	strcmp(argv[i], flag)

int write_result_counter = 0;

/* *********************************** */
/*           Global Variables          */
/* *********************************** */

int PC[NR_THREAD];
int fEOP[NR_THREAD];
// Architecture Register File
int_reg_entry rgiReg[I_REG_MAX * NR_THREAD];
fp_reg_entry rgfReg[FP_REG_MAX * NR_THREAD];

// Function Units
LOAD_UNIT load_unit;
STORE_UNIT store_unit;
INT_UNIT int_unit;
FP_ADD_UNIT fp_add_unit;
FP_MULT_UNIT fp_mult_unit;

// Re-Order Buffer
ROB_TABLE rob_tab[NR_THREAD];

// Global Flag
int fSpeculate[NR_THREAD];

// Input file
FILE *fpInAsm = NULL;
FILE *fpOutResult = NULL;

/* ************************************ */
/*         Functions & Procedures       */
/* ************************************ */

void
printUsage() {
	printf("sgdr_tsim -S <.S file> -m <.m file> -r <.r file> [-xgs]\n");
	printf("sgdr_tsim -h\n\n");
}

void simulate(FILE *fp)
{
  int i, j;
  int instrIssued = 0;
  char tmp = 'c';
  int cycles = 1;
  inst_entry stalled_instruction;
  inst_entry instruction;
  char stalled = 0;

  PC[0] = PC0_INIT_VAL;
  PC[1] = PC1_INIT_VAL;

  //for(i = 0; i < 200; i++) // TODO: needs to be changed to while(!end of program) loop
  while(fEOP[0] == 0 ||  fEOP[1] == 0 || unfinished_rs())
  {
    printf("\n\n**************************** CYCLE=%d | PC={%d,%d} ****************************\n\n", cycles, PC[0], PC[1]);
	
	// Update Reservation Station
    update_rs(); 
	// Update ReOrder Buffer
    update_rob(fp);
	instrIssued += ROB_Issue(NR_INSTR_ISSUE, fp);
	// Clear Temporary Flgas
    clear_flags(); 
	// Print Debug Message
	if(cycles) {
	    print_reg_status();
		for(j = 0; j < NR_THREAD; j++) {
			printf("\nROB of THREAD: %d >>>>>>>>>>>>>>>>>>>>>>\n", j);
			ROB_print(&rob_tab[j]);
		}
	    print_rs_status();
		getc(stdin);
	}

	// Move on to Next Cycle
    cycles++;
	fflush(stdout);
	fflush(fp);
  }
  // print stats now
  printf("CPI: %f\n", instrIssued/cycles);
}

int main(int argc, char** argv) 
{	
	int i;
	int fGui = 0;
	int fDebug = 0;
	int fStep = 0;
	int status = 0;
	char *pchInAsmFile = NULL;
	char *pchInitMemFile = NULL;
	char *pchInitRegFile = NULL;
	FILE *fpInRegFP = NULL;
	FILE *fpInRegInt = NULL;
	FILE *fpInMem = NULL;
	
	if(argc == 1) {
	    printUsage();
	} 
	/* Parse options
	  * -x: start GUI
	  * -g:   Debugging Mode
	  * -s:   Step Mode
	  */
	for (i = 1; i < argc; i++) {
	  if(argv[i][0] == '-') {
	    switch(argv[i][1]) {
	  case 'x':
	    fGui = 1;
	    break;
	  case 'g':
	    fDebug = 1;
	    break;
	  case 's':
	    fStep = 1;
	    break;
	  case 'r':
	    i++;
	    pchInitRegFile = argv[i];
	    break;
	  case 'm':
	    i++;
	    pchInitMemFile = argv[i];
	    break;
	  case 'S':
	    i++;
	    pchInAsmFile = argv[i];
	    break;
	  case 'h':
	    printUsage();
	    return 0;
	  default:
	    printf("error: Unknown Option: %s\n", argv[i]);
	    return -1;
	    }
	  } else {
	  printf("error: Unknown Option: %s\n", argv[i]);
	  return -1;
	  }
	}
	
	// Symbol Table should be created ahead of fetch instructions
	// Syntax Checking on the fly
	// PARSER_CreateSymbolTable();
	fpInAsm = fopen("resources/Instructions.asm", "r");
	get_memory_locations(fpInAsm);
	fpOutResult = init_outfile(fpInAsm);
	
	fpInRegFP = fopen("resources/fRegisters.txt", "r");
	fpInRegInt = fopen("resources/iRegisters.txt", "r");
	
	init_registers(fpInRegFP, fpInRegInt);
	
	  for (i = 0; i < NR_THREAD * I_REG_MAX; i++) {
	  	rgiReg[i].index = i;
	  }
	  for (i = 0; i < NR_THREAD * FP_REG_MAX; i++) {
	  	rgfReg[i].index = i;
	  }
	
	  for (i = 0; i < NR_THREAD; i++) {
	  	PC[i] = 0;
	  	fSpeculate[i] = 0;
		fEOP[i] = 0;
	  }
	
	init_fu(); // zeros out data struct
	for(i = 0; i < NR_THREAD; i++) 
	    ROB_Init(&rob_tab[i]); // zeros out data struct
	
	for(i = 0; i < NR_THREAD; i++) {
		printf("\nROB of THREAD: %d >>>>>>>>>>>>>>>>>>>>>>\n", i);
		ROB_print(&rob_tab[i]);
	}
	print_rs_status();
	simulate(fpOutResult);
}
