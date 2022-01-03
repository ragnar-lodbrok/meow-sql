#ifndef UI_DELEGATES_TABLE_COLUMN_TYPE_DELEGATE_H
#define UI_DELEGATES_TABLE_COLUMN_TYPE_DELEGATE_H

#include <QStyledItemDelegate>
#include "ui/models/table_columns_model.h"

namespace meow {
namespace ui {
namespace delegates {

class TableColumnTypeDelegate : public QStyledItemDelegate
{

public:
    TableColumnTypeDelegate(models::TableColumnsModel * model);

    QWidget * createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;

    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const override;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;
private:
    models::TableColumnsModel * _model;
};

} // namespace delegates
} // namespace ui
} // namespace meow

#endif // UI_DELEGATES_TABLE_COLUMN_TYPE_DELEGATE_H
