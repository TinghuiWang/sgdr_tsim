#include "init.h"
#include "config.h"

int utGetUnitTypeForInstr(inst_entry *inst) {
	switch(inst->iOpcode) {
	case OP_L_D:
		return UNIT_LOAD;
	case OP_MUL_D:
		return UNIT_FP_MULT;
	case OP_ADD_D:
		return UNIT_FP_ADD;
	case OP_S_D:
		return UNIT_STORE;
	case OP_SUB_D:
		return UNIT_FP_ADD;
	case OP_DIV_D:
		return UNIT_FP_MULT;
	case OP_ADDI:
		return UNIT_INT;
	case OP_SUBI:
		return UNIT_INT;
	case OP_BNEZ:
		return UNIT_INT;
	case OP_SLTI:
		return UNIT_INT;
	case OP_EOP:
		return UNIT_EOP;
	default:
		printf("UnitType: Cannot handle Opcode %d\n", inst->iOpcode);
		return -1;
	}
}

char * getfState(int fState)
{
	switch(fState) {
	case ISSUED: 
		return "    ISSUED";
	case EXECUTE: 
		return "   EXECUTE";
	case WRITE_RES: 
		return "WRITE BACK";
	case COMMIT: 
		return "    COMMIT";
	case WAITING: 
		return "   WAITING";
	default: 
		return "      FREE";
    }
}
