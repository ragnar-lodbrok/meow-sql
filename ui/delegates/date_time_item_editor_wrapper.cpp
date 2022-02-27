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

    switch (_format) {
    case Format::DateTime:
        dateTimeEdit->setDisplayFormat(helpers::dateTimeFormatString());
        break;
    case Format::Date:
        dateTimeEdit->setDisplayFormat(helpers::dateFormatString());
        break;
    case Format::Time:
        dateTimeEdit->setDisplayFormat(helpers::timeFormatString());
        break;
    case Format::Year:
        dateTimeEdit->setDisplayFormat(helpers::yearFormatString());
        break;
    default:
        Q_ASSERT(false);
        break;
    };

    dateTimeEdit->setCalendarPopup(
                meow::app()->settings()
                ->dataEditors()->enablePopupForDatetimeEditor());

    _editor = dateTimeEdit;
    return _editor;
}

void DateTimeItemEditorWrapper::setEditorData(QWidget *editor,
                               const QModelIndex &index) const
{
    auto dateTimeEdit = static_cast<QDateTimeEdit *>(editor);

    QString dateTimeString = index.model()->data(index, Qt::EditRole).toString();

    switch (_format) {

    case Format::DateTime: {

        QDateTime dateTime = helpers::parseDateTime(dateTimeString);
        if (!dateTime.isValid()
                && meow::app()->settings()->
                dataEditors()->prefillDatetimeEditorWithCurrent()) {
            // TODO: take server current timestamp, not local time
            dateTime = QDateTime::currentDateTime();
        }

        dateTimeEdit->setDateTime(dateTime);

    } break;

    case Format::Year:
    case Format::Date: {
        QDate date = _format == Format::Date
                ? helpers::parseDate(dateTimeString)
                : helpers::parseYear(dateTimeString);
        if (!date.isValid()
                && meow::app()->settings()->
                dataEditors()->prefillDatetimeEditorWithCurrent()) {
            date = QDate::currentDate();
        }

        dateTimeEdit->setDate(date);
    } break;

    case Format::Time: {
        QTime time = helpers::parseTime(dateTimeString);
        if (!time.isValid()
                && meow::app()->settings()->
                dataEditors()->prefillDatetimeEditorWithCurrent()) {
            time = QTime::currentTime();
        }

        dateTimeEdit->setTime(time);
    } break;

    default:
        Q_ASSERT(false);
        break;
    };
}

void DateTimeItemEditorWrapper::setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const
{
    auto dateTimeEdit = static_cast<QDateTimeEdit *>(editor);
    QDateTime dateTime = dateTimeEdit->dateTime();
    QString formattedDateTime;

    switch (_format) {
    case Format::DateTime:
        formattedDateTime = helpers::formatDateTime(dateTime);
        break;
    case Format::Date:
        formattedDateTime = helpers::formatDate(dateTime);
        break;
    case Format::Time:
        formattedDateTime = helpers::formatTime(dateTime);
        break;
    case Format::Year:
        formattedDateTime = helpers::formatYear(dateTime);
        break;
    default:
        Q_ASSERT(false);
        return;
    };

    model->setData(index, formattedDateTime, Qt::EditRole);
}


} // namespace delegates
} // namespace ui
} // namespace meow
