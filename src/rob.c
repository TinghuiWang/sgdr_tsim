/*
 * rob.c -- ROB related functions
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

#include "glo.h"
#include "config.h"
#include "rob.h"
#include "fu.h" 
#include "init.h" 

int ROB_DoCommit(ROB_ENTRY *entry) {
	if(entry->fSb) {
		// Perform Store
		return;
	}
	if(IsFloatInstr(entry->pInst->iOpcode)) {
		fp_reg_entry* prgfReg = entry->pARF;
		prgfReg->value = entry->fRegValue;
		pregfReg->busy = 0;
		pregfReg->ptr = NULL; 
	} else {
		int_reg_entry* prgiReg = entry->pARF;
		prgiReg->value = entry->iRegValue;
		pregfReg->busy = 0;
		pregfReg->ptr = NULL; 
	}
	// Mark the entry available for Next Cycle
	entry->available_next_cycle = 1;
	return;
}

int ROB_TryCommit(ROB_ENTRY *entry) {
	if (entry->state == WRITE_RES) {
		if(entry->entered_wr_this_cycle == 1) {
			entry->entered_wr_this_cycle = 0;
		} else {	// COmmit
			entry->state = COMMIT;
			ROB_DoCommit(entry);
		}
	}
}

/*
 * check_Unit
 * -----------------------------------------------
 *  @instr - Instruction to be checked
 *
 *  @return - 0: no rs available in the corresponding unit
 *            1: can be issued
 */
int checkUnit(inst_entry *instr) {
	int i;
	i = utGetUnitTypeForInstr(instr);
	switch(i) { 
	case UNIT_LOAD:
		return load_unit.free_stations;
	case UNIT_STORE:
		return store_unit.free_stations;
	case UNIT_INT:
		return int_unit.free_stations;
	case UNIT_FP_ADD:
		return fp_add_unit.free_stations;
	case UNIT_FP_MULT:
		return fp_mult_unit.free_stations;
	default:
		printf("Err: unknown Unit %d\n", i);
		return 0; 
	}
}

/* 
 * ROB_Issue
 * -----------------------------------------------
 *  @InstrNum - Maximum instruction ROB can issue
 *  
 *  @return - actual number of instructions issued
 */

int ROB_Issue(int InstrNum, FILE *fpAsm) {
	int i;
	inst_entry *curInst;
	ROB_ENTRY *curROBEntry;
	int fUnitUsed = 0;
	int fUnitToUse = 0;
	
	for (i = 0; i < InstrNum; i++) {
		curInst = (inst_entry*) malloc (sizeof(struct inst_entry));
		*curInst = inst_fetch(curPC, fpAsm);

		fUnitToUse = utGetUnitTypeForInstr(curInst);
		if(fUnitToUse & fUnitUsed) {
			printf("Cannot inssue two instructions to same unit\n");
			return i;
		}
		// Check ROB Availability
		curROBEntry = getROBEntry();
		if(curROBEntry == NULL) {
			#ifdef DEBUG_ROB_VERBOSE
			printf("Re-order Buffer is full. No Available Entry Found.\n");
			#endif
			free(curInst);
			return i;
		}

		// Check Reservation Avilability
		if(checkUnit(curInst) == 0) { // Structural Hazard
			#ifdef DEBUG_ROB_VERBOSE
			printf("No Reservation Station Found\n");
			#endif
			free(curInst);
			return i;
		}
		
		// Fill in ROB Entry Information
		curROBEntry->fState = ISSUED;
		curROBEntry->fSpec = fSpeculate;
		curROBEntry->iRegValue = 0;
		curROBEntry->fRegValue = 0;
		curROBEntry->entered_wr_this_cycle = 0;
		curROBEntry->available_next_cycle = 0;
		curROBEntry->pInst = curInst;
	
		if(curInst->iOpcode == OP_S_D) {
			curROBEntry->fSb = 1;
			curROBEntry->pARF = NULL;
		} else {
			curROBEntry->fSb = 0;
		}
		
		assign_load(curROBEntry);
		
		if(curInst->iOpcode != OP_S_D) {
			if(curInst->iOpcode & 0x80) {
				rgfReg[curInst->rgiOperand[0]].ptr = curROBEntry;
				curROBEntry->pARF = rgfReg[curInst->rgiOperand[0]];
			} else {
				rgfReg[curInst->rgiOperand[0]].ptr = curROBEntry;
				curROBEntry->pARF = rgfReg[curInst->rgiOperand[0]];
			}
		}
	
		// Take care of the branch
		if(curInst->iOpcode == OP_BNEZ)
			fSpeculate = 1;
	
		PC += 4;
	}
}

int update_rob() {
	int i;

	// Commit results
	for(i = 0; i < NR_ROB_ENT; i++) {
		ROB_Commit(&arROB[i]);
	}
}
