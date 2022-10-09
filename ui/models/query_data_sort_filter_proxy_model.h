#ifndef QUERY_DATA_SORT_FILTER_PROXY_MODEL_H
#define QUERY_DATA_SORT_FILTER_PROXY_MODEL_H

#include <QSortFilterProxyModel>

namespace meow {

namespace db {
    class QueryData;
}

namespace ui {
namespace models {

class QueryDataSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    QueryDataSortFilterProxyModel(meow::db::QueryData * queryData,
                                  QObject *parent = nullptr);
    virtual ~QueryDataSortFilterProxyModel() override;

protected:
    virtual bool filterAcceptsRow(
            int sourceRow,
            const QModelIndex &sourceParent) const override;

    virtual bool lessThan(
            const QModelIndex &left,
            const QModelIndex &right) const override;

private:
    meow::db::QueryData * _queryData;
};

} // namespace models
} // namespace ui
} // namespace meow

#endif // QUERY_DATA_SORT_FILTER_PROXY_MODEL_H
