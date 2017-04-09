QT += core
QT -= gui
QT += network

CONFIG += c++11

TARGET = console-chat
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    myserver.cpp \
    myclient.cpp

HEADERS += \
    myclient.h \
    myserver.h
