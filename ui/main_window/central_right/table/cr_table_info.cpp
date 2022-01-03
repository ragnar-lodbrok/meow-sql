#include "cr_table_info.h"
#include "cr_table_info_basic_tab.h"
#include "cr_table_info_options_tab.h"
#include "cr_table_info_indexes_tab.h"
#include "cr_table_info_foreign_keys_tab.h"
#include "ui/presenters/table_info_form.h"
#include "ui/models/table_indexes_model.h"
#include "ui/models/table_foreign_keys_model.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

TableInfo::TableInfo(presenters::TableInfoForm * form, QWidget *parent) :
    QWidget(parent),
    _form(form)
{
    createTabs();

    connect(_form->indexesModel(),
            &models::TableIndexesModel::modified,
            this,
            &TableInfo::indicesModified
    );

    connect(_form->foreignKeysModel(),
            &models::TableForeignKeysModel::modified,
            this,
            &TableInfo::foreignKeysModified
    );
}

void TableInfo::refreshData()
{   
    _basicTab->refreshData();
    _optionsTab->refreshData();
    _indexesTab->refreshData();
    _fKeysTab->refreshData();
}

void TableInfo::onBeforeEntityEditing()
{
    _rootTabs->setCurrentIndex(
        static_cast<int>(presenters::TableInfoWidgetTabs::Basic));
    _basicTab->onBeforeEntityEditing();
}

void TableInfo::createTabs()
{
    _rootTabs = new QTabWidget();

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    layout->addWidget(_rootTabs);

    _rootTabs->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

#ifdef Q_OS_MAC
    _rootTabs->setDocumentMode(true);
#endif

    _basicTab = new table_info::BasicTab(_form, this);
    _optionsTab = new table_info::OptionsTab(_form, this);
    _indexesTab = new table_info::IndexesTab(_form, this);
    _fKeysTab = new table_info::ForeignKeysTab(_form, this);

    using TableInfoTabs = ui::presenters::TableInfoWidgetTabs;

    _rootTabs->addTab(_basicTab,
                      _model.tabIcon(TableInfoTabs::Basic),
                      _model.tabTitle(TableInfoTabs::Basic));

    _rootTabs->addTab(_optionsTab,
                      _model.tabIcon(TableInfoTabs::Options),
                      _model.tabTitle(TableInfoTabs::Options));

    _rootTabs->addTab(_indexesTab,
                      _model.tabIcon(TableInfoTabs::Indexes),
                      _model.tabTitle(TableInfoTabs::Indexes));

    _rootTabs->addTab(_fKeysTab,
                      _model.tabIcon(TableInfoTabs::ForeignKeys),
                      _model.tabTitle(TableInfoTabs::ForeignKeys));
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
