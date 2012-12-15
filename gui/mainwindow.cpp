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
 *  File:                   mainwindow.cpp                                                                                     *
 *  Description:            Main window description containing all GUI viewers and                                             *
 *                          menu commands                                                                                      *
 *  Last modification date: 2012/11/23                                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <QtGui>
#include <QTimer>
#include "typedef.h"
#include "mainwindow.h"
#include "reorderbufferviewer.h"
#include "reservationstationsviewer.h"
#include "registerfileviewer.h"
#include "memoryviewer.h"

#include "config.h"
#include "fu.h"
#include "fu_p.h"
#include "init.h"
#include "rob.h"
#include "rob_p.h"
#include "stdio.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      MainWindow Class Constructor                                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  Main window layout including creation / allocation of data viewers                                                     *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    reorderBufferViewer = new ReorderBufferViewer;
    reservationStationViewer = new ReservationStationsViewer;
    registerFileViewer = new RegisterFileViewer;
    memoryViewer = new MemoryViewer;

    setCentralWidget(reorderBufferViewer);              // ROB Viewer is the central table

    QDockWidget *rsvWidget = new QDockWidget;
    rsvWidget->setWidget(reservationStationViewer);     // Reservation stations viewer is a docking table
    rsvWidget->setWindowTitle(QString("Reservation Stations"));

    QDockWidget *rfvWidget = new QDockWidget;
    rfvWidget->setWidget(registerFileViewer);           // Register array viewer is a docking table
    rfvWidget->setWindowTitle(QString("Register File"));
    rfvWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Maximum);

    QDockWidget *mvWidget = new QDockWidget;
    mvWidget->setWidget(memoryViewer);                  // Memory viewer is a docking table
    mvWidget->setWindowTitle(QString("Memory"));

    addDockWidget(Qt::BottomDockWidgetArea, rsvWidget); // Reservation stations monitor is at the bottom
    addDockWidget(Qt::LeftDockWidgetArea, rfvWidget);   // Register array monitor is at the left
    addDockWidget(Qt::RightDockWidgetArea, mvWidget);   // Memory monitor is at the right

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  Main menu description                                                                                                  *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    openAsmAction = new QAction(tr("Open ASM File..."), this);
    openFloatRegAction = new QAction(tr("Open FP Register File..."), this);
    openIntRegAction = new QAction(tr("Open Integer Register File..."), this);
    startAction = new QAction(tr("&Start"), this);          // Start processing: it basically enables the timer
    stopAction = new QAction(tr("&Stop"), this);            // Stop processing: disables the timer

    fileMenu = menuBar()->addMenu(tr("&File"));             // File menu only contains the Open command so far
    fileMenu->addAction(openAsmAction);
    fileMenu->addAction(openFloatRegAction);
    fileMenu->addAction(openIntRegAction);

    processingMenu = menuBar()->addMenu(tr("&Processing")); // Processing menu contains Start and Stop commands
    processingMenu->addAction(startAction);
    processingMenu->addAction(stopAction);

    mainTimer = new QTimer;                                 // Timer instantiation for 1-Hz operation
    mainTimer->setInterval(1000);
    mainTimer->setSingleShot(false);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  Signal-slot bindings                                                                                                   *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    connect(openAsmAction, SIGNAL(triggered()), this, SLOT(openAsmFile()));       // Open commands shows Open File dialog
    connect(openFloatRegAction, SIGNAL(triggered()), this, SLOT(openFloatRegFile()));       // Open commands shows Open File dialog
    connect(openIntRegAction, SIGNAL(triggered()), this, SLOT(openIntRegFile()));       // Open commands shows Open File dialog

    connect(startAction, SIGNAL(triggered()), mainTimer, SLOT(start()));    // Start command triggers timer operation
    connect(stopAction, SIGNAL(triggered()), mainTimer, SLOT(stop()));      // Stop command stops timer
    connect(this, SIGNAL(endProgram()), mainTimer, SLOT(stop()));      // Stop command stops timer

    connect(mainTimer, SIGNAL(timeout()), this, SLOT(cycleProcess()));      // Timer periodically triggers cycleProcess method

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  Real program initialization                                                                                            *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    write_result_counter = 0;

    for (int i = 0; i < I_REG_MAX; i++) {
        rgiReg[i].index = i;
    }
    for (int i = 0; i < FP_REG_MAX; i++) {
        rgfReg[i].index = i;
    }

    for (int i = 0; i < NR_THREAD; i++) {
        PC[i] = 0;
        fSpeculate[i] = 0;
        fEOP[i] = 0;
    }

    init_fu();          // zeros out data struct
    ROB_Init(rob_tab);  // zeros out data struct

    cycles = 1;

    PC[0] = PC0_INIT_VAL;
    PC[1] = PC1_INIT_VAL;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      cycleProcess Method Description                                                                                        *
 *  This method will virtually allocate all code related to data structures processing during code execution; most code shown  *
 *   below was implemented for GUI functionality demonstration purposes and will be replaced by actual data processing codes.  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MainWindow::cycleProcess()
{
    //printf("\n\n**************************** CYCLE=%d | PC=%d ****************************\n\n", cycles, PC);

    // Update Reservation Station
    update_rs();
    // Update ReOrder Buffer
    update_rob(fpOutResult);
    ROB_Issue(NR_INSTR_ISSUE, fpOutResult);
    // Clear Temporary Flgas
    clear_flags();
    // Print Debug Message
    print_reg_status();
    ROB_print(rob_tab);
    print_rs_status();

    registerFileViewer->updateViewer2();
    reorderBufferViewer->updateViewer2();
    reservationStationViewer->updateViewer2();

    // Move on to Next Cycle
    ++cycles;
    //getc(stdin);
    if (fEOP[0] == 1 && fEOP[1] == 1)
        emit endProgram();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      openFile Method Description                                                                                            *
 *  This method implement the open file dialog window and reading process; no final char array receiver is declared yet.       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MainWindow::openAsmFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open ASM Code File"), "", tr("ASM Code (*.asm);;All files (*)"));

    if (fileName.isEmpty())
        return;
    else {
        fpInAsm = fopen(fileName.toStdString().c_str(), "r");
        get_memory_locations(fpInAsm);
        fpOutResult = init_outfile(fpInAsm);
    }
}

void MainWindow::openFloatRegFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Floating Point Register File"), "", tr("Text Files (*.txt);;All files (*)"));

    if (fileName.isEmpty())
        return;
    else {
        fpInRegFP = fopen(fileName.toStdString().c_str(), "r");
    }

    if (fpInRegFP != NULL && fpInRegInt != NULL) {
        init_registers(fpInRegFP, fpInRegInt);
    }
}

void MainWindow::openIntRegFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Integer Register File"), "", tr("Text Files (*.txt);;All files (*)"));

    if (fileName.isEmpty())
        return;
    else {
        fpInRegInt = fopen(fileName.toStdString().c_str(), "r");
    }

    if (fpInRegFP != NULL && fpInRegInt != NULL) {
        init_registers(fpInRegFP, fpInRegInt);
    }
}
