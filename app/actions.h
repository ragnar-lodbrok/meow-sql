#ifndef ACTIONS_H
#define ACTIONS_H

#include <QAction>

namespace meow {

class App;

class Actions : public QObject
{
    Q_OBJECT // tr()
public:
    Actions(App * app);

    QAction * disconnect() const { return _disconnect; }
    QAction * sessionManager() const { return _sessionManager; }
    QAction * userManager() const { return _userManager; }
    QAction * globalRefresh() const { return _globalRefresh; }
    QAction * showGlobalFilterPanel() const { return _showGlobalFilterPanel; }

    // TODO: group actions
    QAction * dataInsertRow() const { return _dataInsertRow; }
    QAction * dataDeleteRows() const { return _dataDeleteRows; }
    QAction * dataPostChanges() const { return _dataPostChanges; }
    QAction * dataCancelChanges() const { return _dataCancelChanges; }
    QAction * dataSetNULL() const { return _dataSetNULL; }
    QAction * dataRefresh() const { return _dataRefresh; }
    QAction * dataDuplicateRowWithoutKeys() const {
        return _dataDuplicateRowWithoutKeys;
    }
    QAction * dataDuplicateRowWithKeys() const {
        return _dataDuplicateRowWithKeys;
    }
    QAction * dataResetSort() const {
        return _dataResetSort;
    }
    QAction * dataExport() const { return _dataExport; }

    QAction * logClear() const { return _logClear; }

    QAction * exportDatabase() const { return _exportDatabase; }

    QAction * preferences() const { return _preferences; }

private:

    void createActions(App * app);

    QAction * _disconnect;
    QAction * _sessionManager;
    QAction * _userManager;
    QAction * _globalRefresh;
    QAction * _showGlobalFilterPanel;

    QAction * _dataInsertRow;
    QAction * _dataDeleteRows;
    QAction * _dataPostChanges;
    QAction * _dataCancelChanges;
    QAction * _dataSetNULL;
    QAction * _dataRefresh;
    QAction * _dataDuplicateRowWithoutKeys;
    QAction * _dataDuplicateRowWithKeys;
    QAction * _dataResetSort;
    QAction * _dataExport;

    QAction * _logClear;

    QAction * _exportDatabase;
    QAction * _preferences;
};

} // namespace meow

#endif // ACTIONS_H
