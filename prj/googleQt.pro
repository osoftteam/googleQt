QT       += network xml sql
QT       += gui
QT       += core5compat
CONFIG += staticlib
CONFIG -= flat
TEMPLATE = lib

GM_MODULES = users messages labels threads drafts errors history attachments
GT_MODULES = tasks tasklists
GD_MODULES = about files permissions comments revisions
GC_MODULES = contacts contact_group

SRC_DIR = ../src
G_DIR = $${SRC_DIR}/google
GM_DIR = $${SRC_DIR}/gmail
GT_DIR = $${SRC_DIR}/gtask
GD_DIR = $${SRC_DIR}/gdrive
GC_DIR = $${SRC_DIR}/gcontact

#HEADERS += $${SRC_DIR}/*.h
HEADERS += $$files($${SRC_DIR}/*.h, false)
SOURCES += $$files($${SRC_DIR}/*.cpp, false)
    
HEADERS += $$files($${G_DIR}/endpoint/*.h, false)
SOURCES += $$files($${G_DIR}/endpoint/*.cpp, false)
HEADERS += $$files($${G_DIR}/demo/*.h, false)
SOURCES += $$files($${G_DIR}/demo/*.cpp, false)
HEADERS += $$files($${GM_DIR}/*.h,false) $$files($${GT_DIR}/*.h,false) $$files($${GD_DIR}/*.h,false) $$files($${GC_DIR}/*.h,false)
SOURCES += $$files($${GM_DIR}/*.cpp,false) $$files($${GT_DIR}/*.cpp,false) $$files($${GD_DIR}/*.cpp,false) $$files($${GC_DIR}/*.cpp,false)


for(m, GM_MODULES){
       d = $${GM_DIR}/$${m}
       exists($${d}){
	   HEADERS += $$files($${d}/*.h, false)	
	   SOURCES += $$files($${d}/*.cpp, false)
       }
}

for(m, GT_MODULES){
       d = $${GT_DIR}/$${m}
       exists($${d}){
	   HEADERS += $$files($${d}/*.h, false)
	   SOURCES += $$files($${d}/*.cpp, false)
       }
}

for(m, GD_MODULES){
       d = $${GD_DIR}/$${m}
       exists($${d}){
	   HEADERS += $$files($${d}/*.h, false)
	   SOURCES += $$files($${d}/*.cpp, false)
       }
}

for(m, GC_MODULES){
       d = $${GC_DIR}/$${m}
       exists($${d}){
	   HEADERS += $$files($${d}/*.h, false)
	   SOURCES += $$files($${d}/*.cpp, false)
       }
}

################################################################
# some diagnostics, some non-intence logging/tracing
################################################################
#DEFINES += API_QT_DIAGNOSTICS

################################################################
# autotest generation (internal profiling usage)               
################################################################
ARD_AUTOTEST=$$(ARD_AUTOTEST)
if(!isEmpty( ARD_AUTOTEST )){
    HEADERS += $${G_DIR}/AUTOTEST/*.h
    SOURCES += $${G_DIR}/AUTOTEST/*.cpp
    HEADERS += $${GM_DIR}/AUTOTEST/*.h $${GT_DIR}/AUTOTEST/*.h $${GD_DIR}/AUTOTEST/*.h $${GC_DIR}/AUTOTEST/*.h
    SOURCES += $${GM_DIR}/AUTOTEST/*.cpp $${GT_DIR}/AUTOTEST/*.cpp $${GD_DIR}/AUTOTEST/*.cpp $${GC_DIR}/AUTOTEST/*.cpp
    DEFINES += API_QT_AUTOTEST
    DEFINES += API_QT_DIAGNOSTICS
    !build_pass:message("+autotest")
}
################################################################
ARD_DEBUG = $$(ARD_DEBUG)
if(!isEmpty( ARD_DEBUG )){
	  CONFIG += debug
	  CONFIG -= release
      unix {
           QMAKE_CXXFLAGS += -O0 -std=c++14
           }
      !build_pass:message("+debug")
}

INCLUDEPATH += $${SRC_DIR}

