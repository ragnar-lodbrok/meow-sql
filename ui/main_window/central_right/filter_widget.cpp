#include "filter_widget.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

FilterWidget::FilterWidget(QWidget *parent) : QWidget(parent)
{
    createWidgets();
}

void FilterWidget::createWidgets()
{
    QHBoxLayout * mainLayout = new QHBoxLayout();

    _filterLabel = new QLabel(tr("Filter:"));
    mainLayout->addWidget(_filterLabel, 0);

    _filterEdit = new QLineEdit();
    _filterEdit->setClearButtonEnabled(true);
    _filterLabel->setBuddy(_filterEdit);
    mainLayout->addWidget(_filterEdit, 2);

    _filteredStatsLabel = new QLabel();
    mainLayout->addWidget(_filteredStatsLabel, 2);

    mainLayout->addStretch(3);

    setLayout(mainLayout);

    connect(_filterEdit, &QLineEdit::textChanged,
            this, &FilterWidget::onFilterPatterChanged);
}

void FilterWidget::reset()
{
    _filterEdit->setText(QString());
    setRowCount(0, 0);

    // emit filter changed always when reset called
    emit onFilterPatterChanged(QString());
}

void FilterWidget::setFilterPattern(const QString & pattern)
{
    _filterEdit->setText(pattern);
}

void FilterWidget::setRowCount(int total, int matched)
{
    if (matched < total) {
        _filteredStatsLabel->setText(
            tr("%1 out of %2 matching. %3 hidden.")
            .arg(matched)
            .arg(total)
            .arg(total-matched)
        );
    } else {
        if (_filterEdit->text().isEmpty()) {
            _filteredStatsLabel->setText(QString());
        } else {
            _filteredStatsLabel->setText(
                tr("All %1 matching.").arg(total));
        }
    }
}

void FilterWidget::setFocus()
{
    _filterEdit->setCursorPosition(_filterEdit->text().length());
    _filterEdit->setFocus();

    //QTimer::singleShot(0, _filterEdit, SLOT(setFocus())); // trick
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
