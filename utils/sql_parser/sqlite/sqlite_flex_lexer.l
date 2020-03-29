 /* 
 
 Flex lexer file for SQLite.
 
 To get the C++ code run: 
 $ flex sqlite_flex_lexer.l
 
 Supports only limited set of statements:
 - CREATE TABLE https://www.sqlite.org/lang_createtable.html 
 
 Example: https://github.com/hyrise/sql-parser/blob/master/src/parser/flex_lexer.l
 Example 2: https://github.com/sqlitebrowser/sqlitebrowser/blob/master/src/sql/parser/sqlite3_lexer.ll
 Book: John R. Levine "flex & bison" ISBN: 978-0-596-15597-1
 
 */

 /* declaration and option settings */ 
%{

#include <string>
#include "sqlite_parser.h"
#include "sqlite_bison_parser.hpp"

%}

%option header-file="sqlite_flex_lexer.h"
%option outfile="sqlite_flex_lexer.cpp"
%option never-interactive
%option batch
%option reentrant
%option noyywrap
%option nounput
%option noinput
%option warn
%option case-insensitive
%option nodefault
/*%option bison-locations*/
/*%option bison-bridge*/

%{
#define MSQL_TOKEN(name) \
meow::utils::sql_parser::parser::symbol_type\
(meow::utils::sql_parser::parser::token::MSQL_##name, std::string(yytext), loc)

#define MSQL_MAKE_TOKEN(name, value) \
meow::utils::sql_parser::parser::make_##name((value), loc);

void unescapeInplace(std::string & str, char quote)
{
    const std::string doubleQuote = std::string(2, quote);
    const std::string strQuote = std::string(1, quote);

    size_t startPos = 0;
    while ((startPos = str.find(doubleQuote, startPos)) != std::string::npos) {
        str.replace(startPos, doubleQuote.length(), strQuote);
        ++startPos;
    }
}

std::string unquote(const std::string & str, char quote, bool unescape = true, char endQuote = 0)
{
    if (endQuote == 0) {
        endQuote = quote;
    }
    
    if (str.front() == quote && str.back() == endQuote) {
        std::string res = str.substr(1, str.length() - 2);
        
        if (unescape) {
            unescapeInplace(res, quote);
        }
        
        return res;
    } else {
        return str;
    }
    
}


%}

%{
  // Code run each time a pattern is matched.
  #define YY_USER_ACTION  loc.columns(yyleng);
%}


%s COMMENT

 /* patterns and actions */

%%

%{
  // A handy shortcut to the location held by the driver.
  meow::utils::sql_parser::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step();
%}


 /* Keywords */
 /* CREATE TABLE stmt */
CREATE              { return MSQL_TOKEN(CREATE); }
TEMP|TEMPORARY      { return MSQL_TOKEN(TEMP); }
TABLE               { return MSQL_TOKEN(TABLE); }
WITHOUT             { return MSQL_TOKEN(WITHOUT); }
ROWID               { return MSQL_TOKEN(ROWID); }

 /* column-constraint */
CONSTRAINT          { return MSQL_TOKEN(CONSTRAINT); } 
PRIMARY             { return MSQL_TOKEN(PRIMARY); }
KEY                 { return MSQL_TOKEN(KEY); }
ASC                 { return MSQL_TOKEN(ASC); }
DESC                { return MSQL_TOKEN(DESC); }
AUTOINCREMENT       { return MSQL_TOKEN(AUTOINCREMENT); }
NOT[ \t\n]+NULL     { return MSQL_TOKEN(NOT_NULL); } 
UNIQUE              { return MSQL_TOKEN(UNIQUE); }
CHECK               { return MSQL_TOKEN(CHECK); }
DEFAULT             { return MSQL_TOKEN(DEFAULT); }
COLLATE             { return MSQL_TOKEN(COLLATE); }
FOREIGN             { return MSQL_TOKEN(FOREIGN); }
                    /* TODO: GENERATED ... */
                    
 /* conflict-clause */
ON[ \t\n]+CONFLICT[ \t\n]+ROLLBACK  { return MSQL_TOKEN(ON_CONFLICT_ROLLBACK); } 
ON[ \t\n]+CONFLICT[ \t\n]+ABORT     { return MSQL_TOKEN(ON_CONFLICT_ABORT); } 
ON[ \t\n]+CONFLICT[ \t\n]+FAIL      { return MSQL_TOKEN(ON_CONFLICT_FAIL); } 
ON[ \t\n]+CONFLICT[ \t\n]+IGNORE    { return MSQL_TOKEN(ON_CONFLICT_IGNORE); } 
ON[ \t\n]+CONFLICT[ \t\n]+REPLACE   { return MSQL_TOKEN(ON_CONFLICT_REPLACE); } 

 /* TODO: expr */
 
 /* foreign-key-clause */
REFERENCES          { return MSQL_TOKEN(REFERENCES); }
ON[ \t\n]+DELETE    { return MSQL_TOKEN(ON_DELETE); }
ON[ \t\n]+UPDATE    { return MSQL_TOKEN(ON_UPDATE); }
SET[ \t\n]+NULL     { return MSQL_TOKEN(SET_NULL); }
SET[ \t\n]+DEFAULT  { return MSQL_TOKEN(SET_DEFAULT); }
CASCADE             { return MSQL_TOKEN(CASCADE); }
RESTRICT            { return MSQL_TOKEN(RESTRICT); }
NO[ \t\n]+ACTION    { return MSQL_TOKEN(NO_ACTION); }

 /* numbers */
 
-?[0-9]+            { return MSQL_MAKE_TOKEN(INTNUM, yytext); }

-?[0-9]+"."[0-9]* | 
-?"."[0-9]+         { return MSQL_MAKE_TOKEN(FLOATNUM, yytext); }

 /* strings */
'(\\.|''|[^'\n])*'  { return MSQL_MAKE_TOKEN(STRING, unquote(std::string(yytext), '\'')); }
'(\\.|[^'\n])*$     { throw meow::utils::sql_parser::parser::syntax_error(loc, 
                      "Unterminated string: " + std::string(yytext)); }



 /* operators */
