/*
 * fu.c -- Functional Units and primitive ROB for sgdr_tsim
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

#include "fu.h"



// ROB_ENTRY robs[NUM_ROB_ENTRIES];
int rob_head = 0;
int rob_tail = 0;
int avail_rob_entries = NUM_ROB_ENTRIES;

/********************************************************************************************
*
* INIT AND UTILITY FUNCTIONS
*
*********************************************************************************************/
//***********************state()***********************
char * state(int state)
{
	switch(state) {
	case ST_ISSUED: 
		return "ISSUED";
	case ST_EXECUTE: 
		return "EXECUTE";
	case ST_WRITE_BACK: 
		return "WRITE BACK";
	case ST_COMMIT: 
		return "COMMIT";
	case ST_WAITING: 
		return "WAITING";
    }
}

//***********************op()***********************
// To-Do: Change Case Params to Global Definition
char * op(int op)
{
    switch(op)
    {
      case(0): return "LOAD.D";
      case(1): return "MULT.D";
      case(2): return "ADD.D";
      case(3): return "STORE.D";
      case(4): return "SUB.D";
      case(5): return "ADDI";
      case(6): return "SUBI";
      case(7): return "BNEZ";
      case(8): return "NOP";
      case(9): return "DIV.D";
    }
}

//***********************print_reg_status()***********************
void print_reg_status()
{
 int i;
 printf("\n---------------INTEGER--------------\n");
 for(i = 0; i < I_REG_MAX; i++)
   printf("R%d: busy=%d value=%d ptr=%p\n", i, rgiReg[i].busy, rgiReg[i].value, rgiReg[i].ptr);
 printf("\n---------------FP--------------\n");
 for(i = 0; i < FP_REG_MAX; i++)
   printf("F%d: busy=%d value=%d ptr=%p\n", i, rgfReg[i].busy, rgfReg[i].value, rgfReg[i].ptr);
}

//***********************print_mem_status()***********************
void print_mem_status()
{
 printf("\n---------------Memory--------------\n");

}

//***********************print_rob_status()***********************
void print_rob_status()
{
  int head = rob_head;
  int tail = rob_tail;
  
  printf("\n---------------ROB--------------\n");
  while(head != tail)
  {
    printf("ROB#%d: addr=%p op=%s dest=%d src1=%d src2=%d busy=%d state=%s value=%f done=%d\n", head, &robs[head], op(robs[head].iOpcode), robs[head].dest, robs[head].src1, 
	   robs[head].src2, robs[head].busy, state(robs[head].state), robs[head].value, robs[head].done);
    head++;
    if(head == NUM_ROB_ENTRIES)
      head = 0;
  }
}

