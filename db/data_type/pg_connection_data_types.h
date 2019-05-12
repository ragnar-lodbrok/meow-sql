#ifndef DB_PG_CONNECTION_DATA_TYPES_H
#define DB_PG_CONNECTION_DATA_TYPES_H

#include "connection_data_types.h"
#include <postgresql/libpq-fe.h>

namespace meow {
namespace db {

enum class PGDataTypeNativeCategory : char
{
    // see https://www.postgresql.org/docs/11/catalog-pg-type.html
    Array = 'A',
    Boolean = 'B',
    Composite = 'C',
    DateTime = 'D',
    Enum = 'E',
    Geometric = 'G',
    NetworkAddress = 'I',
    Numeric = 'N',
    Pseudo = 'P',
    Range = 'R',
    String = 'S',
    Timespan = 'T',
    UserDefined = 'U',
    BitString = 'V',
    Unknown = 'X'
    // All other ASCII characters are reserved for user-defined categories.
};

class PGConnection;

class PGConnectionDataTypes : public ConnectionDataTypes
{
public:

    PGConnectionDataTypes(PGConnection * connection);

    virtual const QList<DataTypePtr> & list() override;

    virtual const DataTypePtr defaultType() const override;

    DataTypePtr dataTypeFromNative(const Oid nativeDatatype);

private:

    void fillListManually();
    QList<DataTypePtr> selectListFromDB() const;

    void fillMapFromList();

    DataTypeCategoryIndex category(const char category, Oid type) const;

    bool isIntegerBasicType(Oid type) const;

    bool _init;
    QList<DataTypePtr> _list; // to be sorted
    QMap<Oid, DataTypePtr> _map; // for search
};

} // namespace db
} // namespace meow

#endif // DB_PG_CONNECTION_DATA_TYPES_H
