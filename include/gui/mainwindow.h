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

    QAction *startAction;
    QAction *stopAction;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  1-Hz Timer                                                                                                             *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    QTimer *mainTimer;

    /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
     *  Data structures for management examples                                                                                *
     * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
    MemoryData *memoryData;
    RegisterFileData *registerFileData;
    ReorderBufferData *reorderBufferData;
    ReservationStationsData *reservationStationsData;

public:
    explicit MainWindow(QWidget *parent = 0);
    
signals:

private slots:
    void cycleProcess();    // Main data processing method - will execute all operations belonging to the same clock cycle
    void openFile();        // Open ASM code file method
};

#endif // MAINWINDOW_H