//***********************print_rs_status()***********************
void print_rs_status()
{
  RES_STATION * rs;
  printf("\n---------------RES STATIONS--------------\n");
  rs = load_unit.active;
  while(rs)
  {
    printf("LOAD#%d: busy=%d cycles_remaining=%d op=%s vj=%d vk=%d qj=%p qk=%p dest=%p waiting_for_operands=%d\n", rs->id, rs->busy, 
	   rs->cycles_remaining, op(rs->iOpcode), (int) rs->reg_vj, (int) rs->reg_vk, 
	   rs->reg_qj, rs->reg_qk, rs->dest, rs->waiting_for_operands);
    rs = rs->next;
  }  
  
  rs = store_unit.active;
  while(rs)
  {
    printf("STORE#%d: busy=%d cycles_remaining=%d op=%s vj=%d vk=%d qj=%p qk=%p dest=%p waiting_for_operands=%d\n", rs->id, rs->busy, 
	   rs->cycles_remaining, op(rs->iOpcode), (int) rs->reg_vj, (int) rs->reg_vk, 
	   rs->reg_qj, rs->reg_qk, rs->dest, rs->waiting_for_operands);
    rs = rs->next;
  }
  
  rs = int_unit.active;
  while(rs)
  {
    printf("INT#%d: busy=%d cycles_remaining=%d op=%s vj=%d vk=%d qj=%p qk=%p dest=%p waiting_for_operands=%d\n", rs->id, rs->busy, 
	   rs->cycles_remaining, op(rs->iOpcode), (int) rs->reg_vj, (int) rs->reg_vk, 
	   rs->reg_qj, rs->reg_qk, rs->dest, rs->waiting_for_operands);
    rs = rs->next;
  }
  
  rs = fp_add_unit.active;
  while(rs)
  {
    printf("FP ADD/SUB#%d: busy=%d cycles_remaining=%d op=%s vj=%d vk=%d qj=%p qk=%p dest=%p waiting_for_operands=%d\n", rs->id, rs->busy, 
	   rs->cycles_remaining, op(rs->iOpcode), (int) rs->reg_vj, (int) rs->reg_vk, 
	   rs->reg_qj, rs->reg_qk, rs->dest, rs->waiting_for_operands);
    rs = rs->next;
  }
  
  rs = fp_mult_unit.active;
  while(rs)
  {
    printf("FP MULT/DIV#%d: busy=%d cycles_remaining=%d op=%s vj=%d vk=%d qj=%p qk=%p dest=%p waiting_for_operands=%d\n", rs->id, rs->busy, 
	   rs->cycles_remaining, op(rs->iOpcode), (int) rs->reg_vj, (int) rs->reg_vk, 
	   rs->reg_qj, rs->reg_qk, rs->dest, rs->waiting_for_operands);
    rs = rs->next;
  }
  /* this code is to print ALL reservation stations, both inactive (not busy) and active (busy)
  int i; 
  printf("\n---------------RES STATIONS--------------\n");
  for(i = 0; i < LOAD_RS; i++)
  {
    printf("Load#%d: busy=%d cycles_remaining=%d op=%s vj=%d vk=%d qj=%p qk=%p dest=%p waiting_for_operands=%d\n", i, load_unit.rs[i].busy, 
	   load_unit.rs[i].cycles_remaining, op(load_unit.rs[i].iOpcode), (int) load_unit.rs[i].reg_vj, (int) load_unit.rs[i].reg_vk, 
	   load_unit.rs[i].reg_qj, load_unit.rs[i].reg_qk, load_unit.rs[i].dest, load_unit.rs[i].waiting_for_operands);
  }
  for(i = 0; i < STORE_RS; i++)
  {
    printf("Store#%d: busy=%d cycles_remaining=%d op=%s vj=%d vk=%f qj=%p qk=%p dest=%p waiting_for_operands=%d\n", i, store_unit.rs[i].busy, 
	   store_unit.rs[i].cycles_remaining, op(store_unit.rs[i].iOpcode), (int) store_unit.rs[i].reg_vj, store_unit.rs[i].reg_vk, 
	   store_unit.rs[i].reg_qj, store_unit.rs[i].reg_qk, store_unit.rs[i].dest, store_unit.rs[i].waiting_for_operands);
  }
  for(i = 0; i < INTEGER_RS; i++)
  {
    printf("Integer#%d: busy=%d cycles_remaining=%d op=%s vj=%d vk=%d qj=%p qk=%p dest=%p waiting_for_operands=%d\n", i, int_unit.rs[i].busy, 
	   int_unit.rs[i].cycles_remaining, op(int_unit.rs[i].iOpcode), (int) int_unit.rs[i].reg_vj, (int) int_unit.rs[i].reg_vk, 
	   int_unit.rs[i].reg_qj, int_unit.rs[i].reg_qk, int_unit.rs[i].dest, int_unit.rs[i].waiting_for_operands);
  }
  for(i = 0; i < FP_ADD_RS; i++)
  {
    printf("FP Add/Sub#%d: busy=%d cycles_remaining=%d op=%s vj=%f vk=%f qj=%p qk=%p dest=%p waiting_for_operands=%d\n", i, fp_add_unit.rs[i].busy, 
	   fp_add_unit.rs[i].cycles_remaining, op(fp_add_unit.rs[i].iOpcode), fp_add_unit.rs[i].reg_vj, fp_add_unit.rs[i].reg_vk, 
	   fp_add_unit.rs[i].reg_qj, fp_add_unit.rs[i].reg_qk, fp_add_unit.rs[i].dest, fp_add_unit.rs[i].waiting_for_operands);
  }
  for(i = 0; i < FP_MULT_RS; i++)
  {
    printf("FP Mult/Div#%d: busy=%d cycles_remaining=%d op=%s vj=%f vk=%f qj=%p qk=%p dest=%p waiting_for_operands=%d\n", i, fp_mult_unit.rs[i].busy, 
	   fp_mult_unit.rs[i].cycles_remaining, op(fp_mult_unit.rs[i].iOpcode), fp_mult_unit.rs[i].reg_vj, fp_mult_unit.rs[i].reg_vk, 
	   fp_mult_unit.rs[i].reg_qj, fp_mult_unit.rs[i].reg_qk, fp_mult_unit.rs[i].dest, fp_mult_unit.rs[i].waiting_for_operands);
  }*/
}

//*********************enqueue()*********************
void enqueue(RES_STATION **queue, RES_STATION *q)
{
  RES_STATION *p = *queue;              
  if(!p) 
  {
    q->next = NULL;
    *queue = q;
  }
  else
  {          
    while(p->next) 
      p = p->next;
    q->next = NULL;
    p->next = q;
  }
}

//*********************dequeue()*********************
RES_STATION* dequeue(RES_STATION **queue)
{
  RES_STATION* p = *queue;
         
  if(p)           
    *queue = (*queue)->next;
  return p;
}

//*********************remove_queue()*********************
int remove_queue(RES_STATION **queue, RES_STATION *q)
{
  RES_STATION *p = *queue;
  RES_STATION *prev = NULL;
  //printf("**remove_queue(): trying to remove id=%d\n", q->id);
  if(!p) 
  {
    //printf("**remove_queue(): ERROR: queue empty\n");
    return -1;
  }
  else
  {          
    while(p) 
    {
      if(p->id == q->id)
      {
	//printf("**remove_queue(): found it!\n");
	break;
      }
      prev = p;
      p = p->next;
    }
    if(p == NULL) // was not in list
    {
      //printf("**remove_queue(): ERROR: was not in queue\n");
      return -1;
    }
    if(prev == NULL) // removed first item
    {
      //printf("**remove_queue(): removed first item\n");
      *queue = p->next;
    }
    else // removed at least second item
    {
     // printf("**remove_queue(): removed it, was not first item\n");
      prev->next = p->next;
    }
    return 1;
  }
}

