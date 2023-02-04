#include "connection_params_model.h"
#include "app/app.h"

namespace meow {
namespace ui {
namespace models {

ConnectionParamsModel::ConnectionParamsModel(meow::db::ConnectionParamsManager *manager, QObject *parent)
    :QAbstractTableModel(parent),
     _connectionParamsManager(manager),
     _selectedFormModified(false)
{

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
    case Columns::SessionName: {
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
        case Columns::SessionName:
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

        const meow::db::ConnectionParameters & params
                = _connectionParamsManager->at(index.row());

        switch (static_cast<Columns>(index.column())) {
        case Columns::SessionName: {
            bool modified = false;
            if (_selectedForm) {
                modified = _selectedFormModified && (index.row() == _selectedForm->index());
            }
            return modified ? params.sessionName() + " *" : params.sessionName();
        }
        case Columns::Host:
            if (params.isFilebased()) {
                return params.fileNameShort();
            } else {
                if (params.isSSHTunnel()) {
                    return params.sshTunnel().host() + " (SSH)";
                } else {
                    return params.hostName();
                }
            }
        default:
            break;
        }

    } else if (role == Qt::EditRole) {

        const meow::db::ConnectionParameters params
                = _connectionParamsManager->at(index.row());

        switch (static_cast<Columns>(index.column())) {
        case Columns::SessionName:
            return params.sessionName();
        default:
            break;
        }
    } else if (role == Qt::DecorationRole) {
        if (static_cast<Columns>(index.column()) == Columns::SessionName) {
            const meow::db::ConnectionParameters & params
                    = _connectionParamsManager->at(index.row());

            auto icons = meow::app()->settings()->icons();
            return icons->connection(params.serverType());
        }
    }

    return QVariant();
}

bool ConnectionParamsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid()
            && role == Qt::EditRole
            && static_cast<Columns>(index.column()) == Columns::SessionName) {

        QString newSessionName = value.toString();

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
    case Columns::SessionName:
        return 155;
    case Columns::Host:
        return 155;
    default:
        break;
    }
    return 0;
}

presenters::ConnectionParametersForm * ConnectionParamsModel::selectedForm()
{
    return _selectedForm.get();
}


presenters::ConnectionParametersForm *
ConnectionParamsModel::selectFormAt(int row)
{

    presenters::ConnectionParametersForm * form =
    new presenters::ConnectionParametersForm(
        _connectionParamsManager->at(row)
    );

    _selectedForm.reset(form);
    _selectedFormModified = false;

    connect(_selectedForm.get(), &presenters::ConnectionParametersForm::changed,
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

    const meow::db::ConnectionParameters &oldParams
            = _connectionParamsManager->at(_selectedForm->index());

    return !_selectedForm->isEqualTo(oldParams);
}

void ConnectionParamsModel::saveSelectedConnectionParam()
{
    _selectedFormModified = false;

    if (!_selectedForm) {
        return;
    }

    _connectionParamsManager->updateAndSaveParamsAt(
                _selectedForm->index(), _selectedForm->connectionParams());

    emitSelectedChanged();
}

void ConnectionParamsModel::emitSelectedChanged()
{
    if (!_selectedForm) {
        return;
    }
    QModelIndex indexStart = this->index(_selectedForm->index(),
                                     static_cast<int>(Columns::SessionName));
    QModelIndex indexEnd = this->index(_selectedForm->index(),
                                     static_cast<int>(Columns::Host));
    emit dataChanged(indexStart, indexEnd);
}

presenters::ConnectionParametersForm *
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

    beginRemoveRows(QModelIndex(), index.row(), index.row());
    _connectionParamsManager->removeParamsAndSave(paramsToRemove);
    endRemoveRows();
}

QModelIndex ConnectionParamsModel::indexForSessionNameAt(int row)
{
    return index(row, static_cast<int>(Columns::SessionName));
}

} // namespace models
} // namespace ui
} // namespace meow
