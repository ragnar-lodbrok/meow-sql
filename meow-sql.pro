#-------------------------------------------------
#
# Project created by QtCreator 2016-03-17T23:13:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = meow-sql
TEMPLATE = app

greaterThan(QT_MINOR_VERSION, 4) { # >= 5.5
    CONFIG  += c++14
} else {
    CONFIG  += c++11
}

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
    models/ui/central_right_host_widget_model.cpp \
    models/db/databases_table_model.cpp \
    helpers/formatting.cpp \
    ui/main_window/central_right/database/central_right_database_tab.cpp \
    models/db/database_entities_table_model.cpp \
    helpers/parsing.cpp \
    ui/main_window/central_right/table/central_right_table_tab.cpp \
    ui/main_window/central_right/data/central_right_data_tab.cpp \
    models/db/data_table_model.cpp \
    db/query_criteria.cpp \
    db/query_data.cpp \
    db/query_data_fetcher.cpp \
    db/mysql_query_data_fetcher.cpp \
    db/entity/entity_holder.cpp \
    settings/settings_core.cpp \
    settings/settings_text.cpp \
    db/data_type/data_type.cpp \
    ui/main_window/central_right/query/central_right_query_tab.cpp \
    ui/main_window/central_right/query/cr_query_panel.cpp \
    ui/main_window/central_right/query/cr_query_result.cpp \
    db/user_query/sentences_parser.cpp \
    db/user_query/batch_executor.cpp \
    db/user_query/user_query.cpp \
    ui/main_window/central_right/query/cr_query_data_tab.cpp \
    models/db/base_data_table_model.cpp \
    ui/common/sql_editor.cpp

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
    models/ui/central_right_host_widget_model.h \
    models/db/databases_table_model.h \
    helpers/formatting.h \
    ui/main_window/central_right/database/central_right_database_tab.h \
    models/db/database_entities_table_model.h \
    helpers/parsing.h \
    ui/main_window/central_right/table/central_right_table_tab.h \
    ui/main_window/central_right/data/central_right_data_tab.h \
    models/db/data_table_model.h \
    db/query_criteria.h \
    db/query_data.h \
    db/query_data_fetcher.h \
    db/mysql_query_data_fetcher.h \
    db/entity/entity_holder.h \
    db/data_type/data_type.h \
    settings/settings_core.h \
    db/data_type/data_type_category.h \
    settings/settings_text.h \
    db/query_column.h \
    db/data_type/mysql_data_type.h \
    ui/main_window/central_right/query/central_right_query_tab.h \
    ui/main_window/central_right/query/cr_query_panel.h \
    ui/main_window/central_right/query/cr_query_result.h \
    db/user_query/sentences_parser.h \
    db/user_query/batch_executor.h \
    db/user_query/user_query.h \
    ui/main_window/central_right/query/cr_query_data_tab.h \
    models/db/base_data_table_model.h \
    ui/common/sql_editor.h


QMAKE_CFLAGS += -I/usr/include/mysql

RESOURCES += \
    icons.qrc
