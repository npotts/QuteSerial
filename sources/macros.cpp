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
 * This widget contains all the logic needed to enable and use macros.  Macros,
 * for this program, are short strings of data that are bound to hotkeys F1-F12
 * that allow for repeated commands to be stored and easily sent.
 */
#include "macros.h"

QuteMacros::QuteMacros(QWidget * parent, Qt::WindowFlags f) : QWidget(parent) {
  setupUi(this);
  QCoreApplication::setOrganizationName("Nicksoft");
  QCoreApplication::setOrganizationDomain("nicksoft.com");
  QCoreApplication::setApplicationName("QuteSerial");
  for(int i =0; i<12; i++) {  //Add F1-F12 label on the far left
    QLabel *l = new QLabel(QString("F%1").arg(i+1), this);
    l->setAlignment(Qt::AlignHCenter);
    gridLayout->addWidget(l, i+1, 0, Qt::AlignHCenter);

    //add in QLineEdit for the text field that will contain the serial characters
    data.append(new QLineEdit(cfg.value(QString("macro%1/data").arg(i), "").toString(), this));
    data.at(i)->setPlaceholderText(tr("Characters"));
    data.at(i)->setMinimumWidth(200);
    connect(data.at(i), SIGNAL(editingFinished()), this, SLOT(saveConfig()));
    gridLayout->addWidget(data.at(i), i+1, 1, Qt::AlignHCenter);

    //QLineEdit for the Macro's button name
    names.append(new QLineEdit(cfg.value(QString("macro%1/name").arg(i), "").toString(), this));
    names.at(i)->setPlaceholderText(tr("Name"));
    names.at(i)->setMinimumWidth(200);
    names.at(i)->setMaxLength(8);
    connect(names.at(i), SIGNAL(editingFinished()), this, SLOT(saveConfig()));
    gridLayout->addWidget(names.at(i), i+1, 2, Qt::AlignHCenter);

    //QCheckbox for repeat functionality
    repeats.append(new QCheckBox(this));
    repeats.at(i)->setChecked(cfg.value(QString("macro%1/repeat").arg(i), false).toBool());
    connect(repeats.at(i), SIGNAL(toggled(bool)), this, SLOT(saveConfig()));
    gridLayout->addWidget(repeats.at(i), i+1, 3, Qt::AlignHCenter);
    
    //QSpinBox for delay timer value
    times.append(new QSpinBox(this));
    times.at(i)->setMaximum(0);
    times.at(i)->setMaximum(90000);
    times.at(i)->setValue(cfg.value(QString("macro%1/time").arg(i), 1000).toInt());
    times.at(i)->setSingleStep(100);
    times.at(i)->setSuffix(" ms");
    //the delayMapper settings are to catch when the value of the delay changes and it is repeating
    delayMapper.setMapping(times.at(i), i);
    connect(times.at(i), SIGNAL(valueChanged(int)), this,        SLOT(saveConfig()));
    connect(times.at(i), SIGNAL(valueChanged(int)), &delayMapper, SLOT(map()));
    gridLayout->addWidget(times.at(i), i+1, 4, Qt::AlignHCenter);

    //create timers needed
    timers.append(new QTimer());
    timers.at(i)->setSingleShot(true);
    if (repeats.at(i)->isChecked())
      timers.at(i)->setInterval(cfg.value(QString("macro%1/time").arg(i), 1000).toInt());
    else
      timers.at(i)->setInterval(times.at(i)->maximum() + 1);
    connect(timers.at(i), SIGNAL(timeout()), this, SLOT(serviceTimers()));

    //force checking for enabled timers from settings
    restartTimerWithNewDelay(i);
  }
  connect(&actionMapper, SIGNAL(mapped(int)), this, SLOT(macroTriggered(int)));
  connect(&delayMapper,  SIGNAL(mapped(int)), this, SLOT(restartTimerWithNewDelay(int)));
  connect(this,          SIGNAL(newMacros()), this, SLOT(updateMacros()));
  updateMacros(); //update macros
}
void QuteMacros::saveConfig(void) {
  /*Saves the current macros to disk.  Called when things are
    finished being edited*/
  for(int i=0; i<12; i++) { //go through the grid and pull values out of it
    cfg.setValue(QString("macro%1/name").arg(i),   names.at(i)->text());
    cfg.setValue(QString("macro%1/data").arg(i),  data.at(i)->text());
    cfg.setValue(QString("macro%1/repeat").arg(i), repeats.at(i)->isChecked());
    if (repeats.at(i)->isChecked()) {
      //dont do anyhting if the timer has already started
      if (timers.at(i)->interval() == times.at(i)->maximum() + 1) {
        timers.at(i)->setInterval(times.at(i)->value());
        timers.at(i)->start();
      }
    } else { //unchecked.  set interval to -1 and stop timer
      timers.at(i)->setInterval(times.at(i)->maximum() + 1);
      timers.at(i)->stop();
    }
    cfg.setValue(QString("macro%1/time").arg(i),  times.at(i)->value());
  }
  emit newMacros();
}
void QuteMacros::serviceTimers() {
  /*Service timers.  Sorta complex, but effectively it is just looking for an
   expired (not active) timer where the interval is not 1 greater than the maximum.
   When found, it echo out the chars associated with that timer and restarts it.
   They are stopped by changing the config */
  for(int i=0; i<12; i++) { //look for timers that are active and have timed out
    if ((timers.at(i)->interval() != times.at(i)->maximum() + 1) && !timers.at(i)->isActive()) {
      //found one. send chars send chars to serial port
      macroTriggered(i);
      timers.at(i)->start();
    }
  }
}
void QuteMacros::restartTimerWithNewDelay(int index) {
  /*This checks if the index row currently has an enabled repeat box.  If so,
   it restarts timer with the new time.  This is called when the QSpinBox change
  value as well on initial program start.
  */
  if (repeats.at(index)->isChecked()) {
    timers.at(index)->setInterval(times.at(index)->value());
    timers.at(index)->start();
  }
}
void QuteMacros::setActions(QList<QAction *> macroButtons) {
  /*In order to catch trigger actions, we need to have a local list of QActions from
    the initial gui. When we get this list, we need to disconnect from any previous
    list.  that functionality isnt being used, but might as well be safe than sorry*/
  foreach(QAction *i, macroactions) { //Disconnect old signals
    i->disconnect();
  }
  macroactions = macroButtons;
  int index = 0;
  foreach(QAction *i, macroactions) {
    /*the actionMapper settings are to catch individual macro click events in one
      function the actionMapper's signals are sent to macroTriggered */ 
    actionMapper.setMapping(i, index++);
    connect(i, SIGNAL(triggered()), &actionMapper, SLOT(map()));
  }
}
void QuteMacros::macroTriggered(int index) {
  /*A macro button clicked. - emit the signal to send the characters*/
  emit requestSendData(data.at(index)->text().toLatin1());
}
void QuteMacros::updateMacros() {
  /*Called when the macros are updated.  We should update the macro displays*/
  for(int i=0; i<macroactions.size(); i++) {
    macroactions.at(i)->setText(cfg.value(QString("macro%1/name").arg(i), "").toString());
    if (cfg.value(QString("macro%1/repeat").arg(i)).toBool()) {
      macroactions.at(i)->setIcon(QIcon(":/running_macro.png"));
    } else {
      macroactions.at(i)->setIcon(QIcon(":/macro.png"));
    }
  }
}

