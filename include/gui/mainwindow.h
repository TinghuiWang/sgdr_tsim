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
 *  File:                   mainwindow.h                                                                                       *
 *  Description:            Main window description containing all GUI viewers and                                             *
 *                          menu commands                                                                                      *
 *  Last modification date: 2012/11/23                                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "typedef.h"

#include "config.h"

#include "glo.h"
#include "fu.h"
#include "init.h"
#include "rob.h"

#define IS_FLAG(flag) strcmp(argv[i], flag)

class ReorderBufferViewer;          // Class described in reorderbufferviewer.h - reorderbufferviewer.cpp
class ReservationStationsViewer;    // Class described in reservationstationsviewer.h - reservationstationsviewer.cpp
class RegisterFileViewer;           // Class described in registerfileviewer.h - registerfileviewer.cpp
class MemoryViewer;                 // Class described in memoryviewer.h - memoryviewer.cpp

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  Data table viewers                                                                                                     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    ReorderBufferViewer *reorderBufferViewer;
    ReservationStationsViewer *reservationStationViewer;
    RegisterFileViewer *registerFileViewer;
    MemoryViewer *memoryViewer;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  Menus / action items                                                                                                   *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    QMenu *fileMenu;
    QMenu *processingMenu;

    QAction *openAction;
    QAction *openAsmAction;
    QAction *openFloatRegAction;
    QAction *openIntRegAction;

    QAction *startAction;
    QAction *stopAction;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  1-Hz Timer                                                                                                             *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    QTimer *mainTimer;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  Data structures for real program                                                                                       *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//    int write_result_counter;

//    int PC[NR_THREAD];
//    int fEOP[NR_THREAD];
    // Architecture Register File
//    int_reg_entry rgiReg[I_REG_MAX * NR_THREAD];
//    fp_reg_entry rgfReg[FP_REG_MAX * NR_THREAD];

    // Function Units
//    LOAD_UNIT load_unit;
//    STORE_UNIT store_unit;
//    INT_UNIT int_unit;
//    FP_ADD_UNIT fp_add_unit;
//    FP_MULT_UNIT fp_mult_unit;

    // Re-Order Buffer
//    ROB_TABLE rob_tab[NR_THREAD];

    // Global Flag
//    int fSpeculate[NR_THREAD];

    // Input file
//    FILE *fpInAsm;
//    FILE *fpOutResult;

    char *pchInAsmFile;
    char *pchInitMemFile;
    char *pchInitRegFile;
    FILE *fpInRegFP;
    FILE *fpInRegInt;
    FILE *fpInMem;

    int cycles;

public:
    explicit MainWindow(QWidget *parent = 0);
    
signals:
    void endProgram();

private slots:
    void cycleProcess();        // Main data processing method - will execute all operations belonging to the same clock cycle
    void openAsmFile();         // Open ASM code file method
    void openFloatRegFile();    // Open Flaoting-Point Register file method
    void openIntRegFile();      // Open Integer Register file method
};

#endif // MAINWINDOW_H
