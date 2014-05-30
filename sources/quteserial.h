/*
 * The MIT License (MIT)
 *
 * This file is part of the QuteSerial Project availible at
 * https://github.com/npotts/QuteSerial
 *
 * Copyright (c) 2013-2014 Nicholas Potts (npotts)
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * This is the main widget that holds all the other subcomponents
 */

#ifndef QUTESERIAL_H
#define QUTESERIAL_H

#include <QtWidgets>
#include <QtCore>
#include <QtNetwork>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "txwidget.h"
#include "rxwidget.h"
#include "macros.h"
#include "ui_quteserial.h"

//Will attempt set the index of the combo box to to the specified strValue, if the value does not exist, dont set anything
#define SETIFFOUND(combobox, str) \
  for(int i=0; i < combobox->count(); i++) \
    if (combobox->itemText(i) == str.toString()) \
      combobox->setCurrentIndex(i);

class Qute: public QMainWindow, public Ui::MainWindow {
  Q_OBJECT
public:
  Qute( QWidget * parent = 0, Qt::WindowFlags f = 0 );
private slots:
  void showMacros();
  void sendData(QByteArray);
  void readyRead();
  void rescanPorts();
  void ser_connect();
  void setDtr(bool);
  void setRts(bool);
  void setBreak(bool);
  void startStopLog();
  void readSettings();
  void saveSettings();
  void applySerialSettings();
private:
  QList<QLabel *> status;
  QuteTxWidget  txwidget;
  QuteRxWidget  rxwidget;
  QuteMacros    macros;
  QSerialPort   port;
  QFile log;
signals:
  void dataRxd(QByteArray);
  void dataTxd(QByteArray);
};


// configure each of the labels a bit.  Since this is ugly and verbose (but really simple), it
// should be shoved into a macro to live in the header and expanded here
// Connected | Tx | Rx | CTS | DSR | CD | RI
enum statusLocs {STATUS_CON=0,
                 STATUS_TX=1,
                 STATUS_RX=2,
                 STATUS_CTS=4,
                 STATUS_DSR=6,
                 STATUS_DCD=8,
                 STATUS_RI=10
};
#define CONFIGURE_STATUSBAR() \
  status << new QLabel("Not Connected", statusbar) << new QLabel("Tx: ", statusbar) << new QLabel("Rx: ", statusbar) << new QLabel("CTS", statusbar) << new QLabel(statusbar) << new QLabel("DSR", statusbar) << new QLabel(statusbar) << new QLabel("CD",  statusbar) << new QLabel(statusbar) << new QLabel("RI",  statusbar) << new QLabel(statusbar);\
  status.at( 0)->setMinimumWidth(80);   status.at( 0)->setStatusTip("Connection Status");\
  status.at( 1)->setMinimumWidth(80);   status.at( 1)->setStatusTip("Total Transmitted Bytes");\
  status.at( 2)->setMinimumWidth(80);   status.at( 2)->setStatusTip("Total Receieved Bytes");\
  status.at( 4)->setMaximumSize(16,16); status.at( 4)->setStatusTip("CTS State"); status.at( 4)->setScaledContents(true); status.at( 4)->setPixmap(QPixmap(":/red.png"));\
  status.at( 6)->setMaximumSize(16,16); status.at( 6)->setStatusTip("DSR State"); status.at( 6)->setScaledContents(true); status.at( 6)->setPixmap(QPixmap(":/red.png"));\
  status.at( 8)->setMaximumSize(16,16); status.at( 7)->setStatusTip("DCD State");  status.at( 8)->setScaledContents(true); status.at( 8)->setPixmap(QPixmap(":/red.png"));\
  status.at(10)->setMaximumSize(16,16); status.at(10)->setStatusTip("RI State");  status.at(10)->setScaledContents(true); status.at(10)->setPixmap(QPixmap(":/red.png"));\
  statusbar->layout()->setSpacing(0);\
  for(int i=0; i<status.size(); i++) statusbar->addPermanentWidget(status.at(i))


//This sets the indicators of the CTS/DSR/CD and RI signals based on what the port tells us
#define SET_SIGNAL_BITS() \
  QSerialPort::PinoutSignals s = (port.isOpen()) ?              \
    port.pinoutSignals(): QSerialPort::NoSignal;                \
  status.at(STATUS_CTS)->setPixmap(QPixmap(":/red.png"));       \
  status.at(STATUS_DSR)->setPixmap(QPixmap(":/red.png"));       \
  status.at(STATUS_DCD)->setPixmap(QPixmap(":/red.png"));       \
  status.at(STATUS_RI )->setPixmap(QPixmap(":/red.png"));       \
  if (s & QSerialPort::ClearToSendSignal)                       \
    status.at(STATUS_CTS)->setPixmap(QPixmap(":/green.png"));   \
  if (s & QSerialPort::DataSetReadySignal)                      \
    status.at(STATUS_DSR)->setPixmap(QPixmap(":/green.png"));   \
  if ((s & QSerialPort::DataCarrierDetectSignal))               \
    status.at(STATUS_DCD)->setPixmap(QPixmap(":/green.png"));   \
  if ((s & QSerialPort::RingIndicatorSignal))                   \
    status.at(STATUS_RI )->setPixmap(QPixmap(":/green.png"))
#endif

