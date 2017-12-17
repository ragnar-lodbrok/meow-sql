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
    return _table ? _table->structure()->comment() : QString();
}

const QString TableInfoForm::autoInc() const
{
    auto autoInc = _table ? _table->structure()->autoInc() : 0;
    return autoInc > 0 ? QString::number(autoInc) : QString();
}

const QString TableInfoForm::avgRowLen() const
{
    auto avgRowLen = _table ? _table->structure()->avgRowLen() : 0;
    return avgRowLen > 0 ? QString::number(avgRowLen) : QString();
}

const QString TableInfoForm::maxRows() const
{
    auto maxRows = _table ? _table->structure()->maxRows() : 0;
    return maxRows > 0 ? QString::number(maxRows) : QString();
}

const QString TableInfoForm::collation() const
{
    return _table ? _table->collation() : QString();
}

const QString TableInfoForm::engine() const
{
    return _table ? _table->engineStr() : QString();
}

const QString TableInfoForm::rowFormat() const
{
    if (_table && !_table->structure()->rowFormatStr().isEmpty()) {
        return _table->structure()->rowFormatStr();
    }
    return QString("DEFAULT");
}

bool TableInfoForm::isCheckSum() const
{
    return _table ? _table->structure()->isCheckSum() : false;
}

} // namespace forms
} // namespace models
} // namespace meow
