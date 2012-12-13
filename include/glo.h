/*
 * glo.h -- Global Variable definition
 * should be included in all the files except main.c
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

#ifndef GLO_H
#define GLO_H

#include "config.h"
#include "init.h"
#include "fu.h"
#include "rob.h"

// Architecture Register File
extern int_reg_entry rgiReg[I_REG_MAX * NR_THREAD];
extern fp_reg_entry rgfReg[FP_REG_MAX * NR_THREAD];

// Re-Order Buffer
extern ROB_TABLE rob_tab[NR_THREAD];

// Input file
extern FILE *fpInAsm;
extern FILE *fpOutResult;
// Function Units
extern LOAD_UNIT load_unit;
extern STORE_UNIT store_unit;
extern INT_UNIT int_unit;
extern FP_ADD_UNIT fp_add_unit;
extern FP_MULT_UNIT fp_mult_unit;

// Temporary Variables
extern int fSpeculate[NR_THREAD];
extern int PC[NR_THREAD];
extern int branch;
extern void* pExUnitHeader;
extern void* pROB;

#endif
