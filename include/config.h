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

#ifdef CONFIG_H
#define CONFIG_H

#define	FP_ADDSUB_CYCLE		3
#define FP_MULTIPLY_CYCLE	6
#define FP_DIVISION_CYCLE	13
#define LOAD_CYCLE			2
#define STORE_CYCLE			2
#define INTEGER_CYCLE		1

#define FP_ADDSUB_PIPELINED		1
#define FP_MULTIPLY_PIPELINED	0
#define FP_DIVISION_PIPELINED	1
#define LOAD_PIPELINED			1
#define STORE_PIPELINED			1
#define INTEGER_PIPELINED		1

//OPCODES
#define L_D		0
#define MUL_D	1
#define ADD_D	2
#define S_D		3
#define SUB_D	4
#define ADDI	5
#define SUBI	6
#define BNEZ	7
#define NOP		8

#endif
