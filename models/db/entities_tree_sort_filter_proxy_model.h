#ifndef MODELS_DB_ENTITIES_TREE_SORT_FILTER_PROXY_MODEL_H
#define MODELS_DB_ENTITIES_TREE_SORT_FILTER_PROXY_MODEL_H

#include <QSortFilterProxyModel>

namespace meow {
namespace models {
namespace db {

class EntitiesTreeSortFilterProxyModel : public QSortFilterProxyModel
{
public:
    EntitiesTreeSortFilterProxyModel(QObject * parent = nullptr);
};

} // namespace db
} // namespace models
} // namespace meow

#endif // MODELS_DB_ENTITIES_TREE_SORT_FILTER_PROXY_MODEL_H
