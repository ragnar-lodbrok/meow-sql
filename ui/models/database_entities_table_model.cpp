#include "database_entities_table_model.h"
#include "db/entity/table_entity.h"
#include "db/entity/session_entity.h"
#include "helpers/formatting.h"
#include <QDebug>

namespace meow {
namespace ui {
namespace models {

DatabaseEntitiesTableModel::DatabaseEntitiesTableModel(
        QObject *parent)
    : QAbstractTableModel(parent)
    , _database(nullptr)
    , _session(nullptr)
{

}

int DatabaseEntitiesTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(Columns::Count);
}

int DatabaseEntitiesTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return entitiesCount();
}

Qt::ItemFlags DatabaseEntitiesTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index);
}

QVariant DatabaseEntitiesTableModel::headerData(int section,
                                           Qt::Orientation orientation,
                                           int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (static_cast<Columns>(section)) {
        case Columns::Name:
            return QString(tr("Name"));

        case Columns::Rows:
            return QString(tr("Rows"));

        case Columns::Size:
            return QString(tr("Size"));

        case Columns::Created:
            return QString(tr("Created"));

        case Columns::Updated:
            return QString(tr("Updated"));

        case Columns::Engine:
            return QString(tr("Engine"));

        case Columns::Comment:
            return QString(tr("Comment"));

        case Columns::Version:
            return QString(tr("Version"));

        case Columns::Collation:
            return QString(tr("Collation"));

        default:
            break;
        }
    }

    return QVariant();
}

QVariant DatabaseEntitiesTableModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= entitiesCount()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {

        meow::db::Entity * entity = _entities.at(index.row()).get();

        if (!entity) {
            return QVariant();
        }

        meow::db::TableEntity * table = nullptr;

        if (entity->type() == meow::db::Entity::Type::Table) {
            table = static_cast<meow::db::TableEntity *>(entity);
        }

        switch (static_cast<Columns>(index.column())) {
        case Columns::Name:
            return entity->name();

        case Columns::Rows:
            if (table) {
                return helpers::formatNumber(table->rowsCount());
            } else {
                return QVariant();
            }

        case Columns::Size:
            if (entity->hasDataSize()) {
                return helpers::formatByteSize(entity->dataSize());
            } else {
                return QVariant();
            }

        case Columns::Created: {
            QDateTime created = entity->created();
            if (created.isValid()) {
                return helpers::formatDateTime(created);
            } else {
                return QVariant();
            }
        }

        case Columns::Updated: {
            QDateTime updated = entity->updated();
            if (updated.isValid()) {
                return helpers::formatDateTime(updated);
            } else {
                return QVariant();
            }
        }

        case Columns::Engine:
            if (table) {
                return table->engineStr();
            } else {
                return QVariant();
            }

        case Columns::Comment:
            return QVariant();

        case Columns::Version:
            if (table && table->version()) {
                return table->version();
            } else {
                return QVariant();
            }

        case Columns::Collation:
            if (table) {
                return table->collation();
            } else {
                return QVariant();
            }

        default:
            break;
        }

    } else if (role == Qt::DecorationRole) {

        if (static_cast<Columns>(index.column()) == Columns::Name) {
            meow::db::Entity * entity = _entities.at(index.row()).get();
            if (entity) {
                return entity->icon();
            }
        }
    }

    return QVariant();
}


int DatabaseEntitiesTableModel::columnWidth(int column) const
{
    switch (static_cast<Columns>(column)) {
    case Columns::Name:
        return 240;
    case Columns::Collation:
    case Columns::Comment:
        return 140;
    case Columns::Created:
    case Columns::Updated:
        return 150;
    default:
        return 80;
    }
}

void DatabaseEntitiesTableModel::setDataBase(
        meow::db::DataBaseEntity * database)
{
    removeAllRows();

    meow::db::DataBaseEntity * prevDb = _database.get();

    bool changed = prevDb != database;

    if (prevDb && changed) {
        disconnect(_session.get(),
                   &meow::db::SessionEntity::entityRemoved,
                   this,
                   &DatabaseEntitiesTableModel::afterEntityRemoved);

        disconnect(_session.get(),
                   &meow::db::SessionEntity::entityInserted,
                   this,
                   &DatabaseEntitiesTableModel::onEntityInserted);
    }

    // retain database and its session
    _database = database ? database->retain() : nullptr;
    _session = database ? database->session()->retain() : nullptr;

    if (_database && changed) {
        connect(_database->session(),
                &meow::db::SessionEntity::entityRemoved,
                this,
                &DatabaseEntitiesTableModel::afterEntityRemoved);
        connect(_session.get(),
                   &meow::db::SessionEntity::entityInserted,
                   this,
                   &DatabaseEntitiesTableModel::onEntityInserted);
    }

    insertAllRows();
}

void DatabaseEntitiesTableModel::removeAllRows()
{
    if (entitiesCount()) {
        beginRemoveRows(QModelIndex(), 0, entitiesCount()-1);
        _entities.clear();
        endRemoveRows();
    }
}

void DatabaseEntitiesTableModel::insertAllRows()
{
    // Listening: Arch Enemy - Cruelty Without Beauty

    int rowsCount = _database ? _database->childCount() : 0;
    if (rowsCount) {
        beginInsertRows(QModelIndex(), 0, rowsCount-1);
        _entities = _database->entities();
        endInsertRows();
    }
}

void DatabaseEntitiesTableModel::afterEntityRemoved(
        const meow::db::EntityPtr & entity)
{
    if (entity == _database) {
        setDataBase(nullptr);
    } else {

        meow::db::Entity * database = meow::db::findParentEntityOfType(
                    entity.get(),
                    meow::db::Entity::Type::Database);
        if (database != _database.get()) return;

        int removeRow = _entities.indexOf(entity);
        if (removeRow == -1) return;

        beginRemoveRows(QModelIndex(), removeRow, removeRow);
        _entities.removeOne(entity);
        endRemoveRows();
    }
}

void DatabaseEntitiesTableModel::onEntityInserted(
        const meow::db::EntityPtr & entity)
{
    meow::db::Entity * database = meow::db::findParentEntityOfType(entity.get(),
                                              meow::db::Entity::Type::Database);
    if (database != _database.get()) return;

    int newRow = _database->indexOf(entity.get());
    if (newRow == -1) return;

    beginInsertRows(QModelIndex(), newRow, newRow);
    _entities.insert(newRow, entity);
    endInsertRows();

}

int DatabaseEntitiesTableModel::entitiesCount() const
{
    return _entities.size();
}

} // namespace models
} // namespace ui
} // namespace meow
