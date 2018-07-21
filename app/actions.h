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

private:

    void createActions();

    QAction * _disconnect;
    QAction * _sessionManager;
};

} // namespace meow

#endif // ACTIONS_H
