#-------------------------------------------------
#
# Project created by QtCreator 2011-11-12T13:22:30
#
#-------------------------------------------------

QT       += core gui network xml testlib

TARGET = Upnp_av_MediaServer
TEMPLATE = app
CONFIG  += warn_on

INCLUDEPATH += ../include/
LIBS += -L"../lib/hupnp" -lHUpnp \
        -L"../lib/hupnp_av" -lHUpnpAv \
        -L"../lib/other"

win32 {
    debug {
        LIBS += -lQtSolutions_SOAP-2.7d
    }
    else {
        LIBS += -lQtSolutions_SOAP-2.7
    }

    LIBS += -lws2_32

    DESCRIPTIONS = $$PWD\\descriptions
    DESCRIPTIONS = $${replace(DESCRIPTIONS, /, \\)}
    QMAKE_POST_LINK += xcopy $$DESCRIPTIONS bin\\descriptions /E /Y /C /I $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += copy ..\\lib\\hupnp\\* bin /Y $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += copy ..\\lib\\hupnp_av\\* bin /Y $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += copy ..\\lib\\other\\* bin
}
else {
    LIBS += -lQtSolutions_SOAP-2.7
    !macx:QMAKE_LFLAGS += -Wl,--rpath=\\\$\$ORIGIN

    QMAKE_POST_LINK += cp -Rf $$PWD/descriptions bin/descriptions &
    QMAKE_POST_LINK += cp -Rf ../lib/hupnp/* bin &
    QMAKE_POST_LINK += cp -fR ../lib/hupnp_av/* bin &
    QMAKE_POST_LINK += cp -Rf ../lib/other/* bin
}

macx {
  CONFIG -= app_bundle
  #CONFIG += x86 x86_64
}

DESTDIR = ./bin

SOURCES += main.cpp\
        mediaserverwindow.cpp

HEADERS  += mediaserverwindow.h

FORMS    += mediaserverwindow.ui

