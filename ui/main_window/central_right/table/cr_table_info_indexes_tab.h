#ifndef UI_CENTRAL_RIGHT_TABLE_INFO_INDEXES_TAB_H
#define UI_CENTRAL_RIGHT_TABLE_INFO_INDEXES_TAB_H

#include <QtWidgets>

namespace meow {

namespace models {
namespace forms {
    class TableInfoForm;
}
}

namespace ui {
namespace main_window {
namespace central_right {
namespace table_info {

class IndexesTab : public QWidget
{
    Q_OBJECT
public:
    explicit IndexesTab(models::forms::TableInfoForm * form,
                        QWidget *parent = nullptr);

    void refreshData();

private:
    void createWidgets();
    void fillDataFromForm();

    models::forms::TableInfoForm * _tableForm;
    QTreeView * _indexesTree;
    QHBoxLayout * _mainLayout;
};

} // namespace table_info
} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow

#endif // UI_CENTRAL_RIGHT_TABLE_INFO_INDEXES_TAB_H
