/*

Bison parser file for SQLite 3.

To get the C++ code run:
$ flex sqlite_flex_lexer.l
$ bison sqlite_bison_parser.y

Example: https://github.com/sqlitebrowser/sqlitebrowser/blob/master/src/sql/parser/sqlite3_parser.yy
Example: https://github.com/hyrise/sql-parser/blob/master/src/parser/bison_parser.y

Create table: https://www.sqlite.org/lang_createtable.html

*/ 

%skeleton "lalr1.cc"
%require "3.0"
%language "c++"
%defines

%{
# include <stdio.h>
%}

%define api.value.type variant
%define api.token.constructor
//%define parse.assert
%output "sqlite_bison_parser.cpp"
/*%define api.location.file "sqlite_location.h"*/ /* TODO: uncomment when have fresher bison */

%code requires {

    #include <string>
    #include <sstream>
    #include "sqlite_types.h"
    
    namespace meow { namespace utils { namespace sql_parser { class SQLiteParser; } } }
    typedef void* yyscan_t;
    
}

%param { yyscan_t yyscanner }
%param { SQLiteParser& drv }

%locations

%define parse.trace
%define parse.error verbose


%code {
    #include "sqlite_parser.h"
}

/* declare tokens */


%define api.token.prefix {MSQL_}
%define api.namespace { meow::utils::sql_parser }


%token EOF 0 "End of file" 
//%token MINUS     "-" 
//%token PLUS      "+"
//%token BITAND    "&"
//%token BITOR     "|"
%token STAR      "*"
%token LBRACK    "("
%token RBRACK    ")"
%token COMMA     ","
%token DOT       "."
%token SEMICOLON ";"
%token LBRACK_SQ "["
%token RBRACK_SQ "]"

%token <std::string> CREATE "CREATE"
%token <std::string> TEMP "TEMP"
%token <std::string> TABLE "TABLE"
%token <std::string> WITHOUT "WITHOUT"
%token <std::string> ROWID "ROWID"

%token <std::string> CONSTRAINT "CONSTRAINT"
%token <std::string> PRIMARY "PRIMARY"
%token <std::string> KEY "KEY"
%token <std::string> ASC "ASC"
%token <std::string> DESC "DESC"
%token <std::string> AUTOINCREMENT "AUTOINCREMENT"
%token <std::string> NOT_NULL "NOT_NULL"
%token <std::string> UNIQUE "UNIQUE"
%token <std::string> CHECK "CHECK"
%token <std::string> DEFAULT "DEFAULT"
%token <std::string> COLLATE "COLLATE"
%token <std::string> FOREIGN "FOREIGN"

%token <std::string> REFERENCES "REFERENCES"
%token <std::string> ON_DELETE "ON_DELETE"
%token <std::string> ON_UPDATE "ON_UPDATE"
%token <std::string> SET_NULL "SET_NULL"
%token <std::string> SET_DEFAULT "SET_DEFAULT"
%token <std::string> CASCADE "CASCADE"
%token <std::string> RESTRICT "RESTRICT"
%token <std::string> NO_ACTION "NO_ACTION"

%token <std::string> ID "ID"
%token <std::string> INTNUM "INTNUM"
%token <std::string> FLOATNUM "FLOATNUM"
%token <std::string> STRING "STRING"

%token <std::string> ON_CONFLICT_ROLLBACK "ON_CONFLICT_ROLLBACK"
%token <std::string> ON_CONFLICT_ABORT "ON_CONFLICT_ABORT"
%token <std::string> ON_CONFLICT_FAIL "ON_CONFLICT_FAIL"
%token <std::string> ON_CONFLICT_IGNORE "ON_CONFLICT_IGNORE"
%token <std::string> ON_CONFLICT_REPLACE "ON_CONFLICT_REPLACE"

 /* default literals: */
%token <std::string> CURRENT_DATE "CURRENT_DATE"
%token <std::string> CURRENT_TIME "CURRENT_TIME"
%token <std::string> CURRENT_TIMESTAMP "CURRENT_TIMESTAMP"
%token <std::string> FALSE "FALSE"
%token <std::string> TRUE "TRUE"
%token <std::string> NULL "NULL"

