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
 * This Widget class is the displayed receive widget that contains the
 * bytes receieved
*/

#ifndef QUTE_RXWIDGET_H
#define QUTE_RXWIDGET_H

#include <QtCore>
#include <QtWidgets>
#include <QHexView>
#include "ui_rxwidget.h"

class QuteRxWidget: public QWidget, public Ui::rxwidget {
  Q_OBJECT
public:
  QuteRxWidget(QWidget * parent = 0, Qt::WindowFlags f = 0);

private:
  QHexView hex;
  QBuffer data;
   
private slots:
  void dataRxd(QByteArray);
  void clear();
};

#endif