//***********************init_fu()***********************
int init_fu()
{
  int i;
      
  memset(&load_unit, 0, sizeof(LOAD_UNIT));
  memset(&store_unit, 0, sizeof(STORE_UNIT));
  memset(&int_unit, 0, sizeof(INT_UNIT));
  memset(&fp_add_unit, 0, sizeof(FP_ADD_UNIT));
  memset(&fp_mult_unit, 0, sizeof(FP_MULT_UNIT));
  
  for(i = 0; i < LOAD_RS; i++)
  {
    load_unit.rs[i].id = i;
    load_unit.rs[i].next = &(load_unit.rs[i+1]);
  }
  load_unit.rs[LOAD_RS-1].next = NULL;
  load_unit.free = &load_unit.rs[0];
  
  for(i = 0; i < STORE_RS; i++)
  {
    store_unit.rs[i].id = i;
    store_unit.rs[i].next = &(store_unit.rs[i+1]);
  }
  store_unit.rs[STORE_RS-1].next = NULL;
  store_unit.free = &(store_unit.rs[0]);
  
  for(i = 0; i < INTEGER_RS; i++)
  {
    int_unit.rs[i].id = i;
    int_unit.rs[i].next = &(int_unit.rs[i+1]);
  }
  int_unit.rs[INTEGER_RS-1].next = NULL;
  int_unit.free = &(int_unit.rs[0]);
  
  for(i = 0; i < FP_ADD_RS; i++)
  {
    fp_add_unit.rs[i].id = i;
    fp_add_unit.rs[i].next = &(fp_add_unit.rs[i+1]);
  }
  fp_add_unit.rs[FP_ADD_RS-1].next = NULL;
  fp_add_unit.free = &(fp_add_unit.rs[0]);
  
  for(i = 0; i < FP_MULT_RS; i++)
  {
    fp_mult_unit.rs[i].id = i;
    fp_mult_unit.rs[i].next = &(fp_mult_unit.rs[i+1]);
  }
  fp_mult_unit.rs[FP_MULT_RS-1].next = NULL;
  fp_mult_unit.free = &(fp_mult_unit.rs[0]);
}

//***********************init_rob()***********************
void init_rob()
{
  int i;
  for(i = 0; i < NUM_ROB_ENTRIES; i++)
  {
      memset(&robs[i], 0, sizeof(ROB_ENTRY));
  }
}

//***********************clear_flags()***********************
void clear_flags()
{
  int i;
  for(i = 0; i < LOAD_RS; i++)
  {
    load_unit.rs[i].issued_this_cycle = 0;
    load_unit.rs[i].received_val_this_cycle = 0;
    if(load_unit.rs[i].dest != NULL)
      load_unit.rs[i].dest->entered_wr_this_cycle = 0;
   // printf("**clear_flags(): id=%d busy=%d\n", i, load_unit.rs[i].busy);
    if(load_unit.rs[i].busy == 0) // no longer active
    {
      if(remove_queue(&(load_unit.active), &(load_unit.rs[i])) == 1) // was in active list
      {
	memset(&(load_unit.rs[i]), 0, sizeof(RES_STATION));
	enqueue(&(load_unit.free), &(load_unit.rs[i]));
	load_unit.free_stations++;
      }
    }
  }
  for(i = 0; i < STORE_RS; i++)
  {
    store_unit.rs[i].issued_this_cycle = 0;
    store_unit.rs[i].received_val_this_cycle = 0;
    if(store_unit.rs[i].dest != NULL)
      store_unit.rs[i].dest->entered_wr_this_cycle = 0;
    if(store_unit.rs[i].busy == 0) // no longer active
    {
      if(remove_queue(&(store_unit.active), &(store_unit.rs[i])) == 1) // was in active list
      {
	memset(&(store_unit.rs[i]), 0, sizeof(RES_STATION));
	enqueue(&(store_unit.free), &(store_unit.rs[i]));
	store_unit.free_stations++;
      }
    }
  }
  for(i = 0; i < INTEGER_RS; i++)
  {
    int_unit.rs[i].issued_this_cycle = 0;
    int_unit.rs[i].received_val_this_cycle = 0;
    if(int_unit.rs[i].dest != NULL)
      int_unit.rs[i].dest->entered_wr_this_cycle = 0;
    if(int_unit.rs[i].busy == 0) // no longer active
    {
      if(remove_queue(&(int_unit.active), &(int_unit.rs[i])) == 1) // was in active list
      {
	memset(&(int_unit.rs[i]), 0, sizeof(RES_STATION));
	enqueue(&(int_unit.free), &(int_unit.rs[i]));
	int_unit.free_stations++;
      }
    }
  }
  for(i = 0; i < FP_ADD_RS; i++)
  {
    fp_add_unit.rs[i].issued_this_cycle = 0;
    fp_add_unit.rs[i].received_val_this_cycle = 0;
    if(fp_add_unit.rs[i].dest != NULL)
      fp_add_unit.rs[i].dest->entered_wr_this_cycle = 0;
    if(fp_add_unit.rs[i].busy == 0) // no longer active
    {
      if(remove_queue(&(fp_add_unit.active), &(fp_add_unit.rs[i])) == 1) // was in active list
      {
	memset(&(fp_add_unit.rs[i]), 0, sizeof(RES_STATION));
	enqueue(&(fp_add_unit.free), &(fp_add_unit.rs[i]));
	fp_add_unit.free_stations++;
      }
    }
  }
  for(i = 0; i < FP_MULT_RS; i++)
  {
    fp_mult_unit.rs[i].issued_this_cycle = 0;
    fp_mult_unit.rs[i].received_val_this_cycle = 0;
    if(fp_mult_unit.rs[i].dest != NULL)
      fp_mult_unit.rs[i].dest->entered_wr_this_cycle = 0;
    if(fp_mult_unit.rs[i].busy == 0) // no longer active
    {
      if(remove_queue(&(fp_mult_unit.active), &(fp_mult_unit.rs[i])) == 1) // was in active list
      {
	memset(&(fp_mult_unit.rs[i]), 0, sizeof(RES_STATION));
	enqueue(&(fp_mult_unit.free), &(fp_mult_unit.rs[i]));
	fp_mult_unit.free_stations++;
      }
    }
  }
  load_unit.started_one_this_cycle = 0;
  store_unit.started_one_this_cycle = 0;
  int_unit.started_one_this_cycle = 0;
  fp_add_unit.started_one_this_cycle = 0;
  fp_mult_unit.started_one_this_cycle = 0;
}

