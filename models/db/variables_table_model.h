#ifndef MODELS_VARIABLES_TABLE_MODEL_H
#define MODELS_VARIABLES_TABLE_MODEL_H

#include <QAbstractTableModel>
#include "db/entity/session_entity.h"

// Main Window
//   Central Right Widget
//     Host Tab
//       Databases Tab
//         Variables Model

namespace meow {

namespace db {
    class SessionVariables;
}

namespace models {
namespace db {

class VariablesTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:

    enum class Columns {
        Variable = 0,
        Session,
        Global,
        Count
    };

    VariablesTableModel(QObject *parent = nullptr);

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnWidth(int column) const;
    void setSession(meow::db::SessionEntity * session);
    void refresh();

    Q_SIGNAL void setDataFailed(const QString & message);

private:

    void removeAllRows();
    void insertAllRows();

    int variablesCount() const;

    meow::db::SessionVariables * vars() const;

    meow::db::SessionEntity * _session;
};

} // namespace db
} // namespace models
} // namespace meow

#endif // MODELS_VARIABLES_TABLE_MODEL_H
