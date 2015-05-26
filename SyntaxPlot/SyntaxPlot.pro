TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++1y

SOURCES += main.cpp \
    syntaxplottersyntaxvisitor.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Parser/release/ -lParser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Parser/debug/ -lParser
else:unix: LIBS += -L$$OUT_PWD/../Parser/ -lParser

INCLUDEPATH += $$PWD/../Parser
DEPENDPATH += $$PWD/../Parser

HEADERS += \
    syntaxplottersyntaxvisitor.h
