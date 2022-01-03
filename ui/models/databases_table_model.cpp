#include <QDebug>
#include "databases_table_model.h"
#include "db/entity/database_entity.h"
#include "helpers/formatting.h"

namespace meow {
namespace ui {
namespace models {

DatabasesTableModel::DatabasesTableModel(
        QObject *parent)
    : QAbstractTableModel(parent)
    , _session(nullptr)
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

        meow::db::DataBaseEntity * database = _databases.at(index.row()).get();

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

    meow::db::SessionEntity * prevSession = _session.get();

    bool changed = prevSession != session;

    if (prevSession && changed) {
        disconnect(prevSession,
                &meow::db::SessionEntity::databaseInserted,
                this,
                &DatabasesTableModel::onDatabaseInserted);
        disconnect(prevSession,
                &meow::db::SessionEntity::databaseRemoved,
                this,
                &DatabasesTableModel::onDatabaseRemoved);
    }

    _session = session ? session->retain() : nullptr;

    if (_session && changed) {
        connect(_session.get(),
                &meow::db::SessionEntity::databaseInserted,
                this,
                &DatabasesTableModel::onDatabaseInserted);
        connect(_session.get(),
                &meow::db::SessionEntity::databaseRemoved,
                this,
                &DatabasesTableModel::onDatabaseRemoved);
    }

    insertAllRows();
}

void DatabasesTableModel::removeAllRows()
{
    if (databasesCount()) {
        beginRemoveRows(QModelIndex(), 0, databasesCount()-1);
        _databases.clear();
        endRemoveRows();
    }
}

void DatabasesTableModel::insertAllRows()
{
    int rowsCount = _session ? _session->childCount() : 0;
    if (rowsCount) {
        beginInsertRows(QModelIndex(), 0, rowsCount-1);
        _databases = _session->databases();
        endInsertRows();
    }
}

void DatabasesTableModel::onDatabaseInserted(
        const meow::db::DataBaseEntityPtr & database)
{
    Q_UNUSED(database);
    // lazy solution:
    removeAllRows();
    insertAllRows();
}

void DatabasesTableModel::onDatabaseRemoved(
        const meow::db::DataBaseEntityPtr & database)
{

    int removeRow = _databases.indexOf(database);
    if (removeRow == -1) return;

    beginRemoveRows(QModelIndex(), removeRow, removeRow);
    _databases.removeOne(database);
    endRemoveRows();
}

void DatabasesTableModel::refresh()
{
    setSession(_session.get());
}

int DatabasesTableModel::databasesCount() const
{
    return _databases.size();
}

} // namespace models
} // namespace ui
} // namespace meow