/********************************************************************************************
*
* ROB AND RESERVATION STATION ASSIGNMENT CODE
*
*********************************************************************************************/
//***********************assign_rob()***********************
ROB_ENTRY * assign_rob(inst_entry instruction)
{
  ROB_ENTRY * robe;
  
  robe = &robs[rob_tail++];
  rob_tail %= NUM_ROB_ENTRIES;
  
  robe->busy = 1;
  robe->iOpcode = instruction.iOpcode;
  robe->dest = instruction.rgiOperand[0];
  robe->src1 = instruction.rgiOperand[1];
  robe->src2 = instruction.rgiOperand[2];
  robe->state = ISSUED;
  robe->value = 0;
  robe->done = 0;
  
  if(robe->iOpcode == OP_ADDI || robe->iOpcode == OP_SUBI) // INT dest
  {
    rgiReg[robe->dest].busy = 1;
    rgiReg[robe->dest].ptr = robe;
  }
  else if(robe->iOpcode == OP_L_D || robe->iOpcode == OP_ADD_D
    || robe->iOpcode == OP_SUB_D || robe->iOpcode == OP_MUL_D
    || robe->iOpcode == OP_DIV_D) // FP dest
  {
    rgfReg[robe->dest].busy = 1;
    rgfReg[robe->dest].ptr = robe;
  }
  avail_rob_entries--;
  return robe;
}

//***********************free_rob()***********************
void free_rob()
{
  memset(&robs[rob_head], 0, sizeof(ROB_ENTRY));
  rob_head++;
  rob_head %= NUM_ROB_ENTRIES;
  avail_rob_entries++;
}

//***********************assign_load()***********************
int assign_load(inst_entry instruction)
{
  ROB_ENTRY * robe;
  RES_STATION * rs;
  
  if(avail_rob_entries <= 0 && load_unit.free_stations > 0)
    return -1;
  
  robe = assign_rob(instruction);

  // need to find free stations	
  rs = dequeue(&(load_unit.free));
  enqueue(&(load_unit.active), rs);
  load_unit.free_stations--;

  // assign to rs
  rs->busy = 1;
  rs->issued_this_cycle = 1;
  rs->received_val_this_cycle = 0;
  rs->cycles_remaining = LOAD_CYCLE;
  rs->iOpcode = OP_L_D;
  
  if(rgiReg[robe->src2].busy == 1) // get ptr
  {
    rs->reg_qj = rgiReg[robe->src2].ptr;
    rs->waiting_for_operands = 1;
    robe->state = WAITING;
  }
  else
  {
    rs->reg_vj = rgiReg[robe->src2].value;
    rs->reg_qj = NULL;
    rs->waiting_for_operands = 0;
  }

  rs->reg_vk = robe->src1; // offset
  rs->dest = robe;
  return 1;
}

