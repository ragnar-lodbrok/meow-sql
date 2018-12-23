#ifndef DATABASE_ENTITIES_TABLE_MODEL_H
#define DATABASE_ENTITIES_TABLE_MODEL_H

#include <QObject>
#include <QAbstractTableModel>
#include "db/entity/database_entity.h"

// Main Window
//   Central Right Widget
//     Database Tab
//       Table Model

namespace meow {
namespace models {
namespace db {

class DatabaseEntitiesTableModel : public QAbstractTableModel
{
public:

    enum class Columns {
        Name = 0,
        Rows,
        Size,
        Created,
        Updated,
        Engine,
        Comment,
        Version,
        //RowFormat, // TODO
        //AvgRowLength,
        //MaxDataLength,
        //IndexLength,
        //DataFree,
        //AutoIncrement,
        //CheckTime,
        Collation,
        //Checksum,
        //CreateOptions,
        //Type,
        Count
    };

    DatabaseEntitiesTableModel(QObject *parent = nullptr);
    virtual ~DatabaseEntitiesTableModel() override {}

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    void setDataBase(meow::db::DataBaseEntity * database);

    int columnWidth(int column) const;

private:

    Q_SLOT void beforeDatabaseRemoved(meow::db::Entity * entity);

    int entitiesCount() const;

    meow::db::DataBaseEntity * _database;
};


} // namespace db
} // namespace models
} // namespace meow

#endif // DATABASE_ENTITIES_TABLE_MODEL_H
