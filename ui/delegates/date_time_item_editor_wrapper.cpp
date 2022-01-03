#include "date_time_item_editor_wrapper.h"
#include "helpers/parsing.h"
#include "helpers/formatting.h"
#include <QDateTimeEdit>
#include "app/app.h"
#include <QDebug>

namespace meow {
namespace ui {
namespace delegates {

QWidget * DateTimeItemEditorWrapper::createEditor(QWidget *parent,
                           const QStyleOptionViewItem &option,
                           const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    // TODO: microseconds?
    // TODO: timezone?

    auto dateTimeEdit = new QDateTimeEdit(parent);
    dateTimeEdit->setDisplayFormat(helpers::dateTimeFormatString());
    dateTimeEdit->setCalendarPopup(
                meow::app()->settings()
                ->dataEditors()->enablePopupForDatetimeEditor());

    _editor = dateTimeEdit;
    return _editor;
}

void DateTimeItemEditorWrapper::setEditorData(QWidget *editor,
                               const QModelIndex &index) const
{
    QString dateTimeString = index.model()->data(index, Qt::EditRole).toString();
    QDateTime dateTime = helpers::parseDateTime(dateTimeString);
    if (!dateTime.isValid()
            && meow::app()->settings()->
            dataEditors()->prefillDatetimeEditorWithCurrent()) {
        // TODO: take server current timestamp, not local time
        dateTime = QDateTime::currentDateTime();
    }
    auto dateTimeEdit = static_cast<QDateTimeEdit *>(editor);
    dateTimeEdit->setDateTime(dateTime);
}

void DateTimeItemEditorWrapper::setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const
{
    auto dateTimeEdit = static_cast<QDateTimeEdit *>(editor);
    QDateTime dateTime = dateTimeEdit->dateTime();
    model->setData(index, helpers::formatDateTime(dateTime), Qt::EditRole);
}


} // namespace delegates
} // namespace ui
} // namespace meow
