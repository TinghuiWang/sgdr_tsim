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
 *  File:                   reorderbufferviewer.h                                                                              *
 *  Description:            Reorder Buffer monitor class description                                                           *
 *  Last modification date: 2012/11/23                                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef REORDERBUFFERVIEWER_H
#define REORDERBUFFERVIEWER_H

#include <QTableWidget>
#include "typedef.h"
#include "config.h"

class ReorderBufferViewer : public QTableWidget
{
    Q_OBJECT
private:
    enum { Registers = NR_THREAD * NR_ROB_ENT, Fields = 4 };

public:
    explicit ReorderBufferViewer(QWidget *parent = 0);  // Class constructor

signals:
    
public slots:
    void updateViewer2();
};

#endif // REORDERBUFFERVIEWER_H