//***********************assign_store()***********************
int assign_store(inst_entry instruction)
{
  ROB_ENTRY * robe;
  RES_STATION * rs;
  
  if(avail_rob_entries <= 0 && store_unit.free_stations > 0)
    return -1;
  
  robe = assign_rob(instruction);

  // need to find free stations	
  rs = dequeue(&(store_unit.free));
  enqueue(&(store_unit.active), rs);
  store_unit.free_stations--;

  // assign to rs[i]
  rs->busy = 1;
  rs->issued_this_cycle = 1;
  rs->received_val_this_cycle = 0;
  rs->cycles_remaining = STORE_CYCLE;
  rs->iOpcode = OP_S_D;
  rs->waiting_for_operands = 0;
  
  if(rgfReg[robe->dest].busy == 1) // get ptr
  {
    rs->reg_qk = rgfReg[robe->dest].ptr;
    rs->waiting_for_operands = 1;
    robe->state = WAITING;
  }
  else
  {
    rs->reg_vk = rgfReg[robe->dest].value;
    rs->reg_qk = NULL;
  }
  
  if(rgiReg[robe->src2].busy == 1) // get ptr
  {
    rs->reg_vj = robe->src1; // add offset
    rs->reg_qj = rgiReg[robe->src2].ptr;
    rs->waiting_for_operands = 1;
    robe->state = WAITING;
  }
  else
  {
    rs->reg_vj = rgiReg[robe->src2].value + robe->src1; // add offset
    rs->reg_qj = NULL;
  }

  rs->dest = robe;
  return 1;
}

//***********************assign_int()***********************
int assign_int(inst_entry instruction)
{
  ROB_ENTRY * robe;
  RES_STATION * rs;
  
  if(avail_rob_entries <= 0 && int_unit.free_stations > 0)
    return -1;
  
  robe = assign_rob(instruction);

  // need to find free stations	
  rs = dequeue(&(int_unit.free));
  enqueue(&(int_unit.active), rs);
  int_unit.free_stations--;

  // assign to rs
  rs->busy = 1;
  rs->issued_this_cycle = 1;
  rs->received_val_this_cycle = 0;
  rs->cycles_remaining = INTEGER_CYCLE;
  rs->iOpcode = instruction.iOpcode;
  rs->waiting_for_operands = 0;
  
  if(instruction.iOpcode == OP_ADDI || instruction.iOpcode == OP_SUBI)
  {
    if(rgiReg[robe->src1].busy == 1) // get ptr
    {
      rs->reg_qj = rgiReg[robe->src1].ptr;
      rs->waiting_for_operands = 1;
      robe->state = WAITING;
    }
    else
    {
      rs->reg_vj = rgiReg[robe->src1].value;
      rs->reg_qj = NULL;
    }
    // immediate value
    rs->reg_vk = robe->src2;
    rs->reg_qk = NULL;
  }
  else // branch OP_BNEZ
  {
    if(rgiReg[robe->dest].busy == 1) // get ptr
    {
      rs->reg_qj = rgiReg[robe->dest].ptr;
      rs->waiting_for_operands = 1;
      robe->state = WAITING;
    }
    else
    {
      rs->reg_vj = rgiReg[robe->dest].value;
      rs->reg_qj = NULL;
    }
    // TODO: get mem addr from symbol table for branch
    //rs->reg_vk = (int) symbol_table(robe->src2);
  }
  
  rs->dest = robe;
  return 1;
}

//***********************assign_fp_add()***********************
int assign_fp_add(inst_entry instruction)
{
  ROB_ENTRY * robe;
  RES_STATION * rs;
  
  if(avail_rob_entries <= 0 && fp_add_unit.free_stations > 0)
    return -1;
  
  robe = assign_rob(instruction);

  // need to find free stations	
  rs = dequeue(&(fp_add_unit.free));
  enqueue(&(fp_add_unit.active), rs);
  fp_add_unit.free_stations--;

  // assign to rs
  rs->busy = 1;
  rs->issued_this_cycle = 1;
  rs->cycles_remaining = FP_ADDSUB_CYCLE;
  rs->received_val_this_cycle = 0;
  rs->iOpcode = instruction.iOpcode;
  rs->waiting_for_operands = 0;
  
  if(rgfReg[robe->src1].busy == 1) // get ptr
  {
    rs->reg_qj = rgfReg[robe->src1].ptr;
    rs->waiting_for_operands = 1;
    robe->state = WAITING;
  }
  else
  {
    rs->reg_vj = rgfReg[robe->src1].value;
    rs->reg_qj = NULL;
  }
  if(rgfReg[robe->src2].busy == 1) // get ptr
  {
    rs->reg_qk = rgfReg[robe->src2].ptr;
    rs->waiting_for_operands = 1;
    robe->state = WAITING;
  }
  else
  {
    rs->reg_vk = rgfReg[robe->src2].value;
    rs->reg_qk = NULL;
  }
  
  rs->dest = robe;
  return 1;
}

