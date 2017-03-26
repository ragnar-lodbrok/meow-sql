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
    ui/main_window/central_right_widget.cpp \
    db/connections_manager.cpp \
    db/entity/entity.cpp \
    models/db/entities_tree_model.cpp \
    db/entity/session_entity.cpp \
    db/entity/database_entity.cpp \
    db/entity/entity_list_for_database.cpp \
    db/entity/table_entity.cpp \
    db/entity/entities_fetcher.cpp \
    db/entity/mysql_entities_fetcher.cpp \
    db/entity/view_entity.cpp \
    db/entity/function_entity.cpp \
    db/entity/procedure_entity.cpp \
    db/entity/trigger_entity.cpp \
    ui/main_window/central_right/host/central_right_host_tab.cpp \
    models/ui/central_right_widget_model.cpp \
    ui/main_window/central_right/host/cr_host_databases_tab.cpp \
    models/ui/central_right_host_widget_model.cpp

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
    ui/main_window/central_right_widget.h \
    db/connections_manager.h \
    db/entity/entity.h \
    models/db/entities_tree_model.h \
    db/entity/session_entity.h \
    db/entity/database_entity.h \
    db/entity/entity_list_for_database.h \
    db/entity/table_entity.h \
    db/entity/entities_fetcher.h \
    db/entity/mysql_entities_fetcher.h \
    db/entity/view_entity.h \
    db/entity/function_entity.h \
    db/entity/procedure_entity.h \
    db/entity/trigger_entity.h \
    ui/main_window/central_right/host/central_right_host_tab.h \
    models/ui/central_right_widget_model.h \
    ui/main_window/central_right/host/cr_host_databases_tab.h \
    models/ui/central_right_host_widget_model.h


QMAKE_CFLAGS += -I/usr/include/mysql

RESOURCES += \
    icons.qrc
