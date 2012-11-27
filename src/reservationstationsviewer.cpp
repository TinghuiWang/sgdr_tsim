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
 *  File:                   reservationstationsviewer.cpp                                                                      *
 *  Description:            Reservation Stations monitor class description                                                     *
 *  Last modification date: 2012/11/23                                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "reservationstationsviewer.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *      ReservationStationsViewer Class Constructor                                                                            *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
ReservationStationsViewer::ReservationStationsViewer(QWidget *parent) :
    QTableWidget(parent)
{
    setRowCount(FPUnits + FXUnits + LSUnits + BRUnits);     // Number of rows given by processing units
    setColumnCount(Fields);                                 // 7 column fields

    for (int i = 0; i < columnCount(); ++i) {   // Column name assignment loop
        QTableWidgetItem *itemData = new QTableWidgetItem;
        switch (i) {
        case 0:
            itemData->setText(QString("Busy"));
            break;
        case 1:
            itemData->setText(QString("Op"));
            break;
        case 2:
            itemData->setText(QString("Vj"));
            break;
        case 3:
            itemData->setText(QString("Vk"));
            break;
        case 4:
            itemData->setText(QString("Qj"));
            break;
        case 5:
            itemData->setText(QString("Qk"));
            break;
        case 6:
            itemData->setText(QString("Destination"));
            break;
        default:
            break;
        }
        setHorizontalHeaderItem(i, itemData);
        horizontalHeaderItem(i)->setTextAlignment(Qt::AlignCenter);
    }

    for (int i = 0; i < FPUnits; ++i) {     // Floating point unit name assignment loop
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString("FP%1").arg(i));
        setVerticalHeaderItem(i, item);
        verticalHeaderItem(i)->setTextAlignment(Qt::AlignCenter);
    }
    for (int i = 0; i < FXUnits; ++i) {     // Fixed point unit name assignment loop
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString("FX%1").arg(i));
        setVerticalHeaderItem(i + FPUnits, item);
        verticalHeaderItem(i)->setTextAlignment(Qt::AlignCenter);
    }
    for (int i = 0; i < LSUnits; ++i) {     // Load/store unit name assignment loop
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString("LS%1").arg(i));
        setVerticalHeaderItem(i + FPUnits + FXUnits, item);
        verticalHeaderItem(i)->setTextAlignment(Qt::AlignCenter);
    }
    for (int i = 0; i < BRUnits; ++i) {     // Branch target unit name assignment loop
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(QString("BR%1").arg(i));
        setVerticalHeaderItem(i + FPUnits + FXUnits + LSUnits, item);
        verticalHeaderItem(i)->setTextAlignment(Qt::AlignCenter);
    }
    for (int i = 0; i < rowCount(); ++i) {  // Data cell initialization double loop
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
 *  Displays all Reservation Stations fields                                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
void ReservationStationsViewer::updateViewer(ReservationStationsData *reservationStationsData)
{
    ReservationStationsData *reservationStationsDataIndex = reservationStationsData;    // Data structure array index
    for (int i = 0; i < rowCount(); ++i) {          // Update iteration loop
        if (reservationStationsDataIndex->busy)
            item(i, 0)->setText(QString("Y"));
        else
            item(i, 0)->setText(QString("N"));
        switch (reservationStationsDataIndex->op) {     // Employing instrType for limited opcode name selection
        case ADD:
            item(i, 1)->setText(QString("ADD"));
            break;
        case SUB:
            item(i, 1)->setText(QString("SUB"));
            break;
        case MUL:
            item(i, 1)->setText(QString("MUL"));
            break;
        case DIV:
            item(i, 1)->setText(QString("DIV"));
            break;
        default:
            break;
        }
        item(i, 2)->setText(QString("%1").arg(reservationStationsDataIndex->vj, 8, 16, QChar('0')).toUpper());  // Hex 8-digit
        item(i, 3)->setText(QString("%1").arg(reservationStationsDataIndex->vk, 8, 16, QChar('0')).toUpper());  // Hex 8-digit
        item(i, 4)->setText(QString("%1").arg(reservationStationsDataIndex->qj));                               // String
        item(i, 5)->setText(QString("%1").arg(reservationStationsDataIndex->qk));                               // String
        item(i, 6)->setText(QString("%1").arg(reservationStationsDataIndex->dest));                             // String

        ++reservationStationsDataIndex;
    }
}
