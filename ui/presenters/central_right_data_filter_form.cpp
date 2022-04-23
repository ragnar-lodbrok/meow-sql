#include "central_right_data_filter_form.h"
#include "db/entity/table_entity.h"
#include "ui/models/data_table_model.h"
#include "db/entity/entity.h"
#include "db/entity/database_entity.h"
#include "db/entity/session_entity.h"

namespace meow {
namespace ui {
namespace presenters {

CentralRightDataFilterForm::CentralRightDataFilterForm(
        models::DataTableModel * dataTabelModel)
    : _dataTabelModel(dataTabelModel)
    , _entity(nullptr)
{

}

void CentralRightDataFilterForm::setDBEntity(db::Entity * tableOrViewEntity)
{
    _entity = tableOrViewEntity;
    _sqlFilterText = QString();

    // load recent filters
    QString sessionName = db::sessionForEntity(tableOrViewEntity)->name();
    QString databaseName = db::databaseName(tableOrViewEntity);
    _recentFiltersStorage.setTable(
                sessionName, databaseName, tableOrViewEntity->name());
    _recentFilters = _recentFiltersStorage.load();
    emit recentFiltersChanged();
}

bool CentralRightDataFilterForm::hasRecentFilters() const
{
    return !_recentFilters.isEmpty();
}

QStringList CentralRightDataFilterForm::recentFilters() const
{
    return _recentFilters;
}

void CentralRightDataFilterForm::setFilterEditText(const QString & text)
{
    setSQLFilterText(SQLByFilterText(text));
}

QString CentralRightDataFilterForm::SQLFilterText() const
{
    return _sqlFilterText;
}

void CentralRightDataFilterForm::setSQLFilterText(const QString & text)
{
    if (_sqlFilterText == text) return;
    _sqlFilterText = text;
    emit SQLFilterTextGenerated(_sqlFilterText);
}

void CentralRightDataFilterForm::applyWhereFilter(const QString & whereFilter)
{
    _dataTabelModel->applyWhereFilter(whereFilter);

    if (!whereFilter.isEmpty() && !_recentFilters.contains(whereFilter)) {
        _recentFilters.append(whereFilter);
        while (_recentFilters.count()
               > settings::TableFiltersStorage::maxCount()) {
            if (!_recentFilters.isEmpty()) {
                _recentFilters.pop_front();
            }
        }
        _recentFiltersStorage.save(_recentFilters);
        emit recentFiltersChanged();
    }
}

QString CentralRightDataFilterForm::SQLByFilterText(const QString & text)
{
    if (text.isEmpty()) return QString();

    db::Entity * entity = _dataTabelModel->entity();
    if (!entity) return QString();

    QList<db::TableColumn *> columns = _dataTabelModel->selectedTableColumns();

    return entity->connection()->applyLikeFilter(columns, text);
}

} // namespace presenters
} // namespace ui
} // namespace meow
