#include <QDebug>
#include "databases_table_model.h"
#include "db/entity/database_entity.h"
#include "helpers/formatting.h"

namespace meow {
namespace models {
namespace db {

DatabasesTableModel::DatabasesTableModel(
        QObject *parent)
    :QAbstractTableModel(parent),
     _session(nullptr)
{

}

int DatabasesTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(Columns::Count);
}

int DatabasesTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return databasesCount();
}

Qt::ItemFlags DatabasesTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index);
}

QVariant DatabasesTableModel::headerData(int section,
                                           Qt::Orientation orientation,
                                           int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (static_cast<Columns>(section)) {
        case Columns::Database:
            return QString(tr("Database"));

        case Columns::Size:
            return QString(tr("Size"));

        case Columns::Items:
            return QString(tr("Items"));

        case Columns::LastModification:
            return QString(tr("Last modification"));

        case Columns::Tables:
            return QString(tr("Tables"));

        case Columns::Views:
            return QString(tr("Views"));

        case Columns::Functions:
            return QString(tr("Functions"));

        case Columns::Procedures:
            return QString(tr("Procedures"));

        case Columns::Triggers:
            return QString(tr("Triggers"));

        case Columns::Events:
            return QString(tr("Events"));

        case Columns::DefaultCollation:
            return QString(tr("Default collation"));

        default:
            break;
        }
    }

    return QVariant();
}

QVariant DatabasesTableModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= databasesCount()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole && _session) {

        meow::db::DataBaseEntity * database =
            static_cast<meow::db::DataBaseEntity *>(
                _session->child(index.row())
            );

        bool childrenFetched = database->childrenFetched();

        switch (static_cast<Columns>(index.column())) {

        case Columns::Database:
            return database->name();

        case Columns::Size:
            if (childrenFetched) {
                return helpers::formatByteSize(database->dataSize());
            }
            break;

        case Columns::Items:
            return childrenFetched ? database->childCount() : QVariant();

        case Columns::LastModification:
            return QVariant();

        case Columns::Tables:
            if (childrenFetched) {
                return childCountOfType(database,
                                        meow::db::Entity::Type::Table);
            }
            break;

        case Columns::Views:
            if (childrenFetched) {
                return childCountOfType(database,
                                        meow::db::Entity::Type::View);
            }
            break;

        case Columns::Functions:
            if (childrenFetched) {
                return childCountOfType(database,
                                        meow::db::Entity::Type::Function);
            }
            break;
        case Columns::Procedures:
            if (childrenFetched) {
                return childCountOfType(database,
                                        meow::db::Entity::Type::Procedure);
            }
            break;
        case Columns::Triggers:
            if (childrenFetched) {
                return childCountOfType(database,
                                        meow::db::Entity::Type::Trigger);
            }
            break;

        case Columns::Events:
            //if (childrenFetched) {
            //    return childCountOfType(database,
            //                            meow::db::Entity::Type::Event);
            //}
            return QVariant();
        default:
            break;
        }

    }

    return QVariant();
}

int DatabasesTableModel::columnWidth(int column) const
{
    switch (static_cast<Columns>(column)) {
    case Columns::Database:
    case Columns::LastModification:
    case Columns::DefaultCollation:
        return 150;
    default:
        return 80;
    }
}

void DatabasesTableModel::setSession(meow::db::SessionEntity * session)
{
    removeAllRows();

    bool changed = _session != session;

    if (_session && changed) {
        disconnect(_session,
                &meow::db::SessionEntity::beforeEntityRemoved,
                this,
                &DatabasesTableModel::beforeDatabaseRemoved);
        disconnect(_session,
                &meow::db::SessionEntity::enitityRemoved,
                this,
                &DatabasesTableModel::afterDatabaseRemoved);
    }

    _session = session;

    if (_session && changed) {
        connect(_session,
                &meow::db::SessionEntity::beforeEntityRemoved,
                this,
                &DatabasesTableModel::beforeDatabaseRemoved);
        connect(_session,
                &meow::db::SessionEntity::enitityRemoved,
                this,
                &DatabasesTableModel::afterDatabaseRemoved);
    }

    insertAllRows();
}

void DatabasesTableModel::removeAllRows()
{
    if (databasesCount()) {
        beginRemoveRows(QModelIndex(), 0, databasesCount()-1);
        endRemoveRows();
    }
}

void DatabasesTableModel::insertAllRows()
{
    if (databasesCount()) {
        beginInsertRows(QModelIndex(), 0, databasesCount()-1);
        endInsertRows();
    }
}

void DatabasesTableModel::beforeDatabaseRemoved(meow::db::Entity * entity)
{
    if (entity->type() == meow::db::Entity::Type::Database) {
        meow::db::DataBaseEntity * database
            = static_cast<meow::db::DataBaseEntity *>(entity);
        if (database->session() == _session) {
            removeAllRows();
        }
    }
}

void DatabasesTableModel::afterDatabaseRemoved(meow::db::Entity * entity)
{
    if (entity->type() == meow::db::Entity::Type::Database) {
        meow::db::DataBaseEntity * database
            = static_cast<meow::db::DataBaseEntity *>(entity);
        if (database->session() == _session) {
            insertAllRows();
        }
    }
}

void DatabasesTableModel::refresh()
{
    setSession(_session);
}

int DatabasesTableModel::databasesCount() const
{
    return _session ? _session->childCount() : 0;
}

} // namespace db
} // namespace models
} // namespace meow
