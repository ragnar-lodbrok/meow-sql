#ifndef DB_COMMON
#define DB_COMMON

#define MEOW_SERVER_INT(major, minor, patch) \
    (major)*10000 + (minor)*100 + (patch)

#define MEOW_SORT_FILTER_ENTITIES_TREE 1

namespace meow {
namespace db {

typedef unsigned long long ulonglong;

const int DATA_ROWS_PER_STEP = 1000;
const int DATA_MAX_ROWS = 100 * 1000;
const int DATA_MAX_LOAD_TEXT_LEN = 256;
const int FOREIGN_MAX_ROWS = 10000;

} // namespace db
} // namespace meow


#endif // DB_COMMON

