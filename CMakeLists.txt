# see http://doc.qt.io/qt-5/cmake-manual.html
# https://github.com/rhtyd/cmakeqt/blob/master/CMakeLists.txt
# https://github.com/district10/cmake-templates

cmake_minimum_required(VERSION 3.1)

#set(QT_MINIMUM_VERSION 5.0.0) # TODO

project(meowsql)
set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 4)
set(PROJECT_VERSION_PATCH 8)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON) # TODO

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11) # TODO 14 for Qt >= 5.5

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Sql REQUIRED)

add_definitions(-DYY_NO_UNISTD_H) # fix flex compilation on win

option(WITH_MYSQL "MySQL support" ON)
option(WITH_POSTGRESQL "PostgreSQL support" ON)
option(WITH_SQLITE "SQLite support" ON)
option(WITH_QTSQL "Qt SQL Module" ON)
option(WITH_LIBSSH "Use libssh" OFF) # not finished

if(WITH_POSTGRESQL)
    add_definitions(-DWITH_POSTGRESQL)
endif()

if(WITH_SQLITE)
    add_definitions(-DWITH_SQLITE)
endif()

if(WITH_MYSQL)
    add_definitions(-DWITH_MYSQL)
endif()

if(WITH_QTSQL)
    add_definitions(-DWITH_QTSQL)
endif()

if(WITH_LIBSSH)
    add_definitions(-DWITH_LIBSSH)
endif()

set(HEADER_FILES
    db/data_type/connection_data_types.h
    db/data_type/data_type_category.h
    db/native_query_result_interface.h
    db/collation_fetcher.h
    db/common.h
    db/query_column.h
    ssh/ssh_tunnel_interface.h
    ui/common/mysql_syntax.h
    ui/main_window/central_right/global_data_filter_interface.h
)

if(WITH_MYSQL)
    list(APPEND HEADER_FILES
        db/data_type/mysql_data_type.h
        db/mysql/mysql_query_result.h
    )
endif()

if(WITH_POSTGRESQL)
    list(APPEND HEADER_FILES
        db/data_type/pg_data_type.h
        db/pg/pg_query_result.h
    )
endif()

if(WITH_QTSQL)
    list(APPEND HEADER_FILES
        db/qtsql/qtsql_query_result.h
    )
endif()

