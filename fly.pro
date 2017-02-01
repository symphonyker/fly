#-------------------------------------------------
#
# Project created by QtCreator 2017-01-30T15:39:36
#
#-------------------------------------------------

QT       += core

QT       -= gui

QMAKE_CXXFLAGS += -std=c++0x

TARGET = fly
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    flights.cpp \
    csvtable.cpp \
    departs.cpp \
    fly.cpp

HEADERS += \
    flights.h \
    csvtable.h \
    departs.h \
    fly.h
