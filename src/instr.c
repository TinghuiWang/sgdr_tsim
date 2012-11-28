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
	default:
		printf("UnitType: Cannot handle Opcode %d\n", inst->iOpcode);
		return -1;
	}
}
