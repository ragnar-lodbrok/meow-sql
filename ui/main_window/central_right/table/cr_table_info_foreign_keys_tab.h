#ifndef UI_CENTRAL_RIGHT_TABLE_INFO_FOREIGN_KEYS_TAB_H
#define UI_CENTRAL_RIGHT_TABLE_INFO_FOREIGN_KEYS_TAB_H

#include <QtWidgets>

namespace meow {
namespace ui {
namespace presenters {
    class TableInfoForm;
}
namespace main_window {
namespace central_right {
namespace table_info {

class TableForeignKeysTools;

class ForeignKeysTab : public QWidget
{
    Q_OBJECT
public:
    ForeignKeysTab(presenters::TableInfoForm * form,
                   QWidget *parent = nullptr);

    void refreshData();
    void validateControls();

    Q_SLOT void actionAddKey(bool checked);
    Q_SLOT void actionRemoveKey(bool checked);
    Q_SLOT void actionClearKeys(bool checked);

private:
    void createWidgets();

    Q_SLOT void currentKeyChanged(const QModelIndex &current,
                                  const QModelIndex &previous);

    presenters::TableInfoForm * _tableForm;
    TableForeignKeysTools * _tools;
    QTableView * _fKeysTable;
    QHBoxLayout * _mainLayout;

};

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLE_INFO_FOREIGN_KEYS_TAB_H
