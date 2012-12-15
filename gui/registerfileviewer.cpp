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
 *  File:                   registerfileviewer.cpp                                                                             *
 *  Description:            Register bank monitor class description                                                            *
 *  Last modification date: 2012/11/23                                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "registerfileviewer.h"

#include "glo.h"
#include "rob.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      RegisterFileViewer Class Constructor                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
RegisterFileViewer::RegisterFileViewer(QWidget *parent) :
    QTableWidget(parent)
{
    setRowCount(RegisterCount);         // RegisterCount tracks number of registers assigned to one processor, for 1 class
    setColumnCount(2 * ProcessorCount); // 2 types (floating point and integer) per processor

    for (int i = 0; i < ProcessorCount; ++i) {      // Horizontal headers initializer loop
        QTableWidgetItem *item_h1 = new QTableWidgetItem;
        item_h1->setText(QString("F%1").arg(RegisterCount * i));    // Floating point registers with respective index base
        setHorizontalHeaderItem(2*i, item_h1);

        QTableWidgetItem *item_h2 = new QTableWidgetItem;
        item_h2->setText(QString("R%1").arg(RegisterCount * i));    // Integer registers with respective index base
        setHorizontalHeaderItem(2*i + 1, item_h2);
    }

    for (int i = 0; i < RegisterCount; ++i) {       // Vertical headers assignment loop
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString("%1").arg(i));            // Indexing applied to every processor and register type
        setVerticalHeaderItem(i, item);
    }

    for (int i = 0; i < 2*ProcessorCount; ++i) {    // Data cell initializer double loop
        for (int j = 0; j < RegisterCount; ++j) {
            QTableWidgetItem *itemData = new QTableWidgetItem;
            setItem(j, i, itemData);
            item(j, i)->setText(QString("%1").arg(0, 8, 16, QChar('0')).toUpper()); // Assigning "00000000" to every field
            item(j, i)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      Viewer Update Method                                                                                                   *
 *  Displays stored value fields in hexadecimal format - 8 digits                                                              *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void RegisterFileViewer::updateViewer2()
{
    for (int i = 0; i < I_REG_MAX; ++i)
        item(i, 0)->setText(QString("%1").arg(rgiReg[i].value, 8, 16, QChar('0')).toUpper()); // Int
    for (int i = 0; i < FP_REG_MAX; ++i)
        item(i, 1)->setText(QString("%1").arg(rgfReg[i].value)); // Float
}
