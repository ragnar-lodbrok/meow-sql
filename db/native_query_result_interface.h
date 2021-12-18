#ifndef DB_NATIVE_QUERY_RESULT_INTERFACE_H
#define DB_NATIVE_QUERY_RESULT_INTERFACE_H

#include <memory>
#include "db/common.h"

namespace meow {
namespace db {

// Intent: represents native result of query like MYSQL_RES
class INativeQueryResultInterface
{
public:
    virtual ~INativeQueryResultInterface() {}

    virtual db::ulonglong rowsCount() const = 0;
};

using QueryResultPt = std::shared_ptr<INativeQueryResultInterface>;

} // namespace db
} // namespace meow

#endif // DB_NATIVE_QUERY_RESULT_INTERFACE_H
