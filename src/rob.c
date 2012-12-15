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

int nrCommit;

int ROB_Init(ROB_TABLE *rob) {
	int i, j;
	
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
	printf("%p ", ent);
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
	printf(" inst: %3d %s ", ent->pInst->ploc, op(ent->pInst->iOpcode));
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
	entry->fState = COMMIT;
	// Mark the entry available for Next Cycle
	entry->available_next_cycle = 1;

	if(entry->fSb == 1 || entry->pInst->iOpcode == OP_BNEZ) {
		goto commit_success;
	}

	if(entry->pInst->iOpcode & 0x80) {
		fp_reg_entry* prgfReg = entry->pARF;
		if((unsigned long) prgfReg->ptr == (unsigned long) entry) {
			prgfReg->value = entry->fRegValue;
			prgfReg->busy = 0;
			prgfReg->ptr = NULL;
			goto commit_success; 
		}
	} else {
		int_reg_entry* prgiReg = entry->pARF;
		if((unsigned long) prgiReg->ptr == (unsigned long) entry) {
			prgiReg->value = entry->iRegValue;
			prgiReg->busy = 0;
			prgiReg->ptr = NULL; 
			goto commit_success;
		}
	}

commit_fail:
	return 0;
commit_success:
	return 1;
}

int ROB_TryCommit(ROB_ENTRY *entry) {
	if (entry->fState == WRITE_RES) {
		if(entry->entered_wr_this_cycle == 1) {
			entry->entered_wr_this_cycle = 0;
		} else {	// Commit
			if(nrCommit < NR_INSTR_ISSUE ) {
				entry->fState = COMMIT;
				nrCommit += ROB_DoCommit(entry);
				return 1;
			}
		}
	}
	return 0;
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

int ROB_Issue(int InstrNum, FILE *fp) {
	int i, j, k;
	inst_entry *curInst;
	ROB_ENTRY *curROBEntry;
	int fUnitUsed = 0;
	int fUnitToUse = 0;
	int curThreadId = 0;
	int fThreadBlock[NR_THREAD];

	for (i = 0; i < NR_THREAD; i++) {
		fThreadBlock[i] = 0;
	}

	i = 0;	
	while (i < InstrNum) {
		curInst = (inst_entry*) malloc (sizeof(struct inst_entry));
		*curInst = inst_fetch(PC[curThreadId], fp);
                printf("trying to issue %s %d, %d, %d\n", op(curInst->iOpcode), curInst->rgiOperand[0], curInst->rgiOperand[1], curInst->rgiOperand[2]);
		if(curInst->iOpcode == -1) {
			while(1)
                          printf("iOpcode == -1\n");
		}

		fUnitToUse = utGetUnitTypeForInstr(curInst);
		if(fUnitToUse == UNIT_EOP) {
                        printf("Encountered EOP for thread %d\n", curThreadId);
                        //getc(stdin);
			fEOP[curThreadId] = 1;
			goto get_ready_for_next_instr;
		}
		if(fUnitToUse & fUnitUsed) {
			printf("Structural Hazard Occur\nInstruction Issued: %d\n", i);
			num_structural_hazards++;
			free(curInst);
			fThreadBlock[curThreadId] = 1;
			goto get_ready_for_next_instr;
		}

		// Check ROB Availability
		curROBEntry = ROB_getEntry(&rob_tab[curThreadId]);
		if(curROBEntry == NULL) {
			#ifdef DEBUG_ROB_VERBOSE
			printf("Re-order Buffer is full. No Available Entry Found.\n");
			#endif
			free(curInst);
			fThreadBlock[curThreadId] = 1;
			ROB_MarkFree(&rob_tab[curThreadId], curROBEntry);
			goto get_ready_for_next_instr;
		}

		// Check Reservation Avilability
		if(checkUnit(curInst) == 0) { // Structural Hazard
			#ifdef DEBUG_ROB_VERBOSE
			printf("No Reservation Station Found\n");
			#endif
			free(curInst);
			ROB_MarkFree(&rob_tab[curThreadId], curROBEntry);
			fThreadBlock[curThreadId] = 1;
			goto get_ready_for_next_instr;
		}

issue_instr:		
		fUnitUsed |= fUnitToUse;

		// Fill in ROB Entry Information
		curROBEntry->fState = ISSUED;
		curROBEntry->fSpec = fSpeculate[curThreadId];
		curROBEntry->iRegValue = 0;
		curROBEntry->fRegValue = 0;
		curROBEntry->entered_wr_this_cycle = 0;
		curROBEntry->available_next_cycle = 0;
		curROBEntry->pInst = curInst;
		curROBEntry->pInst->rgiOperand[0] += curThreadId * I_REG_MAX;
		if(curROBEntry->pInst->iOpcode == OP_L_D || 
			curROBEntry->pInst->iOpcode == OP_S_D) {
			curROBEntry->pInst->rgiOperand[2] += curThreadId * I_REG_MAX;
		} else if(curROBEntry->pInst->iOpcode == OP_SUBI || 
			curROBEntry->pInst->iOpcode == OP_ADDI || 
			curROBEntry->pInst->iOpcode == OP_SLTI ) {
			curROBEntry->pInst->rgiOperand[1] += curThreadId * I_REG_MAX;
		} else if(curROBEntry->pInst->iOpcode == OP_BNEZ) {
		} else {
			curROBEntry->pInst->rgiOperand[1] += curThreadId * I_REG_MAX;
			curROBEntry->pInst->rgiOperand[2] += curThreadId * I_REG_MAX;
		} 

		if(curInst->iOpcode == OP_S_D) {
			curROBEntry->fSb = 1;
			curROBEntry->pARF = (void*) &rgfReg[curInst->rgiOperand[0]];
		} else {
			curROBEntry->fSb = 0;
		}
		
		assign_to_rs(curROBEntry, curThreadId);
		
		if(curInst->iOpcode != OP_S_D && curInst->iOpcode != OP_BNEZ) { // added bnez -gs
			if(curInst->iOpcode & 0x80) {
				rgfReg[curInst->rgiOperand[0]].busy = 1;
				rgfReg[curInst->rgiOperand[0]].ptr = curROBEntry;
				curROBEntry->pARF = (void*) &rgfReg[curInst->rgiOperand[0]];
			} else {
				rgiReg[curInst->rgiOperand[0]].busy = 1;
				rgiReg[curInst->rgiOperand[0]].ptr = curROBEntry;
				curROBEntry->pARF = (void*) &rgiReg[curInst->rgiOperand[0]];
			}
		}
		
		if(curInst->iOpcode != OP_BNEZ) {
			PC[curThreadId] += 4;
		}
		i++;
	
get_ready_for_next_instr:
		// Check for Next Thread:
		j = 0;
	
		for(k = 0; k < NR_THREAD; k++) {
			j += fThreadBlock[k] | fEOP[k];
		}
		if(j == NR_THREAD)
			return i;
		do {
			curThreadId ++;
			if(curThreadId == NR_THREAD) {
				curThreadId = 0;
			}
		} while(fThreadBlock[curThreadId] == 1 || fEOP[curThreadId] == 1);
	}
}

void ROB_ClearEntry(ROB_ENTRY* ent) {
	ent->fState = 0;
	ent->fSpec = 0;
	ent->fSb = 0;
	ent->fDesValid = 0;
	ent->iRegValue = 0;
	ent->fRegValue = 0;
	ent->pARF = NULL;
	ent->fFloatInt = 0;
	ent->entered_wr_this_cycle = 0;
	ent->available_next_cycle = 0;
	ent->pInst = NULL;
}

int update_rob(FILE* fp) {
	int i,j;
	int enable_commit = 1;
	ROB_ENTRY* ent;

	nrCommit = 0;
	// Commit results
	for(j = 0; j < NR_THREAD; j++) {
		for(i = 0; i < NR_ROB_ENT; i++) {
			if(rob_tab[j].arROB[i].available_next_cycle == 1) {
				ROB_ClearEntry(&rob_tab[j].arROB[i]);
				ROB_MarkFree(&rob_tab[j], &rob_tab[j].arROB[i]);
			}
		}
		ent = rob_tab[j].busy;
		while(ent != NULL) {
			enable_commit = ROB_TryCommit(ent);
			if(enable_commit == 0)
				break;
			ent = ent->next;
		}
		for(i=0; i< NR_ROB_ENT; i++) {
			if(rob_tab[j].arROB[i].fSb == 1 && rob_tab[j].arROB[i].fState == COMMIT) {
				// Perform Store
				printf("Mem Write: Addr %d, value %d\n", rob_tab[j].arROB[i].pInst->rgiOperand[0], (int) rob_tab[j].arROB[i].fRegValue);
				//getchar();
				mem_store(rob_tab[j].arROB[i].pInst->rgiOperand[0],(int) rob_tab[j].arROB[i].fRegValue,fpOutResult);
				rob_tab[j].arROB[i].available_next_cycle = 1;
			}
		}
	}
}
