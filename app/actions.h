#ifndef ACTIONS_H
#define ACTIONS_H

#include <QAction>

namespace meow {

class Actions : public QObject
{
public:
    Actions();

    QAction * disconnect() const { return _disconnect; }
    QAction * sessionManager() const { return _sessionManager; }

    QAction * dataInsert() const { return _dataInsert; }
    QAction * dataDelete() const { return _dataDelete; }
    QAction * dataPostChanges() const { return _dataPostChanges; }
    QAction * dataCancelChanges() const { return _dataCancelChanges; }

    QAction * dataSetNULL() const { return _dataSetNULL; }

private:

    void createActions();

    QAction * _disconnect;
    QAction * _sessionManager;

    QAction * _dataInsert;
    QAction * _dataDelete;
    QAction * _dataPostChanges;
    QAction * _dataCancelChanges;

    QAction * _dataSetNULL;
};

} // namespace meow

#endif // ACTIONS_H
