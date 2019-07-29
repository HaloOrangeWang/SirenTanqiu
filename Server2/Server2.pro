TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        game.cpp \
        main.cpp \
        portocol.cpp \
        server.cpp \
        variables.cpp

LIBS += -levent

HEADERS += \
    game.h \
    portocol.h \
    server.h \
    variables.h
