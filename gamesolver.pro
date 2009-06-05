# -------------------------------------------------
# Project created by QtCreator 2009-06-04T16:09:26
# -------------------------------------------------
QT += script \
    network \
    webkit
TARGET = GameSolver
TEMPLATE = app
SOURCES += main.cpp \
    GameSolver.cpp \
    ScreenCapture.cpp \
    InputUtils.cpp \
    ScriptEnvironment.cpp \
    ScriptObjects_p.cpp \
    HueClassifier.cpp
HEADERS += GameSolver.h \
    ScreenCapture.h \
    InputUtils.h \
    ScriptEnvironment.h \
    ScriptObjects_p.h \
    HueClassifier.h
FORMS += GameSolverGUI.ui
unix:LIBS += -lXtst
win32:CONFIG += console
OBJECTS_DIR = .obj
MOC_DIR = .obj
RCC_DIR = .obj
UI_DIR = .obj
OTHER_FILES += testscript.js
