#-------------------------------------------------
#
# Project created by QtCreator 2017-07-13T15:14:21
#
#-------------------------------------------------

QT       += core gui sql xlsx printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SharedMemo
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    configurationdialog.cpp \
    tableprinter.cpp

HEADERS  += mainwindow.h \
    configurationdialog.h \
    define.h \
    tableprinter.h

FORMS    += mainwindow.ui \
    configurationdialog.ui

TRANSLATIONS += Lang_ko_KR.ts\
                Lang_en_US.ts

RESOURCES += \
    sharedmemo.qrc

win32
{
   RC_FILE = SharedMemo.rc
   CONFIG += embed_manifest_exe
   QMAKE_LFLAGS_WINDOWS += /MANIFESTUAC:level=\'requireAdministrator\'
}
