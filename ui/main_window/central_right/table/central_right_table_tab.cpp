#include "central_right_table_tab.h"
#include "cr_table_info.h"
#include "cr_table_columns.h"
#include "db/exception.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

TableTab::TableTab(QWidget * parent)
    : BaseRootTab(BaseRootTab::Type::Entity, parent)
{
    createWidgets();

    connect(&_form,
            &models::forms::TableInfoForm::unsavedChanged,
            [=](bool hasUnsavedChanges) {
                Q_UNUSED(hasUnsavedChanges);
                validateControls();
            }
    );
}

TableTab::~TableTab()
{
    saveGeometryToSettings();
}

void TableTab::saveGeometryToSettings()
{
    QSettings settings;
    settings.setValue(
        "ui/main_window/center_right/table_tab/main_splitter",
        _mainSplitter->saveState());

}

void TableTab::loadGeometryFromSettings()
{
    QSettings settings;
    _mainSplitter->restoreState(settings.value(
        "ui/main_window/center_right/table_tab/main_splitter")
        .toByteArray());
}

void TableTab::setTable(const db::TableEntityPtr & table)
{
    _columns->setTable(nullptr); // TODO: dirty

    _form.setTable(table);
    _tableInfo->refreshData();
    _columns->setTable(_form.editableTable().get());

    // TODO: set focus on table name when table->isNew() == true
}

void TableTab::onBeforeEntityEditing()
{
    _tableInfo->onBeforeEntityEditing();
}

void TableTab::createWidgets()
{
    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(4, 4, 4, 4);
    setLayout(mainLayout);

    _mainSplitter = new QSplitter(Qt::Vertical, this);
    _mainSplitter->setChildrenCollapsible(false);
    mainLayout->addWidget(_mainSplitter);

    _tableInfo = new TableInfo(&_form, this);
    _tableInfo->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
    _tableInfo->setMinimumSize(QSize(300, 200));

    _columns = new TableColumns(this);
    _columns->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    _columns->setMinimumSize(QSize(300, 150));

    connect(_columns->model(),
            &meow::models::db::TableColumnsModel::modified,
            [=]() {
                _form.setHasUnsavedChanges(true);
            }
    );

    connect(_tableInfo,
            &TableInfo::indicesModified,
            [=]() {
                _form.setHasUnsavedChanges(true);
            }
    );

    connect(_tableInfo,
            &TableInfo::foreignKeysModified,
            [=]() {
                _form.setHasUnsavedChanges(true);
            }
    );

    _mainSplitter->addWidget(_tableInfo);
    _mainSplitter->setStretchFactor(0, 0);
    _mainSplitter->addWidget(_columns);
    _mainSplitter->setStretchFactor(1, 2);

    createGeneralButtons();
    validateControls();

    loadGeometryFromSettings();
}


void TableTab::createGeneralButtons()
{
    QHBoxLayout * buttonsLayout = new QHBoxLayout();
    ((QVBoxLayout *)this->layout())->addLayout(buttonsLayout);

    _discardButton = new QPushButton(tr("Discard"));
    buttonsLayout->addWidget(_discardButton);
    connect(_discardButton,
            &QAbstractButton::clicked,
            this,
            &TableTab::discardTableEditing
    );

    _saveButton = new QPushButton(tr("Save"));
    buttonsLayout->addWidget(_saveButton);
    connect(_saveButton,
            &QAbstractButton::clicked,
            this,
            &TableTab::saveTableEditing
    );

    buttonsLayout->addStretch(1);
}

void TableTab::discardTableEditing()
{
    this->setTable(_form.sourceTable());
}

void TableTab::saveTableEditing()
{
    try {
        _form.save();
    } catch(meow::db::Exception & ex) {
        QMessageBox msgBox;
        msgBox.setText(ex.message());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void TableTab::validateControls()
{
    bool enableEdit = _form.hasUnsavedChanges() && _form.isEditingSupported();
    _discardButton->setEnabled(enableEdit);
    _saveButton->setEnabled(enableEdit);
}


} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
