QT       += network xml sql
QT       -= gui
CONFIG += staticlib
CONFIG -= flat
TEMPLATE = lib

GM_MODULES = users messages labels threads drafts errors history attachments
GT_MODULES = tasks tasklists
GD_MODULES = about files permissions comments revisions

SRC_DIR = ../src
G_DIR = $${SRC_DIR}/google
GM_DIR = $${SRC_DIR}/gmail
GT_DIR = $${SRC_DIR}/gtask
GD_DIR = $${SRC_DIR}/gdrive

HEADERS += $${SRC_DIR}/*.h
SOURCES += $${SRC_DIR}/*.cpp
HEADERS += $${G_DIR}/endpoint/*.h
SOURCES += $${G_DIR}/endpoint/*.cpp
HEADERS += $${G_DIR}/demo/*.h
SOURCES += $${G_DIR}/demo/*.cpp
HEADERS += $${GM_DIR}/*.h $${GT_DIR}/*.h $${GD_DIR}/*.h
SOURCES += $${GM_DIR}/*.cpp $${GT_DIR}/*.cpp $${GD_DIR}/*.cpp


for(m, GM_MODULES){
       d = $${GM_DIR}/$${m}
       exists($${d}){
	   HEADERS += $${d}/*.h	
	   SOURCES += $${d}/*.cpp
       }
}

for(m, GT_MODULES){
       d = $${GT_DIR}/$${m}
       exists($${d}){
	   HEADERS += $${d}/*.h	
	   SOURCES += $${d}/*.cpp
       }
}

for(m, GD_MODULES){
       d = $${GD_DIR}/$${m}
       exists($${d}){
	   HEADERS += $${d}/*.h	
	   SOURCES += $${d}/*.cpp
       }
}


################################################################
# autotest generation (internal profiling usage)               
################################################################
ARD_AUTOTEST=$$(ARD_AUTOTEST)
if(!isEmpty( ARD_AUTOTEST )){
    HEADERS += $${G_DIR}/AUTOTEST/*.h
    SOURCES += $${G_DIR}/AUTOTEST/*.cpp
    HEADERS += $${GM_DIR}/AUTOTEST/*.h $${GT_DIR}/AUTOTEST/*.h $${GD_DIR}/AUTOTEST/*.h
    SOURCES += $${GM_DIR}/AUTOTEST/*.cpp $${GT_DIR}/AUTOTEST/*.cpp $${GD_DIR}/AUTOTEST/*.cpp
    DEFINES += API_QT_AUTOTEST
}
################################################################


INCLUDEPATH += $${SRC_DIR}

