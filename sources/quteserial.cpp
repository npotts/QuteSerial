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
 *
 *
 *
 * This is the main widget that holds all the other subcomponents
 */

#include "quteserial.h"

Qute::Qute( QWidget * parent, Qt::WindowFlags f) : QMainWindow(parent, f),
                                                   macros(parent, Qt::Tool | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint) {
  /*Setup the GUI for user interaction.  Mostly lots of connect()'s
    to tie input and output signals where they need to be.'*/
  setupUi(this);
  //setup stuff so we know where to get settings from
  QCoreApplication::setOrganizationName("Nicksoft");
  QCoreApplication::setOrganizationDomain("nicksoft.com");
  QCoreApplication::setApplicationName("QuteSerial");
  //add in the tx and rx widgets
  mainGrid->addWidget(&rxwidget, 2, 0, 1, Qt::AlignHCenter);
  mainGrid->addWidget(&txwidget, 3, 0, 1, Qt::AlignHCenter);
  //config && connect items for the macro widget to work
  QList<QAction *> macroactions;
  macroactions << actionAM_1 << actionAM_2 << actionAM_3 << actionAM_4 << actionAM_5 << actionAM_6 << actionAM_7 << actionAM_8 << actionAM_9 << actionAM_10 << actionAM_11 << actionAM_12;
  macros.setActions(macroactions);
  connect(editMacros,      SIGNAL(triggered()),                this,     SLOT(showMacros()));
  //setup permament widgets on the status, details are defined in the CONFIGURE_STATUSBAR macro
  CONFIGURE_STATUSBAR();
  //button connects
  connect(actionRescan,    SIGNAL(triggered()),                this,      SLOT(rescanPorts()));
  connect(actionClear,     SIGNAL(triggered()),                &txwidget, SLOT(clear()));
  connect(actionClear,     SIGNAL(triggered()),                &rxwidget, SLOT(clear()));
  connect(rts,             SIGNAL(toggled(bool)),              this,      SLOT(setRts(bool)));
  connect(dtr,             SIGNAL(toggled(bool)),              this,      SLOT(setDtr(bool)));
  connect(linebreak,       SIGNAL(toggled(bool)),              this,      SLOT(setBreak(bool)));
  connect(actionStart_Log, SIGNAL(triggered()),                this,      SLOT(startStopLog()));  
  //serial port connects
  connect(&port,           SIGNAL(readyRead()),                 this,      SLOT(  readyRead()));
  connect(actionOpen,      SIGNAL(triggered()),                 this,      SLOT(ser_connect()));
  
  //connect tx/rx signals from here
  connect(this,            SIGNAL(dataRxd(QByteArray)),         &rxwidget, SLOT(dataRxd(QByteArray)));
  connect(this,            SIGNAL(dataTxd(QByteArray)),         &txwidget, SLOT(dataTxd(QByteArray)));
  connect(&txwidget,       SIGNAL(requestSendData(QByteArray)), this,      SLOT(sendData(QByteArray)));
  connect(&macros,         SIGNAL(requestSendData(QByteArray)), this,      SLOT(sendData(QByteArray)));

  //changing serial port settings will change the settings on the port
  connect(baud,            SIGNAL(currentIndexChanged(int)),    this,      SLOT(applySerialSettings()));
  connect(bits,            SIGNAL(currentIndexChanged(int)),    this,      SLOT(applySerialSettings()));
  connect(parity,          SIGNAL(currentIndexChanged(int)),    this,      SLOT(applySerialSettings()));
  connect(stopbits,        SIGNAL(currentIndexChanged(int)),    this,      SLOT(applySerialSettings()));
  connect(flow,            SIGNAL(currentIndexChanged(int)),    this,      SLOT(applySerialSettings()));
  connect(rts,             SIGNAL(clicked()),                 this,      SLOT(applySerialSettings()));
  connect(dtr,             SIGNAL(clicked()),                 this,      SLOT(applySerialSettings()));
  connect(linebreak,       SIGNAL(clicked()),                 this,      SLOT(applySerialSettings()));

  //final stuff to make UI ready for user
  rescanPorts();
  readSettings();
}
void Qute::readSettings() {
  /**Read settings from disk*/
  QSettings cfg;
  SETIFFOUND(ports,     cfg.value("serial/port"));
  SETIFFOUND(baud,      cfg.value("serial/baud"));
  SETIFFOUND(bits,      cfg.value("serial/bits"));
  SETIFFOUND(parity,    cfg.value("serial/parity"));
  SETIFFOUND(stopbits,  cfg.value("serial/stopbits"));
  SETIFFOUND(flow,      cfg.value("serial/flow"));
  rts->setChecked(      cfg.value("serial/rts", false).toBool());
  dtr->setChecked(      cfg.value("serial/dtr", false).toBool());
  linebreak->setChecked(cfg.value("serial/break", false).toBool());
}
void Qute::saveSettings() {
  /**Save settings */
  QSettings cfg;
  cfg.setValue("serial/port",     ports->currentText());
  cfg.setValue("serial/baud",     baud->currentText());
  cfg.setValue("serial/bits",     bits->currentText());
  cfg.setValue("serial/parity",   parity->currentText());
  cfg.setValue("serial/stopbits", stopbits->currentText());
  cfg.setValue("serial/flow",     flow->currentText());
  cfg.setValue("serial/rts",      rts->isChecked());
  cfg.setValue("serial/dtr",      dtr->isChecked());
  cfg.setValue("serial/break",    linebreak->isChecked());
}
void Qute::showMacros() {
  /*Show / hide macros windows*/
  if (macros.isVisible())
    macros.setVisible(false);
  else
    macros.setVisible(true);
  saveSettings();
}
void Qute::ser_connect() {
  /*This tries to attach to the serial port.  If we cannot connect, undo button click and show some sort of error message*/
  if (port.isOpen()) {
    port.close();
    status.at(STATUS_CON)->setText("Not Connected");
  } else {
    actionOpen->setChecked(false);
    if (ports->currentIndex() == -1) return;  //not a valid port entered
    port.setPortName(ports->currentText());
    if (! port.open(QIODevice::ReadWrite)) return;
    actionOpen->setChecked(true);
    applySerialSettings();
    saveSettings();
    status.at(STATUS_CON)->setText(QString("Connected to: %1").arg(ports->currentText()));
  }
  SET_SIGNAL_BITS();
}
void Qute::applySerialSettings() {
  /**Apply serial settings*/
  if (!port.isOpen()) return;
  if (port.baudRate() != (QSerialPort::BaudRate) baud->currentText().toInt()) port.setBaudRate((QSerialPort::BaudRate) baud->currentText().toInt());
  if (port.dataBits() != (QSerialPort::DataBits) bits->currentText().toInt()) port.setDataBits((QSerialPort::DataBits) bits->currentText().toInt());
  if (port.parity()      != parity->currentIndex())      port.setParity(      (QSerialPort::Parity)      parity->currentIndex());
  if (port.stopBits()    != stopbits->currentIndex())    port.setStopBits(    (QSerialPort::StopBits)    stopbits->currentIndex());
  if (port.flowControl() != flow->currentIndex())        port.setFlowControl( (QSerialPort::FlowControl) flow->currentIndex());
  
  if (port.isRequestToSend()     != rts->isChecked()) port.setRequestToSend(rts->isChecked());
  if (port.isDataTerminalReady() != dtr->isChecked()) port.setDataTerminalReady(dtr->isChecked());
  port.setBreakEnabled(linebreak->isChecked());
}
void Qute::rescanPorts() {
  /*Scan for new serial ports to appear.  Try to preserve the selected port over
    rescans.  If the selected port disappears, set the port to -1*/
  QString oldPort = (ports->currentIndex() != -1) ? ports->currentText(): "";
  int index = -1; //default to selecting no port
  ports->clear();
  foreach(QSerialPortInfo i, QSerialPortInfo::availablePorts()) {
    if (oldPort == i.systemLocation())
      index = ports->count();
    ports->addItem(i.systemLocation());
  }
  ports->setCurrentIndex(index);
}
void Qute::startStopLog() {
  /**Either close log (if open), or ask to start a new one*/
  if (log.isOpen()) {
    //close file
    log.close();
  } else {
    actionStart_Log->setChecked(false);
    QString d = QFileDialog::getSaveFileName(this, "Save Tx/Rx log", QString(), "Log Files (*.log *.txt);;All Files (*)", 0, QFileDialog::DontConfirmOverwrite);    
    if (d == "") return;
    log.setFileName(d);
    if (!log.open(QIODevice::WriteOnly)) return;
    qDebug() << "File opened: " << d;
    actionStart_Log->setChecked(true);
  }
}
void Qute::readyRead() {
  /*Slot called when serial port has data*/
  QByteArray b = port.readAll();
  //increment rx counter
  status.at(STATUS_RX)->setText( QString("Rx: %1").arg(status.at(STATUS_RX)->text().split(":").last().toInt() + b.size()));
  SET_SIGNAL_BITS();
  if (log.isOpen()) log.write(b);
  emit dataRxd(b);
}
void Qute::sendData(QByteArray data) {
  /*Send some bytes to the serial port.  Take care to send as much as we can, but just
    in case we get a large blob, dont assume we will sent it all in one push. */

  /*First, translate encoded hex values into inline hex chars on the fly
    formats to convert:
    \xAA -> 0xAA
    \r   -> 0x0A
    \n   -> 0x0D
    \t   -> 0x09
    \b   ->
    \a   ->
    \\   -> literal \
   */
  QByteArray converted;
  for(int i=0; i< data.size(); i++) {
    //iter through characters looking for leading \ character
    if ((data.at(i) == '\\') && (i+1 < data.size()) ) {
      //check for next character: allowables are: r,n,t,b,a,\, or x.
      QList<QChar> allowables; allowables << 'r' << 'n' << 't' << 'b'<< 'a' << '\\' << 'x';
      if ( i+1 < data.size() && allowables.contains(data.at(i+1)) ) {
        i++;
        //at a minimum, we are to replace one character.
        if      (data.at(i) == '\\') { converted += '\\';}
        else if (data.at(i) == 'r') { converted += '\r';}
        else if (data.at(i) == 'n') { converted += '\n';}
        else if (data.at(i) == 't') { converted += '\t';}
        else if (data.at(i) == 'b') { converted += '\b';}
        else if (data.at(i) == 'a') { converted += '\a';}
        else if (data.at(i) == 'x') {
          //attempt to convert the next 2 chars into bytes
          if (i+2 < data.size()) { //2 chars to convert
            converted += QByteArray::fromHex(data.mid(i+1,2));
            i+=2;
          } else { //user did something like "\x1", just copy bytes over as is
            converted += data.mid(i-1, 3);
            i+=1;
          }
        } else { //user did something wrong.  Just copy character over
          converted += data.mid(i-1, 2); break;
        }
      } else { //dangling \ at the end of the string.
        converted += QByteArray("\\");
      }
    } else {
      converted += data.at(i);
    }
  }
  //send data over in as large of chunks as we can, but dont assume it all made it
  int sent = 0;
  while(port.isOpen() && sent < converted.size()) {
    int more = port.write(converted.mid(sent));
    if (more > 0) {
      emit dataTxd(converted.mid(sent,more));
      if (log.isOpen()) log.write(converted.mid(sent,more));
      status.at(STATUS_TX)->setText( QString("Tx: %1").arg(status.at(STATUS_TX)->text().split(":").last().toInt() + more));
    }
    sent += more;
    SET_SIGNAL_BITS();
  }
}
//simple oneliners
void Qute::setRts(bool set)   {  /*Set RTS   if opened*/ if (port.isOpen()) port.setRequestToSend(set); }
void Qute::setDtr(bool set)   {  /*Set DTR   if opened*/ if (port.isOpen()) port.setDataTerminalReady(set); }
void Qute::setBreak(bool set) {  /*Set Break if opened*/ if (port.isOpen()) port.setBreakEnabled(set); }
