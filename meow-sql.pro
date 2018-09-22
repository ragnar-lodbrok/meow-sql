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
unix:LIBS += -lpthread # pthread
unix:LIBS += -lrt
unix:LIBS += -lmysqlclient # mysql client
win32:LIBS += -l"$$PWD\third_party\libmysql\windows\libmysql"
unix:LIBS += -lz # zlib - compression/decompression library
unix:LIBS += -lm # math?
unix:LIBS += -ldl # dynamic link

SOURCES += main.cpp\
    app/actions.cpp \
    app/app.cpp \
    db/connection.cpp \
    db/connection_parameters.cpp \
    db/connection_params_manager.cpp \
    db/connections_manager.cpp \
    db/data_type/data_type.cpp \
    db/entity/database_entity.cpp \
    db/entity/entities_fetcher.cpp \
    db/entity/entity.cpp \
    db/entity/entity_filter.cpp \
    db/entity/entity_holder.cpp \
    db/entity/entity_list_for_database.cpp \
    db/entity/function_entity.cpp \
    db/entity/mysql_entities_fetcher.cpp \
    db/entity/mysql_entity_filter.cpp \
    db/entity/procedure_entity.cpp \
    db/entity/session_entity.cpp \
    db/entity/table_entity_comparator.cpp \
    db/entity/table_entity.cpp \
    db/entity/trigger_entity.cpp \
    db/entity/view_entity.cpp \
    db/exception.cpp \
    db/foreign_key.cpp \
    db/mysql_collation_fetcher.cpp \
    db/mysql_connection.cpp \
    db/mysql_query.cpp \
    db/mysql_query_data_fetcher.cpp \
    db/mysql_table_editor.cpp \
    db/mysql_table_engines_fetcher.cpp \
    db/query.cpp \
    db/query_criteria.cpp \
    db/query_data.cpp \
    db/query_data_fetcher.cpp \
    db/table_column.cpp \
    db/table_editor.cpp \
    db/table_index.cpp \
    db/table_structure.cpp \
    db/table_structure_parser.cpp \
    db/user_query/batch_executor.cpp \
    db/user_query/sentences_parser.cpp \
    db/user_query/user_query.cpp \
    helpers/formatting.cpp \
    helpers/parsing.cpp \
    models/db/base_data_table_model.cpp \
    models/db/connection_params_model.cpp \
    models/db/database_entities_table_model.cpp \
    models/db/databases_table_model.cpp \
    models/db/data_table_model.cpp \
    models/db/entities_tree_model.cpp \
    models/db/table_columns_model.cpp \
    models/delegates/checkbox_delegate.cpp \
    models/delegates/foreign_key_columns_delegate.cpp \
    models/delegates/foreign_key_foreign_columns_delegate.cpp \
    models/delegates/foreign_key_reference_option_delegate.cpp \
    models/delegates/foreign_key_reference_table_delegate.cpp \
    models/delegates/table_column_collation_delegate.cpp \
    models/delegates/table_column_default_delegate.cpp \
    models/delegates/table_column_type_delegate.cpp \
    models/delegates/table_index_delegate.cpp \
    models/forms/connection_parameters_form.cpp \
    models/forms/table_foreign_keys_model.cpp \
    models/forms/table_indexes_model.cpp \
    models/forms/table_indexes_model_item.cpp \
    models/forms/table_info_form.cpp \
    models/ui/central_right_host_widget_model.cpp \
    models/ui/central_right_widget_model.cpp \
    models/ui/table_info_widget_model.cpp \
    settings/settings_core.cpp \
    settings/settings_geometry.cpp \
    settings/settings_icons.cpp \
    settings/settings_text.cpp \
    ui/common/data_type_combo_box.cpp \
    ui/common/sql_editor.cpp \
    ui/common/sql_syntax_highlighter.cpp \
    ui/common/table_column_default_editor.cpp \
    ui/common/table_view.cpp \
    ui/main_window/central_left_db_tree.cpp \
    ui/main_window/central_left_widget.cpp \
    ui/main_window/central_right/database/central_right_database_tab.cpp \
    ui/main_window/central_right/data/central_right_data_tab.cpp \
    ui/main_window/central_right/host/central_right_host_tab.cpp \
    ui/main_window/central_right/host/cr_host_databases_tab.cpp \
    ui/main_window/central_right/query/central_right_query_tab.cpp \
    ui/main_window/central_right/query/cr_query_data_tab.cpp \
    ui/main_window/central_right/query/cr_query_panel.cpp \
    ui/main_window/central_right/query/cr_query_result.cpp \
    ui/main_window/central_right/table/central_right_table_tab.cpp \
    ui/main_window/central_right/table/cr_table_columns.cpp \
    ui/main_window/central_right/table/cr_table_columns_tools.cpp \
    ui/main_window/central_right/table/cr_table_foreign_keys_tools.cpp \
    ui/main_window/central_right/table/cr_table_indexes_tools.cpp \
    ui/main_window/central_right/table/cr_table_info_basic_tab.cpp \
    ui/main_window/central_right/table/cr_table_info.cpp \
    ui/main_window/central_right/table/cr_table_info_foreign_keys_tab.cpp \
    ui/main_window/central_right/table/cr_table_info_indexes_tab.cpp \
    ui/main_window/central_right/table/cr_table_info_options_tab.cpp \
    ui/main_window/central_right_widget.cpp \
    ui/main_window/central_widget.cpp \
    ui/main_window/main_window.cpp \
    ui/session_manager/session_form.cpp \
    ui/session_manager/settings_tab.cpp \
    ui/session_manager/start_tab.cpp \
    ui/session_manager/window.cpp \
    db/editable_grid_data.cpp \
    db/query_data_editor.cpp \
    ui/common/editable_data_table_view.cpp

