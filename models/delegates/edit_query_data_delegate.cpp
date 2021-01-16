#include "edit_query_data_delegate.h"
#include "helpers/text.h"

#include "line_edit_query_data_delegate.h"

#include <QDebug>

namespace meow {
namespace models {
namespace delegates {


QWidget * ItemEditorWrapper::createEditor(QWidget *parent,
                       const QStyleOptionViewItem &option,
                       const QModelIndex &index) const
{
    _editor = _delegate->QStyledItemDelegate::createEditor(
                parent, option, index);
    return _editor;
}

void ItemEditorWrapper::destroyEditor(QWidget *editor,
                   const QModelIndex &index) const
{
    Q_ASSERT(editor == _editor);
    _editor = nullptr;
    _delegate->QStyledItemDelegate::destroyEditor(editor, index);
}

void ItemEditorWrapper::setEditorData(QWidget *editor,
                               const QModelIndex &index) const
{
    _delegate->QStyledItemDelegate::setEditorData(editor, index);
}

void ItemEditorWrapper::setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const
{
    _delegate->QStyledItemDelegate::setModelData(editor, model, index);
}

// -----------------------------------------------------------------------------

EditQueryDataDelegate::EditQueryDataDelegate(
        IItemDelegateConfig * delegateConfig,
        QObject * parent)
    : QStyledItemDelegate(parent)
    , _delegateConfig(delegateConfig)
    , _editorWrapper(nullptr)
{

}

QWidget * EditQueryDataDelegate::createEditor(
        QWidget *parent,
        const QStyleOptionViewItem &option,
        const QModelIndex &index) const
{

    switch (_delegateConfig->editorType(index)) {

    case EditorType::defaultEditor: {
        _editorWrapper.reset(
                    new ItemEditorWrapper(
                        const_cast<EditQueryDataDelegate *>(this)
                    ));
    }
    break;

    case EditorType::lineEdit: {
        _editorWrapper.reset(
                    new LineEditItemEditorWrapper(
                        const_cast<EditQueryDataDelegate *>(this)
                    ));
    }
    break;

    default:
        Q_ASSERT(false);
        return QStyledItemDelegate::createEditor(parent, option, index);
    }

    return _editorWrapper->createEditor(parent, option, index);
}

void EditQueryDataDelegate::destroyEditor(QWidget *editor,
                   const QModelIndex &index) const
{
    if (_editorWrapper) {
        _editorWrapper->destroyEditor(editor, index);
        _editorWrapper = nullptr;
    } else {
        QStyledItemDelegate::destroyEditor(editor, index);
    }
}

void EditQueryDataDelegate::setEditorData(QWidget *editor,
                               const QModelIndex &index) const
{
    if (_editorWrapper) {
        _editorWrapper->setEditorData(editor, index);
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void EditQueryDataDelegate::setModelData(QWidget *editor,
                  QAbstractItemModel *model,
                  const QModelIndex &index) const
{
    if (_editorWrapper) {
        _editorWrapper->setModelData(editor, model, index);
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void EditQueryDataDelegate::commit(bool emitCloseEditor)
{
    if (_editorWrapper) {
        emit commitData(_editorWrapper->editor());
        if (emitCloseEditor) { // closing editor causes row change
            emit closeEditor(_editorWrapper->editor());
        }
        _editorWrapper = nullptr;
    }
}

void EditQueryDataDelegate::discard()
{
    if (_editorWrapper) {
        emit closeEditor(_editorWrapper->editor());
        _editorWrapper = nullptr;
    }
}

// -----------------------------------------------------------------------------

FormatTextQueryDataDelegate::FormatTextQueryDataDelegate(
        IItemDelegateConfig * delegateConfig,
        QObject * parent)
    : EditQueryDataDelegate(delegateConfig, parent)
{

}

QString FormatTextQueryDataDelegate::displayText(const QVariant &value,
                                               const QLocale &locale) const
{
    // TODO: better paint \r and \n so you can distinguish \\n from \n ?

    if (value.canConvert<QString>()) {

        return value.toString()
                .replace(QChar('\n'), QChar(' '), Qt::CaseInsensitive)
                .replace(QChar('\r'), QChar(' '), Qt::CaseInsensitive);

    } else {
        return QStyledItemDelegate::displayText(value, locale);
    }
}

} // namespace delegates
} // namespace models
} // namespace meow
