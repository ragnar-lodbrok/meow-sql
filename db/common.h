#ifndef DB_COMMON
#define DB_COMMON

#include <limits>

namespace meow {
namespace db {

typedef unsigned long long ulonglong;

const int DATA_ROWS_PER_STEP = 1000;
const int DATA_MAX_ROWS = 100 * 1000;

} // namespace db
} // namespace meow


#endif // DB_COMMON

