QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
#CONFIG += console


# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    SANDM/virtualmachine.cpp \
    console.cpp \
    highlight.cpp \
    linenumberarea.cpp \
    main.cpp \
    mainwindow.cpp \
    SANDM/CAsm.cpp \
    SANDM/CNameTable.cpp \
    SANDM/Compiler.cpp \
    SANDM/growbuf.cpp \
    SANDM/LineParser.cpp \
    memorymonitor.cpp \
    prompttosave.cpp \
    workspace.cpp

HEADERS += \
    SANDM/virtualmachine.h \
    console.h \
    highlight.h \
    linenumberarea.h \
    mainwindow.h \
    SANDM/byte_code.h \
    SANDM/CAsm.h \
    SANDM/CNameTable.h \
    SANDM/Compiler.h \
    SANDM/growbuf.h \
    SANDM/LineParser.h \
    SANDM/tokens.h \
    memorymonitor.h \
    prompttosave.h \
    workspace.h

FORMS += \
    mainwindow.ui \
    prompttosave.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
