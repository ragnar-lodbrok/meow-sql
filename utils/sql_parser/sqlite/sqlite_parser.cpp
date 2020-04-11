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

bool SQLiteParser::parseCreateTable(const std::string &sql)
{

    scan_begin(sql);

    meow::utils::sql_parser::parser parser(scanner, *this);
    //parse.set_debug_level(true);
    int res = parser.parse();

    scan_end();

    qDebug() << "PARSE RESULT: " << res;


    return res == 0;
}

//void SQLiteParser::scan_begin(); // see impl in sqlite_flex_lexer.l
//void SQLiteParser::scan_end(); // see impl in sqlite_flex_lexer.l


} // namespace sql_parser
} // namespace utils
} // namespace meow
