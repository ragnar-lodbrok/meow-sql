# see http://doc.qt.io/qt-5/cmake-manual.html
# https://github.com/rhtyd/cmakeqt/blob/master/CMakeLists.txt
# https://github.com/district10/cmake-templates

cmake_minimum_required(VERSION 3.1)

#set(QT_MINIMUM_VERSION 5.0.0) # TODO

project(meowsql VERSION 0.4.18)

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
option(USE_CONAN_IO "Use conan.io package manager" OFF)

if(WIN32)
    set(USE_CONAN_IO ON)
endif()

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
    app/actions.h
    app/app.h
    app/log.h
    app/language.h
    db/collation_fetcher.h
    db/common.h
    db/connection.h
    db/connection_parameters.h
    db/connection_features.h
    db/connection_params_manager.h
    db/connections_manager.h
    db/connection_query_killer.h
    db/database_editor.h
    db/data_type/connection_data_types.h
    db/data_type/data_type_category.h
    db/data_type/data_type.h
    db/entity/database_entity.h
    db/entity/entities_fetcher.h
    db/entity/entity_filter.h
    db/entity/entity.h
    db/entity/entity_holder.h
    db/entity/entity_factory.h
    db/entity/routine_entity.h
    db/entity/session_entity.h
    db/entity/table_entity_comparator.h
    db/entity/table_entity.h
    db/entity/trigger_entity.h
    db/entity/view_entity.h
    db/exception.h
    db/editable_grid_data.h
    db/query_data_editor.h
    db/foreign_key.h
    db/native_query_result.h
    db/query_column.h
    db/query_criteria.h
    db/query_data_fetcher.h
    db/routine_editor.h
    db/routine_structure_parser.h
    db/routine_structure.h
    db/session_variables.h
    db/query_data.h
    db/query_results.h
    db/query.h
    db/table_column.h
    db/table_editor.h
    db/table_engines_fetcher.h
    db/table_index.h
    db/table_structure.h
    db/table_structure_parser.h
    db/db_thread_initializer.h
    db/trigger_editor.h
    db/trigger_structure_parser.h
    db/trigger_structure.h
    db/view_editor.h
    db/view_structure.h
    db/view_structure_parser.h
    db/user_manager.h
    db/user_editor_interface.h
    db/user_query/batch_executor.h
    db/user_query/sentences_parser.h
    db/user_query/user_query.h
    db/user_queries_manager.h
    helpers/formatting.h
    helpers/logger.h
    helpers/parsing.h
    helpers/random_password_generator.h
    helpers/text.h
    settings/settings_core.h
    settings/settings_geometry.h
    settings/settings_icons.h
    settings/settings_text.h
    settings/data_editors.h
    settings/general.h
    settings/queries_storage.h
    settings/query_data_export_storage.h
    settings/table_filters_storage.h
    ssh/openssh_tunnel.h
    ssh/ssh_tunnel_factory.h
    ssh/ssh_tunnel_parameters.h
    threads/helpers.h
    threads/mutex.h
    threads/db_thread.h
    threads/queries_task.h
    threads/thread_init_task.h
    threads/thread_task.h
    ui/common/checkbox_list_popup.h
    ui/common/data_type_combo_box.h
    ui/common/editable_query_data_table_view.h
    ui/common/geometry_helpers.h
    ui/common/mysql_syntax.h
    ui/common/sql_editor.h
    ui/common/sql_log_editor.h
    ui/common/sql_syntax_highlighter.h
    ui/common/table_column_default_editor.h
    ui/common/table_cell_line_edit.h
    ui/common/table_view.h
    ui/common/text_editor_popup.h
    ui/delegates/checkbox_delegate.h
    ui/delegates/checkbox_list_item_editor_wrapper.h
    ui/delegates/combobox_delegate.h
    ui/delegates/combobox_item_editor_wrapper.h
    ui/delegates/edit_query_data_delegate.h
    ui/delegates/line_edit_item_editor_wrapper.h
    ui/delegates/date_time_item_editor_wrapper.h
    ui/delegates/foreign_key_columns_delegate.h
    ui/delegates/foreign_key_foreign_columns_delegate.h
    ui/delegates/foreign_key_reference_option_delegate.h
    ui/delegates/foreign_key_reference_table_delegate.h
    ui/delegates/table_column_collation_delegate.h
    ui/delegates/table_column_default_delegate.h
    ui/delegates/table_column_type_delegate.h
    ui/delegates/table_index_delegate.h
    ui/edit_database/dialog.h
    ui/export_database/bottom_widget.h
    ui/export_database/top_widget.h
    ui/export_database/export_dialog.h
    ui/export_query/options_widget.h
    ui/export_query/output_format_widget.h
    ui/export_query/output_target_widget.h
    ui/export_query/export_query_data_dialog.h
    ui/export_query/row_selection_widget.h
    ui/main_window/central_left_db_tree.h
    ui/main_window/central_left_widget.h
    ui/main_window/central_bottom_widget.h
    ui/main_window/central_log_widget.h
    ui/main_window/central_right/base_root_tab.h
    ui/main_window/central_right/database/central_right_database_tab.h
    ui/main_window/central_right/data/central_right_data_tab.h
    ui/main_window/central_right/data/cr_data_filter_widget.h
    ui/main_window/central_right/global_filter_widget.h
    ui/main_window/central_right/global_data_filter_interface.h
    ui/main_window/central_right/host/central_right_host_tab.h
    ui/main_window/central_right/host/cr_host_databases_tab.h
    ui/main_window/central_right/host/cr_host_variables_tab.h
    ui/main_window/central_right/query/central_right_query_tab.h
    ui/main_window/central_right/query/cr_query_data_tab.h
    ui/main_window/central_right/query/cr_query_panel.h
    ui/main_window/central_right/query/cr_query_result.h
    ui/main_window/central_right/table/central_right_table_tab.h
    ui/main_window/central_right/table/cr_table_columns.h
    ui/main_window/central_right/table/cr_table_columns_tools.h
    ui/main_window/central_right/table/cr_table_foreign_keys_tools.h
    ui/main_window/central_right/table/cr_table_indexes_tools.h
    ui/main_window/central_right/table/cr_table_info_basic_tab.h
    ui/main_window/central_right/table/cr_table_info_foreign_keys_tab.h
    ui/main_window/central_right/table/cr_table_info.h
    ui/main_window/central_right/table/cr_table_info_indexes_tab.h
    ui/main_window/central_right/table/cr_table_info_options_tab.h
    ui/main_window/central_right/trigger/central_right_trigger_tab.h
    ui/main_window/central_right/trigger/cr_trigger_body.h
    ui/main_window/central_right/trigger/cr_trigger_options.h
    ui/main_window/central_right/view/central_right_view_tab.h
    ui/main_window/central_right/routine/central_right_routine_tab.h
    ui/main_window/central_right/routine/cr_routine_body.h
    ui/main_window/central_right/routine/cr_routine_info.h
    ui/main_window/central_right/routine/cr_routine_info_options_tab.h
    ui/main_window/central_right/routine/cr_routine_info_parameters_tab.h
    ui/main_window/central_right/routine/cr_routine_parameters_tools.h
    ui/main_window/central_right_widget.h
    ui/main_window/central_widget.h
    ui/main_window/main_window.h
    ui/main_window/main_window_status_bar.h
    ui/models/base_data_table_model.h
    ui/models/connection_params_model.h
    ui/models/database_entities_table_model.h
    ui/models/databases_table_model.h
    ui/models/data_table_model.h
    ui/models/entities_tree_model.h
    ui/models/entities_tree_sort_filter_proxy_model.h
    ui/models/query_data_sort_filter_proxy_model.h
    ui/models/table_columns_model.h
    ui/models/table_foreign_keys_model.h
    ui/models/table_indexes_model.h
    ui/models/table_indexes_model_item.h
    ui/models/routine_parameters_model.h
    ui/models/users_table_model.h
    ui/models/user_privileges_model.h
    ui/models/variables_table_model.h
    ui/models/session_objects_tree_model.h
    ui/preferences/general_tab.h
    ui/preferences/preferences_dialog.h
    ui/presenters/central_right_host_widget_model.h
    ui/presenters/central_right_widget_model.h
    ui/presenters/central_right_data_filter_form.h
    ui/presenters/table_info_widget_model.h
    ui/presenters/routine_info_widget_model.h
    ui/presenters/central_right_query_presenter.h
    ui/presenters/connection_parameters_form.h
    ui/presenters/edit_database_form.h
    ui/presenters/editable_data_context_menu_presenter.h
    ui/presenters/export_database_form.h
    ui/presenters/export_query_presenter.h
    ui/presenters/preferences_presenter.h
    ui/presenters/routine_form.h
    ui/presenters/select_db_object_form.h
    ui/presenters/table_info_form.h
    ui/presenters/trigger_form.h
    ui/presenters/text_editor_popup_form.h
    ui/presenters/view_form.h
    ui/presenters/user_management_form.h
    ui/user_manager/user_manager_window.h
    ui/user_manager/left_widget.h
    ui/user_manager/right_widget.h
    ui/user_manager/options_widget.h
    ui/user_manager/privileges_widget.h
    ui/user_manager/credentials_tab.h
    ui/user_manager/limitations_tab.h
    ui/user_manager/select_db_object.h
    ui/session_manager/session_form.h
    ui/session_manager/settings_tab.h
    ui/session_manager/ssh_tunnel_tab.h
    ui/session_manager/start_tab.h
    ui/session_manager/window.h
    utils/exporting/mysql_dump_console.h
    utils/exporting/query_data_exporter.h
    utils/exporting/query_data_export_formats/format.h
    utils/exporting/query_data_export_formats/format_csv.h
    utils/exporting/query_data_export_formats/format_factory.h
    utils/exporting/query_data_export_formats/format_html_table.h
    utils/exporting/query_data_export_formats/format_json.h
    utils/exporting/query_data_export_formats/format_latex.h
    utils/exporting/query_data_export_formats/format_markdown.h
    utils/exporting/query_data_export_formats/format_php_array.h
    utils/exporting/query_data_export_formats/format_sql.h
    utils/exporting/query_data_export_formats/format_sql_deletes_replaces.h
    utils/exporting/query_data_export_formats/format_sql_inserts.h
    utils/exporting/query_data_export_formats/format_sql_replaces.h
    utils/exporting/query_data_export_formats/format_wiki.h
    utils/exporting/query_data_export_formats/format_xml.h

)

