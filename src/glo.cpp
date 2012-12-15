#include "config.h"
#include "init.h"
#include "fu.h"
#include "rob.h"

// Architecture Register File
int_reg_entry rgiReg[I_REG_MAX * NR_THREAD];
fp_reg_entry rgfReg[FP_REG_MAX * NR_THREAD];

// Re-Order Buffer
ROB_TABLE rob_tab[NR_THREAD];

// Input file
FILE *fpInAsm;
FILE *fpOutResult;
// Function Units
LOAD_UNIT load_unit;
STORE_UNIT store_unit;
INT_UNIT int_unit;
FP_ADD_UNIT fp_add_unit;
FP_MULT_UNIT fp_mult_unit;

// Temporary Variables
int fSpeculate[NR_THREAD];
int fEOP[NR_THREAD];
int PC[NR_THREAD];
int branch;
void* pExUnitHeader;
void* pROB;
int write_result_counter;
