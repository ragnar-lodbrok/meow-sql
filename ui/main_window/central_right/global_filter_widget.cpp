#include "global_filter_widget.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

GlobalFilterWidget::GlobalFilterWidget(QWidget *parent) : QWidget(parent)
{
    createWidgets();
}

void GlobalFilterWidget::createWidgets()
{
    QGridLayout * mainLayout = new QGridLayout();

    int row = 0;
    int col = 0;

    _filterLabel = new QLabel(tr("Filter:"));
    mainLayout->addWidget(_filterLabel, row, col);
    mainLayout->setColumnStretch(col++, 0);

    _filterEdit = new QLineEdit();
    _filterEdit->setClearButtonEnabled(true);
    _filterLabel->setBuddy(_filterEdit);
    _filterEdit->setPlaceholderText(tr("Wildcard pattern"));
    mainLayout->addWidget(_filterEdit, row, col);
    mainLayout->setColumnStretch(col++, 2);

    _filteredStatsLabel = new QLabel();
    mainLayout->addWidget(_filteredStatsLabel, row, col);
    mainLayout->setColumnStretch(col++, 2);

    mainLayout->setColumnStretch(col++, 3);

    ++row;
    col = 1;

    _useRegexpCheckbox = new QCheckBox(
                tr("Use rich Perl-like regular expressions"));
    mainLayout->addWidget(_useRegexpCheckbox, row, col++, 1, 3);

    setLayout(mainLayout);

    connect(_filterEdit, &QLineEdit::textChanged,
            this, &GlobalFilterWidget::onFilterEditTextChanged);

    connect(_useRegexpCheckbox, &QCheckBox::stateChanged,
            this, &GlobalFilterWidget::onUseRegexpCheckboxChanged);
}

void GlobalFilterWidget::reset()
{
    _filterEdit->blockSignals(true);
    _useRegexpCheckbox->blockSignals(true);

    _filterEdit->setText(QString());
    _useRegexpCheckbox->setChecked(false);

    _filterEdit->blockSignals(false);
    _useRegexpCheckbox->blockSignals(false);

    setRowCount(0, 0);

    // emit filter changed always when reset called
    emit onFilterPatternChanged(QString(), _useRegexpCheckbox->isChecked());
}

void GlobalFilterWidget::setFilterPattern(const QString & pattern, bool regexp)
{
    _filterEdit->setText(pattern);
    _useRegexpCheckbox->setChecked(regexp);
}

void GlobalFilterWidget::setRowCount(int total, int matched)
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

void GlobalFilterWidget::setFocus()
{
    _filterEdit->setCursorPosition(_filterEdit->text().length());
    _filterEdit->setFocus();
}

void GlobalFilterWidget::onFilterEditTextChanged(const QString & text)
{
    emit onFilterPatternChanged(text, _useRegexpCheckbox->isChecked());
}

void GlobalFilterWidget::onUseRegexpCheckboxChanged(int state)
{
    emit onFilterPatternChanged(_filterEdit->text(),
                               state == Qt::Checked);
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
