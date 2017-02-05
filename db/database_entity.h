#ifndef DB_DATABASEENTITY_H
#define DB_DATABASEENTITY_H

#include <QString>
#include "entity.h"


namespace meow {
namespace db {

class SessionEntity;

class DataBaseEntity : public Entity
{
public:
    explicit DataBaseEntity(const QString & dbName, SessionEntity * parent);
    virtual QString name() const override;
    virtual Connection * connection() const override;
    virtual int row() const override;
    //virtual int childCount() const override;
    //virtual Entity * child(int row) const override;

    virtual Type type() const override { return Type::Database; }
    virtual QVariant icon() const override;

private:

    SessionEntity * session() const;

    QString _dbName;
};

} // namespace db
} // namespace meow

#endif // DB_DATABASEENTITY_H
