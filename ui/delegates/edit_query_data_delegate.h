#ifndef UI_DELEGATES_EDIT_QUERY_DATA_DELEGATE_H
#define UI_DELEGATES_EDIT_QUERY_DATA_DELEGATE_H

#include <memory>
#include <QStyledItemDelegate>

namespace meow {
namespace ui {
namespace delegates {

enum class EditorType
{
    defaultEditor,
    lineEdit,
    dateTimeEdit,
    timeEdit,
    dateEdit,
    yearEdit,
    comboboxEdit,
    checkboxListEdit
};

// Provides info for configuring delegate editor
class IItemDelegateConfig
{
public:

    virtual ~IItemDelegateConfig() = default;

    virtual EditorType editorType(const QModelIndex &index) const = 0;
};

class EditQueryDataDelegate;

// Wraps editor widget and proxies requests, inherit for custom editor
class ItemEditorWrapper : public QObject
{
    Q_OBJECT
public:
    ItemEditorWrapper(EditQueryDataDelegate * delegate)
        : QObject(nullptr), _delegate(delegate), _editor(nullptr) { }

    virtual ~ItemEditorWrapper() {};

    QWidget * editor() const { return _editor; }

    virtual QWidget * createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const;

    virtual void destroyEditor(QWidget *editor,
                       const QModelIndex &index) const;

    virtual void setEditorData(QWidget *editor,
                               const QModelIndex &index) const;

    virtual void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const;

    virtual void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const {
        Q_UNUSED(editor);
        Q_UNUSED(option);
        Q_UNUSED(index);
        // Do nothing
    }

protected:
    EditQueryDataDelegate * _delegate;
    mutable QWidget * _editor;
};

class EditQueryDataDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit EditQueryDataDelegate(IItemDelegateConfig * delegateConfig,
                                   QObject * parent = nullptr);

    QWidget * createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const override;

    void destroyEditor(QWidget *editor,
                       const QModelIndex &index) const override;

    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const override;

    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

    bool isEditing() const { return _editorWrapper != nullptr; }

    void commit(bool emitCloseEditor = true);
    void discard();

protected:
    IItemDelegateConfig * _delegateConfig;
    mutable std::unique_ptr<ItemEditorWrapper> _editorWrapper;
};

// Formats displayed text
class FormatTextQueryDataDelegate : public EditQueryDataDelegate
{
public:
    explicit FormatTextQueryDataDelegate(IItemDelegateConfig * delegateConfig,
                                       QObject * parent = nullptr);

    // Due to performance reasons don't override this method in basic class
    virtual QString displayText(const QVariant &value,
                        const QLocale &locale) const override;

};


} // namespace delegates
} // namespace ui
} // namespace meow


#endif // UI_DELEGATES_EDIT_QUERY_DATA_DELEGATE_H
