QT       += core network sql xml
QT       += gui
CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

SRC_DIR = ../../src
INCLUDEPATH += $${SRC_DIR}

################################################################
# autotest generation (internal profiling usage)               
# you probably don't need it, unless you design/profile library
################################################################
ARD_AUTOTEST=$$(ARD_AUTOTEST)
if(!isEmpty( ARD_AUTOTEST )){
    DEFINES += API_QT_AUTOTEST
    DEFINES += API_QT_DIAGNOSTICS
    !build_pass:message("+autotest")
}

ARD_DEBUG=$$(ARD_DEBUG)
if(!isEmpty( ARD_DEBUG )){
    unix {
        QMAKE_CXXFLAGS += -O0 -g
        !build_pass:message("+debug")        
    }
}

##############################################################
#  specify path to the gmailQt lib, on windows it can be     #
#   ../../prj/debug/ on unix - shadow build path             #
##############################################################

unix {
     LIBS += -L../../prj/ -lgoogleQt
}

win32 {
      contains(QT_ARCH, i386) {
         LIBS += -L../../prj/debug/ -lgoogleQt
      } else {
         LIBS += -L../../prj/x64/debug/ -lgoogleQt
      }      
}