set(SOURCE_FILES
    main.cpp
    app/actions.cpp
    app/app.cpp
    app/log.cpp
    db/connection.cpp
    db/connection_features.cpp
    db/connection_parameters.cpp
    db/connection_params_manager.cpp
    db/connections_manager.cpp
    db/database_editor.cpp
    db/data_type/data_type.cpp
    db/editable_grid_data.cpp
    db/entity/database_entity.cpp
    db/entity/entities_fetcher.cpp
    db/entity/entity.cpp
    db/entity/entity_filter.cpp
    db/entity/entity_holder.cpp
    db/entity/entity_list_for_database.cpp
    db/entity/routine_entity.cpp
    db/entity/session_entity.cpp
    db/entity/table_entity_comparator.cpp
    db/entity/table_entity.cpp
    db/entity/trigger_entity.cpp
    db/entity/view_entity.cpp
    db/exception.cpp
    db/foreign_key.cpp
    db/query.cpp
    db/query_criteria.cpp
    db/query_data.cpp
    db/query_data_editor.cpp
    db/query_data_fetcher.cpp
    db/routine_editor.cpp
    db/routine_structure_parser.cpp
    db/routine_structure.cpp
    db/session_variables.cpp
    db/table_column.cpp
    db/table_editor.cpp
    db/table_index.cpp
    db/table_structure.cpp
    db/table_structure_parser.cpp
    db/trigger_structure_parser.cpp
    db/trigger_structure.cpp
    db/view_editor.cpp
    db/view_structure.cpp
    db/view_structure_parser.cpp
    db/user_manager.cpp
    db/user_query/batch_executor.cpp
    db/user_query/sentences_parser.cpp
    db/user_query/user_query.cpp
    helpers/formatting.cpp
    helpers/logger.cpp
    helpers/parsing.cpp
    helpers/text.cpp
    models/db/base_data_table_model.cpp
    models/db/connection_params_model.cpp
    models/db/database_entities_table_model.cpp
    models/db/databases_table_model.cpp
    models/db/data_table_model.cpp
    models/db/entities_tree_model.cpp
    models/db/table_columns_model.cpp
    models/db/routine_parameters_model.cpp
    models/db/query_data_sort_filter_proxy_model.cpp
    models/db/variables_table_model.cpp
    models/delegates/checkbox_delegate.cpp
    models/delegates/combobox_delegate.cpp
    models/delegates/edit_query_data_delegate.cpp
    models/delegates/line_edit_item_editor_wrapper.cpp
    models/delegates/date_time_item_editor_wrapper.cpp
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
    models/forms/export_database_form.cpp
    models/forms/routine_form.cpp
    models/forms/table_foreign_keys_model.cpp
    models/forms/table_indexes_model.cpp
    models/forms/table_indexes_model_item.cpp
    models/forms/table_info_form.cpp
    models/forms/trigger_form.cpp
    models/forms/text_editor_popup_form.cpp
    models/forms/view_form.cpp
    models/ui/central_right_host_widget_model.cpp
    models/ui/central_right_widget_model.cpp
    models/ui/table_info_widget_model.cpp
    models/ui/routine_info_widget_model.cpp
    settings/settings_core.cpp
    settings/settings_geometry.cpp
    settings/settings_icons.cpp
    settings/settings_text.cpp
    settings/data_editors.cpp
    ssh/openssh_tunnel.cpp
    ssh/ssh_tunnel_parameters.cpp
    ui/common/checkbox_list_popup.cpp
    ui/common/data_type_combo_box.cpp
    ui/common/geometry_helpers.cpp
    ui/common/editable_data_table_view.cpp
    ui/common/sql_editor.cpp
    ui/common/sql_log_editor.cpp
    ui/common/sql_syntax_highlighter.cpp
    ui/common/table_cell_line_edit.cpp
    ui/common/table_column_default_editor.cpp
    ui/common/table_view.cpp
    ui/common/text_editor_popup.cpp
    ui/edit_database/dialog.cpp
    ui/export_database/bottom_widget.cpp
    ui/export_database/export_dialog.cpp
    ui/export_database/top_widget.cpp
    ui/main_window/central_bottom_widget.cpp
    ui/main_window/central_left_db_tree.cpp
    ui/main_window/central_left_widget.cpp
    ui/main_window/central_log_widget.cpp
    ui/main_window/central_right/database/central_right_database_tab.cpp
    ui/main_window/central_right/data/central_right_data_tab.cpp
    ui/main_window/central_right/host/central_right_host_tab.cpp
    ui/main_window/central_right/host/cr_host_databases_tab.cpp
    ui/main_window/central_right/host/cr_host_variables_tab.cpp
    ui/main_window/central_right/routine/central_right_routine_tab.cpp
    ui/main_window/central_right/routine/cr_routine_body.cpp
    ui/main_window/central_right/routine/cr_routine_info.cpp
    ui/main_window/central_right/routine/cr_routine_info_options_tab.cpp
    ui/main_window/central_right/routine/cr_routine_info_parameters_tab.cpp
    ui/main_window/central_right/routine/cr_routine_parameters_tools.cpp
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
    ui/main_window/central_right/trigger/central_right_trigger_tab.cpp
    ui/main_window/central_right/trigger/cr_trigger_body.cpp
    ui/main_window/central_right/trigger/cr_trigger_options.cpp
    ui/main_window/central_right/view/central_right_view_tab.cpp
    ui/main_window/central_right/filter_widget.cpp
    ui/main_window/central_right_widget.cpp
    ui/main_window/central_widget.cpp
    ui/main_window/main_window.cpp
    ui/main_window/main_window_status_bar.cpp
    ui/session_manager/session_form.cpp
    ui/session_manager/settings_tab.cpp
    ui/session_manager/ssh_tunnel_tab.cpp
    ui/session_manager/start_tab.cpp
    ui/session_manager/window.cpp
    utils/exporting/mysql_dump_console.cpp
)

if (WITH_MYSQL)
    list(APPEND SOURCE_FILES

        db/data_type/mysql_connection_data_types.cpp

        db/entity/mysql_entity_filter.cpp

        db/mysql/mysql_database_editor.cpp
        db/mysql/mysql_entities_fetcher.cpp
        db/mysql/mysql_query.cpp
        db/mysql/mysql_query_data_editor.cpp
        db/mysql/mysql_collation_fetcher.cpp
        db/mysql/mysql_connection.cpp
        db/mysql/mysql_query_data_fetcher.cpp
        db/mysql/mysql_table_editor.cpp
        db/mysql/mysql_table_engines_fetcher.cpp

    )
endif()

if (WITH_POSTGRESQL)
    list(APPEND SOURCE_FILES

        db/data_type/pg_connection_data_types.cpp

        db/pg/pg_connection.cpp
        db/pg/pg_entities_fetcher.cpp
        db/pg/pg_entity_create_code_generator.cpp
        db/pg/pg_query.cpp
        db/pg/pg_query_data_editor.cpp
        db/pg/pg_query_data_fetcher.cpp
    )