HEADERS  +=  app/actions.h \
    app/app.h \
    db/collation_fetcher.h \
    db/common.h \
    db/connection.h \
    db/connection_parameters.h \
    db/connection_params_manager.h \
    db/connections_manager.h \
    db/data_type/data_type_category.h \
    db/data_type/data_type.h \
    db/data_type/mysql_data_type.h \
    db/entity/database_entity.h \
    db/entity/entities_fetcher.h \
    db/entity/entity_filter.h \
    db/entity/entity.h \
    db/entity/entity_holder.h \
    db/entity/entity_list_for_database.h \
    db/entity/function_entity.h \
    db/entity/mysql_entities_fetcher.h \
    db/entity/mysql_entity_filter.h \
    db/entity/procedure_entity.h \
    db/entity/session_entity.h \
    db/entity/table_entity_comparator.h \
    db/entity/table_entity.h \
    db/entity/trigger_entity.h \
    db/entity/view_entity.h \
    db/exception.h \
    db/foreign_key.h \
    db/mysql_collation_fetcher.h \
    db/mysql_connection.h \
    db/mysql_query_data_fetcher.h \
    db/mysql_query.h \
    db/mysql_query_result.h \
    db/mysql_table_editor.h \
    db/mysql_table_engines_fetcher.h \
    db/native_query_result_interface.h \
    db/query_column.h \
    db/query_criteria.h \
    db/query_data_fetcher.h \
    db/query_data.h \
    db/query.h \
    db/table_column.h \
    db/table_editor.h \
    db/table_engines_fetcher.h \
    db/table_index.h \
    db/table_structure.h \
    db/table_structure_parser.h \
    db/user_query/batch_executor.h \
    db/user_query/sentences_parser.h \
    db/user_query/user_query.h \
    helpers/formatting.h \
    helpers/parsing.h \
    models/db/base_data_table_model.h \
    models/db/connection_params_model.h \
    models/db/database_entities_table_model.h \
    models/db/databases_table_model.h \
    models/db/data_table_model.h \
    models/db/entities_tree_model.h \
    models/db/table_columns_model.h \
    models/delegates/checkbox_delegate.h \
    models/delegates/foreign_key_columns_delegate.h \
    models/delegates/foreign_key_foreign_columns_delegate.h \
    models/delegates/foreign_key_reference_option_delegate.h \
    models/delegates/foreign_key_reference_table_delegate.h \
    models/delegates/table_column_collation_delegate.h \
    models/delegates/table_column_default_delegate.h \
    models/delegates/table_column_type_delegate.h \
    models/delegates/table_index_delegate.h \
    models/forms/connection_parameters_form.h \
    models/forms/table_foreign_keys_model.h \
    models/forms/table_indexes_model.h \
    models/forms/table_indexes_model_item.h \
    models/forms/table_info_form.h \
    models/ui/central_right_host_widget_model.h \
    models/ui/central_right_widget_model.h \
    models/ui/table_info_widget_model.h \
    settings/settings_core.h \
    settings/settings_geometry.h \
    settings/settings_icons.h \
    settings/settings_text.h \
    ui/common/data_type_combo_box.h \
    ui/common/mysql_syntax.h \
    ui/common/sql_editor.h \
    ui/common/sql_syntax_highlighter.h \
    ui/common/table_column_default_editor.h \
    ui/common/table_view.h \
    ui/main_window/central_left_db_tree.h \
    ui/main_window/central_left_widget.h \
    ui/main_window/central_right/database/central_right_database_tab.h \
    ui/main_window/central_right/data/central_right_data_tab.h \
    ui/main_window/central_right/host/central_right_host_tab.h \
    ui/main_window/central_right/host/cr_host_databases_tab.h \
    ui/main_window/central_right/query/central_right_query_tab.h \
    ui/main_window/central_right/query/cr_query_data_tab.h \
    ui/main_window/central_right/query/cr_query_panel.h \
    ui/main_window/central_right/query/cr_query_result.h \
    ui/main_window/central_right/table/central_right_table_tab.h \
    ui/main_window/central_right/table/cr_table_columns.h \
    ui/main_window/central_right/table/cr_table_columns_tools.h \
    ui/main_window/central_right/table/cr_table_foreign_keys_tools.h \
    ui/main_window/central_right/table/cr_table_indexes_tools.h \
    ui/main_window/central_right/table/cr_table_info_basic_tab.h \
    ui/main_window/central_right/table/cr_table_info_foreign_keys_tab.h \
    ui/main_window/central_right/table/cr_table_info.h \
    ui/main_window/central_right/table/cr_table_info_indexes_tab.h \
    ui/main_window/central_right/table/cr_table_info_options_tab.h \
    ui/main_window/central_right_widget.h \
    ui/main_window/central_widget.h \
    ui/main_window/main_window.h \
    ui/session_manager/session_form.h \
    ui/session_manager/settings_tab.h \
    ui/session_manager/start_tab.h \
    ui/session_manager/window.h \
    db/editable_grid_data.h \
    db/query_data_editor.h \
    ui/common/editable_data_table_view.h


win32:INCLUDEPATH += "$$PWD\third_party\libmysql\windows\include"
unix:INCLUDEPATH += /usr/include/mysql

RESOURCES += \
    icons.qrc

win32:RC_ICONS += meowsqlico.ico
