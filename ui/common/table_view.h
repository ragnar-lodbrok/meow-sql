#ifndef MEOW_UI_TABLE_VIEW_H
#define MEOW_UI_TABLE_VIEW_H

#include <QTableView>

namespace meow {
namespace ui {

class TableView : public QTableView
{
protected:
    virtual int sizeHintForColumn(int column) const override;
};

} // namespace ui
} // namespace meow

#endif // MEOW_UI_TABLE_VIEW_H
