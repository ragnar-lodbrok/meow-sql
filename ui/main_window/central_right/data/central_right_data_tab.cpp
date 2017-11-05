#include "central_right_data_tab.h"
#include "db/common.h"

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
}

void DataTab::createTopPanel()
{
    _topLayout = new QHBoxLayout();
    _topLayout->setContentsMargins(0, 0, 0, 0);
    _mainLayout->addLayout(_topLayout);

    _dataLabel = new QLabel(tr("Data"));
    _dataLabel->setWordWrap(true);
    _topLayout->addWidget(_dataLabel, 0, Qt::AlignVCenter);

    createToolBar();

}

void DataTab::createToolBar()
{
    _toolBar = new QToolBar();
    _toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    _topLayout->addWidget(_toolBar, 0, Qt::AlignRight);


    _nextRowsAction = new QAction(QIcon(":/icons/next.png"), tr("Next"), this);
    _nextRowsAction->setToolTip(
        QString(tr("Show next %1 rows ...")).arg(meow::db::DATA_ROWS_PER_STEP)
    ); // TODO: hot keys
    connect(_nextRowsAction, &QAction::triggered, this, &DataTab::actionNextRows);
    _toolBar->addAction(_nextRowsAction);


    _showAllRowsAction = new QAction(QIcon(":/icons/show_all.png"),
                                     tr("Show all"),
                                     this);
    _showAllRowsAction->setToolTip(
        tr("Show all rows")
    ); // TODO: hot keys
    connect(_showAllRowsAction, &QAction::triggered, this, &DataTab::actionAllRows);
    _toolBar->addAction(_showAllRowsAction);
}

void DataTab::createDataTable()
{

    _dataTable = new QTableView();
    _dataTable->verticalHeader()->hide();
    _dataTable->horizontalHeader()->setHighlightSections(false);

    _dataTable->setModel(&_model);
    _mainLayout->addWidget(_dataTable);
    _dataTable->setSortingEnabled(false); // TODO
}

void DataTab::actionAllRows(bool checked)
{
    Q_UNUSED(checked);
    _model.setNoRowsCountLimit();
    _model.loadData(true);
    refreshDataLabelText();
    validateToolBarState();
}

void DataTab::actionNextRows(bool checked)
{
    Q_UNUSED(checked);
    _model.incRowsCountForOneStep();
    _model.loadData(true);
    // TODO: select addition
    refreshDataLabelText();
    validateToolBarState();
}

void DataTab::setDBEntity(db::Entity * tableOrViewEntity, bool loadData)
{
    _model.incRowsCountForOneStep(true);
    _model.setEntity(tableOrViewEntity, loadData);
    if (loadData) {
        refreshDataLabelText();
        validateToolBarState();
        //_dataTable->resizeColumnsToContents(); // Too slow,write own
    }
}

void DataTab::loadData()
{
    _model.loadData();
    refreshDataLabelText();
    validateToolBarState();
    //_dataTable->resizeColumnsToContents();
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

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

