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
void ReorderBufferViewer::updateViewer(ReorderBufferData *reorderBufferData)
{
    ReorderBufferData *reorderBufferDataIndex = reorderBufferData;  // ROB data structure array index
    for (int i = 0; i < rowCount(); ++i) {  // Update iteration loop
        switch (reorderBufferDataIndex->instruction) {  // Instruction opcode update employs instrType to randomly select
        case ADD:                                       //  between a limited amount of opcode strings
            item(i, 0)->setText(QString("ADD"));
            break;
        case SUB:
            item(i, 0)->setText(QString("SUB"));
            break;
        case MUL:
            item(i, 0)->setText(QString("MUL"));
            break;
        case DIV:
            item(i, 0)->setText(QString("DIV"));
            break;
        default:
            break;
        }

        item(i, 1)->setText(QString("%1").arg(reorderBufferDataIndex->destination));    // Destination register name update

        item(i, 2)->setText(QString("%1").arg(reorderBufferDataIndex->value, 8, 16, QChar('0')).toUpper()); // Write value

        if (reorderBufferDataIndex->done)       // Boolean done flag update
            item(i, 3)->setText(QString("Y"));
        else
            item(i, 3)->setText(QString("N"));

        ++reorderBufferDataIndex;
    }
}
