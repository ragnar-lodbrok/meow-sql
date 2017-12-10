#include "table_info_form.h"
#include "db/entity/table_entity.h"

namespace meow {
namespace models {
namespace forms {

TableInfoForm::TableInfoForm(QObject *parent) : QObject(parent), _table(nullptr)
{

}

void TableInfoForm::setTable(db::TableEntity * table)
{
    _table = table;
}

const QString TableInfoForm::tableName() const
{
    return _table ? _table->name() : QString();
}

const QString TableInfoForm::tableComment() const
{
    return QString(); // TODO
}

} // namespace forms
} // namespace models
} // namespace meow
