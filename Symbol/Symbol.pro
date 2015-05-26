CONFIG -= qt
CONFIG += c++11

TARGET = Symbol
TEMPLATE = lib

DEFINES += SYMBOL_LIBRARY

SOURCES += symbol.cpp \
    exportedsymbolvisitor.cpp \
    compilationunitsymbolcollection.cpp

HEADERS += symbol.h\
        symbol_global.h \
    exportedsymbolvisitor.h \
    compilationunitsymbolcollection.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Parser/release/ -lParser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Parser/debug/ -lParser
else:unix: LIBS += -L$$OUT_PWD/../Parser/ -lParser

INCLUDEPATH += $$PWD/../Parser
DEPENDPATH += $$PWD/../Parser