endif()

if (WITH_SQLITE)
    list(APPEND SOURCE_FILES

        db/data_type/sqlite_connection_datatypes.cpp

        db/sqlite/sqlite_connection.cpp
        db/sqlite/sqlite_entities_fetcher.cpp
        db/sqlite/sqlite_table_structure_parser.cpp

        utils/sql_parser/sqlite/sqlite_parser.cpp
        utils/sql_parser/sqlite/sqlite_bison_parser.cpp
        utils/sql_parser/sqlite/sqlite_flex_lexer.cpp
        utils/sql_parser/sqlite/sqlite_types.cpp
    )
endif()

if(WITH_QTSQL)
    list(APPEND SOURCE_FILES
        db/qtsql/qtsql_query.cpp
    )
endif()

if(WITH_LIBSSH)
    list(APPEND SOURCE_FILES
        ssh/libssh_tunnel.cpp
    )
endif()

qt5_add_resources(RESOURCE_FILES icons.qrc)

if (WIN32)
    list(APPEND RESOURCE_FILES meowsql.rc)
endif()

if (WIN32)
    SET(EXE_TYPE WIN32)
endif()
if (APPLE)
    SET(EXE_TYPE MACOSX_BUNDLE)
endif()

add_executable(meowsql
    ${EXE_TYPE}
    ${HEADER_FILES}
    ${SOURCE_FILES}
    ${RESOURCE_FILES})

target_link_libraries(meowsql Qt5::Widgets)

if(WITH_QTSQL)
    target_link_libraries(meowsql Qt5::Sql)
endif()

if (WIN32)
    target_link_libraries(meowsql User32) #SetProcessDPIAware()
endif()

# MySQL ---------------------------------------------

if (WITH_MYSQL)

    set(MEOW_MYSQL_CLIENT_LIB mysqlclient) #linux

    if (WIN32)
        # find mysql manually
        list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
        find_package(MySQL REQUIRED)
        include_directories(${MYSQL_INCLUDE_DIR})
        set(MEOW_MYSQL_CLIENT_LIB ${MYSQL_LIBRARY})
    endif()

endif() # if (WITH_MYSQL)

# PG --------------------------------------------------
# ubuntu: apt-get install libpq-dev postgresql-server-dev-all
# win: https://www.enterprisedb.com/downloads/postgres-postgresql-downloads (10.7 32bit)

if(WITH_POSTGRESQL)

if (WIN32)
    # find postgres in third_party
    set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libpq/windows/include")

    #  PostgreSQL_INCLUDE_DIR  - the path to where the PostgreSQL include files are.
    set (PostgreSQL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libpq/windows/include")

    #  PostgreSQL_LIBRARY_DIR  - The path to where the PostgreSQL library files are.
    set (PostgreSQL_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libpq/windows/lib")

endif()


find_package(PostgreSQL REQUIRED)

include_directories(meowsql ${PostgreSQL_INCLUDE_DIRS})

message ("PostgreSQL_VERSION_STRING = ${PostgreSQL_VERSION_STRING}")
message ("PostgreSQL_INCLUDE_DIRS = ${PostgreSQL_INCLUDE_DIRS}")
message ("PostgreSQL_LIBRARY_DIRS = ${PostgreSQL_LIBRARY_DIRS}")
message ("PostgreSQL_LIBRARIES = ${PostgreSQL_LIBRARIES}")

endif() # if(WITH_POSTGRESQL)

# PG end ----------------------------------------------

if(WITH_LIBSSH)
    # https://www.libssh.org/2019/11/07/libssh-0-9-2/
    find_package(libssh REQUIRED)
    message ("LIBSSH_INCLUDE_DIR = ${LIBSSH_INCLUDE_DIR}")
    message ("LIBSSH_LIBRARIES = ${LIBSSH_LIBRARIES}")

    target_link_libraries(meowsql pthread ssh)

endif()

if(WITH_MYSQL)
    target_link_libraries(meowsql
        ${MEOW_MYSQL_CLIENT_LIB}
    )
endif()

if(WITH_POSTGRESQL)
    target_link_libraries(meowsql
        ${PostgreSQL_LIBRARIES}
    )
endif()

if(UNIX)

    if(NOT DEFINED CMAKE_INSTALL_DATAROOTDIR)
        set(CMAKE_INSTALL_DATAROOTDIR "share")
    endif()

    install(TARGETS ${PROJECT_NAME}
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib)

    install(FILES meowsql.png
        DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/256x256/apps/")

    install(FILES meowsql.svg
            DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/scalable/apps/"
            RENAME ${PROJECT_NAME}.svg)

    install(FILES meowsql.desktop
            DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications/")

endif(UNIX)
