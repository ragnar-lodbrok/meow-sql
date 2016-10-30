#include "connection_params_model.h"

#include "db/mysql_connection.h" // test

#include <QDebug>

namespace meow {
namespace models {
namespace db {

ConnectionParamsModel::ConnectionParamsModel(meow::db::ConnectionParamsManager *manager, QObject *parent)
    :QAbstractTableModel(parent),
     _connectionParamsManager(manager),
     _selectedFormModified(false)
{
    //https://doc.qt.io/archives/4.6/itemviews-addressbook.html
}

int ConnectionParamsModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return static_cast<int>(Columns::Count);
}

int ConnectionParamsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return _connectionParamsManager->count();
}

Qt::ItemFlags ConnectionParamsModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    switch (static_cast<Columns>(index.column())) {
    case Columns::Session: {
        return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
    }
    default:
        break;
    }

    return QAbstractItemModel::flags(index);
}

QVariant ConnectionParamsModel::headerData(int section,
                                           Qt::Orientation orientation,
                                           int role) const
{
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal) {
        switch (static_cast<Columns>(section)) {
        case Columns::Session:
            return QString(tr("Session name"));
        case Columns::Host:
            return QString(tr("Host"));
        default:
            break;
        }
    }

    return QVariant();
}

QVariant ConnectionParamsModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid()) {
        return QVariant();
    }

    if (index.row() >= _connectionParamsManager->count()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {

        const meow::db::ConnectionParameters params = _connectionParamsManager->at(index.row());

        switch (static_cast<Columns>(index.column())) {
        case Columns::Session: {
            bool modified = false;
            if (_selectedForm) {
                modified = _selectedFormModified && (index.row() == _selectedForm->index());
            }
            return modified ? params.sessionName() + " *" : params.sessionName();
        }
        case Columns::Host:
            return params.hostName();
        default:
            break;
        }

    } else if (role == Qt::EditRole) {

        const meow::db::ConnectionParameters params = _connectionParamsManager->at(index.row());

        switch (static_cast<Columns>(index.column())) {
        case Columns::Session:
            return params.sessionName();
        default:
            break;
        }
    }

    return QVariant();
}

bool ConnectionParamsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid()
            && role == Qt::EditRole
            && static_cast<Columns>(index.column()) == Columns::Session) {

        QString newSessionName = value.toString();

        qDebug() << "setData: " << newSessionName;

        if (newSessionName.isEmpty()) {
            emit settingEmptySessionName();
            return false;
        }

        bool changedSelectedForm = _selectedForm && _selectedForm->index() == index.row();

        bool renamed = _connectionParamsManager->renameParamsAndSave(
            _connectionParamsManager->at(index.row()),
            value.toString()
        );

        if (!renamed) {
            emit settingTakenSessionName(newSessionName);
            return false;
        }

        qDebug() << "changedSelectedForm: " << changedSelectedForm;

        if (changedSelectedForm) {

            _selectedForm->setSessionName(newSessionName);
        }

        emit dataChanged(index, index);
        return true;
    }

    return false;
}


int ConnectionParamsModel::columnWidth(int column) const
{
    switch (static_cast<Columns>(column)) {
    case Columns::Session:
        return 130;
    case Columns::Host:
        return 80;
    default:
        break;
    }
    return 0;
}

meow::models::forms::ConnectionParametersForm *
ConnectionParamsModel::selectedForm()
{
    return _selectedForm.get();
}


meow::models::forms::ConnectionParametersForm *
ConnectionParamsModel::selectFormAt(int row)
{

    qDebug() << "Select session: " << _connectionParamsManager->at(row).sessionName();

    meow::models::forms::ConnectionParametersForm * form =
    new meow::models::forms::ConnectionParametersForm(
        _connectionParamsManager->at(row)
    );

    meow::db::MySQLConnection connection(_connectionParamsManager->at(row));

    try {
        connection.setActive(true);
    } catch(meow::db::Exception & ex) {
        qDebug() << "Failed to connect: " << ex.message();
    }



    qDebug() << "Fetched DBs: \n" << connection.allDatabases();

    _selectedForm.reset(form);
    _selectedFormModified = false;

    connect(_selectedForm.get(), &forms::ConnectionParametersForm::changed,
            [=]{
                bool modified = checkIsSelectedFormWasModified();

                if (_selectedFormModified != modified) {
                    _selectedFormModified = modified;
                    emitSelectedChanged();
                    emit selectedFormDataModified();
                }
            });

    return form;
}

bool ConnectionParamsModel::checkIsSelectedFormWasModified() const
{
    if (!_selectedForm) {
        return false;
    }

    const meow::db::ConnectionParameters &oldParams = _connectionParamsManager->at(_selectedForm->index());

    return !_selectedForm->isEqualTo(oldParams);
}

void ConnectionParamsModel::saveSelectedConnectionParam()
{
    _selectedFormModified = false;

    if (!_selectedForm) {
        return;
    }

    _connectionParamsManager->updateAndSaveParamsAt(_selectedForm->index(), _selectedForm->connectionParams());

    emitSelectedChanged();
}

void ConnectionParamsModel::emitSelectedChanged()
{
    if (!_selectedForm) {
        return;
    }
    QModelIndex indexStart = this->index(_selectedForm->index(),
                                     static_cast<int>(Columns::Session));
    QModelIndex indexEnd = this->index(_selectedForm->index(),
                                     static_cast<int>(Columns::Host));
    emit dataChanged(indexStart, indexEnd);
}

meow::models::forms::ConnectionParametersForm *
ConnectionParamsModel::createNewForm()
{
    const meow::db::ConnectionParameters &newParams = _connectionParamsManager->createNewParams();

    int addedRowIndex = _connectionParamsManager->indexOfParams(newParams);

    beginInsertRows(QModelIndex(), addedRowIndex, addedRowIndex);
    endInsertRows();

    return selectFormAt(addedRowIndex);

}

void ConnectionParamsModel::deleteSelectedFormAt(const QModelIndex &index)
{

    if (!index.isValid()) {
        return;
    }

    _selectedFormModified = false;
    _selectedForm.reset();

    const meow::db::ConnectionParameters& paramsToRemove = _connectionParamsManager->at(index.row());

    qDebug() << "Removing: " << paramsToRemove.sessionName();
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    _connectionParamsManager->removeParamsAndSave(paramsToRemove);
    qDebug() << "endRemoveRows";
    endRemoveRows();
}

QModelIndex ConnectionParamsModel::indexForSessionNameAt(int row)
{
    return index(row, static_cast<int>(Columns::Session));
}

} // namespace db
} // namespace models
} // namespace meow
