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
    openAction = new QAction(tr("&Open"), this);            // Option not implemented yet: intended to open assembler code
                                                            //  text and run parser
    startAction = new QAction(tr("&Start"), this);          // Start processing: it basically enables the timer
    stopAction = new QAction(tr("&Stop"), this);            // Stop processing: disables the timer

    fileMenu = menuBar()->addMenu(tr("&File"));             // File menu only contains the Open command so far
    fileMenu->addAction(openAction);

    processingMenu = menuBar()->addMenu(tr("&Processing")); // Processing menu contains Start and Stop commands
    processingMenu->addAction(startAction);
    processingMenu->addAction(stopAction);

    mainTimer = new QTimer;                                 // Timer instantiation for 1-Hz operation
    mainTimer->setInterval(1000);
    mainTimer->setSingleShot(false);

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  Signal-slot bindings                                                                                                   *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));       // Open commands shows Open File dialog

    connect(startAction, SIGNAL(triggered()), mainTimer, SLOT(start()));    // Start command triggers timer operation
    connect(stopAction, SIGNAL(triggered()), mainTimer, SLOT(stop()));      // Stop command stops timer

    connect(mainTimer, SIGNAL(timeout()), this, SLOT(cycleProcess()));      // Timer periodically triggers cycleProcess method

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  Database initialization example                                                                                        *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    memoryData = (MemoryData *)malloc(10000);                           // Memory data structure
    registerFileData = (RegisterFileData *)malloc(10000);               // Register array data structure
    reorderBufferData = (ReorderBufferData *)malloc(10000);             // ROB data structure
    reservationStationsData = (ReservationStationsData *)malloc(10000); // Reservation stations data structure

    srand(time(NULL));  // Random generator initializer for databse management example below
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      cycleProcess Method Description                                                                                        *
 *  This method will virtually allocate all code related to data structures processing during code execution; most code shown  *
 *   below was implemented for GUI functionality demonstration purposes and will be replaced by actual data processing codes.  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MainWindow::cycleProcess()
{
    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *      Memory viewer management example                                                                                   *
     *  The procedure fills up one new memory location every time cycleProcess is invoked.                                     *
     *  The memorySize index keeps track of the amount of filled memory locations.                                             *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    static int memorySize = 0;                          // Memory size index
    static MemoryData *memoryDataPoint = memoryData;    // Memory data structure index

    memoryDataPoint->address = rand();  // Filling a random address location with
    memoryDataPoint->value = rand();    // a random value

    ++memoryDataPoint;  // Next cycleProcess invokation will point to a new memory location
    ++memorySize;       //  and size increases by 1

    memoryViewer->updateViewer(memorySize, memoryData); // updateViewer method receives data/size to display at the end

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *      Register file viewer management example                                                                            *
     *  Random update of all register values at every cycle                                                                    *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    RegisterFileData *registerFileDataIndex = registerFileData; // Register array data structure

    for (int i = 0; i < 32; ++i) {                      // Register iteration loop
        registerFileDataIndex->dataFloatProc1 = rand();
        registerFileDataIndex->dataFixProc1 = rand();
        registerFileDataIndex->dataFloatProc2 = rand();
        registerFileDataIndex->dataFixProc2 = rand();
        ++registerFileDataIndex;
    }

    registerFileViewer->updateViewer(registerFileData); // updateViewer method receives data to display at the end

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *      Reorder buffer viewer management example                                                                           *
     *  Random update of all ROB fields at every cycle                                                                         *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    ReorderBufferData *reorderBufferDataIndex = reorderBufferData;  // ROB data structure index
    char robStr[10][10];                                            // Auxilliary array for random register name generation

    for (int i = 0; i < 10; ++i) {                                  // ROB update iteration loop
        reorderBufferDataIndex->instruction = static_cast<instrType>(rand() % 4);   // Instruction opcode random selection
        if (rand() % 2) {                                                           // Register name random generation
            sprintf(robStr[i], "R%d", rand() % 64);
            reorderBufferDataIndex->destination = robStr[i];
        } else {
            sprintf(robStr[i], "F%d", rand() % 64);
            reorderBufferDataIndex->destination = robStr[i];
        }
        reorderBufferDataIndex->value = rand();                                     // Register value random generation
        reorderBufferDataIndex->done = static_cast<bool>(rand() % 2);               // Done flag random selection
        ++reorderBufferDataIndex;
    }

    reorderBufferViewer->updateViewer(reorderBufferData);   // updateViewer method receiver generated data structure pointer

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  Reservation stations viewer management example                                                                         *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    ReservationStationsData *reservationStationsDataIndex = reservationStationsData;    // Reservation station structure index
    char rsvStr[8][3][10];                                                              // Auxilliary array for station names

    for (int i = 0; i < 8; ++i) {                               // Reservation station update iteration loop
        reservationStationsDataIndex->busy = static_cast<bool>(rand() % 2);     // Busy flag random selection
        reservationStationsDataIndex->op = static_cast<instrType>(rand() % 4);  // Instruction opcode random selection
        reservationStationsDataIndex->vj = rand();                              // Vj stored value random selection
        reservationStationsDataIndex->vk = rand();                              // Vk stored value random selection
        sprintf(rsvStr[i][0], "ROB%d", rand() % 10);                            // Qj buffer random name generation
        reservationStationsDataIndex->qj = rsvStr[i][0];
        sprintf(rsvStr[i][1], "ROB%d", rand() % 10);                            // Qk buffer random name generation
        reservationStationsDataIndex->qk = rsvStr[i][1];
        if (rand() % 2) {                                                       // Destination register random name generation
            sprintf(rsvStr[i][2], "R%d", rand() % 64);
            reservationStationsDataIndex->dest = rsvStr[i][2];
        } else {
            sprintf(rsvStr[i][2], "F%d", rand() % 64);
            reservationStationsDataIndex->dest = rsvStr[i][2];
        }
        ++reservationStationsDataIndex;
    }

    reservationStationViewer->updateViewer(reservationStationsData);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      openFile Method Description                                                                                            *
 *  This method implement the open file dialog window and reading process; no final char array receiver is declared yet.       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open ASM Code File"), "", tr("ASM Code (*.asm);;All files (*)"));
    QString line;

    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::information(this, tr("Unable to open file"), file.errorString());
            return;
        }

        QTextStream in(&file);

        do {
            line = in.readLine();
        } while (!line.isNull());
    }

    QFileDialog dialog(this);
    QStringList fileNames;

    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(tr("Assembler code files (*.asm)"));

    if (dialog.exec()) {
        fileNames = dialog.selectedFiles();
    }
}
