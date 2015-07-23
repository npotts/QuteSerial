#
#  The MIT License (MIT)
#
#  This file is part of the QuteSerial Project availible at
#  https://github.com/npotts/QuteSerial
#
#  Copyright (c) 2013-2014 Nicholas Potts (npotts)
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.


TEMPLATE = app
QT = gui core widgets network serialport 
CONFIG += qt warn_on stl build_all
DEPLOYNAME=QuteSerial
DESTDIR = bin

CONFIG(debug, debug|release) {
  OBJECTS_DIR = debug
  PACKAGE_DIR = debug
  MOC_DIR = debug
  UI_DIR = debug
  CONFIG += console
  TARGET = $${DEPLOYNAME}-dev
} else {
  OBJECTS_DIR = release
  PACKAGE_DIR = release
  MOC_DIR = release
  UI_DIR = release
  CONFIG += windows
  TARGET = $${DEPLOYNAME}
}

INCLUDEPATH += .

SOURCES += sources/main.cpp       \
           sources/qhexview.cpp   \
           sources/txwidget.cpp   \
           sources/rxwidget.cpp   \
           sources/macros.cpp     \
           sources/quteserial.cpp

HEADERS += sources/qhexview.h   \
           sources/txwidget.h   \
           sources/rxwidget.h   \
           sources/macros.h     \
           sources/quteserial.h
           
FORMS +=   sources/macros.ui     \
           sources/quteserial.ui \
           sources/rxwidget.ui   \
           sources/txwidget.ui

RESOURCES += resources/icons.qrc

QMAKE_CLEAN += -r bin build debug release object_script*
