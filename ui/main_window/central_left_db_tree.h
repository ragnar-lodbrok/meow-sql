#ifndef UI_CENTRAL_LEFT_DB_TREE_H
#define UI_CENTRAL_LEFT_DB_TREE_H

#include <QTreeView>

namespace meow {
namespace ui {
namespace main_window {

class DbTree : public QTreeView
{
public:
    explicit DbTree(QWidget * parent = nullptr);
    virtual ~DbTree() override {}

    void refresh();

protected:
    virtual void contextMenuEvent(QContextMenuEvent * event) override;
private:

    void createActions();

    bool currentItemSupportsDumping() const;
    bool currentItemSupportsEditing() const;

    QAction * _editAction;
    QAction * _dropAction;

    QAction * _createTableAction;
    QAction * _createDatabaseAction;
    QAction * _createViewAction;
    QAction * _createRoutineAction;
    QAction * _createTriggerAction;

    QAction * _refreshAction;
};

} // namespace meow
} // namespace ui
} // namespace main_window

#endif // UI_CENTRAL_LEFT_DB_TREE_H
