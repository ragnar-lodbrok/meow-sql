#ifndef EDIT_QUERY_DATA_DELEGATE_H
#define EDIT_QUERY_DATA_DELEGATE_H

#include <QStyledItemDelegate>

namespace meow {
namespace models {
namespace delegates {

class EditQueryDataDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit EditQueryDataDelegate(QObject * parent = nullptr);

    QWidget * createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;

    void destroyEditor(QWidget *editor,
                       const QModelIndex &index) const override;

    bool isEditing() const { return _editor != nullptr; }

    void commit(bool emitCloseEditor = true);
    void discard();

protected:

    mutable QWidget * _editor;
};

class EditTextQueryDataDelegate : public EditQueryDataDelegate
{
    Q_OBJECT
public:
    explicit EditTextQueryDataDelegate(QObject * parent = nullptr);

    QWidget * createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;

    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const override;

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    QString displayText(const QVariant &value,
                        const QLocale &locale) const override;

    Q_SLOT void onPopupTextEditorClosed(bool accepted);

};


} // namespace delegates
} // namespace models
} // namespace meow


#endif // EDIT_QUERY_DATA_DELEGATE_H