if(WITH_MYSQL)
    list(APPEND HEADER_FILES
        db/data_type/mysql_data_type.h
    )
endif()

if(WITH_POSTGRESQL)
    list(APPEND HEADER_FILES
        db/data_type/pg_data_type.h
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
    db/connection_query_killer.cpp
    db/connections_manager.cpp
    db/database_editor.cpp
    db/db_thread_initializer.cpp
    db/data_type/data_type.cpp
    db/editable_grid_data.cpp
    db/entity/database_entity.cpp
    db/entity/entities_fetcher.cpp
    db/entity/entity.cpp
    db/entity/entity_factory.cpp
    db/entity/entity_filter.cpp
    db/entity/entity_holder.cpp
    db/entity/routine_entity.cpp
    db/entity/session_entity.cpp
    db/entity/table_entity_comparator.cpp
    db/entity/table_entity.cpp
    db/entity/trigger_entity.cpp
    db/entity/view_entity.cpp
    db/exception.cpp
    db/foreign_key.cpp
    db/native_query_result.cpp
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
    db/trigger_editor.cpp
    db/trigger_structure_parser.cpp
    db/trigger_structure.cpp
    db/view_editor.cpp
    db/view_structure.cpp
    db/view_structure_parser.cpp
    db/user_queries_manager.cpp
    db/user_query/batch_executor.cpp
    db/user_query/sentences_parser.cpp
    db/user_query/user_query.cpp
    helpers/formatting.cpp
    helpers/logger.cpp
    helpers/parsing.cpp
    helpers/random_password_generator.cpp
    helpers/text.cpp
    settings/settings_core.cpp
    settings/settings_geometry.cpp
    settings/settings_icons.cpp
    settings/settings_text.cpp
    settings/data_editors.cpp
    settings/general.cpp
    settings/queries_storage.cpp
    settings/query_data_export_storage.cpp
    settings/table_filters_storage.cpp
    ssh/openssh_tunnel.cpp
    ssh/ssh_tunnel_factory.cpp
    ssh/ssh_tunnel_parameters.cpp
    threads/db_thread.cpp
    threads/queries_task.cpp
    threads/thread_task.cpp
    threads/thread_init_task.cpp
    ui/common/checkbox_list_popup.cpp
    ui/common/data_type_combo_box.cpp
    ui/common/geometry_helpers.cpp
    ui/common/editable_query_data_table_view.cpp
    ui/common/sql_editor.cpp
    ui/common/sql_log_editor.cpp
    ui/common/sql_syntax_highlighter.cpp
    ui/common/table_cell_line_edit.cpp
    ui/common/table_column_default_editor.cpp
    ui/common/table_view.cpp
    ui/common/text_editor_popup.cpp
    ui/delegates/checkbox_delegate.cpp
    ui/delegates/checkbox_list_item_editor_wrapper.cpp
    ui/delegates/combobox_delegate.cpp
    ui/delegates/combobox_item_editor_wrapper.cpp
    ui/delegates/edit_query_data_delegate.cpp
    ui/delegates/line_edit_item_editor_wrapper.cpp
    ui/delegates/date_time_item_editor_wrapper.cpp
    ui/delegates/foreign_key_columns_delegate.cpp
    ui/delegates/foreign_key_foreign_columns_delegate.cpp
    ui/delegates/foreign_key_reference_option_delegate.cpp
    ui/delegates/foreign_key_reference_table_delegate.cpp
    ui/delegates/table_column_collation_delegate.cpp
    ui/delegates/table_column_default_delegate.cpp
    ui/delegates/table_column_type_delegate.cpp
    ui/delegates/table_index_delegate.cpp
    ui/edit_database/dialog.cpp
    ui/export_query/row_selection_widget.cpp
    ui/export_query/export_query_data_dialog.cpp
    ui/export_query/output_target_widget.cpp
    ui/export_query/output_format_widget.cpp
    ui/export_query/options_widget.cpp
    ui/export_database/bottom_widget.cpp
    ui/export_database/export_dialog.cpp
    ui/export_database/top_widget.cpp
    ui/main_window/central_bottom_widget.cpp
    ui/main_window/central_left_db_tree.cpp
    ui/main_window/central_left_widget.cpp
    ui/main_window/central_log_widget.cpp
    ui/main_window/central_right/database/central_right_database_tab.cpp
    ui/main_window/central_right/data/central_right_data_tab.cpp
    ui/main_window/central_right/data/cr_data_filter_widget.cpp
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
    ui/main_window/central_right/global_filter_widget.cpp
    ui/main_window/central_right_widget.cpp
    ui/main_window/central_widget.cpp
    ui/main_window/main_window.cpp
    ui/main_window/main_window_status_bar.cpp
    ui/models/base_data_table_model.cpp
    ui/models/connection_params_model.cpp
    ui/models/database_entities_table_model.cpp
    ui/models/databases_table_model.cpp
    ui/models/data_table_model.cpp
    ui/models/entities_tree_model.cpp
    ui/models/entities_tree_sort_filter_proxy_model.cpp
    ui/models/query_data_sort_filter_proxy_model.cpp
    ui/models/table_columns_model.cpp
    ui/models/table_foreign_keys_model.cpp
    ui/models/table_indexes_model.cpp
    ui/models/table_indexes_model_item.cpp
    ui/models/routine_parameters_model.cpp
    ui/models/users_table_model.cpp
    ui/models/user_privileges_model.cpp
    ui/models/variables_table_model.cpp
    ui/models/session_objects_tree_model.cpp
    ui/preferences/general_tab.cpp
    ui/preferences/preferences_dialog.cpp
    ui/presenters/central_right_host_widget_model.cpp
    ui/presenters/central_right_widget_model.cpp
    ui/presenters/central_right_data_filter_form.cpp
    ui/presenters/table_info_widget_model.cpp
    ui/presenters/routine_info_widget_model.cpp
    ui/presenters/central_right_query_presenter.cpp
    ui/presenters/connection_parameters_form.cpp
    ui/presenters/edit_database_form.cpp
    ui/presenters/editable_data_context_menu_presenter.cpp
    ui/presenters/export_database_form.cpp
    ui/presenters/export_query_presenter.cpp
    ui/presenters/preferences_presenter.cpp
    ui/presenters/routine_form.cpp
    ui/presenters/select_db_object_form.cpp
    ui/presenters/table_info_form.cpp
    ui/presenters/trigger_form.cpp
    ui/presenters/text_editor_popup_form.cpp
    ui/presenters/view_form.cpp
    ui/presenters/user_management_form.cpp
    ui/session_manager/session_form.cpp
    ui/session_manager/settings_tab.cpp
    ui/session_manager/ssh_tunnel_tab.cpp
    ui/session_manager/start_tab.cpp
    ui/session_manager/window.cpp
    ui/user_manager/user_manager_window.cpp
    ui/user_manager/left_widget.cpp
    ui/user_manager/right_widget.cpp
    ui/user_manager/options_widget.cpp
    ui/user_manager/privileges_widget.cpp
    ui/user_manager/credentials_tab.cpp
    ui/user_manager/limitations_tab.cpp
    ui/user_manager/select_db_object.cpp
    utils/exporting/query_data_exporter.cpp
    utils/exporting/query_data_export_formats/format_factory.cpp
    utils/exporting/mysql_dump_console.cpp
    utils/exporting/query_data_export_formats/format.cpp
)

