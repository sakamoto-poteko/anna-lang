TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Parser/release/ -lParser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Parser/debug/ -lParser
else:unix: LIBS += -L$$OUT_PWD/../Parser/ -lParser

INCLUDEPATH += $$PWD/../Parser
DEPENDPATH += $$PWD/../Parser

LIBS += -lfl
