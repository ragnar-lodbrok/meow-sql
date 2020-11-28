#include "app.h"

static meow::App * g_app = nullptr;

namespace meow {

App::App()
    : _dbConnectionParamsManager()
    , _dbConnectionsManager()
    , _settingsCore()
    , _actions(this) // after settings
    , _log()
{
    g_app = this;
    _dbConnectionParamsManager.load();
}

db::ConnectionParamsManager * App::dbConnectionParamsManager()
{
    return &_dbConnectionParamsManager;
}

db::ConnectionsManager * App::dbConnectionsManager()
{
    return &_dbConnectionsManager;
}

App * app()
{
    return g_app;
}

} // namespace meow

