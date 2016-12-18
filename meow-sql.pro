#-------------------------------------------------
#
# Project created by QtCreator 2016-03-17T23:13:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = meow-sql
TEMPLATE = app

CONFIG  += c++14

QMAKE_LFLAGS += -Wl,--no-as-needed # ?

# (mysql_config --libs)
LIBS += -lpthread # pthread
LIBS += -lrt
LIBS += -lmysqlclient # mysql client
LIBS += -lz # zlib - compression/decompression library
LIBS += -lm # math?
LIBS += -ldl # dynamic link

SOURCES += main.cpp\
    app.cpp \
    ui/session_manager/start_tab.cpp \
    ui/session_manager/window.cpp \
    ui/session_manager/settings_tab.cpp \
    ui/session_manager/session_form.cpp \
    models/db/connection_params_model.cpp \
    models/forms/connection_parameters_form.cpp \
    db/connection_parameters.cpp \
    db/connection_params_manager.cpp \
    db/connection.cpp \
    db/mysql_connection.cpp \
    db/query.cpp \
    db/mysql_query.cpp \
    db/exception.cpp \
    ui/main_window/central_widget.cpp \
    ui/main_window/main_window.cpp \
    ui/main_window/central_left_widget.cpp \
    ui/main_window/central_right_widget.cpp

HEADERS  += app.h \
    ui/session_manager/session_form.h \
    ui/session_manager/start_tab.h \
    ui/session_manager/window.h \
    ui/session_manager/settings_tab.h \
    models/db/connection_params_model.h \
    models/forms/connection_parameters_form.h \
    db/connection_parameters.h \
    db/connection_params_manager.h \
    db/connection.h \
    db/mysql_connection.h \
    db/query.h \
    db/mysql_query.h \
    db/exception.h \
    db/common.h \
    ui/main_window/central_widget.h \
    ui/main_window/main_window.h \
    ui/main_window/central_left_widget.h \
    ui/main_window/central_right_widget.h


QMAKE_CFLAGS += -I/usr/include/mysql

RESOURCES += \
    icons.qrc