%type <std::string> name_with_schema
%type <std::string> table_id
%type <std::string> column_id
%type <std::vector<std::string>> column_id_list
%type <std::vector<std::string>> opt_column_id_list
%type <std::string> type_name_list
%type <std::string> type_name
%type <std::string> opt_type_name
%type <std::string> column_name
%type <std::string> opt_constraint_name
%type <std::string> numeric_literal
%type <std::string> signed_number
%type <meow::utils::sql_parser::SQLiteLiteralValue> literal_value
%type <meow::utils::sql_parser::SQLiteLiteralValue> expr // temp since we support literal only in expr
%type <bool> opt_autoincrement
%type <bool> opt_temporary
%type <bool> opt_without_rowid
%type <meow::utils::sql_parser::SQLiteDoOnConflict> opt_conflict_clause
%type <meow::utils::sql_parser::SQLiteTablePtr> create_table_stmt
%type <std::vector<meow::utils::sql_parser::SQLiteColumnPtr>> create_column_list
%type <meow::utils::sql_parser::SQLiteColumnPtr> column_definition
%type <std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr>> opt_column_constraint_list
%type <std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr>> column_constraint_list
%type <meow::utils::sql_parser::SQLiteColumnConstraintPtr> column_constraint
%type <meow::utils::sql_parser::SQLiteDoOnAction> foreign_key_do_on_action
%type <meow::utils::sql_parser::SQLiteForeignKeyAction> foreign_key_action
%type <std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction>> foreign_key_action_list
%type <std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction>> opt_foreign_key_action_list
%type <meow::utils::sql_parser::SQLiteForeignDataPtr> foreign_key_clause
%type <meow::utils::sql_parser::SQLiteTableConstraintPtr> table_constraint
%type <std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr>> table_constraint_list
%type <std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr>> opt_table_constraint_list

%nonassoc ON;

%start statements;

%%


statements:
    statement
    | statement ";"
    ;

statement:
    create_table_stmt  { drv._parsedTable = $1; }
    ;

create_table_stmt:
    CREATE opt_temporary TABLE name_with_schema "(" create_column_list opt_table_constraint_list ")" opt_without_rowid {

        $$ = std::make_shared<meow::utils::sql_parser::SQLiteTable>();
        $$->setTemp($2);
        $$->setName($4);
        $$->setColumns($6);
        $$->setConstraints($7);
        $$->setWithoutRowID($9);

    }
    ;
 
    
name_with_schema:
    ID          { $$ = $1; }
    | ID "." ID { $$ = $3; }
    ;
    
create_column_list:
    column_definition                          { $$.push_back($1); }
    | create_column_list "," column_definition { $$ = $1; $$.push_back($3); }
    //| create_column_list error "," { yyerrok; } // TODO
    //| create_column_list error "\n" { yyerrok; }
    ;

column_definition:
    column_name opt_type_name opt_column_constraint_list { 
        $$ = std::make_shared<meow::utils::sql_parser::SQLiteColumn>();
        $$->setName($1);
        $$->setType($2);
        $$->setConstraints($3);
    
    }
    ;
    
opt_temporary:
    %empty { $$ = false; }
    | TEMP { $$ = true; }
    ;

opt_without_rowid:
    %empty          { $$ = false; }
    | WITHOUT ROWID { $$ = true; }
    ;


opt_column_constraint_list:
    %empty                    { $$ = {}; }
    | column_constraint_list  { $$ = $1; }
    ;
    
    
column_constraint_list:
    column_constraint   { $$.push_back($1); }
    | column_constraint_list column_constraint  { $$ = $1; $$.push_back($2); }
    ;    

