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

#define IS_FLAG(flag)  \
	strcmp(argv[i], flag)

int PC = 0;
char branch = 0;

void
printUsage() {
	printf("sgdr_tsim -S <.S file> -m <.m file> -r <.r file> [-xgs]\n");
	printf("sgdr_tsim -h\n\n");
}

void simulate(FILE *fpInAsm)
{
  int i;
  int cycles = 1;
  inst_entry stalled_instruction;
  inst_entry instruction;
  char stalled = 0;

  for(i = 0; i < 6; i++) // TODO: needs to be changed to while(!end of program) loop
  {
    printf("\n\n**************************** CYCLE=%d | PC=%d ****************************\n\n", cycles, PC);
    if(stalled)
    {
      //if so, try and issue the stalled instruction again
      instruction = stalled_instruction;
      stalled = 0;
    }
    else
    {
      //else not stalled
      instruction = inst_fetch(PC, fpInAsm);
    }

    if(instruction.iOpcode == OP_NOP || branch == 1) // TODO: 100% branch pred -> nop til WR of branch?
    {
      printf("**simulate(): performing a NOP\n");
      goto update;
    }
    
    printf("**simulate(): IF: %s %d %d %d\n", op(instruction.iOpcode), instruction.rgiOperand[0], instruction.rgiOperand[1], instruction.rgiOperand[2]);
    if(assign_to_rs(instruction) == -1)
    {
      printf("**simulate(): instruction stalled\n");
      stalled_instruction = instruction;
      stalled = 1;
    }
    
    if(instruction.iOpcode == OP_BNEZ)
    {
      printf("**simulate(): branch instruction, will NOP next cycle\n");
      branch = 1; // needed for 1 nop after branch instruction
    }
    
update:
    update_rs(); // update reservation stations, calls write_result() if execution is complete
    update_rob(); // checks to see if the head of the rob is ready to commit
    clear_flags(); // resets 'parallel' flags to 0
    print_reg_status();
    print_rob_status();
    print_rs_status();
    cycles++; // move on to next cycle
    PC+=4; // only other place this can be changed is when a branch enters write_result phase
  }
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
  FILE *fpInAsm = NULL;
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

  fpInRegFP = fopen("resources/fRegisters.txt", "r");
  fpInRegInt = fopen("resources/iRegisters.txt", "r");
  init_registers(fpInRegFP, fpInRegInt);

  init_fu(); // zeros out data struct
  init_rob(); // zeros out data struct
  simulate(fpInAsm);
}
