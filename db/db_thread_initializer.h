#ifndef MEOW_DB_DB_THREAD_INITIALIZER_H
#define MEOW_DB_DB_THREAD_INITIALIZER_H

#include <memory>

namespace meow {
namespace db {

// Intent: inits/deinits thread for database.
// Does nothing, extend to add custom behaviour.
class DbThreadInitializer
{
public:
    DbThreadInitializer(bool deinitOnDestroy = false)
        : _deinitOnDestroy(deinitOnDestroy)
    {

    }
    virtual ~DbThreadInitializer() {
        if (_deinitOnDestroy) {
            deinit();
        }
    }

    virtual void init() {}
    virtual void deinit() {}
protected:
    bool _deinitOnDestroy;
};

enum class ServerType;

std::unique_ptr<DbThreadInitializer> createThreadInitializer(
        ServerType serverType);

} // namespace db
} // namespace meow

#endif // MEOW_DB_DB_THREAD_INITIALIZER_H
