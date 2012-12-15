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
void ReservationStationsViewer::updateViewer2()
{
    int baseIndex = 0;
    for (int i = baseIndex; i < baseIndex + LOAD_RS; ++i) {
        item(baseIndex + i, 0)->setText(QString("%1").arg(QChar(load_unit.rs[i].busy)));
        item(baseIndex + i, 1)->setText(displayOpCode(load_unit.rs[i].iOpcode));
        item(baseIndex + i, 2)->setText(QString("%1").arg(load_unit.rs[i].reg_vj));
        item(baseIndex + i, 3)->setText(QString("%1").arg(load_unit.rs[i].reg_vk));
        item(baseIndex + i, 4)->setText(QString("%1").arg(load_unit.rs[i].reg_qj->fRegValue));
        item(baseIndex + i, 5)->setText(QString("%1").arg(load_unit.rs[i].reg_qk->fRegValue));
        item(baseIndex + i, 6)->setText(QString("%1").arg(load_unit.rs[i].dest->fRegValue));
    }

    baseIndex = baseIndex + LOAD_RS;

    for (int i = 0; i < STORE_RS; ++i) {
        item(baseIndex + i, 0)->setText(QString("%1").arg(QChar(store_unit.rs[i].busy)));
        item(baseIndex + i, 1)->setText(displayOpCode(store_unit.rs[i].iOpcode));
        item(baseIndex + i, 2)->setText(QString("%1").arg(store_unit.rs[i].reg_vj));
        item(baseIndex + i, 3)->setText(QString("%1").arg(store_unit.rs[i].reg_vk));
        item(baseIndex + i, 4)->setText(QString("%1").arg(store_unit.rs[i].reg_qj->fRegValue));
        item(baseIndex + i, 5)->setText(QString("%1").arg(store_unit.rs[i].reg_qk->fRegValue));
        item(baseIndex + i, 6)->setText(QString("%1").arg(store_unit.rs[i].dest->fRegValue));
    }

    baseIndex = baseIndex + STORE_RS;

    for (int i = 0; i < INTEGER_RS; ++i) {
        item(baseIndex + i, 0)->setText(QString("%1").arg(QChar(int_unit.rs[i].busy)));
        item(baseIndex + i, 1)->setText(displayOpCode(int_unit.rs[i].iOpcode));
        item(baseIndex + i, 2)->setText(QString("%1").arg(int_unit.rs[i].reg_vj));
        item(baseIndex + i, 3)->setText(QString("%1").arg(int_unit.rs[i].reg_vk));
        item(baseIndex + i, 4)->setText(QString("%1").arg(int_unit.rs[i].reg_qj->fRegValue));
        item(baseIndex + i, 5)->setText(QString("%1").arg(int_unit.rs[i].reg_qk->fRegValue));
        item(baseIndex + i, 6)->setText(QString("%1").arg(int_unit.rs[i].dest->fRegValue));
    }

    baseIndex = baseIndex + INTEGER_RS;

    for (int i = 0; i < FP_ADD_RS; ++i) {
        item(baseIndex + i, 0)->setText(QString("%1").arg(QChar(fp_add_unit.rs[i].busy)));
        item(baseIndex + i, 1)->setText(displayOpCode(fp_add_unit.rs[i].iOpcode));
        item(baseIndex + i, 2)->setText(QString("%1").arg(fp_add_unit.rs[i].reg_vj));
        item(baseIndex + i, 3)->setText(QString("%1").arg(fp_add_unit.rs[i].reg_vk));
        item(baseIndex + i, 4)->setText(QString("%1").arg(fp_add_unit.rs[i].reg_qj->fRegValue));
        item(baseIndex + i, 5)->setText(QString("%1").arg(fp_add_unit.rs[i].reg_qk->fRegValue));
        item(baseIndex + i, 6)->setText(QString("%1").arg(fp_add_unit.rs[i].dest->fRegValue));
    }

    baseIndex = baseIndex + FP_ADD_RS;

    for (int i = 0; i < FP_MULT_RS; ++i) {
        item(baseIndex + i, 0)->setText(QString("%1").arg(QChar(fp_mult_unit.rs[i].busy)));
        item(baseIndex + i, 1)->setText(displayOpCode(fp_mult_unit.rs[i].iOpcode));
        item(baseIndex + i, 2)->setText(QString("%1").arg(fp_mult_unit.rs[i].reg_vj));
        item(baseIndex + i, 3)->setText(QString("%1").arg(fp_mult_unit.rs[i].reg_vk));
        item(baseIndex + i, 4)->setText(QString("%1").arg(fp_mult_unit.rs[i].reg_qj->fRegValue));
        item(baseIndex + i, 5)->setText(QString("%1").arg(fp_mult_unit.rs[i].reg_qk->fRegValue));
        item(baseIndex + i, 6)->setText(QString("%1").arg(fp_mult_unit.rs[i].dest->fRegValue));
    }
}

QString ReservationStationsViewer::displayOpCode(int iOpcode)
{
    switch (iOpcode) {     // Employing instrType for limited opcode name selection
    case 0:
        return QString("ADD");
        break;
    case 1:
        return QString("SUB");
        break;
    case 2:
        return QString("MUL");
        break;
    case 3:
        return QString("DIV");
        break;
    default:
        return QString(" ");
        break;
    }
}
