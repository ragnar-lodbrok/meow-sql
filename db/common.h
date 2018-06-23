#ifndef DB_COMMON
#define DB_COMMON


namespace meow {
namespace db {

typedef unsigned long long ulonglong;

const int DATA_ROWS_PER_STEP = 1000;
const int DATA_MAX_ROWS = 100 * 1000;
const int DATA_MAX_LOAD_TEXT_LEN = 256;

} // namespace db
} // namespace meow


#endif // DB_COMMON

