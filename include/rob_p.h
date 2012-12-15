#ifndef ROB_P_H
#define ROB_P_H

#include "rob.h"

int ROB_Init(ROB_TABLE *rob);
ROB_ENTRY *ROB_dequeue(ROB_ENTRY **list, ROB_ENTRY *ent);

int ROB_enqueue(ROB_ENTRY **list, ROB_ENTRY *ent, int fhead);
int ROB_MarkFree(ROB_TABLE *rob, ROB_ENTRY *ent);
ROB_ENTRY *ROB_getEntry(ROB_TABLE *rob);
int ROB_printEntry(ROB_ENTRY *ent);
int ROB_printList(ROB_ENTRY *list);
int ROB_print(ROB_TABLE *rob);
int ROB_DoCommit(ROB_ENTRY *entry);
int ROB_TryCommit(ROB_ENTRY *entry);
int checkUnit(inst_entry *instr);
int ROB_Issue(int InstrNum, FILE *fp);
void ROB_ClearEntry(ROB_ENTRY* ent);
int update_rob(FILE* fp);

#endif // ROB_P_H
