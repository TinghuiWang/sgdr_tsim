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

#include "config.h"
#include "rob.h"
#include "fu.h" 
#include "init.h" 
#include "glo.h"

#define DEBUG_ROB_VERBOSE

int ROB_Init(ROB_TABLE *rob) {
	int i;

	rob->iAvail = NR_ROB_ENT;
	rob->busy = NULL;
	rob->free = rob->arROB;
	
	for(i = 0; i < NR_ROB_ENT - 1; i++) {
		rob->arROB[i].iIndex = i;
		rob->arROB[i].fState = FREE;
		rob->arROB[i].next = &(rob->arROB[i+1]);
	}
	
	rob->arROB[NR_ROB_ENT - 1].iIndex = NR_ROB_ENT - 1;
	rob->arROB[NR_ROB_ENT - 1].fState = FREE;
	rob->arROB[NR_ROB_ENT - 1].next = NULL;
}

/*
 * ROB_dequeue
 * -----------------------------------------------------
 *  list: pointer to the list header
 *  ent: pointer to the ent to be removed, 
 *       NULL to remove the first entry in the list
 *  return: Return the pointer to the ROB_ENTRY removed
 */
ROB_ENTRY *ROB_dequeue(ROB_ENTRY **list, ROB_ENTRY *ent) {
	ROB_ENTRY* tmp;

	if(*list == NULL) {
		printf("[ROB_DEQUEUE] err: List is empty\n");
		return NULL;
	}

	if(ent == NULL) { // Dequeue the first entry from the list
		tmp = *list;
		*list = (*list)->next;
		tmp->next = NULL;
		return tmp;
	} else {
		if(*list == ent) {
			*list = ent->next;
			ent->next = NULL;
			return ent;
		} else {
			tmp = *list;
			while(tmp->next != ent) {
				tmp = tmp->next;
				if(tmp == NULL) {
					printf("[ROB_DEQUEUE] err: ent not found in the list\n");
					return NULL;
				}
			}
			tmp->next = ent->next;
			ent->next = NULL;
			return ent;
		}
	}
	return NULL;
}

int ROB_enqueue(ROB_ENTRY **list, ROB_ENTRY *ent, int fhead) {
	if(fhead) {
		ent->next = *list;
		*list = ent;
	} else {
		ROB_ENTRY** tmp = list;
		while(*tmp != NULL) {
			tmp = &((*tmp)->next);
		}
		ent->next = NULL;
		*tmp = ent;
	}
	return 0;
}

int ROB_MarkFree(ROB_TABLE *rob, ROB_ENTRY *ent) {
	ROB_ENTRY *tmp = ROB_dequeue(&(rob->busy), ent);

	if(tmp == NULL) {
		return -1;
	}

	ROB_enqueue(&(rob->free), tmp, 0); 
	rob->iAvail ++;
	return 0;
}

ROB_ENTRY *ROB_getEntry(ROB_TABLE *rob) {
	ROB_ENTRY *tmp = ROB_dequeue(&(rob->free), NULL);
	
	if(tmp == NULL) {
		return NULL;
	}

	rob->iAvail--;
	ROB_enqueue(&(rob->busy), tmp, 0);
	return tmp;
}

int ROB_printEntry(ROB_ENTRY *ent) {
	printf("%2d ", ent->iIndex);
	printf("%s ", getfState(ent->fState));
	if(ent->fState == FREE) {
		printf("\n");
		return 0;
	}
	printf("Spec:%d ", ent->fSpec);
	printf("Store:%d ", ent->fSb);
	if(ent->pInst->iOpcode & 0x80) {
		printf("Value: %8f ", ent->fRegValue);
		if(ent->pARF != NULL) 
			printf("Dest: F%d ", ((fp_reg_entry *) (ent->pARF))->index);
	} else {
		printf("Value: %8d ", ent->iRegValue);
		if(ent->pARF != NULL) 
			printf("Dest: R%d ", ((int_reg_entry *) (ent->pARF))->index);
	}
	printf("\n");
	return 0;
}

int ROB_printList(ROB_ENTRY *list) {
	ROB_ENTRY *tmp = list;

	while(tmp != NULL) {
		printf("ROB&     ");
		ROB_printEntry(tmp);
		tmp = tmp->next;
	}
	
	return 0;
}

int ROB_print(ROB_TABLE *rob) {	
	printf("\nRe-Order Buffer 0x%x\n", rob);
	printf("ROB& Available Entry (for Next Cycle): %d\n", rob->iAvail);
	printf("ROB& Busy List:\n");
	ROB_printList(rob->busy);
	//printf("ROB& Free List:\n");
	//ROB_printList(rob->free);
	printf("\n");
	return 0;
}

int ROB_DoCommit(ROB_ENTRY *entry) {
	if(entry->fSb) {
		// Perform Store
		return;
	}
	if(entry->pInst->iOpcode & 0x80) {
		fp_reg_entry* prgfReg = entry->pARF;
		prgfReg->value = entry->fRegValue;
		prgfReg->busy = 0;
		prgfReg->ptr = NULL; 
	} else {
		int_reg_entry* prgiReg = entry->pARF;
		prgiReg->value = entry->iRegValue;
		prgiReg->busy = 0;
		prgiReg->ptr = NULL; 
	}

	entry->fState = COMMIT;
	// Mark the entry available for Next Cycle
	entry->available_next_cycle = 1;
	return;
}

int ROB_TryCommit(ROB_ENTRY *entry) {
	if (entry->fState == WRITE_RES) {
		if(entry->entered_wr_this_cycle == 1) {
			entry->entered_wr_this_cycle = 0;
		} else {	// COmmit
			entry->fState = COMMIT;
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
		*curInst = inst_fetch(PC, fpAsm);

		fUnitToUse = utGetUnitTypeForInstr(curInst);
		if(fUnitToUse & fUnitUsed) {
			printf("Structural Hazard Occur\nInstruction Issued: %d\n", i);
			return i;
		}

		// Check ROB Availability
		curROBEntry = ROB_getEntry(rob_tab);
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
		
		fUnitUsed |= fUnitToUse;

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
		
		assign_to_rs(curROBEntry);
		
		if(curInst->iOpcode != OP_S_D) {
			if(curInst->iOpcode & 0x80) {
				rgfReg[curInst->rgiOperand[0]].ptr = curROBEntry;
				curROBEntry->pARF = (void*) &rgfReg[curInst->rgiOperand[0]];
			} else {
				rgfReg[curInst->rgiOperand[0]].ptr = curROBEntry;
				curROBEntry->pARF = (void*) &rgfReg[curInst->rgiOperand[0]];
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
		ROB_TryCommit(&rob_tab->arROB[i]);
	}
}
