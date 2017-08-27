#include "cr_query_result.h"

namespace meow {
namespace ui {
namespace main_window {
namespace central_right {

QueryResult::QueryResult(db::UserQuery * userQuery, QWidget *parent)
    : QWidget(parent),
      _userQuery(userQuery)
{

}

void QueryResult::showQueryData()
{

}

} // namespace central_right
} // namespace main_window
} // namespace ui
} // namespace meow
