#ifndef ACTIONS_H
#define ACTIONS_H

#include <QAction>

namespace meow {

class Actions : public QObject
{
public:
    Actions();

    QAction * disconnect() const { return _disconnect; }

private:

    void createActions();

    QAction * _disconnect;
};

} // namespace meow

#endif // ACTIONS_H
