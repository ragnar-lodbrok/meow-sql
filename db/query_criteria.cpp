#include "query_criteria.h"

namespace meow {
namespace db {

QueryCriteria::QueryCriteria()
    :quotedDbAndTableName(""),
     limit(0),
     offset(0)
{

}

} // namespace db
} // namespace meow
