
#include "init.h"

typedef struct
{
	char szLabel[25];
	int iAddr;
} sym_entry;

sym_entry *rgSymbol;
int iSymTableMax = 20;
int iSymTableCur = 0;

long int *rgliMemLocation;
int iMemMax = 100;
int iMemCur = 0;

int_reg_entry rgiReg[I_REG_MAX];
fp_reg_entry rgfReg[FP_REG_MAX];

void get_memory_locations(FILE *fpAsm)
{
	char szInstruction[50];
	char szTemp[25];
	char *pszTemp;
	char * pEnd;
	long int liPos;
	long int liMem;
	int i = 0;
	rgliMemLocation = (long int *) calloc(iMemMax, sizeof(long int));
	rgSymbol= (sym_entry *) calloc(iSymTableMax, sizeof(sym_entry));
	while(!feof(fpAsm))
	{
		liPos = ftell(fpAsm);
		fgets(szInstruction, 50, fpAsm);
		pszTemp = strtok(szInstruction, " :\n\t,)");
		strcpy(szTemp, pszTemp);
		liMem = strtol(szTemp, &pEnd, 10);
		if(szTemp == pEnd)
		{
			strcpy(rgSymbol[iSymTableCur].szLabel, szTemp);
			pszTemp = strtok(NULL, " :\n,)");
			liPos += (strlen(szTemp) + 1);
			liMem = strtol(pszTemp, &pEnd, 10);
			rgSymbol[iSymTableCur].iAddr = liMem;
			iSymTableCur++;
		}
		
		rgliMemLocation[(liMem/4)] = liPos;
		iMemCur++;
	}
}

void init_registers(FILE *fpFReg, FILE *fpIReg)
{
	char szLine[10];
	char *pszReg;
	char *pszVal;
	while(!feof(fpFReg))
	{
		fgets(szLine, 10, fpFReg);
		pszReg = strtok(szLine, " ");
		pszVal = strtok(NULL, " \n");
		pszReg = strtok(pszReg, "F");
		rgfReg[atoi(pszReg)].value = atoi(pszVal);
	}

	while(!feof(fpIReg))
	{
		fgets(szLine, 10, fpIReg);
		pszReg = strtok(szLine, " ");
		pszVal = strtok(NULL, " \n");
		pszReg = strtok(pszReg, "R ");
		rgiReg[atoi(pszReg)].value = atoi(pszVal);
	}
}

inst_entry inst_fetch(int iAddr, FILE *fpAsm)
{
	char szInstruction[50];
	char *pszName;
	char *rgszOperand[3];
	int i = 0;
	char *pszTemp;
	int iSymAddr = 0;
	inst_entry temp;

	fseek(fpAsm,rgliMemLocation[(iAddr/4)], SEEK_SET);
	fgets(szInstruction, 50, fpAsm);
	pszName = strtok(szInstruction, " \n\t,)");
	pszName= strtok(NULL, " \n\t,)");
	for(i = 0; i <3; i++)
	{
		rgszOperand[i] = strtok(NULL, " \n\t,)");
	}

	if(strcmp(pszName, "L.D") == 0)
	{
		temp.iOpcode = OP_L_D;
	}
	else if(strcmp(pszName, "MUL.D") == 0)
	{
		temp.iOpcode = OP_MUL_D;
	}
	else if(strcmp(pszName, "ADD.D") == 0)
	{
		temp.iOpcode = OP_ADD_D;
	}
	else if(strcmp(pszName, "S.D") == 0)
	{
		temp.iOpcode = OP_S_D;
	}
	else if(strcmp(pszName, "SUB.D") == 0)
	{
		temp.iOpcode = OP_SUB_D;
	}
	else if(strcmp(pszName, "ADDI") == 0)
	{
		temp.iOpcode = OP_ADDI;
	}
	else if(strcmp(pszName, "SUBI") == 0)
	{
		temp.iOpcode = OP_SUBI;
	}
	else if(strcmp(pszName, "BNEZ") == 0)
	{
		temp.iOpcode = OP_BNEZ; 
	}
	else if(strcmp(pszName, "NOP") == 0)
	{
		temp.iOpcode = OP_NOP;
	}
	else
	{
		temp.iOpcode = -1;
	}

	if(temp.iOpcode == -1)
	{
		temp.rgiOperand[0] = atoi(pszName);
	}
	else
	{
		if(rgszOperand[0] != NULL)
		{
			if((iSymAddr = symbol_to_addr(rgszOperand[0])) != -1)
				temp.rgiOperand[0] = iSymAddr;
			else
			{
				rgszOperand[0] = strtok(rgszOperand[0], "RF");
				temp.rgiOperand[0] = atoi(rgszOperand[0]);
			}
		}

		if(rgszOperand[1] != NULL)
		{
			if((iSymAddr = symbol_to_addr(rgszOperand[1])) != -1)
				temp.rgiOperand[1] = iSymAddr;
			else
			{
			rgszOperand[1] = strtok(rgszOperand[1], "RF(#");
			temp.rgiOperand[1] = atoi(rgszOperand[1]);
			}
		}

		if((pszTemp = strtok(NULL, "RF(")) != NULL)
		{
			temp.rgiOperand[2] = atoi(pszTemp);
		}
		else if(rgszOperand[2] != NULL)
		{
			if((iSymAddr = symbol_to_addr(rgszOperand[2])) != -1)
				temp.rgiOperand[2] = iSymAddr;
			else
			{
				rgszOperand[2] = strtok(rgszOperand[2], "RF#");
				temp.rgiOperand[2] = atoi(rgszOperand[2]);
			}
		}
	}

	return temp;
}

int symbol_to_addr (char * pszOperand)
{
	int i = 0;

	for (i = 0; i < iSymTableCur; i++)
	{
		if(strcmp(rgSymbol[i].szLabel, pszOperand) == 0)
			return rgSymbol[i].iAddr;
	}
	return -1;
}
