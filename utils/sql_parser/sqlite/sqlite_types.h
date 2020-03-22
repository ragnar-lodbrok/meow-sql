#ifndef UTILS_SQL_PARSER_SQLITE_TYPES_H
#define UTILS_SQL_PARSER_SQLITE_TYPES_H

#include <memory>
#include <string>
#include <vector>

namespace meow {
namespace utils {
namespace sql_parser {

// ----------------------------------------------------------------------------

enum class SQLiteDoOnConflict {
    None,
    Rollback,
    Abort,
    Fail,
    Ignore,
    Replace
};

enum class SQLiteAction {
    OnDelete,
    OnUpdate,
    Match
};

enum class SQLiteDoOnAction {
    NoAction,
    SetNull,
    SetDefault,
    Cascade,
    Restrict
};

class SQLiteForeignKeyAction
{
public:
    SQLiteAction action;
    SQLiteDoOnAction doOnAction = SQLiteDoOnAction::NoAction;

    std::string toString() const;
};

class SQLiteColumnConstraint
{
public:
    enum class Type {
        PrimaryKey,
        NotNull,
        Unique,
        Check,
        Default,
        Collate,
        ForeignKey
    };
    explicit SQLiteColumnConstraint(Type type);

    Type type() const { return _type; }
    bool isAutoincrement() const { return _isAutoincrement; }
    SQLiteDoOnConflict onConflict() const { return _onConflict; }

    void setIsAutoincrement(bool value) { _isAutoincrement = value; }
    void setOnConflict(SQLiteDoOnConflict value) { _onConflict = value; }

    std::string toString() const;

private:
    Type _type;
    bool _isAutoincrement;
    SQLiteDoOnConflict _onConflict;
};

using SQLiteColumnConstraintPtr = std::shared_ptr<SQLiteColumnConstraint>;

// ----------------------------------------------------------------------------

class SQLiteColumn
{
public:
    SQLiteColumn();
    void setName(const std::string & name) { _name = name; }
    std::string name() const { return _name; }

    void setType(const std::string & type) { _type = type; }
    std::string type() const { return _type; }

    std::string toString() const;

    void setConstraints(
            const std::vector<SQLiteColumnConstraintPtr> & constraints) {
        _constraints = constraints;
    }

private:
    std::string _name;
    std::string _type;

    std::vector<SQLiteColumnConstraintPtr> _constraints;
};
using SQLiteColumnPtr = std::shared_ptr<SQLiteColumn>;

// ----------------------------------------------------------------------------

class SQLiteTableConstraint
{
public:
    enum class Type {
        PrimaryKey,
        Unique,
        Check,
        ForeignKey
    };
    explicit SQLiteTableConstraint(Type type_);
    virtual ~SQLiteTableConstraint();

    Type type;

    virtual std::string toString() const = 0;

};

using SQLiteTableConstraintPtr = std::shared_ptr<SQLiteTableConstraint>;

// ----------------------------------------------------------------------------


class SQLiteForeignData
{
public:
    std::string foreignTableName;
    std::vector<std::string> foreignColumnNames;
    std::vector<SQLiteForeignKeyAction> actions;

    std::string toString() const;
};
using SQLiteForeignDataPtr = std::shared_ptr<SQLiteForeignData>;

// ----------------------------------------------------------------------------

class SQLiteTableForeignKeyConstraint : public SQLiteTableConstraint
{
public:
    SQLiteTableForeignKeyConstraint();

    std::vector<std::string> columnNames;

    SQLiteForeignDataPtr foreignData;

    virtual std::string toString() const override;

};
using SQLiteTableForeignKeyConstraintPtr
    = std::shared_ptr<SQLiteTableForeignKeyConstraint>;


// ----------------------------------------------------------------------------

class SQLiteTable
{
public:
    SQLiteTable();
    void setName(const std::string & name) { _name = name; }

    std::string toString() const;

    void setColumns(const std::vector<SQLiteColumnPtr> & columns) {
        _columns = columns;
    }

    void setConstraints(const std::vector<SQLiteTableConstraintPtr> & constraints) {
        _constraints = constraints;
    }

private:
    std::string _name;
    std::vector<SQLiteColumnPtr> _columns;
    std::vector<SQLiteTableConstraintPtr> _constraints;
};

using SQLiteTablePtr = std::shared_ptr<SQLiteTable>;

} // namespace sql_parser
} // namespace utils
} // namespace meow

#endif // UTILS_SQL_PARSER_SQLITE_TYPES_H
