#include "database_entities_table_model.h"
#include "db/entity/table_entity.h"
#include "db/entity/session_entity.h"
#include "helpers/formatting.h"
#include <QDebug>

namespace meow {
namespace models {
namespace db {

DatabaseEntitiesTableModel::DatabaseEntitiesTableModel(
        QObject *parent)
    :QAbstractTableModel(parent),
     _database(nullptr)
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

    if (_database && _database->childrenFetched()) {
        if (role == Qt::DisplayRole) {

            meow::db::Entity * entity = _database->child(index.row());

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
                meow::db::Entity * entity = _database->child(index.row());
                if (entity) {
                    return entity->icon();
                }
            }
        }
    }

    return QVariant();
}


int DatabaseEntitiesTableModel::columnWidth(int column) const
{
    switch (static_cast<Columns>(column)) {
    case Columns::Name:
        return 180;
    case Columns::Collation:
        return 140;
    case Columns::Created:
    case Columns::Updated:
        return 130;
    default:
        return 80;
    }
}

void DatabaseEntitiesTableModel::setDataBase(meow::db::DataBaseEntity * database)
{
    if (entitiesCount()) {
        beginRemoveRows(QModelIndex(), 0, entitiesCount()-1);
        endRemoveRows();
    }

    _database = database;

    if (_database) {
        connect(_database->session(),
                &meow::db::SessionEntity::beforeEntityRemoved,
                this,
                &DatabaseEntitiesTableModel::beforeDatabaseRemoved);
    }

    if (entitiesCount()) {
        beginInsertRows(QModelIndex(), 0, entitiesCount()-1);
        endInsertRows();
    }
}

void DatabaseEntitiesTableModel::beforeDatabaseRemoved(
        meow::db::Entity * entity)
{
    if (entity == _database) {
        setDataBase(nullptr);
    }
}

int DatabaseEntitiesTableModel::entitiesCount() const
{
    return _database ? _database->childCount() : 0;
}

} // namespace db
} // namespace models
} // namespace meow
