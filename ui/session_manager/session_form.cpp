#include "session_form.h"

namespace meow {
namespace ui {
namespace session_manager {

SessionForm::SessionForm(QWidget *parent)
    : QWidget(parent),
      _connectionParamsForm(nullptr),
      _startTabIsHidden(true),
      _settingsTabIsHidden(true)
{
    createDetailsTabs();
    fillDataFromForm();
}

void SessionForm::createDetailsTabs()
{
    // http://doc.qt.io/qt-5/qtwidgets-dialogs-tabdialog-example.html
    _detailsTabs = new QTabWidget();

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    this->setLayout(layout);

    layout->addWidget(_detailsTabs);

    _detailsTabs->setSizePolicy(QSizePolicy(QSizePolicy::Expanding,
                                            QSizePolicy::Expanding));

    _startTab = new StartTab(this);
    _settingsTab = new SettingsTab(this);

}

void SessionForm::setConnectionParamsForm(models::forms::ConnectionParametersForm * form)
{
    _connectionParamsForm = form;
    fillDataFromForm();
}

void SessionForm::fillDataFromForm()
{
    _settingsTab->setForm(_connectionParamsForm);
    _startTab->setForm(_connectionParamsForm);

    if (_connectionParamsForm) {
        if (!_startTabIsHidden) {
            _startTabIsHidden = true;
            _detailsTabs->removeTab(0);
        }
        if (_settingsTabIsHidden) {
            _settingsTabIsHidden = false;
            _detailsTabs->insertTab(0, _settingsTab,
                                    QIcon(":/icons/wrench.png"),
                                    tr("Settings"));
        }
    } else {
        if (!_settingsTabIsHidden) {
            _settingsTabIsHidden = true;
            _detailsTabs->removeTab(0);
        }
        if (_startTabIsHidden) {
            _startTabIsHidden = false;
            _detailsTabs->insertTab(0, _startTab,
                                    QIcon(":/icons/star.png"),
                                    tr("Start"));
            _detailsTabs->setCurrentIndex(0);
        }
    }

    // TODO: start tab help text update
}

} // namespace session_manager
} // namespace ui
} // namespace meow
