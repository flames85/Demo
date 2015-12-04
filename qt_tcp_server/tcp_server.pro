#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T09:14:58
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = tcp_server
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

CONFIG    += link_pkgconfig
PKGCONFIG += eventdispatcher_epoll


SOURCES += main.cpp \
    myqtcpserver.cpp

HEADERS += \
    myqtcpserver.h
