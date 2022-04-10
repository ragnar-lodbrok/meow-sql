#include "central_right_data_filter_form.h"
#include "db/entity/table_entity.h"
#include "ui/models/data_table_model.h"

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
    emit SQLFilterTextChanged(_sqlFilterText);
}

void CentralRightDataFilterForm::applyWhereFilter(const QString & whereFilter)
{
    _dataTabelModel->applyWhereFilter(whereFilter);
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
