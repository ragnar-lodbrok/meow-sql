#include "table_info_form.h"
#include "db/entity/table_entity.h"
#include "models/forms/table_indexes_model.h"
#include "models/forms/table_foreign_keys_model.h"
#include "app.h"

namespace meow {
namespace models {
namespace forms {

TableInfoForm::TableInfoForm(QObject *parent)
    : QObject(parent),
      _table(nullptr),
      _sourceTable(nullptr),
      _indexesModel(nullptr),
      _fKeysModel(nullptr),
      _hasUnsavedChanges(false)
{

}

TableInfoForm::~TableInfoForm()
{
    delete _table;
    delete _indexesModel;
    delete _fKeysModel;
}

void TableInfoForm::setTable(meow::db::TableEntity * table)
{
    // TODO: copy only when we start editing

    if (_indexesModel) {
        _indexesModel->setTable(nullptr);
    }
    if (_fKeysModel) {
        _fKeysModel->setTable(nullptr);
    }

    delete _table;
    if (table->isNew()) {
        _sourceTable = nullptr;
        _table = table; // take ownership
    } else {
        _sourceTable = table; // just hold a ref to table
        _table = _sourceTable->deepCopy(); // and edit copy
    }

    if (_indexesModel) {
        _indexesModel->setTable(_table);
    }
    if (_fKeysModel) {
        _fKeysModel->setTable(_table);
    }

    setHasUnsavedChanges(false);
}

const QString TableInfoForm::tableName() const
{
    return _table ? _table->name() : QString();
}

void TableInfoForm::setTableName(const QString & name)
{
    _table->setName(name);
    setHasUnsavedChanges(true);
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

TableIndexesModel * TableInfoForm::indexesModel()
{
    if (!_indexesModel) {
        _indexesModel = new TableIndexesModel();
        _indexesModel->setTable(_table);
    }
    return _indexesModel;
}

TableForeignKeysModel * TableInfoForm::foreignKeysModel()
{
    if (!_fKeysModel) {
        _fKeysModel = new TableForeignKeysModel();
        _fKeysModel->setTable(_table);
    }
    return _fKeysModel;
}

void TableInfoForm::save()
{
    if (_table->isNew()) { // insert
        meow::app()->dbConnectionsManager()->activeSession()->insertTableToDB(
            _table);
        _table = nullptr; // transfer ownership to session/manager
    } else { // update
        meow::app()->dbConnectionsManager()->activeSession()->editTableInDB(
            _sourceTable, _table);
    }

    setHasUnsavedChanges(false);
}

void TableInfoForm::setHasUnsavedChanges(bool modified)
{
    if (_hasUnsavedChanges != modified) {
        _hasUnsavedChanges = modified;
        emit unsavedChanged(_hasUnsavedChanges);
    }
}

} // namespace forms
} // namespace models
} // namespace meow
