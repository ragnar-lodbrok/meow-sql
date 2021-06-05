#include "table_info_form.h"
#include "models/forms/table_indexes_model.h"
#include "models/forms/table_foreign_keys_model.h"
#include "app/app.h"
#include "db/connection.h"

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
    delete _indexesModel;
    delete _fKeysModel;
}

void TableInfoForm::setTable(const meow::db::TableEntityPtr & table)
{
    // TODO: copy only when we start editing

    if (_indexesModel) {
        _indexesModel->setTable(nullptr);
    }
    if (_fKeysModel) {
        _fKeysModel->setTable(nullptr);
    }

    auto oldTable = _table;
    if (table->isNew()) {
        _sourceTable = nullptr;
        _table = table; // take ownership
    } else {
        _sourceTable = table; // just hold a ref to table
        _table = _sourceTable->deepCopy(); // and edit copy
    }

    if (_indexesModel) {
        _indexesModel->setTable(_table.get());
    }
    if (_fKeysModel) {
        _fKeysModel->setTable(_table.get());
    }

    setHasUnsavedChanges(false);

    QObject::connect(
        _table->structure(),
        &meow::db::TableStructure::beforeColumnRemoved,
        this,
        &TableInfoForm::tableColumnRemoved);
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

void TableInfoForm::setTableComment(const QString & name)
{
    _table->structure()->setComment(name);
    setHasUnsavedChanges(true);
}

const QString TableInfoForm::autoInc() const
{
    auto autoInc = _table ? _table->structure()->autoInc() : 0;
    return autoInc > 0 ? QString::number(autoInc) : QString();
}

void TableInfoForm::setAutoInc(const QString & value)
{
    bool ok = true;
    db::ulonglong valueInt = value.isEmpty() ? 0 : value.toULongLong(&ok);
    if (ok && _table) {
        _table->structure()->setAutoInc(valueInt);
        setHasUnsavedChanges(true);
    }
}

const QString TableInfoForm::avgRowLen() const
{
    auto avgRowLen = _table ? _table->structure()->avgRowLen() : 0;
    return avgRowLen > 0 ? QString::number(avgRowLen) : QString();
}

void TableInfoForm::setAvgRowLen(const QString & value)
{
    bool ok = true;
    db::ulonglong valueInt = value.isEmpty() ? 0 : value.toULongLong(&ok);
    if (ok && _table) {
        _table->structure()->setAvgRowLen(valueInt);
        setHasUnsavedChanges(true);
    }

}

const QString TableInfoForm::maxRows() const
{
    auto maxRows = _table ? _table->structure()->maxRows() : 0;
    return maxRows > 0 ? QString::number(maxRows) : QString();
}

void TableInfoForm::setMaxRows(const QString & value)
{
    bool ok = true;
    db::ulonglong valueInt = value.isEmpty() ? 0 : value.toULongLong(&ok);
    if (ok && _table) {
        _table->structure()->setMaxRows(valueInt);
        setHasUnsavedChanges(true);
    }
}

const QStringList TableInfoForm::allCollations() const
{
    if (_table) {
        return _table->connection()->collationList();
    }
    return {};
}

const QString TableInfoForm::collation() const
{
    return _table ? _table->collation() : QString();
}

void TableInfoForm::setCollation(const QString & value)
{
    if (_table) {
        _table->setCollation(value);
        setHasUnsavedChanges(true);
    }
}

const QStringList TableInfoForm::allEngines() const
{
    if (_table) {
        return _table->connection()->tableEnginesList();
    }
    return {};
}

const QString TableInfoForm::engine() const
{
    QString engine = _table ? _table->engineStr() : QString();
    if (engine.isEmpty()) {
        return _table->connection()->defaultTableEngine();
    }
    return engine;
}

void TableInfoForm::setEngine(const QString & value)
{
    if (_table && (value != _table->engineStr()) ) {
        _table->setEngine(value);
        setHasUnsavedChanges(true);
        emit tableEngineChanged(value);
    }
}

const QStringList TableInfoForm::allRowFormats() const
{
    return _table ? _table->connection()->tableRowFormats() : QStringList();
}

const QString TableInfoForm::rowFormat() const
{
    if (_table && !_table->structure()->rowFormatStr().isEmpty()) {
        return _table->structure()->rowFormatStr();
    }
    return QString("DEFAULT");
}

void TableInfoForm::setRowFormat(const QString & value)
{
    if (_table) {
        _table->structure()->setRowFormat(value);
        setHasUnsavedChanges(true);
    }
}

bool TableInfoForm::isCheckSum() const
{
    return _table ? _table->structure()->isCheckSum() : false;
}

void TableInfoForm::setCheckSum(bool value)
{
    if (_table && value != _table->structure()->isCheckSum()) {
        _table->structure()->setCheckSum(value);
        setHasUnsavedChanges(true);
    }
}

TableIndexesModel * TableInfoForm::indexesModel()
{
    if (!_indexesModel) {
        _indexesModel = new TableIndexesModel();
        _indexesModel->setTable(_table.get());
    }
    return _indexesModel;
}

TableForeignKeysModel * TableInfoForm::foreignKeysModel()
{
    if (!_fKeysModel) {
        _fKeysModel = new TableForeignKeysModel(this);
        _fKeysModel->setTable(_table.get());
    }
    return _fKeysModel;
}

bool TableInfoForm::supportsForeignKeys() const
{
    if (_table) {
        return _table->connection()->features()
                ->supportsForeignKeys(_table.get());
    }
    return false;
}

bool TableInfoForm::isEditingSupported() const
{
    if (_table == nullptr) return false;
    return _table->connection()->features()->supportsEditingTablesStructure();
}

void TableInfoForm::save()
{
    // TODO: adding foreign keys may add indices
    if (_table->isNew()) { // insert
        meow::app()->dbConnectionsManager()->activeSession()->insertEntityToDB(
            _table.get());
    } else { // update
        meow::app()->dbConnectionsManager()->activeSession()->editEntityInDB(
            _sourceTable.get(), _table.get());
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
