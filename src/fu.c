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
#include "glo.h"


/********************************************************************************************
*
* INIT AND UTILITY FUNCTIONS
*
*********************************************************************************************/

//***********************op()***********************
// To-Do: Change Case Params to Global Definition
char * op(int op)
{
    switch(op) {
   	case OP_L_D: 
		return " LOAD.D";
   	case OP_MUL_D: 
    	return " MULT.D";
   	case OP_ADD_D: 
    	return "  ADD.D";
   	case OP_S_D: 
    	return "STORE.D";
   	case OP_SUB_D: 
    	return "  SUB.D";
   	case OP_ADDI: 
    	return "   ADDI";
   	case OP_SUBI: 
    	return "   SUBI";
   	case OP_SLTI: 
    	return "   SLTI";
   	case OP_BNEZ: 
    	return "   BNEZ";
   	case OP_NOP: 
    	return "    NOP";
   	case OP_DIV_D: 
    	return "  DIV.D";
   	default: 
		return "       ";
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
   printf("F%d: busy=%d value=%f ptr=%p\n", i, rgfReg[i].busy, rgfReg[i].value, rgfReg[i].ptr);
}

//***********************print_mem_status()***********************
void print_mem_status()
{
 printf("\n---------------Memory--------------\n");

}

//***********************print_rs_status()***********************
void print_rs_status()
{/*
  RES_STATION * rs;
  printf("\n---------------RES STATIONS--------------\n");
  rs = load_unit.active;
  while(rs)
  {
    printf("(%p)LOAD#%d: busy=%d cycles_remaining=%d op=%s vj=%d vk=%d qj=%x qk=%x dest=%x waiting_for_operands=%d\n", rs, rs->id, rs->busy, 
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
  }*/
  // this code is to print ALL reservation stations, both inactive (not busy) and active (busy)
  int i; 
  printf("\n---------------RES STATIONS--------------\n");
  for(i = 0; i < LOAD_RS; i++)
  {
    printf("Load#%d %p: next: %p busy=%d cycles_remaining=%d op=%s vj=%d vk=%d qj=%p qk=%p dest=%p waiting_for_operands=%d\n", i, &load_unit.rs[i], load_unit.rs[i].next, load_unit.rs[i].busy, 
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
    printf("FP Add/Sub#%d %p: next: %p busy=%d cycles_remaining=%d op=%s vj=%f vk=%f qj=%p qk=%p dest=%p waiting_for_operands=%d\n", i, &fp_add_unit.rs[i], fp_add_unit.rs[i].next, fp_add_unit.rs[i].busy, 
	   fp_add_unit.rs[i].cycles_remaining, op(fp_add_unit.rs[i].iOpcode), fp_add_unit.rs[i].reg_vj, fp_add_unit.rs[i].reg_vk, 
	   fp_add_unit.rs[i].reg_qj, fp_add_unit.rs[i].reg_qk, fp_add_unit.rs[i].dest, fp_add_unit.rs[i].waiting_for_operands);
  }
  for(i = 0; i < FP_MULT_RS; i++)
  {
    printf("FP Mult/Div#%d: busy=%d cycles_remaining=%d op=%s vj=%f vk=%f qj=%p qk=%p dest=%p waiting_for_operands=%d\n", i, fp_mult_unit.rs[i].busy, 
	   fp_mult_unit.rs[i].cycles_remaining, op(fp_mult_unit.rs[i].iOpcode), fp_mult_unit.rs[i].reg_vj, fp_mult_unit.rs[i].reg_vk, 
	   fp_mult_unit.rs[i].reg_qj, fp_mult_unit.rs[i].reg_qk, fp_mult_unit.rs[i].dest, fp_mult_unit.rs[i].waiting_for_operands);
  }
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
      if(p == q)
      {
        //printf("curr %p to be removed %p \n", p, q);
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
      //printf("**remove_queue(): removed it, was not first item\n");
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

	load_unit.free_stations = LOAD_RS;
	store_unit.free_stations = STORE_RS;
	int_unit.free_stations = INTEGER_RS;
	fp_add_unit.free_stations = FP_ADD_RS;
	fp_mult_unit.free_stations = FP_MULT_RS;

  
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

    if(load_unit.rs[i].busy == 0) // no longer active
    {
      if(remove_queue(&(load_unit.active), &(load_unit.rs[i])) == 1) // was in active list
      {
       // printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$clear_flags(): after remove from active and beofre insert into free\n");
    //printf("load_unit.active %p load_unit.free %p\n", load_unit.active, load_unit.free);
   // print_rs_status();
	memset(&(load_unit.rs[i]), 0, sizeof(RES_STATION));
        load_unit.rs[i].id = i;
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
        store_unit.rs[i].id = i;
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
        int_unit.rs[i].id = i;
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
    printf("\n\n**&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&clear_flags(): id=%d busy=%d\n", i, fp_add_unit.rs[i].busy);
    printf("fp_add_unit.active %p fp_add_unit.free %p\n", fp_add_unit.active, fp_add_unit.free);
    print_rs_status();
    if(fp_add_unit.rs[i].busy == 0) // no longer active
    {
      if(remove_queue(&(fp_add_unit.active), &(fp_add_unit.rs[i])) == 1) // was in active list
      {
	memset(&(fp_add_unit.rs[i]), 0, sizeof(RES_STATION));
        fp_add_unit.rs[i].id = i;
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
        fp_mult_unit.rs[i].id = i;
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

//***********************assign_load()***********************
int assign_load(ROB_ENTRY * robe)
{
  RES_STATION * rs;
  
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
  
  if(rgiReg[robe->pInst->rgiOperand[2]].busy == 1) // get ptr
  {
    rs->reg_qj = rgiReg[robe->pInst->rgiOperand[2]].ptr;
    rs->waiting_for_operands = 1;
    robe->fState = WAITING;
  }
  else
  {
    rs->reg_vj = rgiReg[robe->pInst->rgiOperand[2]].value;
    rs->reg_qj = NULL;
    rs->waiting_for_operands = 0;
  }

  rs->reg_vk = robe->pInst->rgiOperand[1]; // offset
  rs->dest = robe;
  return 1;
}

//***********************assign_store()***********************
int assign_store(ROB_ENTRY * robe)
{
  RES_STATION *rs;
  
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
  
  if(rgfReg[robe->pInst->rgiOperand[0]].busy == 1) // get ptr
  {
    rs->reg_qk = rgfReg[robe->pInst->rgiOperand[0]].ptr;
    rs->waiting_for_operands = 1;
    robe->fState = WAITING;
  }
  else
  {
    rs->reg_vk = rgfReg[robe->pInst->rgiOperand[0]].value;
    rs->reg_qk = NULL;
  }
  
  if(rgiReg[robe->pInst->rgiOperand[2]].busy == 1) // get ptr
  {
    rs->reg_vj = robe->pInst->rgiOperand[1]; // add offset
    rs->reg_qj = rgiReg[robe->pInst->rgiOperand[2]].ptr;
    rs->waiting_for_operands = 1;
    robe->fState = WAITING;
  }
  else
  {
    rs->reg_vj = rgiReg[robe->pInst->rgiOperand[2]].value + robe->pInst->rgiOperand[1]; // add offset
    rs->reg_qj = NULL;
  }

  rs->dest = robe;
  return 1;
}

//***********************assign_int()***********************
int assign_int(ROB_ENTRY * robe, int thread)
{
  RES_STATION * rs;
  
  // need to find free stations	
  rs = dequeue(&(int_unit.free));
  enqueue(&(int_unit.active), rs);
  int_unit.free_stations--;

  // assign to rs
  rs->busy = 1;
  rs->issued_this_cycle = 1;
  rs->received_val_this_cycle = 0;
  rs->cycles_remaining = INTEGER_CYCLE;
  rs->iOpcode = robe->pInst->iOpcode;
  rs->waiting_for_operands = 0;
  
  if(robe->pInst->iOpcode == OP_ADDI || robe->pInst->iOpcode == OP_SUBI || robe->pInst->iOpcode == OP_SLTI)
  {
    if(rgiReg[robe->pInst->rgiOperand[1]].busy == 1) // get ptr
    {
      rs->reg_qj = rgiReg[robe->pInst->rgiOperand[1]].ptr;
      rs->waiting_for_operands = 1;
      robe->fState = WAITING;
    }
    else
    {
      rs->reg_vj = rgiReg[robe->pInst->rgiOperand[1]].value;
      rs->reg_qj = NULL;
    }
    // immediate value
    rs->reg_vk = robe->pInst->rgiOperand[2];
    rs->reg_qk = NULL;
  }
  else // branch OP_BNEZ
  {
    if(rgiReg[robe->pInst->rgiOperand[0]].busy == 1) // get ptr
    {
      rs->reg_qj = rgiReg[robe->pInst->rgiOperand[0]].ptr;
      rs->waiting_for_operands = 1;
      robe->fState = WAITING;
    }
    else
    {
      rs->reg_vj = rgiReg[robe->pInst->rgiOperand[0]].value;
      rs->reg_qj = NULL;
    }
    // TODO: get mem addr from symbol table for branch
    rs->reg_vk = (int) robe->pInst->rgiOperand[1];
    if((int) rs->reg_vj != 0) // Branch if vj not equal to zero 
    {
      if(thread == 0)
        PC[thread] = (int) rs->reg_vk;
      if(thread == 1)
        PC[thread] = (int) rs->reg_vk + 144; // TODO check this!
	  printf("\e[32m Reg Value %d, Thread %d Goto %d \e[0m\n", rs->reg_vj, thread, PC[thread]);
    }
    printf("**assign_int(): pc[%d]=%d\n", thread, PC[thread]);
  }
  
  rs->dest = robe;
  return 1;
}

//***********************assign_fp_add()***********************
int assign_fp_add(ROB_ENTRY * robe)
{
  RES_STATION * rs;
  
  // need to find free stations	
  rs = dequeue(&(fp_add_unit.free));
  enqueue(&(fp_add_unit.active), rs);
  fp_add_unit.free_stations--;

  // assign to rs
  rs->busy = 1;
  rs->issued_this_cycle = 1;
  rs->cycles_remaining = FP_ADDSUB_CYCLE;
  rs->received_val_this_cycle = 0;
  rs->iOpcode = robe->pInst->iOpcode;
  rs->waiting_for_operands = 0;
  
  if(rgfReg[robe->pInst->rgiOperand[1]].busy == 1) // get ptr
  {
    rs->reg_qj = rgfReg[robe->pInst->rgiOperand[1]].ptr;
    rs->waiting_for_operands = 1;
    robe->fState = WAITING;
  }
  else
  {
    rs->reg_vj = rgfReg[robe->pInst->rgiOperand[1]].value;
    rs->reg_qj = NULL;
  }
  if(rgfReg[robe->pInst->rgiOperand[2]].busy == 1) // get ptr
  {
    rs->reg_qk = rgfReg[robe->pInst->rgiOperand[2]].ptr;
    rs->waiting_for_operands = 1;
    robe->fState = WAITING;
  }
  else
  {
    rs->reg_vk = rgfReg[robe->pInst->rgiOperand[2]].value;
    rs->reg_qk = NULL;
  }
  
  rs->dest = robe;
  return 1;
}

//***********************assign_fp_mult()***********************
int assign_fp_mult(ROB_ENTRY *robe) // need special case for divide
{
  RES_STATION * rs;

  // need to find free stations	
  rs = dequeue(&(fp_mult_unit.free));
  enqueue(&(fp_mult_unit.active), rs);
  fp_mult_unit.free_stations--;

  // assign to rs[i]
  rs->busy = 1;
  rs->issued_this_cycle = 1;
  rs->received_val_this_cycle = 0;
  rs->iOpcode = robe->pInst->iOpcode;
  rs->waiting_for_operands = 0;

  if(robe->pInst->iOpcode == OP_MUL_D)
    rs->cycles_remaining = FP_MULTIPLY_CYCLE;
  else // division OP_DIV_D
    rs->cycles_remaining = FP_DIVISION_CYCLE;

  if(rgfReg[robe->pInst->rgiOperand[1]].busy == 1) // get ptr
  {
    rs->reg_qj = rgfReg[robe->pInst->rgiOperand[1]].ptr;
    rs->waiting_for_operands = 1;
    robe->fState = WAITING;
  }
  else
  {
    rs->reg_vj = rgfReg[robe->pInst->rgiOperand[1]].value;
    rs->reg_qj = NULL;
  }
  if(rgfReg[robe->pInst->rgiOperand[2]].busy == 1) // get ptr
  {
    rs->reg_qk = rgfReg[robe->pInst->rgiOperand[2]].ptr;
    rs->waiting_for_operands = 1;
    robe->fState = WAITING;
  }
  else
  {
    rs->reg_vk = rgfReg[robe->pInst->rgiOperand[2]].value;
    rs->reg_qk = NULL;
  }

  rs->dest = robe;
  return 1;
}

//***********************assign_to_rs()***********************
int assign_to_rs(ROB_ENTRY *rob_ent, int thread)
{
	int i;
	i = utGetUnitTypeForInstr(rob_ent->pInst);
	switch(i) { 
	case UNIT_LOAD:
    	return assign_load(rob_ent);
	case UNIT_STORE:
    	return assign_store(rob_ent);
	case UNIT_INT:
    	return assign_int(rob_ent, thread);
	case UNIT_FP_ADD:
    	return assign_fp_add(rob_ent);
	case UNIT_FP_MULT:
    	return assign_fp_mult(rob_ent);
	default:
		printf("Err: unknown Unit %d\n", i);
		return 0; 
	}
}

/********************************************************************************************
*
* ROB AND RESERVATION STATION UPDATE CODE
*
*********************************************************************************************/
//***********************write_result()***********************
void write_result(RES_STATION * rs)
{
  printf("**write_result(): rob:%p is done res:%p\n", rs->dest, rs);
  write_result_counter++;
  RES_STATION * res;
  res = load_unit.active;
  while(res)
  {
    if(res->reg_qj == rs->dest)
    {
      res->received_val_this_cycle = 1;
      res->reg_vj = rs->dest->fRegValue;
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
      res->reg_vj = res->reg_vj + rs->dest->iRegValue; // reg_vj holds offset,  need to add it to val
      res->reg_qj = NULL;
    }
    if(res->reg_qk == rs->dest)
    {
      res->received_val_this_cycle = 1;
      res->reg_vk = rs->dest->fRegValue;
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
	res->reg_vj = rs->dest->iRegValue;
	res->reg_qj = NULL;
	res->waiting_for_operands = 0;
      }
    }
    else // ADDI SUBI SLTI
    {
      if(res->reg_qj == rs->dest)
      {
	res->received_val_this_cycle = 1;
	res->reg_vj = rs->dest->iRegValue;
	res->reg_qj = NULL;
      }
      if(res->reg_qk == rs->dest) // TODO: don't need this check because it is an immediate
      {
	res->received_val_this_cycle = 1;
	res->reg_vk = rs->dest->iRegValue;
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
     res->reg_vj = rs->dest->fRegValue;
     res->reg_qj = NULL;
    }
    if(res->reg_qk == rs->dest)
    {
     res->received_val_this_cycle = 1;
     res->reg_vk = rs->dest->fRegValue;
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
     res->reg_vj = rs->dest->fRegValue;
     res->reg_qj = NULL;
    }
    if(res->reg_qk == rs->dest)
    {
     res->received_val_this_cycle = 1;
     res->reg_vk = rs->dest->fRegValue;
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
  inst_entry instr;
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
        {
          rs = rs->next;
	  continue; // cannot start another
        }
	load_unit.started_one_this_cycle = 1;
	rs->dest->fState = EXECUTE;
	//printf("**update_load(): starting one!\n");
      }
      rs->cycles_remaining--;
      if(rs->cycles_remaining == 0) // this instr is done with execution phase
      {
	printf("**update_load(): execution complete, writing result\n");
	// write result stage/notify ROB
	rs->busy = 0;
	rs->dest->fState = WRITE_RES;
	// TODO: get correct mem val
	instr = inst_fetch((int)(rs->reg_vj + rs->reg_vk), fpOutResult);
	rs->dest->fRegValue = instr.rgiOperand[0];
	printf("**update_load(): value=%f\n", rs->dest->fRegValue);
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
        {
          rs = rs->next;
	  continue; // cannot start another
        }
	store_unit.started_one_this_cycle = 1;
	rs->dest->fState = EXECUTE;
      }
      rs->cycles_remaining--;
      if(rs->cycles_remaining == 0) // this instr is done with execution phase
      {
	printf("**update_store(): execution complete, enter write result stage\n");
	// write result stage/notify ROB
	rs->busy = 0;
	rs->dest->fState = WRITE_RES;
	// TODO: get correct mem val
	rs->dest->pInst->rgiOperand[0] = rs->reg_vj; // offset was already computed in vj
	rs->dest->fRegValue = rs->reg_vk;
	printf("**update_store(): value=%f to be stored at mem=%d\n", rs->reg_vk, (int)(rs->reg_vj));
	rs->dest->entered_wr_this_cycle = 1;
	//write_result(rs); no other instrs are waiting for a store to complete
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
        {
          rs = rs->next;
	  continue; // cannot start another
        }
	int_unit.started_one_this_cycle = 1;
	rs->dest->fState = EXECUTE;
      }
      rs->cycles_remaining--;
      if(rs->cycles_remaining == 0) // this instr is done with execution phase
      {
	printf("**update_int(): execution complete, enter write result stage\n");
	// write result stage/notify ROB
	rs->busy = 0;
	rs->dest->fState = WRITE_RES;
	
	if(rs->iOpcode == OP_BNEZ) // branch
	{
	  rs->dest->entered_wr_this_cycle = 1;
	  // don't send branch to write result, not needed by any other res station
	}
	else 
	{
	  if(rs->iOpcode == OP_ADDI) // add immediate
	    rs->dest->iRegValue = (int)(rs->reg_vj + rs->reg_vk);
	  else if(rs->iOpcode == OP_SUBI) // subtract immediate
	    rs->dest->iRegValue = (int)(rs->reg_vj - rs->reg_vk);
	  else // slti immediate
          {
            if(rs->reg_vj < rs->reg_vk)
	      rs->dest->iRegValue = 1;
            else
	      rs->dest->iRegValue = 0;
          }
	  printf("**update_int(): value=%d\n", rs->dest->iRegValue);
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
   // printf("\n\n**&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&update_fp_add(): id=%d busy=%d rs=%p\n", rs->id, rs->busy, rs);
   // printf("fp_add_unit.active %p fp_add_unit.free %p\n", fp_add_unit.active, fp_add_unit.free);
   // print_rs_status();
    if(rs->busy == 1 && rs->issued_this_cycle == 0 
      && rs->waiting_for_operands == 0 && rs->received_val_this_cycle == 0)
    {
      if(rs->cycles_remaining == FP_ADDSUB_CYCLE)
      { //just entering execution phase
        if(fp_add_unit.started_one_this_cycle == 1)
        {
          rs = rs->next;
	  continue; // cannot start another
        }
	fp_add_unit.started_one_this_cycle = 1;
	rs->dest->fState = EXECUTE;
      }
      rs->cycles_remaining--;
      if(rs->cycles_remaining == 0) // this instr is done with execution phase
      {
	printf("**update_fp_add(): execution complete, enter write result stage\n");
	// write result stage/notify ROB
	rs->busy = 0;
	rs->dest->fState = WRITE_RES;

	if(rs->iOpcode == OP_ADD_D) // add immediate
	  rs->dest->fRegValue = rs->reg_vj + rs->reg_vk;
	else // OP_SUB_D
	  rs->dest->fRegValue = rs->reg_vj - rs->reg_vk;
	printf("**update_fp_add(): value=%f\n", rs->dest->fRegValue);
	rs->dest->entered_wr_this_cycle = 1;
	write_result(rs);
      }
    }
    //printf("\n\n^^^^^^^^^^^^^^^^^^^^^^rs %p, rs->next %p\n", rs, rs->next);
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
        {
          rs = rs->next;
	  continue; // cannot start another
        }
	  fp_mult_unit.started_one_this_cycle = 1;
	  rs->dest->fState = EXECUTE;
	}
	else if(rs->cycles_remaining == FP_DIVISION_CYCLE)// if(rs->cycles_remaining == FP_DIVISION_CYCLE) // need to start a divide
	{
	  fp_mult_unit.divide = 1; 
	  if(fp_mult_unit.started_one_this_cycle == 1)
	    continue; // cannot start another
	  fp_mult_unit.started_one_this_cycle = 1;
	  rs->dest->fState = EXECUTE;
	}
      }
      else
	printf("**update_fp_mult(): in divide mode, do not start another fp_mult/div instruction\n");
      if(rs->dest->fState == EXECUTE)
        rs->cycles_remaining--;
      if(rs->cycles_remaining == 0) // this instr is done with execution phase
      {
	printf("**update_fp_mult(): execution complete, enter write result stage\n");
	// write result stage/notify ROB
	rs->busy = 0;
	rs->dest->fState = WRITE_RES;

	if(rs->iOpcode == OP_MUL_D) // mult fp
	  rs->dest->fRegValue = rs->reg_vj * rs->reg_vk;
	else // OP_DIV_D
	{
	  rs->dest->fRegValue = (float) (rs->reg_vj) / rs->reg_vk; // check this
	  fp_mult_unit.divide = 0; // yay! done with non pipelined divide
	}
	printf("**update_fp_mult(): value=%f\n", rs->dest->fRegValue);
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
