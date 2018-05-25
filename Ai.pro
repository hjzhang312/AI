TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    src/thread.cpp \
    src/capdevices.cpp

HEADERS += \
    src/singleton.h \
    src/thread.h \
    src/capdevices.h \
    inc/capdevices.h \
    inc/singleton.h \
    inc/thread.h


INCLUDEPATH += inc
DEPENDPATH += inc src

LIBS += -lasound -lpthread

