# see http://doc.qt.io/qt-5/cmake-manual.html
# https://github.com/rhtyd/cmakeqt/blob/master/CMakeLists.txt
# https://github.com/district10/cmake-templates

cmake_minimum_required(VERSION 3.1)

#set(QT_MINIMUM_VERSION 5.0.0) # TODO

project(meowsql)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 2)
set(PROJECT_VERSION_PATCH 0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON) # TODO

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11) # TODO 14 for Qt >= 5.5

find_package(Qt5Widgets CONFIG REQUIRED)

set(HEADER_FILES
    db/collation_fetcher.h
    db/common.h
    db/data_type/data_type_category.h
    db/data_type/mysql_data_type.h
    db/native_query_result_interface.h
    db/query_column.h
    ui/common/mysql_syntax.h
)

set(SOURCE_FILES
    main.cpp
    app/actions.cpp
    app/app.cpp
    app/log.cpp
    db/connection.cpp
    db/connection_parameters.cpp
    db/connection_params_manager.cpp
    db/connections_manager.cpp
    db/database_editor.cpp
    db/data_type/data_type.cpp
    db/entity/database_entity.cpp
    db/entity/entities_fetcher.cpp
    db/entity/entity.cpp
    db/entity/entity_filter.cpp
    db/entity/entity_holder.cpp
    db/entity/entity_list_for_database.cpp
    db/entity/function_entity.cpp
    db/entity/mysql_entities_fetcher.cpp
    db/entity/mysql_entity_filter.cpp
    db/entity/procedure_entity.cpp
    db/entity/session_entity.cpp
    db/entity/table_entity_comparator.cpp
    db/entity/table_entity.cpp
    db/entity/trigger_entity.cpp
    db/entity/view_entity.cpp
    db/exception.cpp
    db/foreign_key.cpp
    db/mysql_collation_fetcher.cpp
    db/mysql_connection.cpp
    db/mysql_query.cpp
    db/mysql_query_data_fetcher.cpp
    db/mysql_table_editor.cpp
    db/mysql_table_engines_fetcher.cpp
    db/query.cpp
    db/query_criteria.cpp
    db/query_data.cpp
    db/query_data_fetcher.cpp
    db/table_column.cpp
    db/table_editor.cpp
    db/table_index.cpp
    db/table_structure.cpp
    db/table_structure_parser.cpp
    db/user_query/batch_executor.cpp
    db/user_query/sentences_parser.cpp
    db/user_query/user_query.cpp
    helpers/formatting.cpp
    helpers/logger.cpp
    helpers/parsing.cpp
    models/db/base_data_table_model.cpp
    models/db/connection_params_model.cpp
    models/db/database_entities_table_model.cpp
    models/db/databases_table_model.cpp
    models/db/data_table_model.cpp
    models/db/entities_tree_model.cpp
    models/db/table_columns_model.cpp
    models/delegates/checkbox_delegate.cpp
    models/delegates/foreign_key_columns_delegate.cpp
    models/delegates/foreign_key_foreign_columns_delegate.cpp
    models/delegates/foreign_key_reference_option_delegate.cpp
    models/delegates/foreign_key_reference_table_delegate.cpp
    models/delegates/table_column_collation_delegate.cpp
    models/delegates/table_column_default_delegate.cpp
    models/delegates/table_column_type_delegate.cpp
    models/delegates/table_index_delegate.cpp
    models/forms/connection_parameters_form.cpp
    models/forms/edit_database_form.cpp
    models/forms/table_foreign_keys_model.cpp
    models/forms/table_indexes_model.cpp
    models/forms/table_indexes_model_item.cpp
    models/forms/table_info_form.cpp
    models/ui/central_right_host_widget_model.cpp
    models/ui/central_right_widget_model.cpp
    models/ui/table_info_widget_model.cpp
    settings/settings_core.cpp
    settings/settings_geometry.cpp
    settings/settings_icons.cpp
    settings/settings_text.cpp
    ui/common/data_type_combo_box.cpp
    ui/common/sql_editor.cpp
    ui/common/sql_log_editor.cpp
    ui/common/sql_syntax_highlighter.cpp
    ui/common/table_column_default_editor.cpp
    ui/common/table_view.cpp
    ui/edit_database/dialog.cpp
    ui/main_window/central_bottom_widget.cpp
    ui/main_window/central_left_db_tree.cpp
    ui/main_window/central_left_widget.cpp
    ui/main_window/central_log_widget.cpp
    ui/main_window/central_right/database/central_right_database_tab.cpp
    ui/main_window/central_right/data/central_right_data_tab.cpp
    ui/main_window/central_right/host/central_right_host_tab.cpp
    ui/main_window/central_right/host/cr_host_databases_tab.cpp
    ui/main_window/central_right/query/central_right_query_tab.cpp
    ui/main_window/central_right/query/cr_query_data_tab.cpp
    ui/main_window/central_right/query/cr_query_panel.cpp
    ui/main_window/central_right/query/cr_query_result.cpp
    ui/main_window/central_right/table/central_right_table_tab.cpp
    ui/main_window/central_right/table/cr_table_columns.cpp
    ui/main_window/central_right/table/cr_table_columns_tools.cpp
    ui/main_window/central_right/table/cr_table_foreign_keys_tools.cpp
    ui/main_window/central_right/table/cr_table_indexes_tools.cpp
    ui/main_window/central_right/table/cr_table_info_basic_tab.cpp
    ui/main_window/central_right/table/cr_table_info.cpp
    ui/main_window/central_right/table/cr_table_info_foreign_keys_tab.cpp
    ui/main_window/central_right/table/cr_table_info_indexes_tab.cpp
    ui/main_window/central_right/table/cr_table_info_options_tab.cpp
    ui/main_window/central_right_widget.cpp
    ui/main_window/central_widget.cpp
    ui/main_window/main_window.cpp
    ui/main_window/main_window_status_bar.cpp
    ui/session_manager/session_form.cpp
    ui/session_manager/settings_tab.cpp
    ui/session_manager/start_tab.cpp
    ui/session_manager/window.cpp
    db/editable_grid_data.cpp
    db/query_data_editor.cpp
    ui/common/editable_data_table_view.cpp
    models/delegates/edit_query_data_delegate.cpp
)

qt5_add_resources(RESOURCE_FILES icons.qrc)

add_executable(meowsql ${HEADER_FILES} ${SOURCE_FILES} ${RESOURCE_FILES})

target_link_libraries(meowsql Qt5::Widgets)

set(MEOW_MYSQL_CLIENT_LIB mysqlclient) #linux

if (WIN32)

    # find mysql manually
    list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    find_package(MySQL REQUIRED)
    include_directories(${MYSQL_INCLUDE_DIR})
    set(MEOW_MYSQL_CLIENT_LIB ${MYSQL_LIBRARY})

endif()

target_link_libraries(meowsql ${MEOW_MYSQL_CLIENT_LIB})