//***********************assign_fp_mult()***********************
int assign_fp_mult(inst_entry instruction) // need special case for divide
{
  ROB_ENTRY * robe;
  RES_STATION * rs;

  if(avail_rob_entries <= 0 && fp_mult_unit.free_stations > 0)
    return -1;

  robe = assign_rob(instruction);

  // need to find free stations	
  rs = dequeue(&(fp_mult_unit.free));
  enqueue(&(fp_mult_unit.active), rs);
  fp_mult_unit.free_stations--;

  // assign to rs[i]
  rs->busy = 1;
  rs->issued_this_cycle = 1;
  rs->received_val_this_cycle = 0;
  rs->iOpcode = instruction.iOpcode;
  rs->waiting_for_operands = 0;

  if(instruction.iOpcode == OP_MUL_D)
    rs->cycles_remaining = FP_MULTIPLY_CYCLE;
  else // division OP_DIV_D
    rs->cycles_remaining = FP_DIVISION_CYCLE;

  if(rgfReg[robe->src1].busy == 1) // get ptr
  {
    rs->reg_qj = rgfReg[robe->src1].ptr;
    rs->waiting_for_operands = 1;
    robe->state = WAITING;
  }
  else
  {
    rs->reg_vj = rgfReg[robe->src1].value;
    rs->reg_qj = NULL;
  }
  if(rgfReg[robe->src2].busy == 1) // get ptr
  {
    rs->reg_qk = rgfReg[robe->src2].ptr;
    rs->waiting_for_operands = 1;
    robe->state = WAITING;
  }
  else
  {
    rs->reg_vk = rgfReg[robe->src2].value;
    rs->reg_qk = NULL;
  }

  rs->dest = robe;
  return 1;
}

//***********************assign_to_rs()***********************
int assign_to_rs(inst_entry instruction)
{
  if(instruction.iOpcode == OP_L_D)
    return assign_load(instruction);
  else if(instruction.iOpcode == OP_S_D)
    return assign_store(instruction);
  else if(instruction.iOpcode == OP_ADDI || instruction.iOpcode == OP_SUBI 
    || instruction.iOpcode == OP_BNEZ) // branch treated as int
    return assign_int(instruction);
  else if(instruction.iOpcode == OP_ADD_D || instruction.iOpcode == OP_SUB_D)
    return assign_fp_add(instruction);
  else if(instruction.iOpcode == OP_MUL_D || instruction.iOpcode == OP_DIV_D)
    return assign_fp_mult(instruction);
}

/********************************************************************************************
*
* ROB AND RESERVATION STATION UPDATE CODE
*
*********************************************************************************************/
//***********************write_result()***********************
void write_result(RES_STATION * rs)
{
  printf("**write_result(): %p is done\n", rs->dest);
  RES_STATION * res;
  res = load_unit.active;
  while(res)
  {
    if(res->reg_qj == rs->dest)
    {
      res->received_val_this_cycle = 1;
      res->reg_vj = rs->dest->value;
      res->reg_qj = NULL;
      res->waiting_for_operands = 0;
    }
    res = res->next;
  }
  res = store_unit.active;
  while(res)
  {
    if(res->reg_qj == rs->dest)
    {
      res->received_val_this_cycle = 1;
      res->reg_vj = res->reg_vj + rs->dest->value; // reg_vj holds offset,  need to add it to val
      res->reg_qj = NULL;
    }
    if(res->reg_qk == rs->dest)
    {
      res->received_val_this_cycle = 1;
      res->reg_vk = rs->dest->value;
      res->reg_qk = NULL;
    }
    if(res->reg_qj == NULL && res->reg_qk == NULL)
      res->waiting_for_operands = 0;
    res = res->next;
  }
  res = int_unit.active;
  while(res)
  {
    if(res->iOpcode == OP_BNEZ)
    {
      if(res->reg_qj == rs->dest)
      {
	res->received_val_this_cycle = 1;
	res->reg_vj = rs->dest->value;
	res->reg_qj = NULL;
	res->waiting_for_operands = 0;
      }
    }
    else // ADDI SUBI
    {
      if(res->reg_qj == rs->dest)
      {
	res->received_val_this_cycle = 1;
	res->reg_vj = rs->dest->value;
	res->reg_qj = NULL;
      }
      if(res->reg_qk == rs->dest)
      {
	res->received_val_this_cycle = 1;
	res->reg_vk = rs->dest->value;
	res->reg_qk = NULL;
      }
      if(res->reg_qj == NULL && res->reg_qk == NULL)
	res->waiting_for_operands = 0;
    }
    res = res->next;
  }
  res = fp_add_unit.active;
  while(res)
  {
    if(res->reg_qj == rs->dest)
    {
     res->received_val_this_cycle = 1;
     res->reg_vj = rs->dest->value;
     res->reg_qj = NULL;
    }
    if(res->reg_qk == rs->dest)
    {
     res->received_val_this_cycle = 1;
     res->reg_vk = rs->dest->value;
     res->reg_qk = NULL;
    }
    if(res->reg_qj == NULL && res->reg_qk == NULL)
      res->waiting_for_operands = 0;
    res = res->next;
  }
  res = fp_mult_unit.active;
  while(res)
  {
    if(res->reg_qj == rs->dest)
    {
     res->received_val_this_cycle = 1;
     res->reg_vj = rs->dest->value;
     res->reg_qj = NULL;
    }
    if(res->reg_qk == rs->dest)
    {
     res->received_val_this_cycle = 1;
     res->reg_vk = rs->dest->value;
     res->reg_qk = NULL;
    }
    if(res->reg_qj == NULL && res->reg_qk == NULL)
      res->waiting_for_operands = 0;
    res = res->next;
  }
}

