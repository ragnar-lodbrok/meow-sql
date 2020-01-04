#include "central_right_data_tab.h"
#include "db/common.h"
#include "app/app.h"
#include "ui/common/editable_data_table_view.h"
#include "models/delegates/edit_query_data_delegate.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {


DataTab::DataTab(QWidget *parent) :
    QWidget(parent),
    _model(),
    _skipApplyModifications(false)
{
    _mainLayout = new QVBoxLayout();
    _mainLayout->setContentsMargins(2, 2, 2, 2);
    setLayout(_mainLayout);

    createTopPanel();
    createDataTable();

    connect(meow::app()->actions()->dataSetNULL(),
            &QAction::triggered,
            this,
            &DataTab::onDataSetNULLAction);

    connect(meow::app()->actions()->dataPostChanges(),
            &QAction::triggered,
            this,
            &DataTab::onDataPostChanges);

    connect(meow::app()->actions()->dataCancelChanges(),
            &QAction::triggered,
            this,
            &DataTab::onDataCancelChanges);

    connect(meow::app()->actions()->dataDelete(),
            &QAction::triggered,
            this,
            &DataTab::onDataDelete);

    connect(meow::app()->actions()->dataInsert(),
            &QAction::triggered,
            this,
            &DataTab::onDataInsert);

    connect(&_model, &meow::models::db::DataTableModel::editingStarted,
            this, &DataTab::validateControls);

    validateControls();
}

void DataTab::createTopPanel()
{
    _topLayout = new QHBoxLayout();
    _topLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->addLayout(_topLayout);

    createDataToolBar();

    _dataLabel = new QLabel(tr("Data"));
    _dataLabel->setWordWrap(true);
    _topLayout->addWidget(_dataLabel, 0, Qt::AlignVCenter | Qt::AlignLeft);
    _topLayout->addStretch(1);

    createShowToolBar();

}

void DataTab::createDataToolBar()
{
    _dataToolBar = new QToolBar();
    _dataToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    _dataToolBar->setIconSize(QSize(16, 16));
    // see https://stackoverflow.com/questions/21363350/remove-gradient-from-qtoolbar-in-os-x
    #ifdef Q_OS_MAC
        _dataToolBar->setStyle(QStyleFactory::create("windows"));
    #endif
    _topLayout->addWidget(_dataToolBar, 0, Qt::AlignLeft);

    _dataToolBar->addAction( meow::app()->actions()->dataInsert() );
    _dataToolBar->addAction( meow::app()->actions()->dataDelete() );
    _dataToolBar->addAction( meow::app()->actions()->dataPostChanges() );
    _dataToolBar->addAction( meow::app()->actions()->dataCancelChanges() );
}

void DataTab::createShowToolBar()
{
    _showToolBar = new QToolBar();
    _showToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    _showToolBar->setIconSize(QSize(16, 16));
    // see https://stackoverflow.com/questions/21363350/remove-gradient-from-qtoolbar-in-os-x
    #ifdef Q_OS_MAC
        _showToolBar->setStyle(QStyleFactory::create("windows"));
    #endif
    _topLayout->addWidget(_showToolBar, 0, Qt::AlignRight);


    _nextRowsAction = new QAction(QIcon(":/icons/next.png"), tr("Next"), this);
    _nextRowsAction->setToolTip(
        QString(tr("Show next %1 rows ...")).arg(meow::db::DATA_ROWS_PER_STEP)
    ); // TODO: hot keys
    connect(_nextRowsAction, &QAction::triggered,
            this, &DataTab::actionNextRows);
    _showToolBar->addAction(_nextRowsAction);


    _showAllRowsAction = new QAction(QIcon(":/icons/show_all.png"),
                                     tr("Show all"),
                                     this);
    _showAllRowsAction->setToolTip(
        tr("Show all rows")
    ); // TODO: hot keys
    connect(_showAllRowsAction, &QAction::triggered,
            this, &DataTab::actionAllRows);
    _showToolBar->addAction(_showAllRowsAction);
}

