#ifndef MEOW_APP_H
#define MEOW_APP_H

#import "db/connection_params_manager.h"

namespace meow {

class App
{
public:
    App();
    db::ConnectionParamsManager * dbConnectionManager();
private:
    meow::db::ConnectionParamsManager _dbConnectionParamsManager;
};

App * app();

} // namespace meow

#endif // MEOW_APP_H
