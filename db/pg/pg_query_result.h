#ifndef DB_PG_QUERY_RESULT_H
#define DB_PG_QUERY_RESULT_H

#include <libpq-fe.h>
#include "db/native_query_result.h"
#include "db/common.h"

namespace meow {
namespace db {

// Wraps and owns PGresult ptr
class PGQueryResult : public NativeQueryResult
{
public:
    PGQueryResult(Connection * connection) :
        NativeQueryResult(connection),
        _res(nullptr),
        _currentResult(nullptr),
        _connectionHandle(nullptr),
        _columnsParsed(false)
    {

    }

    void init(PGresult * res, PGconn * connectionHandle);

    virtual ~PGQueryResult() override {
        freeNative();
    }

    virtual db::ulonglong nativeRowsCount() const override {
        return static_cast<db::ulonglong>(PQntuples(_res));
    }

    virtual bool hasData() const override;

    virtual void seekRecNo(db::ulonglong value) override;

    virtual QString curRowColumn(std::size_t index,
                                 bool ignoreErrors = false) override;

    virtual bool isNull(std::size_t index) override;

    void clearAll();

    PGresult * nativePtr() const { return _res; }
protected:
    virtual void prepareResultForEditing(NativeQueryResult * result) override;
private:

    void freeNative() {
        if (_res) {
            PQclear(_res);
            _res = nullptr;
        }
    }

    void clearColumnData();
    void addColumnData(PGresult * res);
    std::vector<PGresult *> resultList() const;
    QString rowDataToString(PGresult * result,
                            int row,
                            int col,
                            int dataLen);

    PGresult * _res;
    PGresult * _currentResult;
    PGconn * _connectionHandle;
    std::vector<unsigned int> _columnLengths;
    bool _columnsParsed;
    db::ulonglong _curRecNoLocal;
};

using PGQueryResultPtr = std::shared_ptr<PGQueryResult>;

} // namespace db
} // namespace meow

#endif // DB_PG_QUERY_RESULT_H
