#ifndef MEOW_UI_TABLE_VIEW_H
#define MEOW_UI_TABLE_VIEW_H

#include <QTableView>

namespace meow {
namespace ui {

class TableView : public QTableView
{
public:
    explicit TableView(QWidget * parent = nullptr);
protected:
    virtual int sizeHintForColumn(int column) const override;
};

} // namespace ui
} // namespace meow

#endif // MEOW_UI_TABLE_VIEW_H
