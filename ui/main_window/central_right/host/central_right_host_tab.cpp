#include "central_right_host_tab.h"
#include "db/exception.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

HostTab::HostTab(QWidget *parent)
    : BaseRootTab(BaseRootTab::Type::Host, parent)
    , _variablesTab(nullptr)
{
    createRootTabs();
}

void HostTab::createRootTabs()
{
    _rootTabs = new QTabWidget();

    connect(_rootTabs,
            &QTabWidget::currentChanged,
            this,
            &HostTab::rootTabChanged);

    QHBoxLayout * layout = new QHBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(2,2,2,2);
    this->setLayout(layout);

    layout->addWidget(_rootTabs);

    _rootTabs->setSizePolicy(
                QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

#ifdef Q_OS_MAC
    _rootTabs->setDocumentMode(true);
#endif

    _databasesTab = new HostDatabasesTab();
    _rootTabs->addTab(_databasesTab,
                      QIcon(":/icons/database.png"),
                      _model.titleForDatabasesTab());
}

HostVariablesTab * HostTab::variablesTab()
{
    if (_variablesTab == nullptr) {
        _variablesTab = new HostVariablesTab();
        _model.setVariablesModel(_variablesTab->model());
        _rootTabs->insertTab(
                    (int)Tabs::Variables,
                    _variablesTab,
                    QIcon(":/icons/trigger.png"),
                    _model.titleForVariablesTab());
    }
    return _variablesTab;
}

bool HostTab::removeVariablesTab()
{
    if (removeTab(_variablesTab)) {
        _variablesTab = nullptr;
        return true;
    }
    return false;
}

bool HostTab::removeTab(QWidget * tab)
{
    if (tab) {
        int tabIndex = _rootTabs->indexOf(tab);
        if (tabIndex >= 0) {
            _rootTabs->removeTab(tabIndex);
        }
        delete tab;
        return (tabIndex >= 0);
    }
    return false;
}

void HostTab::setCurrentEntity(meow::db::SessionEntity * curEntity)
{
    bool sessionChanged = _model.setCurrentEntity(curEntity);
    if (sessionChanged) {
        onSessionChanged(curEntity);
    }
}

void HostTab::onSessionChanged(meow::db::SessionEntity * session)
{
    _databasesTab->setSession(session);
    _rootTabs->setTabText(static_cast<int>(Tabs::Databases),
                          _model.titleForDatabasesTab());

    if (_model.showVariablesTab()) {

        variablesTab(); // create tab

        if (static_cast<Tabs>(_rootTabs->currentIndex()) == Tabs::Variables) {
            // lazy load
            try {
                variablesTab()->setSession(session);
            } catch(meow::db::Exception & ex) {
                showErrorMessage(ex.message());
            }

        }
        _rootTabs->setTabText(static_cast<int>(Tabs::Variables),
                              _model.titleForVariablesTab());

    } else {
        removeVariablesTab();
    }
}

void HostTab::rootTabChanged(int index)
{
    if (_model.showVariablesTab() && static_cast<Tabs>(index) == Tabs::Variables) {
        try {
            variablesTab()->setSession(_model.currentSession());
        } catch(meow::db::Exception & ex) {
            showErrorMessage(ex.message());
        }
        _rootTabs->setTabText(static_cast<int>(Tabs::Variables),
                              _model.titleForVariablesTab());
    }
}

void HostTab::showErrorMessage(const QString& message)
{
    QMessageBox msgBox;
    msgBox.setText(message);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.exec();
}

void HostTab::onGlobalRefresh()
{
    if (static_cast<Tabs>(_rootTabs->currentIndex()) == Tabs::Databases) {
        // TODO
    } else if (static_cast<Tabs>(_rootTabs->currentIndex()) == Tabs::Variables) {
        try {
            variablesTab()->model()->refresh();
        } catch(meow::db::Exception & ex) {
            showErrorMessage(ex.message());
        }
        _rootTabs->setTabText(static_cast<int>(Tabs::Variables),
                              _model.titleForVariablesTab());
    }
}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