if (WITH_MYSQL)
    list(APPEND SOURCE_FILES

        db/data_type/mysql_connection_data_types.cpp

        db/entity/mysql_entity_filter.cpp

        db/mysql/mysql_database_editor.cpp
        db/mysql/mysql_entities_fetcher.cpp
        db/mysql/mysql_library_initializer.cpp
        db/mysql/mysql_query_result.cpp
        db/mysql/mysql_query_data_editor.cpp
        db/mysql/mysql_collation_fetcher.cpp
        db/mysql/mysql_connection.cpp
        db/mysql/mysql_connection_query_killer.cpp
        db/mysql/mysql_query_data_fetcher.cpp
        db/mysql/mysql_table_editor.cpp
        db/mysql/mysql_table_engines_fetcher.cpp

        db/mysql/mysql_user_manager.cpp
        db/mysql/mysql_user_editor.cpp

        db/mysql/mysql_thread_initializer.cpp
    )

    list(APPEND HEADER_FILES

        db/data_type/mysql_data_type.h
        db/mysql/mysql_query_result.h
        db/data_type/mysql_connection_data_types.h
        db/entity/mysql_entity_filter.h
        db/mysql/mysql_database_editor.h
        db/mysql/mysql_entities_fetcher.h
        db/mysql/mysql_query_result.h
        db/mysql/mysql_query_data_editor.h
        db/mysql/mysql_collation_fetcher.h
        db/mysql/mysql_connection.h
        db/mysql/mysql_connection_query_killer.h
        db/mysql/mysql_query_data_fetcher.h
        db/mysql/mysql_table_editor.h
        db/mysql/mysql_table_engines_fetcher.h
        db/mysql/mysql_user_manager.h
        db/mysql/mysql_user_editor.h
        db/mysql/mysql_library_initializer.h
        db/mysql/mysql_thread_initializer.h
    )

