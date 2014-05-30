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
 * This widget contains all the logic needed to enable and use macros.  Macros,
 * for this program, are short strings of data that are bound to hotkeys F1-F12
 * that allow for repeated commands to be stored and easily sent.
 */

#ifndef QUTE_MACROS_H
#define QUTE_MACROS_H

#include <QtCore>
#include <QtWidgets>
#include "ui_macros.h"

class QuteMacros: public QWidget, public Ui::macroswidget {
  Q_OBJECT
public:
  QuteMacros(QWidget * parent = 0, Qt::WindowFlags f = 0);
  void setActions(QList<QAction *>);

private slots:
  void saveConfig(); //save config to disk.
  void serviceTimers(); //work with timers to issue commands on the correct routine
  void updateMacros();
  void macroTriggered(int);
  void restartTimerWithNewDelay(int);
  
private:
  QSettings cfg;
  QList<QAction *> macroactions;
  QList<QLineEdit *> names;
  QList<QLineEdit *> data;
  QList<QCheckBox *> repeats;
  QList<QSpinBox *> times;
  QList<QTimer *> timers;
  QSignalMapper delayMapper, actionMapper;

signals:
  void newMacros(); //emitted when a setting is changes so we can update parent
  void requestSendData(QByteArray);
};

#endif