"*"                 { return MSQL_TOKEN(STAR); }
"("                 { return MSQL_TOKEN(LBRACK); }
")"                 { return MSQL_TOKEN(RBRACK); }
","                 { return MSQL_TOKEN(COMMA); }
"."                 { return MSQL_TOKEN(DOT); }
";"                 { return MSQL_TOKEN(SEMICOLON); }
"["                 { return MSQL_TOKEN(LBRACK_SQ); }
"]"                 { return MSQL_TOKEN(RBRACK_SQ); }


 /* identifiers https://www.sqlite.org/lang_keywords.html */
\"[^\"\n]+\"          { return MSQL_MAKE_TOKEN(ID, unquote(std::string(yytext), '"')); }
[A-Za-z][A-Za-z0-9_]* { return MSQL_MAKE_TOKEN(ID, yytext); }
`[^`/\\.\n]+`         { return MSQL_MAKE_TOKEN(ID, unquote(std::string(yytext), '`')); }
\[[^\[\]\n]+\]        { return MSQL_MAKE_TOKEN(ID, unquote(std::string(yytext), '[', false, ']')); }
 
 /* comment */
 
"--"[ \t].*           ; /* TODO: eat comment? */
"/*"                  { BEGIN COMMENT; }
<COMMENT>"*/"         { BEGIN INITIAL; }
<COMMENT>.|\n         ; /* TODO: eat comment? */
<COMMENT><<EOF>>      { throw meow::utils::sql_parser::parser::syntax_error(loc, 
                        "Unclosed comment"); }
 
 
[ \t\r]+              loc.step();
\n+                   loc.lines(yyleng); loc.step();
 
.                     { throw meow::utils::sql_parser::parser::syntax_error(loc, 
                        "Invalid character: " + std::string(yytext)); /*loc.step();*/ }
                    
<<EOF>>               { return meow::utils::sql_parser::parser::make_EOF(loc); }

 /* code */
%%

namespace meow {
namespace utils {
namespace sql_parser {

void SQLiteParser::scan_begin(const std::string & str)
{
    yylex_init(&scanner);
    location.initialize();
    yyset_debug(false, scanner);
    buffer = yy_scan_string(str.c_str(), scanner);
}

void SQLiteParser::scan_end()
{
    yy_delete_buffer(buffer, scanner);
    yylex_destroy(scanner);
}


} // namespace sql_parser
} // namespace utils
} // namespace meow

