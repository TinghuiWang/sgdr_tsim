#ifndef FU_P_H
#define FU_P_H

#include "fu.h"

void print_reg_status();
void print_mem_status();
void print_rs_status();
void enqueue(RES_STATION **queue, RES_STATION *q);
RES_STATION* dequeue(RES_STATION **queue);
int remove_queue(RES_STATION **queue, RES_STATION *q);
void clear_flags();
int unfinished_rs();
int assign_load(ROB_ENTRY * robe);
int assign_store(ROB_ENTRY * robe);
int assign_int(ROB_ENTRY * robe, int thread);
int assign_fp_add(ROB_ENTRY * robe);
int assign_fp_mult(ROB_ENTRY *robe);
void write_result(RES_STATION * rs);
int update_load();
int update_store();
int update_int();
int update_fp_add();
int update_fp_mult();

#endif // FU_P_H
