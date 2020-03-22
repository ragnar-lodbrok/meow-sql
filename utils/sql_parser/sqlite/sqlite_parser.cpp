#include "sqlite_parser.h"
#include <QDebug>

namespace meow {
namespace utils {
namespace sql_parser {

SQLiteParser::SQLiteParser()
    : _parsedTable(nullptr)
    , scanner(nullptr)
    , buffer(nullptr)
{

}

db::TableStructure * SQLiteParser::parseCreateTable(const QString & sql)
{

    scan_begin(sql.toStdString());

    meow::utils::sql_parser::parser parser(scanner, *this);
    //parse.set_debug_level(true);
    int res = parser.parse();

    scan_end();

    qDebug() << "PARSE RESULT: " << res;


    return nullptr;
}

//void SQLiteParser::scan_begin(); // see impl in sqlite_flex_lexer.l
//void SQLiteParser::scan_end(); // see impl in sqlite_flex_lexer.l


} // namespace sql_parser
} // namespace utils
} // namespace meow
