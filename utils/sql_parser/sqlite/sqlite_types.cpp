#include "sqlite_types.h"
#include <sstream>

namespace meow {
namespace utils {
namespace sql_parser {

std::string SQLiteForeignKeyAction::toString() const
{
    std::stringstream ss;

    switch (action) {
    case SQLiteAction::OnDelete:
        ss << " ON DELETE";
        break;
    case SQLiteAction::OnUpdate:
        ss << " ON UPDATE";
        break;
    case SQLiteAction::Match:
        ss << " MATCH";
        break;
    }

    if (action == SQLiteAction::OnDelete || action == SQLiteAction::OnUpdate) {
        switch (doOnAction) {
        case SQLiteDoOnAction::NoAction:
            ss << " NO ACTION";
            break;
        case SQLiteDoOnAction::SetNull:
            ss << " SET NULL";
            break;
        case SQLiteDoOnAction::SetDefault:
            ss << " SET DEFAULT";
            break;
        case SQLiteDoOnAction::Cascade:
            ss << " CASCADE";
            break;
        case SQLiteDoOnAction::Restrict:
            ss << " RESTRICT";
            break;
        }
    }

    return ss.str();
}

// ---------------------------------

SQLiteColumnConstraint::SQLiteColumnConstraint(Type type)
    : _type(type)
    , _isAutoincrement(false)
    , _onConflict(SQLiteDoOnConflict::None)
{

}

std::string SQLiteColumnConstraint::toString() const
{
    std::stringstream ss;

    switch (_type) {
    case Type::PrimaryKey:
        ss << " PRIMARY KEY";
        if (_isAutoincrement) {
            ss << " AUTOINCREMENT";
        }
        break;
    case Type::NotNull:
        ss << " NOT NULL";
        break;
    case Type::Unique:
        ss << " UNIQUE";
        break;
    case Type::Check:
        ss << " CHECK";
        break;
    case Type::Default:
        ss << " DEFAULT";
        break;
    case Type::Collate:
        ss << " COLLATE";
        break;
    case Type::ForeignKey:
        ss << " FOREIGN KEY";
        break;
    }

    return ss.str();
}

// -------------------------------------

SQLiteColumn::SQLiteColumn() {}

std::string SQLiteColumn::toString() const
{
    std::stringstream ss;

    ss << "name: " << _name;
    ss << " type: " << _type;
    ss << " constraints:";
    for (const SQLiteColumnConstraintPtr & c : _constraints) {
        ss << " " << c->toString();
    }

    return ss.str();
}

// -------------------------------------

std::string SQLiteForeignData::toString() const
{
    std::stringstream ss;

    ss << "REFERENCES " << foreignTableName;
    ss << " (";
    bool first = true;
    for (const auto& col : foreignColumnNames) {
        if (!first)  {
            ss << ", ";
        } else {
            first = true;
        }
        ss << col;
    }

    ss << ") ";

    for (const auto& action : actions) {
        ss << action.toString();
    }

    return ss.str();
}

// --------------------------------------

SQLiteTableConstraint::SQLiteTableConstraint(Type type_) : type(type_) {}

SQLiteTableConstraint::~SQLiteTableConstraint() {}

SQLiteTableForeignKeyConstraint::SQLiteTableForeignKeyConstraint()
    : SQLiteTableConstraint(Type::ForeignKey) {}

std::string SQLiteTableForeignKeyConstraint::toString() const
{
    std::stringstream ss;

    ss << " FOREIGN KEY (";
    bool first = true;
    for (const auto& col : columnNames) {
        if (!first)  {
            ss << ", ";
        } else {
            first = true;
        }
        ss << col;
    }
    ss << ") ";

    if (foreignData) {
        ss << foreignData->toString();
    }

    return ss.str();
}

// --------------------------------------

SQLiteTable::SQLiteTable() {}

std::string SQLiteTable::toString() const
{
    std::stringstream ss;

    ss << "TABLE\n";
    ss << "\tname:" << _name << '\n';

    ss << "\tCOLUMNS:\n";
    for (const SQLiteColumnPtr & column : _columns) {
        ss << "\t\t" << column->toString() << "\n";
    }

    ss << "\tCONSTRAINTS:\n";
    for (const auto & c : _constraints) {
        ss << "\t\t" << c->toString() << "\n";
    }



    return ss.str();
}

} // namespace sql_parser
} // namespace utils
} // namespace meow