column_constraint:
    PRIMARY KEY opt_conflict_clause opt_autoincrement {  
        using Constraint = meow::utils::sql_parser::SQLiteColumnConstraint;
        $$ = std::make_shared<Constraint>(Constraint::Type::PrimaryKey);
        $$->setIsAutoincrement($4);
        $$->setOnConflict($3);
    }
    | NOT_NULL opt_conflict_clause {
        using Constraint = meow::utils::sql_parser::SQLiteColumnConstraint;
        $$ = std::make_shared<Constraint>(Constraint::Type::NotNull);
        $$->setOnConflict($2);
    }
    | UNIQUE opt_conflict_clause {
        using Constraint = meow::utils::sql_parser::SQLiteColumnConstraint;
        $$ = std::make_shared<Constraint>(Constraint::Type::Unique);
        $$->setOnConflict($2);
    }
    | DEFAULT signed_number {
        auto DC = std::make_shared<meow::utils::sql_parser::SQLiteDefaultColumnConstraint>();
        DC->defaultValue.value = $2;
        DC->defaultValue.type = meow::utils::sql_parser::SQLiteLiteralValueType::Numeric;
        $$ = DC;
    }
    | DEFAULT literal_value {
        auto DC = std::make_shared<meow::utils::sql_parser::SQLiteDefaultColumnConstraint>();
        DC->defaultValue = $2;
        $$ = DC;
    }
    | DEFAULT "(" expr ")" {
        auto DC = std::make_shared<meow::utils::sql_parser::SQLiteDefaultColumnConstraint>();
        DC->defaultValue = $3;
        $$ = DC;
    }
    | foreign_key_clause {
        auto FK = std::make_shared<meow::utils::sql_parser::SQLiteForeignKeyColumnConstraint>();
        FK->foreignData = $1;
        $$ = FK;
    }
    //| column_constraint error "\n" { yyerrok; }
    ;
    
opt_autoincrement:
    %empty          { $$ = false; }
    | AUTOINCREMENT { $$ = true; }
    ;
    
opt_conflict_clause:
    %empty                  { $$ = meow::utils::sql_parser::SQLiteDoOnConflict::None; };
    | ON_CONFLICT_ROLLBACK  { $$ = meow::utils::sql_parser::SQLiteDoOnConflict::Rollback; }
    | ON_CONFLICT_ABORT     { $$ = meow::utils::sql_parser::SQLiteDoOnConflict::Abort; }
    | ON_CONFLICT_FAIL      { $$ = meow::utils::sql_parser::SQLiteDoOnConflict::Fail; }
    | ON_CONFLICT_IGNORE    { $$ = meow::utils::sql_parser::SQLiteDoOnConflict::Ignore; }
    | ON_CONFLICT_REPLACE   { $$ = meow::utils::sql_parser::SQLiteDoOnConflict::Replace; }
    ;
    
opt_table_constraint_list:
    %empty                    { $$ = {}; }
    | "," table_constraint_list   { $$ = $2; }
    ;    
    
table_constraint_list:
    table_constraint                             { $$.push_back($1); }
    | table_constraint_list "," table_constraint { $$ = $1; $$.push_back($3);}
    ;
    
table_constraint:
    opt_constraint_name FOREIGN KEY "(" column_id_list ")" foreign_key_clause {

        auto FK = std::make_shared<meow::utils::sql_parser::SQLiteTableForeignKeyConstraint>();

        FK->name = $1;
        FK->columnNames = $5;
        FK->foreignData = $7;
        $$ = FK;
    }
    | opt_constraint_name PRIMARY KEY "(" column_id_list ")" opt_conflict_clause {

        auto PK = std::make_shared<meow::utils::sql_parser::SQLiteTablePrimaryKeyConstraint>();

        PK->name = $1;
        PK->indexedColumnNames = $5;
        PK->conflict = $7;
        $$ = PK;
    }
    | opt_constraint_name UNIQUE "(" column_id_list ")" opt_conflict_clause {

        auto UK = std::make_shared<meow::utils::sql_parser::SQLiteTableUniqueConstraint>();

        UK->name = $1;
        UK->indexedColumnNames = $4;
        UK->conflict = $6;
        $$ = UK;
    }
    ;
    
foreign_key_clause:
    REFERENCES table_id opt_column_id_list opt_foreign_key_action_list {
        $$ = std::make_shared<meow::utils::sql_parser::SQLiteForeignData>();
        $$->foreignTableName = $2;
        $$->foreignColumnNames = $3;
        $$->actions = $4;
    }
    ;
    
opt_foreign_key_action_list:
    %empty                    { $$ = {}; }
    | foreign_key_action_list { $$ = $1; }
    ;

foreign_key_action_list:
    foreign_key_action                           { $$.push_back($1); }
    | foreign_key_action_list foreign_key_action { $$ = $1; $$.push_back($2); }
    ;    
    
