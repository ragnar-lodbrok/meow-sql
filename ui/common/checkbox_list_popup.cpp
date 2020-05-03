#include "checkbox_list_popup.h"

static const int maxRowsBeforeScroll = 15;
static const int maxListWidth = 200;

namespace meow {
namespace ui {

CheckboxListPopup::CheckboxListPopup(QWidget *parent)
    : QFrame(parent)
    , _listWidget(new QListWidget(this))
{
    setFrameStyle(QFrame::StyledPanel);
    setLineWidth(2);
    setAutoFillBackground(true);
    setWindowFlags(Qt::Popup);

    QVBoxLayout * mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->addWidget(_listWidget);
    this->setLayout(mainLayout);
}

void CheckboxListPopup::setData(const QStringList & allOptions,
                                const QStringList & selectedOptions)
{
    _listWidget->clear();

    for (const QString & optionName : allOptions) {

        QListWidgetItem * item = new QListWidgetItem(optionName);

        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

        if (selectedOptions.contains(optionName, Qt::CaseInsensitive)) {
            item->setCheckState(Qt::Checked);
        } else {
            item->setCheckState(Qt::Unchecked);
        }

        _listWidget->addItem(item);
    }
}

QStringList CheckboxListPopup::selectedOptions() const
{
    QStringList selected;

    for (int i = 0; i < _listWidget->count(); ++i) {
        QListWidgetItem * item = _listWidget->item(i);
        if (item->checkState() == Qt::Checked) {
            selected.append(item->text());
        }
    }

    return selected;
}

int CheckboxListPopup::heightHint() const
{
    int rows = std::min(_listWidget->count(), maxRowsBeforeScroll);
    int height = _listWidget->sizeHintForRow(0) * rows;
    height += 2 * _listWidget->frameWidth();
    height += 2 * this->frameWidth();

    return height;
}

int CheckboxListPopup::widthHint() const
{
    int width = _listWidget->sizeHintForColumn(0);
    width += 2 * _listWidget->frameWidth();
    width += 2 * this->frameWidth();

    return std::min(maxListWidth, width);
}

} // namespace ui
} // namespace meow
