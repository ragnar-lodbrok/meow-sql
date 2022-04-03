#include "central_right_data_filter_form.h"
#include "db/entity/table_entity.h"

namespace meow {
namespace ui {
namespace presenters {

CentralRightDataFilterForm::CentralRightDataFilterForm()
    : _entity(nullptr)
{

}

void CentralRightDataFilterForm::setDBEntity(db::Entity * tableOrViewEntity)
{
    _entity = tableOrViewEntity;
    _sqlFilterText = QString();
}

bool CentralRightDataFilterForm::hasRecentFilters() const
{
    return false;
}

QStringList CentralRightDataFilterForm::recentFilters() const
{
    return {};
}

void CentralRightDataFilterForm::setFilterEditText(const QString & text)
{
    _sqlFilterText = SQLByFilterText(text);
}

QString CentralRightDataFilterForm::SQLFiltertext() const
{
    return _sqlFilterText;
}

QString CentralRightDataFilterForm::SQLByFilterText(const QString & text)
{
    return QString();
}

} // namespace presenters
} // namespace ui
} // namespace meow