endif()

if (WITH_POSTGRESQL)
    list(APPEND SOURCE_FILES

        db/data_type/pg_connection_data_types.cpp

        db/pg/pg_connection.cpp
        db/pg/pg_connection_query_killer.cpp
        db/pg/pg_entities_fetcher.cpp
        db/pg/pg_entity_create_code_generator.cpp
        db/pg/pg_query_data_editor.cpp
        db/pg/pg_query_data_fetcher.cpp
        db/pg/pg_query_result.cpp
    )

    list(APPEND HEADER_FILES
        db/data_type/pg_connection_data_types.h
        db/data_type/pg_data_type.h
        db/pg/pg_query_result.h
        db/pg/pg_connection.h
        db/pg/pg_connection_query_killer.h
        db/pg/pg_entities_fetcher.h
        db/pg/pg_entity_create_code_generator.h
        db/pg/pg_query_data_editor.h
        db/pg/pg_query_data_fetcher.h
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

    list(APPEND HEADER_FILES
        db/data_type/sqlite_connection_datatypes.cpp
        db/sqlite/sqlite_connection.h
        db/sqlite/sqlite_entities_fetcher.h
        db/sqlite/sqlite_table_structure_parser.h
        utils/sql_parser/sqlite/sqlite_parser.h
        utils/sql_parser/sqlite/sqlite_bison_parser.hpp
        utils/sql_parser/sqlite/sqlite_flex_lexer.h
        utils/sql_parser/sqlite/sqlite_types.h
    )

endif()

if(WITH_QTSQL)
    list(APPEND SOURCE_FILES
        db/qtsql/qtsql_query_result.cpp
    )
    list(APPEND HEADER_FILES
        db/qtsql/qtsql_query_result.h
    )
endif()

if(WITH_LIBSSH)
    list(APPEND HEADER_FILES
            ssh/sockets/connection.h
            ssh/sockets/socket.h
            ssh/sockets/connection_receiver_interface.h
            ssh/sockets/socket_receiver_interface.h
            ssh/libssh.h
            ssh/libssh_channel.h
            ssh/libssh_tunnel.h
            ssh/libssh_connection.h
    )
    list(APPEND SOURCE_FILES
            ssh/sockets/connection.cpp
            ssh/sockets/socket.cpp
            ssh/libssh.cpp
            ssh/libssh_channel.cpp
            ssh/libssh_tunnel.cpp
            ssh/libssh_connection.cpp
    )
endif()

if(WIN32)
    list(APPEND SOURCE_FILES
        ssh/plink_ssh_tunnel.cpp
    )
endif()

qt5_add_resources(RESOURCE_FILES icons.qrc translations.qrc)

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

target_include_directories(meowsql PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/third_party/libasio/asio/include)

target_link_libraries(meowsql Qt5::Widgets)

if(WITH_QTSQL)
    target_link_libraries(meowsql Qt5::Sql)
endif()

if (WIN32)
    target_link_libraries(meowsql
        User32 #SetProcessDPIAware()
        ws2_32 # WSAStartup() - sockets
    )
endif()

if (USE_CONAN_IO)
	# assume you run conan from ./conan folder
	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/conan)
