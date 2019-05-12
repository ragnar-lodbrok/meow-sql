#ifndef TABLE_COLUMN_TYPE_DELEGATE_H
#define TABLE_COLUMN_TYPE_DELEGATE_H

#include <QStyledItemDelegate>
#include "models/db/table_columns_model.h"

namespace meow {
namespace models {
namespace delegates {

class TableColumnTypeDelegate : public QStyledItemDelegate
{

public:
    TableColumnTypeDelegate(db::TableColumnsModel * model);

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
    db::TableColumnsModel * _model;
};

} // namespace delegates
} // namespace models
} // namespace meow

#endif // TABLE_COLUMN_TYPE_DELEGATE_H
