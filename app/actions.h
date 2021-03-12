#ifndef ACTIONS_H
#define ACTIONS_H

#include <QAction>

namespace meow {

class App;

class Actions : public QObject
{
public:
    Actions(App * app);

    QAction * disconnect() const { return _disconnect; }
    QAction * sessionManager() const { return _sessionManager; }
    QAction * userManager() const { return _userManager; }
    QAction * globalRefresh() const { return _globalRefresh; }
    QAction * showGlobalFilterPanel() const { return _showGlobalFilterPanel; }

    // TODO: group actions
    QAction * dataInsert() const { return _dataInsert; }
    QAction * dataDelete() const { return _dataDelete; }
    QAction * dataPostChanges() const { return _dataPostChanges; }
    QAction * dataCancelChanges() const { return _dataCancelChanges; }
    QAction * dataSetNULL() const { return _dataSetNULL; }
    QAction * dataRefresh() const { return _dataRefresh; }

    QAction * logClear() const { return _logClear; }

    QAction * exportDatabase() const { return _exportDatabase; }

private:

    void createActions(App * app);

    QAction * _disconnect;
    QAction * _sessionManager;
    QAction * _userManager;
    QAction * _globalRefresh;
    QAction * _showGlobalFilterPanel;

    QAction * _dataInsert;
    QAction * _dataDelete;
    QAction * _dataPostChanges;
    QAction * _dataCancelChanges;
    QAction * _dataSetNULL;
    QAction * _dataRefresh;

    QAction * _logClear;

    QAction * _exportDatabase;
};

} // namespace meow

#endif // ACTIONS_H
