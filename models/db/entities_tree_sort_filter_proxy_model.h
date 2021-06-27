#ifndef MODELS_DB_ENTITIES_TREE_SORT_FILTER_PROXY_MODEL_H
#define MODELS_DB_ENTITIES_TREE_SORT_FILTER_PROXY_MODEL_H

#include <QSortFilterProxyModel>
#include <QRegularExpression>

namespace meow {
namespace models {
namespace db {

class EntitiesTreeSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    EntitiesTreeSortFilterProxyModel(QObject * parent = nullptr);
    void setDatabaseRegexpFilter(const QString & filter);
    void setTableRegexpFilter(const QString & filter);

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const override;
private:
    QRegularExpression _databaseFilter;
    QRegularExpression _tableFilter;
};

} // namespace db
} // namespace models
} // namespace meow

#endif // MODELS_DB_ENTITIES_TREE_SORT_FILTER_PROXY_MODEL_H
