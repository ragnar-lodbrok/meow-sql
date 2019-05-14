#ifndef DB_PG_QUERY_DATA_EDITOR_H
#define DB_PG_QUERY_DATA_EDITOR_H

#include "db/query_data_editor.h"

namespace meow {
namespace db {

class PGQueryDataEditor : public QueryDataEditor
{

    virtual bool loadModificationsResult() const override {
        return _modificationsLoaded == false;
    }

protected:
    virtual void insert(QueryData * data,
                const QStringList & columns,
                const QStringList & values) override;
private:
    bool _modificationsLoaded = false;
};

} // namespace db
} // namespace meow

#endif // DB_PG_QUERY_DATA_EDITOR_H