void DataTab::createDataTable()
{

    _dataTable = new EditableDataTableView();
    _dataTable->verticalHeader()->hide();
     auto geometrySettings = meow::app()->settings()->geometrySettings();
    _dataTable->verticalHeader()->setDefaultSectionSize(
        geometrySettings->tableViewDefaultRowHeight());
    _dataTable->horizontalHeader()->setHighlightSections(false);
    _dataTable->horizontalHeader()->setResizeContentsPrecision(
        meow::app()->settings()
            ->textSettings()->tableAutoResizeRowsLookupCount()
    );

    _dataTable->setSelectionBehavior(
        QAbstractItemView::SelectionBehavior::SelectRows);

    _dataTable->setModel(&_model);
    _mainLayout->addWidget(_dataTable);
    _dataTable->setSortingEnabled(false); // TODO

    connect(_dataTable->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this,
            &DataTab::currentRowChanged
    );

    connect(_dataTable->selectionModel(),
            &QItemSelectionModel::currentChanged,
            this,
            &DataTab::currentChanged
    );

    connect(_dataTable->selectionModel(),
            &QItemSelectionModel::selectionChanged,
            [=](const QItemSelection &selected,
                const QItemSelection &deselected)
            {
                Q_UNUSED(selected);
                Q_UNUSED(deselected);
                this->validateDataDeleteActionState();
            }
    );

    models::delegates::EditQueryDataDelegate * delegate
        = new models::delegates::EditQueryDataDelegate(&_model);
    _dataTable->setItemDelegate(delegate);
}

void DataTab::actionAllRows(bool checked)
{

    try {
        Q_UNUSED(checked);
        _model.setNoRowsCountLimit();
        _model.loadData(true);
        refreshDataLabelText();
        validateShowToolBarState();
    } catch(meow::db::Exception & ex) {
        errorDialog(ex.message());
    }
}

void DataTab::actionNextRows(bool checked)
{
    try {
        Q_UNUSED(checked);
        _model.incRowsCountForOneStep();
        _model.loadData(true);
        // TODO: select addition
        refreshDataLabelText();
        validateShowToolBarState();
    } catch(meow::db::Exception & ex) {
        errorDialog(ex.message());
    }
}

void DataTab::onDataPostChanges(bool checked)
{
    Q_UNUSED(checked);
    applyModifications();
}

void DataTab::onDataCancelChanges(bool checked)
{
    Q_UNUSED(checked);
    discardModifications();
}

void DataTab::onDataDelete(bool checked)
{
    Q_UNUSED(checked);
    deleteSelectedRows();
}

void DataTab::onDataInsert(bool checked)
{
    Q_UNUSED(checked);
    insertEmptyRow();
}

void DataTab::setDBEntity(db::Entity * tableOrViewEntity, bool loadData)
{
    _model.incRowsCountForOneStep(true);
    _model.setEntity(tableOrViewEntity, loadData);
    if (loadData) {
        refreshDataLabelText();
        validateControls();
        if (meow::app()->settings()->textSettings()->autoResizeTableColumns()) {
            _dataTable->resizeColumnsToContents();
        }
    }
}

void DataTab::loadData()
{
    _model.loadData();
    refreshDataLabelText();
    validateControls();
    if (meow::app()->settings()->textSettings()->autoResizeTableColumns()) {
        _dataTable->resizeColumnsToContents();
    }
}

void DataTab::refreshDataLabelText()
{
    _dataLabel->setText(_model.rowCountStats());
}

void DataTab::validateShowToolBarState()
{
    _nextRowsAction->setDisabled(_model.allDataLoaded());
    _showAllRowsAction->setDisabled(_model.allDataLoaded());
}

