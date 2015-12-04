#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T09:11:51
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = tcp_client
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


CONFIG    += link_pkgconfig
PKGCONFIG += eventdispatcher_epoll
PKGCONFIG += eventdispatcher_libev

SOURCES += main.cpp \
    mytcpclient.cpp

HEADERS += \
    mytcpclient.h
