#ifndef DB_CONNECTION_FEATURES_H
#define DB_CONNECTION_FEATURES_H

#include "db/entity/entity.h"

namespace meow {
namespace db {

class TableEntity;
class Connection;

// Intent: holds functonality that connection supports
class ConnectionFeatures
{
public:

    ConnectionFeatures(Connection * connection);
    virtual ~ConnectionFeatures();

    virtual bool supportsViewingTables() const {
        return false;
    }

    virtual bool supportsViewingTablesData() const {
        return false;
    }

    virtual bool supportsDrop(Entity::Type type) const {
        Q_UNUSED(type);
        return false;
    }

    virtual bool supportsCreation(Entity::Type type) const {
        Q_UNUSED(type);
        return false;
    }

    virtual bool supportsForeignKeys(const TableEntity * table) const {
        Q_UNUSED(table);
        return false;
    }
};

// -----------------------------------------------------------------------------

class MySQLConnectionFeatures : public ConnectionFeatures
{
public:

    using ConnectionFeatures::ConnectionFeatures;

    virtual bool supportsViewingTables() const override {
        return true;
    }

    virtual bool supportsViewingTablesData() const override {
        return true;
    }

    virtual bool supportsDrop(Entity::Type type) const override {
        switch (type) {
            case meow::db::Entity::Type::Table: // only impl-ed
            case meow::db::Entity::Type::Database:
            return true;
        default:
            return false;
        };
    }

    virtual bool supportsCreation(Entity::Type type) const override {
        switch (type) {
            case meow::db::Entity::Type::Table: // only impl-ed
            case meow::db::Entity::Type::Database:
            return true;
        default:
            return false;
        };
    }

    virtual bool supportsForeignKeys(const TableEntity * table) const override;
};

// -----------------------------------------------------------------------------

class PGConnectionFeatures : public ConnectionFeatures
{
public:

    using ConnectionFeatures::ConnectionFeatures;

    virtual bool supportsViewingTables() const override {
        // not yet implemented
        return false;
    }

    virtual bool supportsViewingTablesData() const override {
        return true;
    }

    virtual bool supportsForeignKeys(const TableEntity * table) const override {
        Q_UNUSED(table);
        // not yet implemented
        return false;
    }
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTION_FEATURES_H
