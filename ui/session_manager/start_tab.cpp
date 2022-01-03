#include "ui/session_manager/start_tab.h"

namespace meow {
namespace ui {
namespace session_manager {

StartTab::StartTab(QWidget *parent) : QWidget(parent) {

    _mainLayout = new QVBoxLayout();
    _mainLayout->setAlignment(Qt::AlignTop);
    setLayout(_mainLayout);

    _helpLabel = new QLabel("");
    _helpLabel->setWordWrap(true);
    _mainLayout->addWidget(_helpLabel);

    _mainLayout->addStretch(1);

    QHBoxLayout * importSettingsButtonLayout = new QHBoxLayout();
    // temp hide as not implemented
    //_importSettingsButton = new QPushButton(tr("Import settings file ..."));
    //importSettingsButtonLayout->addWidget(_importSettingsButton);
    //importSettingsButtonLayout->addStretch(1);
    _mainLayout->addLayout(importSettingsButtonLayout);

    _mainLayout->addStretch(2);
}

void StartTab::setForm(presenters::ConnectionParametersForm * form)
{
    _form = form;
    fillDataFromForm();
}

void StartTab::fillDataFromForm()
{
    // TODO
    //if (connectionsCount > 0) {
        // TODO: ORIGINAL: Please click a session on the left list to edit parameters, doubleclick to open it.
    //    _helpLabel->setText(tr("Please click a session on the left list to edit parameters."));
    //} else {
        _helpLabel->setText(tr("New here? In order to connect to a server, " \
                            "you have to create a so called " \
                            "\"session\" at first. Just click the \"New\" " \
                            "button on the bottom left to create your first session. " \
                            "Give it a friendly name (e.g. \"Local DB server\") " \
                            "so you'll recall it the next time you start %1").arg("MeowSQL"));

    //}
}

} // namespace session_manager
} // namespace ui
} // namespace meow


