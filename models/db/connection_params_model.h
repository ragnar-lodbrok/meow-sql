#ifndef MODELS_DB_CONNECTION_PARAMS_MODEL_H
#define MODELS_DB_CONNECTION_PARAMS_MODEL_H

#include <memory>
#include <QAbstractTableModel>
#include <QString>
#include <QObject>

#include "db/connection_params_manager.h"
#include "models/forms/connection_parameters_form.h"

namespace meow {
namespace models {
namespace db {

class ConnectionParamsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum class Columns {
      SessionName = 0,
      Host,
      Count
    };
public:
    ConnectionParamsModel(meow::db::ConnectionParamsManager *manager,
                          QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index,
                 const QVariant &value,
                 int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnWidth(int column) const;
    bool isSelectedConnectionParamModified() const {
        return _selectedFormModified;
    }

    meow::models::forms::ConnectionParametersForm * selectFormAt(int row);
    meow::models::forms::ConnectionParametersForm * createNewForm();
    meow::models::forms::ConnectionParametersForm * selectedForm();

    void deleteSelectedFormAt(const QModelIndex &index);
    bool checkIsSelectedFormWasModified() const;
    void saveSelectedConnectionParam();

    QModelIndex indexForSessionNameAt(int row);

    Q_SIGNAL void selectedFormDataModified();
    Q_SIGNAL void settingEmptySessionName();
    Q_SIGNAL void settingTakenSessionName(const QString& newName);

private:

    void emitSelectedChanged();
    meow::db::ConnectionParamsManager * _connectionParamsManager;
    std::shared_ptr<meow::models::forms::ConnectionParametersForm> _selectedForm;
    bool _selectedFormModified;
};

} // namespace db
} // namespace models
} // namespace meow

#endif // MODELS_DB_CONNECTION_PARAMS_MODEL_H