void DataTab::validateDataToolBarState()
{
    bool canPost = _model.isEditing();
    if (canPost) {
        auto delegate = static_cast<models::delegates::EditQueryDataDelegate *>(
            _dataTable->itemDelegate()
        );
        canPost = delegate->isEditing() || _model.isModified();
    }

    meow::app()->actions()->dataPostChanges()->setEnabled(canPost);
    meow::app()->actions()->dataCancelChanges()->setEnabled(canPost);

    validateDataDeleteActionState();
}

void DataTab::validateDataDeleteActionState()
{   
    meow::app()->actions()->dataInsert()->setEnabled(_model.isEditable());
    meow::app()->actions()->dataDelete()->setEnabled(
        _dataTable->selectionModel()->hasSelection() && _model.isEditable());
}

void DataTab::validateControls()
{
    validateDataToolBarState();
    validateShowToolBarState();
}

void DataTab::currentRowChanged(const QModelIndex &current,
                                const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);

    applyModifications();

    QModelIndex curIndex = _dataTable->selectionModel()->currentIndex();
    _model.setCurrentRowNumber(curIndex.isValid() ? curIndex.row() : -1);

    validateDataToolBarState();
}

void DataTab::currentChanged(const QModelIndex &current,
                             const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);

    validateDataToolBarState();
}

void DataTab::onDataSetNULLAction(bool checked)
{
    Q_UNUSED(checked);
    QModelIndex curIndex = _dataTable->selectionModel()->currentIndex();
    if (curIndex.isValid()) {
        _model.setData(curIndex, QString());
    }
}

void DataTab::applyModifications()
{
    if (_skipApplyModifications) return;

    commitTableEditor();
    try {
        _model.applyModifications();
    } catch(meow::db::Exception & ex) {
        errorDialog(ex.message());
    }

    validateDataToolBarState();
}

void DataTab::discardModifications()
{
    discardTableEditor();
    _model.discardModifications();
    validateDataToolBarState();
}

void DataTab::deleteSelectedRows()
{
    QModelIndexList selected = _dataTable->selectionModel()->selectedIndexes();
    QList<int> selectedRows;

    for (const QModelIndex & index: selected) {
        if (selectedRows.contains(index.row()) == false) {
            selectedRows << index.row();
        }
    }

    if (selectedRows.count() == 0) {
        errorDialog(tr("Please select one or more rows to delete them."));
        return;
    }

    QMessageBox msgBox;
    msgBox.setText(tr("Delete %1 row(s)?").arg(selectedRows.count()));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();
    if (ret != QMessageBox::Yes) {
        return;
    }

    auto reverseCompare = [=](const int & first, const int & second) -> bool {
        return first > second;
    };

    std::sort(selectedRows.begin(), selectedRows.end(), reverseCompare);

    try {
        for (int row : selectedRows) {
            _model.deleteRowInDB(row);
        }
    } catch(meow::db::Exception & ex) {
        errorDialog(ex.message());
    }

    validateControls();
}

void DataTab::insertEmptyRow()
{
    int newRowIntIndex = _model.insertEmptyRow();
    if (newRowIntIndex == -1) return;

    // select
    _skipApplyModifications = true;
    QModelIndex newRowIndex = _dataTable->model()->index(newRowIntIndex, 0);
    _dataTable->selectionModel()
        ->setCurrentIndex(newRowIndex, QItemSelectionModel::Clear);
    _dataTable->scrollTo(_dataTable->selectionModel()->currentIndex());
    //_dataTable->edit(newRowIndex);
    _skipApplyModifications = false;
}

void DataTab::commitTableEditor()
{
    auto delegate = static_cast<models::delegates::EditQueryDataDelegate *>(
        _dataTable->itemDelegate()
    );
    delegate->commit();
}

void DataTab::discardTableEditor()
{
    auto delegate = static_cast<models::delegates::EditQueryDataDelegate *>(
        _dataTable->itemDelegate()
    );
    delegate->discard();
}

void DataTab::errorDialog(const QString & message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

