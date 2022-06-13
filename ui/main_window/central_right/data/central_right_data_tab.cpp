#include "central_right_data_tab.h"
#include "db/common.h"
#include "app/app.h"
#include "ui/common/editable_data_table_view.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {


DataTab::DataTab(QWidget *parent) :
    BaseRootTab(BaseRootTab::Type::Data, parent),
    _model(),
    _defaultTableDelegate(
            new delegates::EditQueryDataDelegate(&_model, &_model)),
    _textColumnTableDelegate(
            new delegates::FormatTextQueryDataDelegate(&_model, &_model)),
    _skipApplyModifications(false)
{
    _mainLayout = new QVBoxLayout();
    _mainLayout->setContentsMargins(2, 2, 2, 2);
    setLayout(_mainLayout);

    createTopPanel();

    _dataFilter = new DataFilterWidget(&_model);
    _dataFilter->hide();
    _mainLayout->addWidget(_dataFilter, 0);

    createDataTable();

    connect(meow::app()->actions()->dataSetNULL(),
            &QAction::triggered,
            this,
            &DataTab::onDataSetNULLAction);

    connect(meow::app()->actions()->dataRefresh(),
            &QAction::triggered,
            this,
            &DataTab::onDataRefreshAction);

    connect(meow::app()->actions()->dataPostChanges(),
            &QAction::triggered,
            this,
            &DataTab::onDataPostChanges);

    connect(meow::app()->actions()->dataCancelChanges(),
            &QAction::triggered,
            this,
            &DataTab::onDataCancelChanges);

    connect(meow::app()->actions()->dataDeleteRows(),
            &QAction::triggered,
            this,
            &DataTab::onDataDeleteRows);

    connect(meow::app()->actions()->dataInsertRow(),
            &QAction::triggered,
            this,
            &DataTab::onDataInsertRow);

    connect(meow::app()->actions()->dataDuplicateRowWithoutKeys(),
            &QAction::triggered,
            this,
            &DataTab::onDataDuplicateRowWithoutKeys);

    connect(meow::app()->actions()->dataDuplicateRowWithKeys(),
            &QAction::triggered,
            this,
            &DataTab::onDataDuplicateRowWithKeys);

    connect(&_model, &models::DataTableModel::editingStarted,
            this, &DataTab::validateControls);

    connect(&_model, &models::DataTableModel::dataRefreshed,
            this, &DataTab::onDataRefreshed);

    validateControls();
}

void DataTab::createTopPanel()
{
    _topLayout = new QHBoxLayout();
    _topLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->addLayout(_topLayout);

    createDataActionsToolBar();

    _dataLabel = new QLabel(tr("Data"));
    _dataLabel->setWordWrap(true);
    _topLayout->addWidget(_dataLabel, 10, Qt::AlignVCenter | Qt::AlignLeft);
    _topLayout->addStretch(1);

    createDataButtonsToolBar();
}

void DataTab::createDataActionsToolBar()
{
    _dataActionsToolBar = new QToolBar();
    _dataActionsToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    _dataActionsToolBar->setIconSize(QSize(16, 16));
    // see https://stackoverflow.com/questions/21363350/remove-gradient-from-qtoolbar-in-os-x
#ifdef Q_OS_MAC
    _dataActionsToolBar->setStyle(QStyleFactory::create("windows"));
#endif
    _topLayout->addWidget(_dataActionsToolBar, 0, Qt::AlignLeft);

    _dataActionsToolBar->addAction( meow::app()->actions()->dataInsertRow() );
    _dataActionsToolBar->addAction( meow::app()->actions()->dataDeleteRows() );
    _dataActionsToolBar->addAction( meow::app()->actions()->dataPostChanges() );
    _dataActionsToolBar->addAction( meow::app()->actions()->dataCancelChanges() );
    _dataActionsToolBar->addAction( meow::app()->actions()->dataRefresh() );
}

