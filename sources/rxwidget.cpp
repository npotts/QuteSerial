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
 * This Widget class is the displayed receive widget that contains the
 * bytes receieved
*/
#include "rxwidget.h"

QuteRxWidget::QuteRxWidget(QWidget * parent, Qt::WindowFlags f) : QWidget(parent), hex() {
  setupUi(this);
  //add in all the ASCII stuffs
  QStringList items;
  QString ascii_[] = {"(off)", "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "\\a", "BS", "\\t", "\\n", "VT", "FF", "\\r", "SO", "SI", "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB", "CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US", "Space", "!", "\"", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", ":", ";", "<", "=", ">", "?", "@", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "[", "\\", "]", "^", "_", "`", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "{", "|", "}", "~", "DEL"};
  for (int i=0; i< 129; i++) {
    items << (QString("%1").arg(i-1, 3, 10, QChar(' ')) + "   " + ascii_[i]);
  }
  for (int i= 128; i<256; i++)
    items << QString("%1").arg(i, 3, 10, QChar(' '));
  autoclear_->addItems(items);
  //setup buffer for use by the hex widget
  data.open(QBuffer::ReadWrite);
  hex.setData(&data);
  hex.setShowAddress(false);
  hex.setShowComments(false);
  gridLayout_3->addWidget(&hex, 0, 0, 1, 1);
  //connect the clear button to the clear function
  connect(clear_, SIGNAL(clicked()), this, SLOT(clear()));
}
void QuteRxWidget::dataRxd(QByteArray read) {
  /*Slot that is called when someone has ready data and echos it to us*/
  QScrollBar *sb;
  //check if auto-clear is enabled.
  if (autoclear_->currentIndex() > 0) { //chop down read if it has the bad character
    if (read.contains((char) (autoclear_->currentIndex() - 1 ) ) ) {
      //call clear and reset the byte array read
      read = read.split( (char) (autoclear_->currentIndex() - 1 ) ).last();
    }
  }
  rxascii->moveCursor(QTextCursor::End);
  rxascii->insertPlainText(QString(read));
  rxascii->moveCursor (QTextCursor::End);
  data.seek(data.size());
  data.write(read);
  hex.setData(&data);
  sb = rxascii->verticalScrollBar();
  sb->setValue(sb->maximum());
  sb = hex.verticalScrollBar();
  sb->setValue(sb->maximum());
}
void QuteRxWidget::clear() {
  /*Pretty straight forward:  Clear displays. */
  rxascii->clear();
  data.close();
  data.setData(QByteArray()); 
  data.open(QBuffer::ReadWrite);
  hex.setData(&data);
}
