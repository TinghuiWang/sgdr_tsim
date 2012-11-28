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
	inst_entry *pInst;
} ROB_ENTRY;

#endif
