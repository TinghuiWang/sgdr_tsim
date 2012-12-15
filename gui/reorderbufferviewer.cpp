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
 *  File:                   reorderbufferviewer.cpp                                                                            *
 *  Description:            Reorder Buffer monitor class description                                                           *
 *  Last modification date: 2012/11/23                                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "reorderbufferviewer.h"

#include "glo.h"
#include "rob.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      ReorderBufferViewer Class Constructor                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ReorderBufferViewer::ReorderBufferViewer(QWidget *parent) :
    QTableWidget(parent)
{
    setRowCount(Registers);
    setColumnCount(Fields);                     // 4 fields
    setWindowTitle(QString("Reorder Buffer"));

    for (int i = 0; i < columnCount(); ++i) {       // Field name assignment loop
        QTableWidgetItem *itemColHeader = new QTableWidgetItem;
        switch (i) {
        case 0:
            itemColHeader->setText(QString("Instruction"));
            break;
        case 1:
            itemColHeader->setText(QString("Destination"));
            break;
        case 2:
            itemColHeader->setText(QString("Write Value"));
            break;
        case 3:
            itemColHeader->setText(QString("Done"));
            break;
        default:
            break;
        }
        setHorizontalHeaderItem(i, itemColHeader);
        horizontalHeaderItem(i)->setTextAlignment(Qt::AlignCenter);
    }

    for (int i = 0; i < rowCount(); ++i) {      // ROB register name assigment loop
        QTableWidgetItem *itemRowHeader = new QTableWidgetItem;
        itemRowHeader->setText(QString("ROB%1").arg(i));
        setVerticalHeaderItem(i, itemRowHeader);
        verticalHeaderItem(i)->setTextAlignment(Qt::AlignCenter);
    }

    for (int i = 0; i < rowCount(); ++i) {      // Data cell initialization double loop
        for (int j = 0; j < columnCount(); ++j) {
            QTableWidgetItem *itemData = new QTableWidgetItem;
            itemData->setText(QString(" "));
            setItem(i, j, itemData);
            item(i, j)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      Viewer Update Method                                                                                                   *
 *  Displays ROB table fields                                                                                                  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ReorderBufferViewer::updateViewer2()
{
    for (int i = 0; i < NR_THREAD; ++i) {
        for (int j = 0; j < NR_ROB_ENT; ++j) {
//            item(NR_ROB_ENT * i + j, 0)->setText(QString("%1").arg(rob_tab[i].arROB[j].pInst->iOpcode));
            item(NR_ROB_ENT * i + j, 0)->setText(QString("%1").arg(123));
//            item(NR_ROB_ENT * i + j, 2)->setText(QString("%1").arg(rob_tab[i].arROB[j].iRegValue));
        }
    }
}
