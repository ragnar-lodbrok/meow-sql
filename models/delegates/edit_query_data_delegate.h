#ifndef EDIT_QUERY_DATA_DELEGATE_H
#define EDIT_QUERY_DATA_DELEGATE_H

#include <QStyledItemDelegate>

namespace meow {
namespace models {
namespace delegates {

class EditQueryDataDelegate : public QStyledItemDelegate
{
public:
    explicit EditQueryDataDelegate(QObject * parent = nullptr);

    QWidget * createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;

    void destroyEditor(QWidget *editor,
                       const QModelIndex &index) const override;

    bool isEditing() const { return _editor != nullptr; }


    //void setEditorData(QWidget *editor,
    //                   const QModelIndex &index) const override;
    //void setModelData(QWidget *editor,
    //                  QAbstractItemModel *model,
    //                  const QModelIndex &index) const override;

    //void updateEditorGeometry(QWidget *editor,
    //                          const QStyleOptionViewItem &option,
    //                          const QModelIndex &index) const override;


    void commit();
    void discard();
private:

    mutable QWidget * _editor;
};

class EditTextQueryDataDelegate : public EditQueryDataDelegate
{
public:
    explicit EditTextQueryDataDelegate(QObject * parent = nullptr);

    QString displayText(const QVariant &value,
                        const QLocale &locale) const override;

};


} // namespace delegates
} // namespace models
} // namespace meow


#endif // EDIT_QUERY_DATA_DELEGATE_H
