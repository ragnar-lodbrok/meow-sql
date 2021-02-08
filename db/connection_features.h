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

    explicit ConnectionFeatures(Connection * connection);
    virtual ~ConnectionFeatures();

    virtual bool supportsViewingTables() const {
        return false;
    }

    virtual bool supportsViewingTablesData() const {
        return false;
    }

    virtual bool supportsEditingTablesData() const {
        return false;
    }

    virtual bool supportsEditingViewsData() const {
        return false;
    }

    virtual bool supportsEditingTablesStructure() const {
        return false;
    }

    virtual bool supportsEditingViewsStructure() const {
        return false;
    }

    virtual bool supportsEditingDatabase() const {
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

    virtual bool supportsDumping() const {
        return false;
    }

    virtual bool supportsViewingVariables() const {
        return false;
    }

    virtual bool supportsViewingViews() const { // r/o support for views
        return false;
    }

    virtual bool supportsViewingRoutines() const {
        return false;
    }

    virtual bool supportsEditingRoutinesStructure() const {
        return false;
    }

    virtual bool supportsViewingTriggers() const {
        return false;
    }

    virtual bool supportsEditingTriggers() const {
        return false;
    }
};

// -----------------------------------------------------------------------------

class MySQLConnectionFeatures : public ConnectionFeatures
{
public:

	//using ConnectionFeatures::ConnectionFeatures;

    explicit MySQLConnectionFeatures(Connection * connection);

    virtual bool supportsViewingTables() const override {
        return true;
    }

    virtual bool supportsViewingTablesData() const override {
        return true;
    }

    virtual bool supportsEditingTablesData() const override {
        return true;
    }

    virtual bool supportsEditingTablesStructure() const override {
        return true;
    }

    virtual bool supportsEditingViewsStructure() const override {
        return true;
    }

    virtual bool supportsEditingDatabase() const override {
        return true;
    }

    virtual bool supportsDrop(Entity::Type type) const override {
        switch (type) {
            case meow::db::Entity::Type::Table: // only impl-ed
            case meow::db::Entity::Type::Database:
            case meow::db::Entity::Type::View:
            case meow::db::Entity::Type::Procedure:
            case meow::db::Entity::Type::Function:
            case meow::db::Entity::Type::Trigger:
            return true;
        default:
            return false;
        };
    }

    virtual bool supportsCreation(Entity::Type type) const override {
        switch (type) {
            case meow::db::Entity::Type::Table: // only impl-ed
            case meow::db::Entity::Type::Database:
            case meow::db::Entity::Type::View:
            case meow::db::Entity::Type::Procedure:
            case meow::db::Entity::Type::Function:
            case meow::db::Entity::Type::Trigger:
            return true;
        default:
            return false;
        };
    }

    virtual bool supportsForeignKeys(const TableEntity * table) const override;

    virtual bool supportsDumping() const override {
        return true;
    }

    virtual bool supportsViewingVariables() const override {
        return true;
    }

    virtual bool supportsViewingViews() const override {
        return true;
    }

    virtual bool supportsViewingRoutines() const override {
        return true;
    }

    virtual bool supportsEditingRoutinesStructure() const override {
        return true;
    }

    virtual bool supportsViewingTriggers() const override {
        return true;
    }

    virtual bool supportsEditingTriggers() const override {
        return true;
    }
};

// -----------------------------------------------------------------------------

class PGConnectionFeatures : public ConnectionFeatures
{
public:

	//using ConnectionFeatures::ConnectionFeatures;

    explicit PGConnectionFeatures(Connection * connection);

    virtual bool supportsViewingTables() const override {
        // not yet implemented
        return false;
    }

    virtual bool supportsViewingTablesData() const override {
        return true;
    }

    virtual bool supportsEditingTablesData() const override {
        return true;
    }

    virtual bool supportsForeignKeys(const TableEntity * table) const override {
        Q_UNUSED(table);
        // not yet implemented
        return false;
    }

    virtual bool supportsDumping() const override {
        // TODO: implement
        return false;
    }

    virtual bool supportsViewingViews() const override {
        return true;
    }
};

// -----------------------------------------------------------------------------

class QtSQLiteConnectionFeatures : public ConnectionFeatures
{
public:

    explicit QtSQLiteConnectionFeatures(Connection * connection);

    virtual bool supportsViewingTablesData() const override {
        return true;
    }

    virtual bool supportsViewingTables() const override {
        return true;
    }

    virtual bool supportsEditingTablesStructure() const override {
        return false; // allow to view structure, not edit
    }

    virtual bool supportsForeignKeys(const TableEntity * table) const override {
        Q_UNUSED(table);
        return true;
    }
};

} // namespace db
} // namespace meow

#endif // DB_CONNECTION_FEATURES_H