foreign_key_action:
    ON_DELETE foreign_key_do_on_action {
        $$.action = meow::utils::sql_parser::SQLiteAction::OnDelete;
        $$.doOnAction = $2;
    }
    | ON_UPDATE foreign_key_do_on_action {
        $$.action = meow::utils::sql_parser::SQLiteAction::OnUpdate;
        $$.doOnAction = $2;
    }
    ;

foreign_key_do_on_action:
    SET_NULL        { $$ = meow::utils::sql_parser::SQLiteDoOnAction::SetNull; }
    | SET_DEFAULT   { $$ = meow::utils::sql_parser::SQLiteDoOnAction::SetDefault; }
    | CASCADE       { $$ = meow::utils::sql_parser::SQLiteDoOnAction::Cascade; }
    | RESTRICT      { $$ = meow::utils::sql_parser::SQLiteDoOnAction::Restrict; }
    | NO_ACTION     { $$ = meow::utils::sql_parser::SQLiteDoOnAction::NoAction; }
    ;
    
column_name:
    ID { $$ = $1; }
    ;
    
opt_type_name:
    %empty                  { $$ = {}; }
    | type_name             { $$ = $1; }
    ;
    
type_name:
    type_name_list  { $$ = $1; }
    | type_name_list "(" INTNUM ")" {  // e.g. VARCHAR(255)
        $$ = $1 + "(" + $3 + ")"; /* sqlite ignores length */ 
    } 
    | type_name_list "(" INTNUM "," INTNUM ")" { // e.g. DECIMAL(10,5)
        $$ = $1 + "(" + $3 + "," + $5 + ")"; 
    } 
    ;    
    
type_name_list:
    table_id                    { $$ = $1; }
    | type_name_list table_id   { $$ = $1 + " " + $2; } // e.g. UNSIGNED BIG INT
    ;    

opt_column_id_list:
    %empty                    { $$ = {}; }
    | "(" column_id_list ")"  { $$ = $2; }
    ;    
    
column_id_list:
    column_id                        { $$.push_back($1); }
    | column_id_list "," column_id   { $$ = $1; $$.push_back($3); } 
    ;     
    
table_id:
    ID { $$ = $1; }
    ;
    
column_id:
    ID { $$ = $1; }
    ;    
    
opt_constraint_name:
    %empty          { $$ = {}; }
    | CONSTRAINT ID { $$ = $2; }
    ;

signed_number:
    numeric_literal         { $$ = $1; }
    | "+" numeric_literal   { $$ = "+" + $2; }
    | "-" numeric_literal   { $$ = "-" + $2; }
    ;

numeric_literal:
    INTNUM     { $$ = $1; }
    | FLOATNUM { $$ = $1; }
    ;

literal_value:
    numeric_literal {
        $$.type = meow::utils::sql_parser::SQLiteLiteralValueType::Numeric;
        $$.value = $1;
    }
    | STRING {
        $$.type = meow::utils::sql_parser::SQLiteLiteralValueType::String;
        $$.value = $1;
    }
//    | blob_literal // TODO
    | NULL {
        $$.type = meow::utils::sql_parser::SQLiteLiteralValueType::Null;
        $$.value = $1;
    }
    | TRUE {
        $$.type = meow::utils::sql_parser::SQLiteLiteralValueType::True;
        $$.value = $1;
    }
    | FALSE {
        $$.type = meow::utils::sql_parser::SQLiteLiteralValueType::False;
        $$.value = $1;
    }
    | CURRENT_TIME {
        $$.type = meow::utils::sql_parser::SQLiteLiteralValueType::CurrentTime;
        $$.value = $1;
    }
    | CURRENT_DATE {
        $$.type = meow::utils::sql_parser::SQLiteLiteralValueType::CurrentDate;
        $$.value = $1;
    }
    | CURRENT_TIMESTAMP {
        $$.type = meow::utils::sql_parser::SQLiteLiteralValueType::CurrentTimestamp;
        $$.value = $1;
    }
    ;

expr:
    literal_value { $$ = $1; }
    // TODO other
    ;

%%

void meow::utils::sql_parser::parser::error(const location_type& l, const std::string& m)
{
    std::stringstream ss;

    ss << l << ": " << m;

    drv._lastError = ss.str(); 
}

