#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "config.h"

#define IS_FLAG(flag)  \
	strcmp(argv[i], flag)

int 
main(int argc, char** argv) {
	
	int i;
	int fGui = 0;
	int fDebug = 0;
	int fStep = 0;
	int status = 0;
	char *pchInAsmFile = NULL;
	FILE *fpInAsm = NULL;
 
	/* Parse options
	 * -x: start GUI
	 * -g:   Debugging Mode
     * -s:   Step Mode
	 */
	for (i = 0; i < argc; i++) {
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
				default:
					printf("error: Unknown Option: %s\n", argv[i]);
					return -1;
			}
		} else {
			pchInAsmFile = argv[i];
		}
	}

	fpInAsm = fopen(pchInAsmFile, "r");

	// Symbol Table should be created ahead of fetch instructions
	// Syntax Checking on the fly
	// PARSER_CreateSymbolTable();

	//Simulate();
}
