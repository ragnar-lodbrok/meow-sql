#include "central_right_data_tab.h"
#include "db/common.h"
#include "app/app.h"
#include "ui/common/editable_data_table_view.h"
#include "models/delegates/edit_query_data_delegate.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {


DataTab::DataTab(QWidget *parent) : QWidget(parent), _model()
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
            [=](bool checked) {
                Q_UNUSED(checked);
                applyModifications();
            });

    connect(meow::app()->actions()->dataCancelChanges(),
            &QAction::triggered,
            [=](bool checked) {
                Q_UNUSED(checked);
                discardModifications();
            });
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
    _topLayout->addWidget(_showToolBar, 0, Qt::AlignRight);


    _nextRowsAction = new QAction(QIcon(":/icons/next.png"), tr("Next"), this);
    _nextRowsAction->setToolTip(
        QString(tr("Show next %1 rows ...")).arg(meow::db::DATA_ROWS_PER_STEP)
    ); // TODO: hot keys
    connect(_nextRowsAction, &QAction::triggered, this, &DataTab::actionNextRows);
    _showToolBar->addAction(_nextRowsAction);


    _showAllRowsAction = new QAction(QIcon(":/icons/show_all.png"),
                                     tr("Show all"),
                                     this);
    _showAllRowsAction->setToolTip(
        tr("Show all rows")
    ); // TODO: hot keys
    connect(_showAllRowsAction, &QAction::triggered, this, &DataTab::actionAllRows);
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
        meow::app()->settings()->textSettings()->tableAutoResizeRowsLookupCount()
    );

    _dataTable->setModel(&_model);
    _mainLayout->addWidget(_dataTable);
    _dataTable->setSortingEnabled(false); // TODO

    connect(_dataTable->selectionModel(),
            &QItemSelectionModel::currentRowChanged,
            this,
            &DataTab::currentRowChanged
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
        validateToolBarState();
    } catch(meow::db::Exception & ex) {
        QMessageBox msgBox;
        msgBox.setText(ex.message());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
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
        validateToolBarState();
    } catch(meow::db::Exception & ex) {
        QMessageBox msgBox;
        msgBox.setText(ex.message());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void DataTab::setDBEntity(db::Entity * tableOrViewEntity, bool loadData)
{
    _model.incRowsCountForOneStep(true);
    _model.setEntity(tableOrViewEntity, loadData);
    if (loadData) {
        refreshDataLabelText();
        validateToolBarState();
        if (meow::app()->settings()->textSettings()->autoResizeTableColumns()) {
            _dataTable->resizeColumnsToContents();
        }
    }
}

void DataTab::loadData()
{
    _model.loadData();
    refreshDataLabelText();
    validateToolBarState();
    if (meow::app()->settings()->textSettings()->autoResizeTableColumns()) {
        _dataTable->resizeColumnsToContents();
    }
}

void DataTab::refreshDataLabelText()
{
    _dataLabel->setText(_model.rowCountStats());
}

void DataTab::validateToolBarState()
{
    _nextRowsAction->setDisabled(_model.allDataLoaded());
    _showAllRowsAction->setDisabled(_model.allDataLoaded());
}

void DataTab::currentRowChanged(const QModelIndex &current,
                                const QModelIndex &previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);

    QModelIndex curIndex = _dataTable->selectionModel()->currentIndex();
    _model.setCurrentRowNumber(curIndex.row());

    applyModifications();

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
    commitTableEditor();
    try {
        _model.applyModifications();
    } catch(meow::db::Exception & ex) {

        discardModifications();

        QMessageBox msgBox;
        msgBox.setText(ex.message());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
    }
}

void DataTab::discardModifications()
{
    discardTableEditor();
    _model.discardModifications();
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

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

