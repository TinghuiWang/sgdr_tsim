/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              EE524 - Advanced Computer Architecture                                                         *
 *                                  2012 Fall - Final Project                                                                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      Intel Core i7 Group:                                                                                                   *
 *      - Daniel Iparraguirre                                                                                                  *
 *      - Ryan Kim                                                                                                             *
 *      - Gina Sprint                                                                                                          *
 *      - Steve Wang                                                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      Graphic User Interface / Execution Loop                                                                                *
 *  File:                   typedef.h                                                                                          *
 *  Description:            enum/struc types to model the interface between data structures and data viewers                   *
 *  Last modification date: 2012/11/23                                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef TYPEDEF_H
#define TYPEDEF_H

/*  instrType: enum type used to model the instruction opcode list for random assignment and visualization                     */
enum instrType {
    ADD = 0,
    SUB = 1,
    MUL = 2,
    DIV = 3
};

/*  MemoryData: struc type used to model the interface between memory data structure and GUI viewer                            */
typedef struct {
    int address;
    int value;
} MemoryData;

/*  RegisterFileData: struc type used to model the interface between register array data structure and GUI viewer              */
typedef struct {
    int dataFloatProc1;
    int dataFixProc1;
    int dataFloatProc2;
    int dataFixProc2;
} RegisterFileData;

/*  ReorderBufferData: struc type used to model the interface between ROB data structure and GUI viewer                        */
typedef struct {
    instrType instruction;
    char *destination;
    int value;
    bool done;
} ReorderBufferData;

/*  ReservationStationsData: struc type used to model the interface between reservation stations data structure and GUI viewer */
typedef struct {
    bool busy;
    instrType op;
    int vj;
    int vk;
    char *qj;
    char *qk;
    char *dest;
} ReservationStationsData;

/*
typedef struct inst_entry
{
    int iOpcode;
    int rgiOperand[3];
    int ploc;
} inst_entry;
*/

#endif // TYPEDEF_H
