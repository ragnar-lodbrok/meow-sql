#ifndef UTILS_SQL_PARSER_SQLITE_PARSER_H
#define UTILS_SQL_PARSER_SQLITE_PARSER_H

// see https://www.gnu.org/software/bison/manual/html_node/A-Complete-C_002b_002b-Example.html

#include "sqlite_types.h"
#include "sqlite_bison_parser.hpp"

// Give Flex the prototype of yylex we want ...
typedef void* yyscan_t;
namespace meow { namespace utils { namespace sql_parser { class SQLiteParser; }}}
# define YY_DECL \
  meow::utils::sql_parser::parser::symbol_type yylex(yyscan_t yyscanner, meow::utils::sql_parser::SQLiteParser& drv)
// ... and declare it for the parser's sake.
YY_DECL;

namespace meow {
namespace utils {
namespace sql_parser {

class SQLiteParser
{

public:
    SQLiteParser();
    bool parseCreateTable(const std::string & sql);

    // The token's location used by the scanner.
    meow::utils::sql_parser::location location; // TODO: getter

    SQLiteTablePtr _parsedTable;

private:

    //std::string _parsingStr;

    // Handling the scanner.
    void scan_begin(const std::string & str);
    void scan_end();

    // Whether to generate scanner debug traces.
    //bool trace_scanning;

    // Scanner state
    yyscan_t scanner;

    // Scanner buffer
    typedef struct yy_buffer_state* YY_BUFFER_STATE;
    YY_BUFFER_STATE buffer;

};

} // namespace sql_parser
} // namespace utils
} // namespace meow

#endif // UTILS_SQL_PARSER_SQLITE_PARSER_H

