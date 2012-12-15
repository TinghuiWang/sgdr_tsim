/*
 * fu.h -- functional unit param header
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

#ifndef FU_H
#define FU_H

#include "config.h"
#include "init.h"
#include "rob.h"

typedef struct res_station
{
	char id;
	char busy;
	int cycles_remaining;
	int iOpcode;
	float reg_vj;
	float reg_vk;
	char waiting_for_operands;
	char issued_this_cycle;
	char received_val_this_cycle;
	int busy_counter;
	ROB_ENTRY *reg_qj;
	ROB_ENTRY *reg_qk;
	ROB_ENTRY *dest;
	struct res_station *next; // needed for priority queues
} RES_STATION;

typedef struct
{
	int free_stations;
	char started_one_this_cycle;
	RES_STATION rs[LOAD_RS];
	RES_STATION *active;
	RES_STATION *free;
} LOAD_UNIT;

typedef struct
{
	int free_stations;
	char started_one_this_cycle;
	RES_STATION rs[STORE_RS];
	RES_STATION *active;
	RES_STATION *free;
} STORE_UNIT;

typedef struct
{
	int free_stations;
	char started_one_this_cycle;
	RES_STATION rs[INTEGER_RS];
	RES_STATION *active;
	RES_STATION *free;
} INT_UNIT;

typedef struct
{
	int free_stations;
	char started_one_this_cycle;
	RES_STATION rs[FP_ADD_RS];
	RES_STATION *active;
	RES_STATION *free;
} FP_ADD_UNIT;

typedef struct
{
	char divide; // needed because divide is NOT pipelined
	int free_stations;
	char started_one_this_cycle;
	RES_STATION rs[FP_MULT_RS];
	RES_STATION *active;
	RES_STATION *free;
} FP_MULT_UNIT;

// prototypes
int init_fu();
int assign_to_rs(ROB_ENTRY * robe, int thread);
int update_rs();
char * op(int op);

#endif

