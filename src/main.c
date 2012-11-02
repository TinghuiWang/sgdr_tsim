/*
 * main.c -- Main for sgdr_tsim
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

 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"

#define IS_FLAG(flag)  \
	strcmp(argv[i], flag)

void
printUsage() {
	printf("sgdr_tsim -S <.S file> -m <.m file> -r <.r file> [-xgs]\n");
	printf("sgdr_tsim -h\n\n");
}

int 
main(int argc, char** argv) {
	
	int i;
	int fGui = 0;
	int fDebug = 0;
	int fStep = 0;
	int status = 0;
	char *pchInAsmFile = NULL;
	char *pchInitMemFile = NULL;
	char *pchInitRegFile = NULL;
	FILE *fpInAsm = NULL;


	if(argc == 1) {
		printUsage();
	} 
	/* Parse options
	 * -x: start GUI
	 * -g:   Debugging Mode
     * -s:   Step Mode
	 */
	for (i = 1; i < argc; i++) {
		if(argv[i][0] == '-') {
			switch(argv[i][1]) {
				case 'x':
					fGui = 1;
					break;
				case 'g':
					fDebug = 1;
					break;
				case 's':
					fStep = 1;
					break;
				case 'r':
					i++;
					pchInitRegFile = argv[i];
					break;
				case 'm':
					i++;
					pchInitMemFile = argv[i];
					break;
				case 'S':
					i++;
					pchInAsmFile = argv[i];
					break;
				case 'h':
					printUsage();
					return 0;
				default:
					printf("error: Unknown Option: %s\n", argv[i]);
					return -1;
			}
		} else {
			printf("error: Unknown Option: %s\n", argv[i]);
			return -1;
		}
	}

	fpInAsm = fopen(pchInAsmFile, "r");

	// Symbol Table should be created ahead of fetch instructions
	// Syntax Checking on the fly
	// PARSER_CreateSymbolTable();

	//Simulate();
}
