################################################################
# autotest generation (internal profiling usage)               #
#                                                              #
################################################################


QT       += core network sql xml
QT       += gui
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

SRC_DIR = ../src
INCLUDEPATH += $${SRC_DIR}

##############################################################
#  specify path to the gmailQt lib, on windows it can be     #
#   ../../prj/debug/ on unix - shadow build path             #
##############################################################

unix {
     LIBS += -L../prj/ -lgoogleQt
}

win32 {
      contains(QT_ARCH, i386) {
         LIBS += -L../prj/debug/ -lgoogleQt
      } else {
         LIBS += -L../prj/x64/debug/ -lgoogleQt
      }      
}

DEFINES += API_QT_AUTOTEST

SOURCES += main.cpp

