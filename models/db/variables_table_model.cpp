#include "variables_table_model.h"
#include "db/connection.h"

namespace meow {
namespace models {
namespace db {

VariablesTableModel::VariablesTableModel(QObject *parent)
    : QAbstractTableModel(parent)
    , _session(nullptr)
{

}

int VariablesTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(Columns::Count);
}

int VariablesTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return variablesCount();
}

Qt::ItemFlags VariablesTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractItemModel::flags(index);
}

QVariant VariablesTableModel::headerData(int section,
                                           Qt::Orientation orientation,
                                           int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (static_cast<Columns>(section)) {
        case Columns::Variable:
            return QString(tr("Variable"));

        case Columns::Session:
            return QString(tr("Session"));

        case Columns::Global:
            return QString(tr("Global"));

        default:
            break;
        }
    }

    return QVariant();
}

QVariant VariablesTableModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= variablesCount()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {

        const QStringList & names = vars()->allNames();
        const QString & name = names[index.row()];

        switch (static_cast<Columns>(index.column())) {

        case Columns::Variable:
            return name;

        case Columns::Session:
            return vars()->value(name, false);

        case Columns::Global:
            return vars()->value(name, true);

        default:
            break;
        }

    }

    return QVariant();
}

int VariablesTableModel::columnWidth(int column) const
{
    switch (static_cast<Columns>(column)) {
    case Columns::Variable:
    case Columns::Session:
        return 250;
    case Columns::Global:
        return 400;
    default:
        return 200;
    }
}

void VariablesTableModel::setSession(meow::db::SessionEntity * session)
{
    if (_session == session) {
        return;
    }

    removeAllRows();

    _session = session;
    if (vars()) {
        vars()->fetch();
    }

    insertAllRows();
}

void VariablesTableModel::refresh()
{
    removeAllRows();
    if (vars()) {
        vars()->fetch(true);
    }
    insertAllRows();
}

void VariablesTableModel::removeAllRows()
{
    if (variablesCount()) {
        beginRemoveRows(QModelIndex(), 0, variablesCount()-1);
        endRemoveRows();
    }
}

void VariablesTableModel::insertAllRows()
{
    if (variablesCount()) {
        beginInsertRows(QModelIndex(), 0, variablesCount()-1);
        endInsertRows();
    }
}

int VariablesTableModel::variablesCount() const
{
    if (!vars()) {
        return 0;
    }
    return vars()->totalCount();
}

meow::db::SessionVariables * VariablesTableModel::vars() const
{
    if (_session) {
        return _session->connection()->variables();
    }
    return nullptr;
}

} // namespace db
} // namespace models
} // namespace meow
