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
 * This Widget class is the displayed transmit widget that contains the
 * bytes transmitted
 */
#include "txwidget.h"

QuteTxWidget::QuteTxWidget(QWidget * parent, Qt::WindowFlags f) : QWidget(parent) {
  setupUi(this);
  //connect the clear button to the clear function
  data.open(QBuffer::ReadWrite);
  connect(clear_,  SIGNAL(clicked()),       this, SLOT(clear()));
  connect(txentry, SIGNAL(returnPressed()), this, SLOT(send()));
  connect(tx,      SIGNAL(clicked()),       this, SLOT(send()));
  connect(file,    SIGNAL(clicked()),       this, SLOT(sendFile()));
  //setup eventFilter so we can capture keypresses over txascii
  txascii->installEventFilter(this);
  gridLayout_3->addWidget(&hex, 0, 0, 1, 1);
}
bool QuteTxWidget::eventFilter(QObject *src, QEvent *ev) {
  if ((src == txascii) && (ev->type() == QEvent::KeyPress)) {
    //because we are translating exscape sequences starting with \, 
    //make sure to escape the \ character alone
    if (((QKeyEvent *) ev)->text() == "\\")
      emit requestSendData(QByteArray("\\"));
    else 
      emit requestSendData(((QKeyEvent *) ev)->text().toLatin1());
    return false;
  } else {
    return QWidget::eventFilter(src, ev);
  }
}
void QuteTxWidget::dataTxd(QByteArray wrote) {
  /*Slot that is called when someone has ready data and echos it to us*/
  QScrollBar *sb;
  //send data to hex entry first
  data.seek(data.size());
  data.write(wrote);
  hex.setData(&data);
  txascii->moveCursor(QTextCursor::End);
  //wrote.replace("\n", "<br>");
  txascii->insertPlainText(QString(wrote));
  txascii->moveCursor (QTextCursor::End);
  //set scrolls bars to bottom
  sb = txascii->verticalScrollBar(); sb->setValue(sb->maximum());
  sb = hex.verticalScrollBar();      sb->setValue(sb->maximum());
}
void QuteTxWidget::clear() {
  /*Pretty straight forward:  Clear displays. */
  txascii->clear();
  data.close();
  data.setData(QByteArray()); 
  data.open(QBuffer::ReadWrite);
  hex.setData(&data);
}
void QuteTxWidget::sendFile() {
  /*Send the characters from a user selected file*/
  QString filename = QFileDialog::getOpenFileName(this,  "Select File to Send", "", "All Files (*)", NULL);
  if (filename == "")
    return;
  QFile o(filename);
  o.open(QIODevice::ReadOnly);
  emit requestSendData(o.readAll());
}

void QuteTxWidget::send() {
  /*Send the characters in the buffer*/
  QString txd = txentry->text() +  (addcr->isChecked() ? "\r" : "");
  emit requestSendData(txd.toLatin1());
}

