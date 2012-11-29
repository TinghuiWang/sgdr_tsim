/*
 * config.h -- config param header
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

#ifndef CONFIG_H
#define CONFIG_H

#define TRUE			1
#define FALSE			0

#define NR_ROB_ENT		32
#define NR_THREAD	1

#define FP_ADD_RS		4
#define FP_MULT_RS		3
#define LOAD_RS			4
#define STORE_RS		4
#define INTEGER_RS		4

#define	FP_ADDSUB_CYCLE		3
#define FP_MULTIPLY_CYCLE	6
#define FP_DIVISION_CYCLE	13
#define LOAD_CYCLE		2
#define STORE_CYCLE		1
#define INTEGER_CYCLE		1

#define FP_ADDSUB_PIPELINED		1
#define FP_MULTIPLY_PIPELINED	0
#define FP_DIVISION_PIPELINED	1
#define LOAD_PIPELINED			1
#define STORE_PIPELINED			1
#define INTEGER_PIPELINED		1

#define FP_REG_MAX		16
#define I_REG_MAX		16

// OPCODES
#define OP_MEM		-1 //Indicates only memory
#define OP_L_D		0x80
#define OP_MUL_D	0x81
#define OP_ADD_D	0x82
#define OP_S_D		0x84
#define OP_SUB_D	0x88
#define OP_DIV_D	0x90
#define OP_ADDI		0x00
#define OP_SUBI		0x01
#define OP_BNEZ		0x02
#define OP_NOP		0x04

// UNIT_TYPE
#define UNIT_LOAD		0x01
#define UNIT_STORE		0x02
#define UNIT_INT		0x04
#define UNIT_FP_ADD		0x08
#define UNIT_FP_MULT	0x10

//STATES
#define ISSUED		0
#define EXECUTE		1
#define WRITE_RES	2
#define COMMIT		4
#define WAITING		8
#define FREE	    16

#endif
