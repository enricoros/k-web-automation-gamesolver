# -------------------------------------------------
# Project created by QtCreator 2009-06-04T16:09:26
# -------------------------------------------------
QT += script \
    webkit
TARGET = bjsolver
TEMPLATE = app
SOURCES += main.cpp \
    GameSolver.cpp \
    ScreenCapture.cpp \
    InputUtils.cpp
HEADERS += GameSolver.h \
    ScreenCapture.h \
    InputUtils.h
FORMS += GameSolverGUI.ui
unix:LIBS += -lXtst
win32:CONFIG += console
OBJECTS_DIR = .obj
MOC_DIR = .obj
RCC_DIR = .obj
UI_DIR = .obj
