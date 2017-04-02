#include "databases_table_model.h"
#include "db/entity/database_entity.h"
#include <QDebug>

namespace meow {
namespace models {
namespace db {

DatabasesTableModel::DatabasesTableModel(
        meow::db::SessionEntity * session,
        QObject *parent)
    :QAbstractTableModel(parent),
     _session(session)
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

        switch (static_cast<Columns>(index.column())) {

        case Columns::Database:
            return database->name();

        case Columns::Size:
            return QString::number(database->dataSize());

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
    if (databasesCount()) {
        beginRemoveRows(QModelIndex(), 0, databasesCount()-1);
        endRemoveRows();
    }

    _session = session;

    if (databasesCount()) {
        beginInsertRows(QModelIndex(), 0, databasesCount()-1);
        endInsertRows();
    }
}

int DatabasesTableModel::databasesCount() const
{
    return _session ? _session->childCount() : 0;
}

} // namespace db
} // namespace models
} // namespace meow
