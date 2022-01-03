#include "routine_parameters_model.h"
#include "db/entity/routine_entity.h"
#include "ui/presenters/routine_form.h"
#include "db/connection.h"
#include <QIcon>
#include <QDebug>

namespace meow {
namespace ui {
namespace models {

RoutineParametersModel::RoutineParametersModel(
        presenters::RoutineForm * parent)
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

    if (isEditingSupported() && isEditingAllowed(index.row(), index.column())) {
        flags |= Qt::ItemIsEditable;
    }

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

    case Qt::EditRole:
        return editDataAt(index.row(), index.column());

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

bool RoutineParametersModel::setData(const QModelIndex &index,
                                     const QVariant &value,
                                     int role)
{
    if (!index.isValid() || role != Qt::EditRole) {
        return false;
    }

    if (setEditData(index, value)) {
        emit dataChanged(index, index);
        emit modified();
        return true;
    }

    return false;
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

bool RoutineParametersModel::canAddParam() const
{
    if (!isEditingSupported()) return false;
    return true;
}

bool RoutineParametersModel::canRemoveParam(const QModelIndex & curIndex) const
{
    if (!isEditingSupported()) return false;
    return _routine->structure()->canRemoveParam(curIndex.row());
}

bool RoutineParametersModel::canRemoveAllParams() const
{
    if (!isEditingSupported()) return false;
    return _routine->structure()->canRemoveAllParams();
}

bool RoutineParametersModel::canMoveParamUp(const QModelIndex & curIndex) const
{
    if (!isEditingSupported()) return false;
    return _routine->structure()->canMoveParamUp(curIndex.row());
}

bool RoutineParametersModel::canMoveParamDown(const QModelIndex & curIndex) const
{
    if (!isEditingSupported()) return false;
    return _routine->structure()->canMoveParamDown(curIndex.row());
}

int RoutineParametersModel::insertEmptyDefaultParam(int afterIndex)
{
    int insertIndex = rowCount();
    if (afterIndex > -1 && afterIndex < rowCount()) {
        insertIndex = afterIndex + 1;
    }
    beginInsertRows(QModelIndex(), insertIndex, insertIndex);
    int newRowIndex
            = _routine->structure()->insertEmptyDefaultParam(afterIndex);
    endInsertRows();

    emit modified();

    return newRowIndex;
}

bool RoutineParametersModel::removeParamAt(int index)
{
    if (!_routine->structure()->canRemoveParam(index)) {
        return false;
    }

    beginRemoveRows(QModelIndex(), index, index);
    _routine->structure()->removeParamAt(index);
    endRemoveRows();

    emit modified();

    return true;
}

void RoutineParametersModel::removeAllParams()
{
    removeData();
    emit modified();
}

bool RoutineParametersModel::moveParamUp(int index)
{
    if (_routine->structure()->canMoveParamUp(index)) {
        beginMoveRows(QModelIndex(), index, index, QModelIndex(), index - 1);
        _routine->structure()->moveParamUp(index);
        endMoveRows();

        emit modified();

        return true;
    }
    return false;
}

bool RoutineParametersModel::moveParamDown(int index)
{
    if (_routine->structure()->canMoveParamDown(index)) {
        beginMoveRows(QModelIndex(), index, index, QModelIndex(), index + 2);
        _routine->structure()->moveParamDown(index);
        endMoveRows();
        emit modified();
        return true;
    }
    return false;
}

bool RoutineParametersModel::isEditingSupported() const
{
    if (_routine == nullptr) return false;
    return _routine->connection()
            ->features()->supportsEditingRoutinesStructure();
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

QVariant RoutineParametersModel::editDataAt(int row, int col) const
{

    switch (static_cast<Columns>(col)) {
    case Columns::DataType:{
        QStringList names;

        const auto & list = _routine->connection()->dataTypes()->list();
        for (const meow::db::DataTypePtr & type : list) {
            QString name = type->name;
            if (type->hasLength && !type->defLengthSet.isEmpty()) {
                name += "(" + type->defLengthSet + ")";
            }
            names << name;
        }

        return names;
    }
    case Columns::Context: {
        QStringList contexts;
        contexts << "IN" << "OUT" << "INOUT";
        return contexts;
    }
    default:
        return textDataAt(row, col);
    }
}

bool RoutineParametersModel::setEditData(const QModelIndex &index,
                                         const QVariant &value)
{
    int row = index.row();
    int col = index.column();

    meow::db::RoutuneStructureParam & param = _routine->structure()->params[row];

    switch (static_cast<Columns>(col)) {

    case Columns::Name: {
        param.name = value.toString();
        return true;
    }

    case Columns::DataType: {
        param.dataType = value.toString();
        return true;
    }

    case Columns::Context: {
        param.context = value.toString();
        return true;
    }

    default:
        break;
    }

    return false;
}

bool RoutineParametersModel::isEditingAllowed(int row, int col) const
{
    Q_UNUSED(row);
    return static_cast<Columns>(col) != Columns::Number;
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

} // namespace models
} // namespace ui
} // namespace meow
