#ifndef QUERY_DATA_SORT_FILTER_PROXY_MODEL_H
#define QUERY_DATA_SORT_FILTER_PROXY_MODEL_H

#include <QSortFilterProxyModel>

namespace meow {

namespace db {
    class QueryData;
}

namespace models {
namespace db {

class QueryDataSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    QueryDataSortFilterProxyModel(meow::db::QueryData * queryData,
                                  QObject *parent = nullptr);
    virtual ~QueryDataSortFilterProxyModel() override;

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex &sourceParent) const override;

private:
    meow::db::QueryData * _queryData;
};

} // namespace db
} // namespace models
} // namespace meow

#endif // QUERY_DATA_SORT_FILTER_PROXY_MODEL_H
