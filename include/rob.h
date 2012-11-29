/*
 * rob.h -- Re-Order Buffer Header
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

#ifndef ROB_H
#define ROB_H

#include "config.h"
#include "init.h"

#define ROB_ST_FREE		0
#define ROB_ST_IS		1
#define ROB_ST_EX		2
#define ROB_ST_WB		4
#define ROB_ST_CO		8

/*
 * ROB_ENTRY: Re-order Buffer Entry
 * ------------------------------------
 * fState: State Flag for Entry: FREE, IS, EX, WB, CO
 * fSpec: Speculative Bit
 * fSb: Store Bit Flag
 * fDesValid : Destination Valid Flag
 * iRegValue : Register Value Stored in ROB
 * pARF: pointer to targeted Architecture Register File
 * ppInst: pointer to Instruction Architecture
 */

typedef struct rob_entry {
	char fState;
	char fSpec;
	char fSb;
	char fDesValid;
	int iRegValue;
	double fRegValue;
	void *pARF;
	// For Simulation only
	int iIndex;
	char fFloatInt;
	char entered_wr_this_cycle;
	char available_next_cycle;
	struct inst_entry *pInst;
	struct rob_entry *next;
} ROB_ENTRY;

/*
 * ROB_TABLE: Re-order Buffer Structure
 * --------------------------------------
 * iAvail - Count of available entry
 * busy - List of busy ROB_ENTRY
 * free - List of free ROB_ENTRY
 * arROB - ROB Entry Array
 */
typedef struct rob_table {
	ROB_ENTRY arROB[NR_ROB_ENT];
	ROB_ENTRY *busy;
	ROB_ENTRY *free;
	int iAvail;
} ROB_TABLE;

typedef struct inst_reg_entry
{
	int index;
	char busy;
	ROB_ENTRY * ptr;
	int value;
} int_reg_entry;

typedef struct fp_reg_entry
{
	int index;
	char busy;
	ROB_ENTRY * ptr;
	double value;
} fp_reg_entry;

#endif