void DataTab::createDataButtonsToolBar()
{
    _dataButtonsToolBar = new QToolBar();
    _dataButtonsToolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    _dataButtonsToolBar->setIconSize(QSize(16, 16));
    // see https://stackoverflow.com/questions/21363350/remove-gradient-from-qtoolbar-in-os-x
#ifdef Q_OS_MAC
    _showToolBar->setStyle(QStyleFactory::create("windows"));
#endif
    _topLayout->addWidget(_dataButtonsToolBar, 0, Qt::AlignRight);


    // Next rows
    _nextRowsAction = new QAction(QIcon(":/icons/next.png"), tr("Next"), this);
    _nextRowsAction->setToolTip(
        QString(tr("Show next %1 rows ...")).arg(meow::db::DATA_ROWS_PER_STEP)
    ); // TODO: hot keys
    connect(_nextRowsAction, &QAction::triggered,
            this, &DataTab::onActionNextRows);
    _dataButtonsToolBar->addAction(_nextRowsAction);


    // All rows
    _showAllRowsAction = new QAction(QIcon(":/icons/show_all.png"),
                                     tr("Show all"),
                                     this);
    _showAllRowsAction->setToolTip(
        tr("Show all rows")
    ); // TODO: hot keys
    connect(_showAllRowsAction, &QAction::triggered,
            this, &DataTab::onActionAllRows);
    _dataButtonsToolBar->addAction(_showAllRowsAction);

    // Separator
    _dataButtonsToolBar->addSeparator();

    // Filter
    _showFilterPanelAction = new QAction(QIcon(":/icons/dropdown_normal"),
                                         tr("Filter"),
                                         this);
    _showFilterPanelAction->setCheckable(true);
    connect(_showFilterPanelAction, &QAction::triggered,
            this, &DataTab::onActionShowFilter);
    _dataButtonsToolBar->addAction(_showFilterPanelAction);
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
    connect(_dataTable->horizontalHeader(), &QHeaderView::sectionClicked,
            this, &DataTab::onDataTableHeaderClicked);

    _dataTable->setSelectionBehavior(
        QAbstractItemView::SelectionBehavior::SelectRows);

    _dataTable->setModel(_model.createSortFilterModel());
    _mainLayout->addWidget(_dataTable, 1);
    _dataTable->setSortingEnabled(false);

    connectRowChanged();

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

    connect(this, &DataTab::changeRowSelection,
            this, &DataTab::onChangeRowSelectionRequest,
            Qt::QueuedConnection);
}

void DataTab::onActionAllRows()
{

    try {
        _model.setNoRowsCountLimit();
        _model.loadData(true);
        refreshDataLabelText();
        validateShowToolBarState();
    } catch(meow::db::Exception & ex) {
        errorDialog(ex.message());
    }
}

void DataTab::onActionNextRows()
{
    try {
        _model.incRowsCountForOneStep();
        _model.loadData(true);
        // TODO: select addition
        refreshDataLabelText();
        validateShowToolBarState();
    } catch(meow::db::Exception & ex) {
        errorDialog(ex.message());
    }
}

void DataTab::onActionShowFilter(bool checked)
{
    // Listening: Moonspell - Alma Matter
    _dataFilter->setHidden(!checked);
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

void DataTab::onDataDeleteRows()
{
    deleteSelectedRows();
}

void DataTab::onDataInsertRow()
{
    insertEmptyRow();
}

void DataTab::onDataDuplicateRowWithoutKeys()
{
    duplicateCurrentRowWithoutKeys();
}

void DataTab::onDataDuplicateRowWithKeys()
{
    duplicateCurrentRowWithKeys();
}

void DataTab::onDataRefreshed()
{
    // TODO: just call onLoadData() ?
    if (meow::app()->settings()->textSettings()->autoResizeTableColumns()) {
        _dataTable->resizeColumnsToContents();
    }

    refreshDataLabelText();
    validateControls();
}

void DataTab::setDBEntity(db::Entity * tableOrViewEntity, bool loadData)
{
    applyModifications(); // close pending to avoid crash
    _model.incRowsCountForOneStep(true);
    _model.resetWhereFilter();
    _model.setEntity(tableOrViewEntity, loadData);
    if (loadData) {
        onLoadData();
    }
    _dataFilter->setDBEntity(tableOrViewEntity);
}

void DataTab::refresh()
{
    // TODO: catch db exception?
    applyModifications(); // close pending to avoid crash
    _model.refresh();
    onLoadData();
}

void DataTab::loadData()
{
    _model.loadData();
    onLoadData();
}

void DataTab::invalidateData()
{
    _model.invalidateData();
}

void DataTab::setFilterPattern(const QString & filter, bool regexp)
{
    _model.setFilterPattern(filter, regexp);
}

QString DataTab::filterPattern() const
{
    return _model.filterPattern();
}

bool DataTab::filterPatternIsRegexp() const
{
    return _model.filterPatternIsRegexp();
}

int DataTab::totalRowCount() const
{
    return _model.rowCount();
}

int DataTab::filterMatchedRowCount() const
{
    return _model.filterMatchedRowCount();
}

void DataTab::onLoadData()
{
    refreshDataLabelText();
    validateControls();
    if (meow::app()->settings()->textSettings()->autoResizeTableColumns()) {
        _dataTable->resizeColumnsToContents();
    }

    for (int c = 0; c < _model.columnCount(); ++c) {
        if (_model.typeCategoryForColumn(c) == db::DataTypeCategoryIndex::Text) {
            _dataTable->setItemDelegateForColumn(c, _textColumnTableDelegate);
        } else {
            _dataTable->setItemDelegateForColumn(c, _defaultTableDelegate);
        }
    }
}

void DataTab::refreshDataLabelText()
{
    _dataLabel->setText(_model.rowCountStats());
}

void DataTab::connectRowChanged()
{
    connect(_dataTable->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this,
            &DataTab::currentRowChanged//,
            //Qt::QueuedConnection
    );
}

void DataTab::disconnectRowChanged()
{
    disconnect(_dataTable->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this,
            &DataTab::currentRowChanged
    );
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
        auto delegate = static_cast<delegates::EditQueryDataDelegate *>(
            currentItemDelegate()
        );
        canPost = (delegate ? delegate->isEditing() : false)
                || _model.isModified();
    }

    meow::app()->actions()->dataPostChanges()->setEnabled(canPost);
    meow::app()->actions()->dataCancelChanges()->setEnabled(canPost);

    validateDataDeleteActionState();

    // Listening: Rob Zombie - The Triumph of King Freak
}

