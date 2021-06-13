#ifndef DATABASES_TABLE_MODEL_H
#define DATABASES_TABLE_MODEL_H

#include <QString>
#include <QObject>
#include <QAbstractTableModel>
#include "db/entity/session_entity.h"

// Main Window
//   Central Right Widget
//     Host Tab
//       Databases Tab
//         Table Model

namespace meow {
namespace models {
namespace db {

class DatabasesTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum class Columns {
        Database = 0,
        Size,
        Items,
        LastModification,
        Tables,
        Views,
        Functions,
        Procedures,
        Triggers,
        Events,
        DefaultCollation,
        Count
    };

public:
    DatabasesTableModel(QObject *parent = nullptr);
    virtual ~DatabasesTableModel() override {}

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    int columnWidth(int column) const;
    void setSession(meow::db::SessionEntity * session);

private:

    Q_SLOT void onDatabaseInserted(const meow::db::DataBaseEntityPtr & database);
    Q_SLOT void onDatabaseRemoved(const meow::db::DataBaseEntityPtr & database);

    void removeAllRows();
    void insertAllRows();
    void refresh();

    int databasesCount() const;

    meow::db::SessionEntityPtr _session;
    QList<meow::db::DataBaseEntityPtr> _databases;
};

} // namespace db
} // namespace models
} // namespace meow

#endif // DATABASES_TABLE_MODEL_H
