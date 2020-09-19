#include "variables_table_model.h"
#include "db/connection.h"
#include "app/app.h"
#include <QGuiApplication>
#include <QPalette>
#include <QDebug>

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

    switch (static_cast<Columns>(index.column())) {

    case Columns::Session:
    case Columns::Global:
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;

    default:
        break;
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

    if (role == Qt::DisplayRole || role == Qt::EditRole) {

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
    } else if (role == Qt::BackgroundRole) {
        if (static_cast<Columns>(index.column()) == Columns::Session) {

            // highlight(red) if session != global
            const QStringList & names = vars()->allNames();
            const QString & name = names[index.row()];

            if (vars()->sessionAndGlobalDiffers(name)) {
                QColor bgColor = QGuiApplication::palette()
                        .color(QPalette::Window);
                bgColor.setRedF(qMin(bgColor.redF() + 0.075, 1.0));
                return QVariant::fromValue(bgColor);
            }


        }
    } else if (role == Qt::ForegroundRole) {

        switch (static_cast<Columns>(index.column())) {

        case Columns::Session:
        case Columns::Global: {
            auto textSettings = meow::app()->settings()->textSettings();
            const QStringList & names = vars()->allNames();
            const QString & name = names[index.row()];
            return textSettings->colorForDataType(vars()->dataType(name));
        }

        default:
            break;
        }
    }

    return QVariant();
}

bool VariablesTableModel::setData(const QModelIndex &index,
                                const QVariant &value,
                                int role)
{
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    Columns column = static_cast<Columns>(index.column());
    bool isGlobal = column == Columns::Global;

    if (column == Columns::Session || isGlobal) {

        const QStringList & names = vars()->allNames();
        const QString & name = names[index.row()];

        bool edited = false;

        try {
            edited = vars()->editVariableInDB(name, value.toString(), isGlobal);
        } catch(meow::db::Exception & ex) {
            emit setDataFailed(ex.message());
        }


        if (edited) {
            refresh(); // editing global may change local
            return true;
        }
    }

    return false;
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
