#ifndef MEOW_APP_H
#define MEOW_APP_H

#include "db/connection_params_manager.h"
#include "db/connections_manager.h"

namespace meow {

class App
{
public:
    App();
    db::ConnectionParamsManager * dbConnectionParamsManager();
    db::ConnectionsManager * dbConnectionsManager();
private:
    meow::db::ConnectionParamsManager _dbConnectionParamsManager;
    meow::db::ConnectionsManager _dbConnectionsManager;
};

App * app();

} // namespace meow

#endif // MEOW_APP_H
