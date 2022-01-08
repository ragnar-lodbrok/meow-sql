#include "app.h"

static meow::App * g_app = nullptr;

namespace meow {

App::App()
    : _log() // before _dbConnectionsManager
    , _dbConnectionParamsManager()
    , _dbConnectionsManager(db::ConnectionsManager::create())
    , _settingsCore()
    , _actions(this) // after settings
{
    g_app = this;
    _dbConnectionParamsManager.load();
    _dbConnectionsManager->init();
}

db::ConnectionParamsManager * App::dbConnectionParamsManager()
{
    return &_dbConnectionParamsManager;
}

db::ConnectionsManager * App::dbConnectionsManager()
{
    return _dbConnectionsManager.get();
}

App * app()
{
    return g_app;
}

} // namespace meow

