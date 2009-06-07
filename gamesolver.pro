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
    HueClassifier.cpp \
    bindings/cv/Capture.cpp \
    bindings/cv/Image.cpp \
    bindings/cv/classifiers/Color.cpp
HEADERS += GameSolver.h \
    ScreenCapture.h \
    InputUtils.h \
    ScriptEnvironment.h \
    HueClassifier.h \
    bindings/cv/Capture.h \
    bindings/cv/Image.h \
    bindings/cv/classifiers/Color.h \
    bindings/Bindings.h
FORMS += GameSolverGUI.ui
unix:LIBS += -lXtst
win32:CONFIG += console
OBJECTS_DIR = .obj
MOC_DIR = .obj
RCC_DIR = .obj
UI_DIR = .obj
OTHER_FILES += testscript.js