//***********************update_load()***********************
int update_load()
{
  RES_STATION * rs;
  rs = load_unit.active;
  while(rs)
  {
    //printf("**upate_load(): op %d rob %p busy %d issued this cycle %d waiting for ops %d received val this cycle %d\n", rs->iOpcode, rs->dest, rs->busy, rs->issued_this_cycle,
	//   rs->waiting_for_operands,rs->received_val_this_cycle);
    if(rs->busy == 1 && rs->issued_this_cycle == 0 
      && rs->waiting_for_operands == 0 && rs->received_val_this_cycle == 0)
    {
      if(rs->cycles_remaining == LOAD_CYCLE)
      { //just entering execution phase
       // printf("**update_load(): one is ready to go!\n");
        if(load_unit.started_one_this_cycle == 1) // cannot start another
	  continue;
	load_unit.started_one_this_cycle = 1;
	rs->dest->state = EXECUTE;
	//printf("**update_load(): starting one!\n");
      }
      rs->cycles_remaining--;
      if(rs->cycles_remaining == 0) // this instr is done with execution phase
      {
	printf("**update_load(): execution complete, writing result\n");
	// write result stage/notify ROB
	rs->busy = 0;
	rs->dest->state = WRITE_RES;
	rs->dest->done = 1;
	// TODO: get correct mem val
	rs->dest->value = rgliMemLocation[(int)(rs->reg_vj + rs->reg_vk)/4]; // not correct!
	printf("**update_load(): value=%f\n", rs->dest->value);
	rs->dest->entered_wr_this_cycle = 1;
	write_result(rs);
      }
    }
    rs = rs->next;
  }
}

//***********************update_store()***********************
int update_store()
{
  RES_STATION * rs;
  rs = store_unit.active;
  while(rs)
  {
    if(rs->busy == 1 && rs->issued_this_cycle == 0 
      && rs->waiting_for_operands == 0 && rs->received_val_this_cycle == 0)
    {
      if(rs->cycles_remaining == STORE_CYCLE)
      { //just entering execution phase
        if(store_unit.started_one_this_cycle == 1)
	  continue; // cannot start another
	store_unit.started_one_this_cycle = 1;
	rs->dest->state = EXECUTE;
      }
      rs->cycles_remaining--;
      if(rs->cycles_remaining == 0) // this instr is done with execution phase
      {
	printf("**update_store(): execution complete, enter write result stage\n");
	// write result stage/notify ROB
	rs->busy = 0;
	rs->dest->state = WRITE_RES;
	rs->dest->done = 1;
	// TODO: get correct mem val
	rs->dest->dest = rs->reg_vj; // offset was already computed in vj
	rs->dest->value = rs->reg_vk;
	printf("**update_store(): value=%f to be stored at mem=%d\n", rs->reg_vk, (int)(rs->reg_vj));
	rs->dest->entered_wr_this_cycle = 1;
	write_result(rs);
      }
    }
    rs = rs->next;
  }
}

//***********************update_int()***********************
int update_int()
{
  RES_STATION * rs;
  rs = int_unit.active;
  while(rs)
  {
    if(rs->busy == 1 && rs->issued_this_cycle == 0 
      && rs->waiting_for_operands == 0 && rs->received_val_this_cycle == 0)
    {
      if(rs->cycles_remaining == INTEGER_CYCLE)
      { //just entering execution phase
        if(int_unit.started_one_this_cycle == 1)
	  continue; // cannot start another
	int_unit.started_one_this_cycle = 1;
	rs->dest->state = EXECUTE;
      }
      rs->cycles_remaining--;
      if(rs->cycles_remaining == 0) // this instr is done with execution phase
      {
	printf("**update_int(): execution complete, enter write result stage\n");
	// write result stage/notify ROB
	rs->busy = 0;
	rs->dest->state = WRITE_RES;
	rs->dest->done = 1;
	
	if(rs->iOpcode == OP_BNEZ) // branch
	{
	  if((int) rs->reg_vj != 0) // Branch if vj not equal to zero
	    PC = (int) rs->reg_vk; // TODO: coordinate this line with the branch vk assignment in assign_int()
	  branch = 0; // allow IF to continue in simulate()
	  printf("**update_int(): value=%f\n", rs->dest->value);
	  rs->dest->entered_wr_this_cycle = 1;
	  // don't send branch to write result, not needed by any other res station
	}
	else 
	{
	  if(rs->iOpcode == OP_ADDI) // add immediate
	    rs->dest->value = (int)(rs->reg_vj + rs->reg_vk);
	  else // subtract immediate
	    rs->dest->value = (int)(rs->reg_vj - rs->reg_vk);
	  printf("**update_int(): value=%f\n", rs->dest->value);
	  rs->dest->entered_wr_this_cycle = 1;
	  write_result(rs);
	}
      }
    }
    rs = rs->next;
  }
}