void DataTab::validateDataDeleteActionState()
{   
    // Disallow insert while any editing since it leads to many bugs and
    // complexities I can't solve now
    bool canInsert = _model.isEditable();
    if (canInsert) {
        auto delegate = static_cast<delegates::EditQueryDataDelegate *>(
            currentItemDelegate()
        );
        if (delegate && delegate->isEditing()) {
            canInsert = false;
        }
        if (canInsert) {
            canInsert = !_model.queryData()->isInserted();
        }
    }

    meow::app()->actions()->dataInsertRow()->setEnabled(canInsert);
    meow::app()->actions()->dataDuplicateRowWithoutKeys()->setEnabled(
            canInsert);
    meow::app()->actions()->dataDuplicateRowWithKeys()->setEnabled(canInsert);

    meow::app()->actions()->dataDeleteRows()->setEnabled(
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

    QModelIndex prevIndexMapped = _model.mapToSource(previous);

    bool error = !applyModifications(prevIndexMapped.row());

    if (error && previous.isValid()) {
        emit changeRowSelection(previous);
        return;
    }

    QModelIndex curIndex = currentIndexMapped();

    _model.setCurrentRowNumber(curIndex.isValid() ? curIndex.row() : -1);

    validateDataToolBarState();
}

void DataTab::onChangeRowSelectionRequest(const QModelIndex &index)
{
    disconnectRowChanged();
    _dataTable->selectionModel()
        ->setCurrentIndex(index, QItemSelectionModel::Current);

    _dataTable->selectionModel()
        ->select(index, QItemSelectionModel::ClearAndSelect);

    connectRowChanged();
}

void DataTab::onDataTableHeaderClicked(int index)
{
    _model.changeColumnSort(index);
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
    QModelIndex curIndex = currentIndexMapped();
    if (curIndex.isValid()) {
        _model.setData(curIndex, QString());
    }
}

void DataTab::onDataRefreshAction(bool checked)
{
    Q_UNUSED(checked);
    refresh();
}

bool DataTab::applyModifications(int rowToApply)
{
    if (_skipApplyModifications) return true;

    // TODO: doesn't work when you leave tab with text data edited

    bool error = false;

    commitTableEditor();

    // applying may change the row and start new apply before this finishes
    _skipApplyModifications = true;

    try {
        _model.applyModifications(rowToApply);
    } catch(meow::db::Exception & ex) {
        errorDialog(ex.message());
        error = true;
    }

    _skipApplyModifications = false;

    validateDataToolBarState();

    return !error;
}

void DataTab::discardModifications()
{
    discardTableEditor();
    _model.discardModifications();
    validateDataToolBarState();
}

void DataTab::deleteSelectedRows()
{
    QModelIndexList selected = _model.mapToSource(
                _dataTable->selectionModel()->selectedIndexes());
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
    insertNewRow(false);
}

void DataTab::duplicateCurrentRowWithoutKeys()
{
    insertNewRow(true, false);
}

void DataTab::duplicateCurrentRowWithKeys()
{
    insertNewRow(true, true);
}

void DataTab::insertNewRow(bool duplicateCurrent, bool withKeys)
{
    int newRowIntIndex = -1;
    if (duplicateCurrent) {
        if (withKeys) {
            newRowIntIndex = _model.duplicateCurrentRowWithKeys();
        } else {
            newRowIntIndex = _model.duplicateCurrentRowWithoutKeys();
        }
    } else {
        Q_ASSERT(withKeys != true); // Not implemented
        newRowIntIndex = _model.insertEmptyRow();
    }

    if (newRowIntIndex == -1) return;

    // select
    _skipApplyModifications = true; // TODO: something better
    QModelIndex newRowIndex = _model.mapFromSource(
                _model.createIndexForRow(newRowIntIndex));
    _dataTable->selectionModel()
        ->setCurrentIndex(newRowIndex, QItemSelectionModel::Clear);
    _dataTable->scrollTo(_dataTable->selectionModel()->currentIndex());
    _skipApplyModifications = false;
}

void DataTab::commitTableEditor()
{
    auto delegate = static_cast<delegates::EditQueryDataDelegate *>(
        currentItemDelegate()
    );
    if (delegate) {
        delegate->commit();
    }
}

void DataTab::discardTableEditor()
{
    auto delegate = static_cast<delegates::EditQueryDataDelegate *>(
        currentItemDelegate()
    );
    if (delegate) {
        delegate->discard();
    }
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

QAbstractItemDelegate * DataTab::currentItemDelegate() const
{
    QModelIndex index = _dataTable->currentIndex();
    if (index.isValid() == false)  {
        return nullptr;
    }
    return _dataTable->itemDelegateForColumn(index.column());
}

QModelIndex DataTab::currentIndexMapped() const
{
    QModelIndex curIndex = _dataTable->selectionModel()->currentIndex();

    return _model.mapToSource(curIndex);
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

