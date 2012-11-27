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
 *  File:                   memoryviewer.cpp                                                                                   *
 *  Description:            Memory viewer class description                                                                    *
 *  Last modification date: 2012/11/23                                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "memoryviewer.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      MemoryViewer Class Constructor                                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
MemoryViewer::MemoryViewer(QWidget *parent) :
    QTableWidget(parent)
{
    setColumnCount(Fields);  // Column 1: Address; Column 2: Data

    QTableWidgetItem *item_1 = new QTableWidgetItem;    // Assigning title to Address column
    item_1->setText(QString("Address"));
    setHorizontalHeaderItem(0, item_1);

    QTableWidgetItem *item_2 = new QTableWidgetItem;    // Assigning title to Data column
    item_2->setText(QString("Data"));
    setHorizontalHeaderItem(1, item_2);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      Viewer Update Method                                                                                                   *
 *  Displays Address and Data fields in hexadecimal format - 8 digits                                                          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void MemoryViewer::updateViewer(int size, MemoryData *memoryData)
{
    clearContents();    // Erasing all contents since some memory locations may have been cleared or dismissed
    setRowCount(size);  // Updating amount of rows to show
    for (int i = 0; i < rowCount(); ++i) {
        if (!item(i, 0)) {  // A new QTableWidgetItem object pair (address & value) is created if the row wasn't used before
            QTableWidgetItem *newItem_0 = new QTableWidgetItem;
            QTableWidgetItem *newItem_1 = new QTableWidgetItem;
            newItem_0->setText(QString("%1").arg(memoryData->address, 8, 16, QChar('0')).toUpper());    // Applying hex format
            newItem_1->setText(QString("%1").arg(memoryData->value, 8, 16, QChar('0')).toUpper());      // Applying hex format
            newItem_0->setTextAlignment(Qt::AlignCenter);   // Center aligned
            newItem_1->setTextAlignment(Qt::AlignCenter);   // Center aligned
            setItem(i, 0, newItem_0);
            setItem(i, 1, newItem_1);
        } else {    // Only values are updated if the row was already initialized
            item(i, 0)->setText(QString("%1").arg(memoryData->address, 8, 16, QChar('0')).toUpper());
            item(i, 1)->setText(QString("%1").arg(memoryData->value, 8, 16, QChar('0')).toUpper());
        }
        ++memoryData;
    }
}

