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
 *  File:                   reservationstationsviewer.h                                                                        *
 *  Description:            Reservation Stations monitor class description                                                     *
 *  Last modification date: 2012/11/23                                                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef RESERVATIONSTATIONSVIEWER_H
#define RESERVATIONSTATIONSVIEWER_H

#include <QTableWidget>
#include "typedef.h"

#include "fu.h"
#include "glo.h"

class ReservationStationsViewer : public QTableWidget
{
    Q_OBJECT

private:
    enum { FPUnits = 2, FXUnits = 2, LSUnits = 2, BRUnits = 2, Fields = 7 };    // 2 floating-point units, 2 fixed-point units
                                                                                //  2 load-store units, 2 branch-target units
    QString displayOpCode(int iOpcode);

public:
    explicit ReservationStationsViewer(QWidget *parent = 0);    // Class constructor

signals:
    
public slots:
    void updateViewer2();
};

#endif // RESERVATIONSTATIONSVIEWER_H