endif()

# Config from CMake
configure_file(meowsql_config.h.in meowsql_config.h)
target_include_directories(meowsql PUBLIC
                           "${PROJECT_BINARY_DIR}")
add_definitions(-DGENERATED_BY_CMAKE)

# MySQL ---------------------------------------------

if (WITH_MYSQL)

    set(MEOW_MYSQL_CLIENT_LIB mysqlclient) #linux

    if (WIN32)
		if (USE_CONAN_IO)
			find_package(mysql-connector-c REQUIRED)
			include_directories(${mysql-connector-c_INCLUDE_DIR})
			message("mysql-connector-c_INCLUDE_DIR = ${mysql-connector-c_INCLUDE_DIR}")
			set(MEOW_MYSQL_CLIENT_LIB ${mysql-connector-c_LIBRARY_LIST})
			target_link_directories(meowsql PUBLIC ${mysql-connector-c_LIB_DIRS})
			message("mysql-connector-c_LIB_DIRS = ${mysql-connector-c_LIB_DIRS}")
		else()
			# find mysql manually
			list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
			find_package(MySQL REQUIRED)
			include_directories(${MYSQL_INCLUDE_DIR})
			set(MEOW_MYSQL_CLIENT_LIB ${MYSQL_LIBRARY})
		endif()
    endif()

endif() # if (WITH_MYSQL)

# PG --------------------------------------------------
# ubuntu: apt-get install libpq-dev postgresql-server-dev-all
# win: https://www.enterprisedb.com/downloads/postgres-postgresql-downloads (10.7 32bit)
# win2: conan install .. --settings arch=x86 --settings arch_build=x86 --settings build_type=Release --build=missing

if(WITH_POSTGRESQL)

if (WIN32)
	if (${USE_CONAN_IO} EQUAL OFF)
		# find postgres in third_party
		set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libpq/windows/include/postgresql")

		#  PostgreSQL_INCLUDE_DIR  - the path to where the PostgreSQL include files are.
		set (PostgreSQL_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libpq/windows/include/postgresql")

		#  PostgreSQL_LIBRARY_DIR  - The path to where the PostgreSQL library files are.
		set (PostgreSQL_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/libpq/windows/lib")
	endif()
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

	
	# Win: vcpkg install libssh
	

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
