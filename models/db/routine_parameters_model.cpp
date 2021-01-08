#include "routine_parameters_model.h"
#include "db/entity/routine_entity.h"
#include "models/forms/routine_form.h"
#include <QIcon>
#include <QDebug>

namespace meow {
namespace models {
namespace db {

RoutineParametersModel::RoutineParametersModel(
        meow::models::forms::RoutineForm * parent)
    : QAbstractTableModel(parent)
    , _form(parent)
    , _routine(nullptr)
{

}

void RoutineParametersModel::setRoutine(meow::db::RoutineEntity * routine)
{
    removeData();
    _routine = routine;
    insertData();
}

int RoutineParametersModel::columnCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(Columns::Count);
}

int RoutineParametersModel::rowCount(const QModelIndex & parent) const
{
    Q_UNUSED(parent);
    if (_routine && _routine->structure()) {
        return _routine->structure()->params.size();
    }
    return 0;
}

Qt::ItemFlags RoutineParametersModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return {};
    }

    Qt::ItemFlags flags = QAbstractItemModel::flags(index);

    // TODO: editing flags

    return flags;
}

QVariant RoutineParametersModel::headerData(
                                    int section,
                                    Qt::Orientation orientation,
                                    int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (static_cast<Columns>(section)) {

        case Columns::Number:
            return QString(tr("#"));

        case Columns::Name:
            return QString(tr("Name"));

        case Columns::DataType:
            return QString(tr("Datatype"));

        case Columns::Context:
            return QString(tr("Context"));

        default:
            break;
        }
    }

    return QVariant();
}

QVariant RoutineParametersModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() || index.row() >= rowCount()) {
        return QVariant();
    }

    switch (role) {

    case Qt::DisplayRole:
        return textDataAt(index.row(), index.column());

    case Qt::DecorationRole:
        if (static_cast<Columns>(index.column()) == Columns::Context) {
            const meow::db::RoutuneStructureParam & param
                    = _routine->structure()->params.at(index.row());
            if (param.context == "IN") {
                return QIcon(":/icons/go_right.png");
            } else if (param.context == "OUT") {
                return QIcon(":/icons/go_left.png");
            } else if (param.context == "INOUT") {
                return QIcon(":/icons/go_both.png");
            }
        }

    default:
        break;
    }

    return QVariant();
}

int RoutineParametersModel::columnWidth(int column) const
{
    switch (static_cast<Columns>(column)) {

    case Columns::Number:
        return 60;

    case Columns::Name:
        return 250;

    case Columns::DataType:
        return 200;

    case Columns::Context:
        return 200;

    default:
       return 130;
    }
}

QString RoutineParametersModel::textDataAt(int row, int col) const
{
    const meow::db::RoutuneStructureParam & param
            = _routine->structure()->params.at(row);

    switch (static_cast<Columns>(col)) {

    case Columns::Number:
        return QString::number(row+1);

    case Columns::Name:
        return param.name;

    case Columns::DataType:
        return param.dataType;

    case Columns::Context:
        return param.context;

    default:
       return QString();
    }
}

void RoutineParametersModel::removeData()
{
    if (rowCount()) {
        beginRemoveRows(QModelIndex(), 0, rowCount()-1);
        endRemoveRows();
    }
}

void RoutineParametersModel::insertData()
{
    if (rowCount() > 0) {
        beginInsertRows(QModelIndex(), 0, rowCount()-1);
        endInsertRows();
    }
}

} // namespace forms
} // namespace models
} // namespace meow
