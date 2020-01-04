#ifndef PG_ENTITY_CREATE_CODE_GENERATOR_H
#define PG_ENTITY_CREATE_CODE_GENERATOR_H

#include <QString>

namespace meow {
namespace db {

class PGConnection;
class Entity;
class TableEntity;

class PGEntityCreateCodeGenerator
{
public:
    explicit PGEntityCreateCodeGenerator(PGConnection * connection);
    QString run(const Entity * entity);
private:

    QString run(const TableEntity * table);

    QString SQLToSelectColumnsInfo(const QString & tableName);
    QString createColumnsSQL(const QString & tableName);

    QString SQLToSelectKeysInfo(const QString & tableName);
    QString createKeysSQL(const QString & tableName);

    PGConnection * _connection;
};

} // namespace db
} // namespace meow

#endif // PG_ENTITY_CREATE_CODE_GENERATOR_H
