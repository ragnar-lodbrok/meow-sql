#include "database_entities_table_model.h"
#include "db/entity/table_entity.h"

namespace meow {
namespace models {
namespace db {

DatabaseEntitiesTableModel::DatabaseEntitiesTableModel(
        meow::db::DataBaseEntity * database,
        QObject *parent)
    :QAbstractTableModel(parent),
     _database(database)
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

    if (role == Qt::DisplayRole && _database && _database->childrenFetched()) {

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
                return table->rowsCount(); // TODO format
            } else {
                return QVariant();
            }

        case Columns::Size:
            if (entity->hasDataSize()) {
                return helpers::formatByteSize(entity->dataSize());
            } else {
                return QVariant();
            }

        case Columns::Created:

        case Columns::Updated:
            return QVariant(); // TODO

        case Columns::Engine:
            if (table) {
                return table->engine();
            } else {
                return QVariant();
            }

        case Columns::Comment:

        case Columns::Version:
            return QVariant(); // TODO

        case Columns::Collation:
            if (table) {
                return table->collation();
            } else {
                return QVariant();
            }

        default:
            break;
        }

    }

    return QVariant();
}


int DatabaseEntitiesTableModel::columnWidth(int column) const
{
    switch (static_cast<Columns>(column)) {
    case Columns::Name:
    case Columns::Collation:
        return 180;
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

    if (entitiesCount()) {
        beginInsertRows(QModelIndex(), 0, entitiesCount()-1);
        endInsertRows();
    }
}

int DatabaseEntitiesTableModel::entitiesCount() const
{
    return _database ? _database->childCount() : 0;
}

} // namespace db
} // namespace models
} // namespace meow