//***********************update_fp_add()***********************
int update_fp_add()
{
  RES_STATION * rs;
  rs = fp_add_unit.active;
  while(rs)
  {
    if(rs->busy == 1 && rs->issued_this_cycle == 0 
      && rs->waiting_for_operands == 0 && rs->received_val_this_cycle == 0)
    {
      if(rs->cycles_remaining == FP_ADDSUB_CYCLE)
      { //just entering execution phase
        if(fp_add_unit.started_one_this_cycle == 1)
	  continue; // cannot start another
	fp_add_unit.started_one_this_cycle = 1;
	rs->dest->state = EXECUTE;
      }
      rs->cycles_remaining--;
      if(rs->cycles_remaining == 0) // this instr is done with execution phase
      {
	printf("**update_fp_add(): execution complete, enter write result stage\n");
	// write result stage/notify ROB
	rs->busy = 0;
	rs->dest->state = WRITE_RES;
	rs->dest->done = 1;

	if(rs->iOpcode == OP_ADD_D) // add immediate
	  rs->dest->value = rs->reg_vj + rs->reg_vk;
	else // OP_SUB_D
	  rs->dest->value = rs->reg_vj - rs->reg_vk;
	printf("**update_fp_add(): value=%f\n", rs->dest->value);
	rs->dest->entered_wr_this_cycle = 1;
	write_result(rs);
      }
    }
    rs = rs->next;
  }
}

//***********************update_fp_mult()***********************
int update_fp_mult()
{
  RES_STATION * rs;
  rs = fp_mult_unit.active;
  while(rs)
  {
    if(rs->busy == 1 && rs->issued_this_cycle == 0 
      && rs->waiting_for_operands == 0 && rs->received_val_this_cycle == 0)
    {
      if(fp_mult_unit.divide == 0) // not in non pipelined divide mode, can start another instruction
      {
	if(rs->cycles_remaining == FP_MULTIPLY_CYCLE)
	{ //just entering execution phase
	  if(fp_mult_unit.started_one_this_cycle == 1)
	    continue; // cannot start another
	  fp_mult_unit.started_one_this_cycle = 1;
	  rs->dest->state = EXECUTE;
	}
	else if(rs->cycles_remaining == FP_DIVISION_CYCLE)// if(rs->cycles_remaining == FP_DIVISION_CYCLE) // need to start a divide
	{
	  fp_mult_unit.divide = 1; 
	}
      }
      else
	printf("**update_fp_mult(): in divide mode, do not start another fp_mult/div instruction\n");
      rs->cycles_remaining--;
      if(rs->cycles_remaining == 0) // this instr is done with execution phase
      {
	printf("**update_fp_mult(): execution complete, enter write result stage\n");
	// write result stage/notify ROB
	rs->busy = 0;
	rs->dest->state = WRITE_RES;
	rs->dest->done = 1;

	if(rs->iOpcode == OP_MUL_D) // mult fp
	  rs->dest->value = rs->reg_vj * rs->reg_vk;
	else // OP_DIV_D
	{
	  rs->dest->value = (float) (rs->reg_vj) / rs->reg_vk; // check this
	  fp_mult_unit.divide = 0; // yay! done with non pipelined divide
	}
	printf("**update_fp_mult(): value=%f\n", rs->dest->value);
	rs->dest->entered_wr_this_cycle = 1;
	write_result(rs);
      }
    }
    rs = rs->next;
  }
}

//***********************update_rs()***********************
int update_rs()
{
  update_load();
  update_store();
  update_int();
  update_fp_add();
  update_fp_mult();
}

//***********************update_rob()***********************
/*
int update_rob() // TODO: need to seperate ROB register status 'file' from actual RF on write_result and commit
{
  if(robs[rob_head].busy == 1 && robs[rob_head].state == WRITE_RES)
  {
    if(robs[rob_head].entered_wr_this_cycle == 1)
      robs[rob_head].entered_wr_this_cycle = 0;
    else // commit
    {
      robs[rob_head].state = COMMIT;
      if(robs[rob_head].iOpcode == OP_S_D)
      {
	rgliMemLocation[robs[rob_head].dest / 4] = robs[rob_head].value; // TODO: get correct mem value, wrong as is!
      }
      else if(robs[rob_head].iOpcode == OP_ADDI || robs[rob_head].iOpcode == OP_SUBI)
      {
	rgiReg[robs[rob_head].dest].value = (int) robs[rob_head].value;
	rgiReg[robs[rob_head].dest].busy = 0;
	rgiReg[robs[rob_head].dest].ptr = NULL;
      }
      else if(robs[rob_head].iOpcode == OP_ADD_D || robs[rob_head].iOpcode == OP_SUB_D
	|| robs[rob_head].iOpcode == OP_MUL_D || robs[rob_head].iOpcode == OP_DIV_D
	|| robs[rob_head].iOpcode == OP_L_D)
      {
	rgfReg[robs[rob_head].dest].value = robs[rob_head].value;
	rgfReg[robs[rob_head].dest].busy = 0;
	rgfReg[robs[rob_head].dest].ptr = NULL;
      }   
      //else - ignore branches for now
      free_rob(&robs[rob_head]);
    }
  }
}
*/
