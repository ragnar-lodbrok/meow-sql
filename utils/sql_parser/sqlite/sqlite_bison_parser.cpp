// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.
#line 21 "sqlite_bison_parser.y" // lalr1.cc:404

# include <stdio.h>

#line 40 "sqlite_bison_parser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "sqlite_bison_parser.hpp"

// User implementation prologue.

#line 54 "sqlite_bison_parser.cpp" // lalr1.cc:412
// Unqualified %code blocks.
#line 51 "sqlite_bison_parser.y" // lalr1.cc:413

    #include "sqlite_parser.h"

#line 60 "sqlite_bison_parser.cpp" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 59 "sqlite_bison_parser.y" // lalr1.cc:479
namespace  meow { namespace utils { namespace sql_parser  {
#line 146 "sqlite_bison_parser.cpp" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  parser::parser (yyscan_t yyscanner_yyarg, SQLiteParser& drv_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      yyscanner (yyscanner_yyarg),
      drv (drv_yyarg)
  {}

  parser::~parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  parser::symbol_number_type
  parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 61: // opt_temporary
      case 62: // opt_without_rowid
      case 66: // opt_autoincrement
        value.move< bool > (that.value);
        break;

      case 65: // column_constraint
        value.move< meow::utils::sql_parser::SQLiteColumnConstraintPtr > (that.value);
        break;

      case 60: // column_definition
        value.move< meow::utils::sql_parser::SQLiteColumnPtr > (that.value);
        break;

      case 75: // foreign_key_do_on_action
        value.move< meow::utils::sql_parser::SQLiteDoOnAction > (that.value);
        break;

      case 67: // opt_conflict_clause
        value.move< meow::utils::sql_parser::SQLiteDoOnConflict > (that.value);
        break;

      case 71: // foreign_key_clause
        value.move< meow::utils::sql_parser::SQLiteForeignDataPtr > (that.value);
        break;

      case 74: // foreign_key_action
        value.move< meow::utils::sql_parser::SQLiteForeignKeyAction > (that.value);
        break;

      case 87: // literal_value
      case 88: // expr
        value.move< meow::utils::sql_parser::SQLiteLiteralValue > (that.value);
        break;

      case 70: // table_constraint
        value.move< meow::utils::sql_parser::SQLiteTableConstraintPtr > (that.value);
        break;

      case 57: // create_table_stmt
        value.move< meow::utils::sql_parser::SQLiteTablePtr > (that.value);
        break;

      case 11: // "CREATE"
      case 12: // "TEMP"
      case 13: // "TABLE"
      case 14: // "WITHOUT"
      case 15: // "ROWID"
      case 16: // "CONSTRAINT"
      case 17: // "PRIMARY"
      case 18: // "KEY"
      case 19: // "ASC"
      case 20: // "DESC"
      case 21: // "AUTOINCREMENT"
      case 22: // "NOT_NULL"
      case 23: // "UNIQUE"
      case 24: // "CHECK"
      case 25: // "DEFAULT"
      case 26: // "COLLATE"
      case 27: // "FOREIGN"
      case 28: // "REFERENCES"
      case 29: // "ON_DELETE"
      case 30: // "ON_UPDATE"
      case 31: // "SET_NULL"
      case 32: // "SET_DEFAULT"
      case 33: // "CASCADE"
      case 34: // "RESTRICT"
      case 35: // "NO_ACTION"
      case 36: // "ID"
      case 37: // "INTNUM"
      case 38: // "FLOATNUM"
      case 39: // "STRING"
      case 40: // "ON_CONFLICT_ROLLBACK"
      case 41: // "ON_CONFLICT_ABORT"
      case 42: // "ON_CONFLICT_FAIL"
      case 43: // "ON_CONFLICT_IGNORE"
      case 44: // "ON_CONFLICT_REPLACE"
      case 45: // "CURRENT_DATE"
      case 46: // "CURRENT_TIME"
      case 47: // "CURRENT_TIMESTAMP"
      case 48: // "FALSE"
      case 49: // "TRUE"
      case 50: // "NULL"
      case 58: // name_with_schema
      case 76: // column_name
      case 77: // opt_type_name
      case 78: // type_name
      case 79: // type_name_list
      case 82: // table_id
      case 83: // column_id
      case 84: // opt_constraint_name
      case 85: // signed_number
      case 86: // numeric_literal
        value.move< std::string > (that.value);
        break;

      case 63: // opt_column_constraint_list
      case 64: // column_constraint_list
        value.move< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > (that.value);
        break;

      case 59: // create_column_list
        value.move< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > (that.value);
        break;

      case 72: // opt_foreign_key_action_list
      case 73: // foreign_key_action_list
        value.move< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > (that.value);
        break;

      case 68: // opt_table_constraint_list
      case 69: // table_constraint_list
        value.move< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > (that.value);
        break;

      case 80: // opt_column_id_list
      case 81: // column_id_list
        value.move< std::vector<std::string> > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 61: // opt_temporary
      case 62: // opt_without_rowid
      case 66: // opt_autoincrement
        value.copy< bool > (that.value);
        break;

      case 65: // column_constraint
        value.copy< meow::utils::sql_parser::SQLiteColumnConstraintPtr > (that.value);
        break;

      case 60: // column_definition
        value.copy< meow::utils::sql_parser::SQLiteColumnPtr > (that.value);
        break;

      case 75: // foreign_key_do_on_action
        value.copy< meow::utils::sql_parser::SQLiteDoOnAction > (that.value);
        break;

      case 67: // opt_conflict_clause
        value.copy< meow::utils::sql_parser::SQLiteDoOnConflict > (that.value);
        break;

      case 71: // foreign_key_clause
        value.copy< meow::utils::sql_parser::SQLiteForeignDataPtr > (that.value);
        break;

      case 74: // foreign_key_action
        value.copy< meow::utils::sql_parser::SQLiteForeignKeyAction > (that.value);
        break;

      case 87: // literal_value
      case 88: // expr
        value.copy< meow::utils::sql_parser::SQLiteLiteralValue > (that.value);
        break;

      case 70: // table_constraint
        value.copy< meow::utils::sql_parser::SQLiteTableConstraintPtr > (that.value);
        break;

      case 57: // create_table_stmt
        value.copy< meow::utils::sql_parser::SQLiteTablePtr > (that.value);
        break;

      case 11: // "CREATE"
      case 12: // "TEMP"
      case 13: // "TABLE"
      case 14: // "WITHOUT"
      case 15: // "ROWID"
      case 16: // "CONSTRAINT"
      case 17: // "PRIMARY"
      case 18: // "KEY"
      case 19: // "ASC"
      case 20: // "DESC"
      case 21: // "AUTOINCREMENT"
      case 22: // "NOT_NULL"
      case 23: // "UNIQUE"
      case 24: // "CHECK"
      case 25: // "DEFAULT"
      case 26: // "COLLATE"
      case 27: // "FOREIGN"
      case 28: // "REFERENCES"
      case 29: // "ON_DELETE"
      case 30: // "ON_UPDATE"
      case 31: // "SET_NULL"
      case 32: // "SET_DEFAULT"
      case 33: // "CASCADE"
      case 34: // "RESTRICT"
      case 35: // "NO_ACTION"
      case 36: // "ID"
      case 37: // "INTNUM"
      case 38: // "FLOATNUM"
      case 39: // "STRING"
      case 40: // "ON_CONFLICT_ROLLBACK"
      case 41: // "ON_CONFLICT_ABORT"
      case 42: // "ON_CONFLICT_FAIL"
      case 43: // "ON_CONFLICT_IGNORE"
      case 44: // "ON_CONFLICT_REPLACE"
      case 45: // "CURRENT_DATE"
      case 46: // "CURRENT_TIME"
      case 47: // "CURRENT_TIMESTAMP"
      case 48: // "FALSE"
      case 49: // "TRUE"
      case 50: // "NULL"
      case 58: // name_with_schema
      case 76: // column_name
      case 77: // opt_type_name
      case 78: // type_name
      case 79: // type_name_list
      case 82: // table_id
      case 83: // column_id
      case 84: // opt_constraint_name
      case 85: // signed_number
      case 86: // numeric_literal
        value.copy< std::string > (that.value);
        break;

      case 63: // opt_column_constraint_list
      case 64: // column_constraint_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > (that.value);
        break;

      case 59: // create_column_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > (that.value);
        break;

      case 72: // opt_foreign_key_action_list
      case 73: // foreign_key_action_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > (that.value);
        break;

      case 68: // opt_table_constraint_list
      case 69: // table_constraint_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > (that.value);
        break;

      case 80: // opt_column_id_list
      case 81: // column_id_list
        value.copy< std::vector<std::string> > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (yyscanner, drv));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 61: // opt_temporary
      case 62: // opt_without_rowid
      case 66: // opt_autoincrement
        yylhs.value.build< bool > ();
        break;

      case 65: // column_constraint
        yylhs.value.build< meow::utils::sql_parser::SQLiteColumnConstraintPtr > ();
        break;

      case 60: // column_definition
        yylhs.value.build< meow::utils::sql_parser::SQLiteColumnPtr > ();
        break;

      case 75: // foreign_key_do_on_action
        yylhs.value.build< meow::utils::sql_parser::SQLiteDoOnAction > ();
        break;

      case 67: // opt_conflict_clause
        yylhs.value.build< meow::utils::sql_parser::SQLiteDoOnConflict > ();
        break;

      case 71: // foreign_key_clause
        yylhs.value.build< meow::utils::sql_parser::SQLiteForeignDataPtr > ();
        break;

      case 74: // foreign_key_action
        yylhs.value.build< meow::utils::sql_parser::SQLiteForeignKeyAction > ();
        break;

      case 87: // literal_value
      case 88: // expr
        yylhs.value.build< meow::utils::sql_parser::SQLiteLiteralValue > ();
        break;

      case 70: // table_constraint
        yylhs.value.build< meow::utils::sql_parser::SQLiteTableConstraintPtr > ();
        break;

      case 57: // create_table_stmt
        yylhs.value.build< meow::utils::sql_parser::SQLiteTablePtr > ();
        break;

      case 11: // "CREATE"
      case 12: // "TEMP"
      case 13: // "TABLE"
      case 14: // "WITHOUT"
      case 15: // "ROWID"
      case 16: // "CONSTRAINT"
      case 17: // "PRIMARY"
      case 18: // "KEY"
      case 19: // "ASC"
      case 20: // "DESC"
      case 21: // "AUTOINCREMENT"
      case 22: // "NOT_NULL"
      case 23: // "UNIQUE"
      case 24: // "CHECK"
      case 25: // "DEFAULT"
      case 26: // "COLLATE"
      case 27: // "FOREIGN"
      case 28: // "REFERENCES"
      case 29: // "ON_DELETE"
      case 30: // "ON_UPDATE"
      case 31: // "SET_NULL"
      case 32: // "SET_DEFAULT"
      case 33: // "CASCADE"
      case 34: // "RESTRICT"
      case 35: // "NO_ACTION"
      case 36: // "ID"
      case 37: // "INTNUM"
      case 38: // "FLOATNUM"
      case 39: // "STRING"
      case 40: // "ON_CONFLICT_ROLLBACK"
      case 41: // "ON_CONFLICT_ABORT"
      case 42: // "ON_CONFLICT_FAIL"
      case 43: // "ON_CONFLICT_IGNORE"
      case 44: // "ON_CONFLICT_REPLACE"
      case 45: // "CURRENT_DATE"
      case 46: // "CURRENT_TIME"
      case 47: // "CURRENT_TIMESTAMP"
      case 48: // "FALSE"
      case 49: // "TRUE"
      case 50: // "NULL"
      case 58: // name_with_schema
      case 76: // column_name
      case 77: // opt_type_name
      case 78: // type_name
      case 79: // type_name_list
      case 82: // table_id
      case 83: // column_id
      case 84: // opt_constraint_name
      case 85: // signed_number
      case 86: // numeric_literal
        yylhs.value.build< std::string > ();
        break;

      case 63: // opt_column_constraint_list
      case 64: // column_constraint_list
        yylhs.value.build< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > ();
        break;

      case 59: // create_column_list
        yylhs.value.build< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > ();
        break;

      case 72: // opt_foreign_key_action_list
      case 73: // foreign_key_action_list
        yylhs.value.build< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > ();
        break;

      case 68: // opt_table_constraint_list
      case 69: // table_constraint_list
        yylhs.value.build< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > ();
        break;

      case 80: // opt_column_id_list
      case 81: // column_id_list
        yylhs.value.build< std::vector<std::string> > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 4:
#line 169 "sqlite_bison_parser.y" // lalr1.cc:859
    { drv._parsedTable = yystack_[0].value.as< meow::utils::sql_parser::SQLiteTablePtr > (); }
#line 882 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 173 "sqlite_bison_parser.y" // lalr1.cc:859
    {

        yylhs.value.as< meow::utils::sql_parser::SQLiteTablePtr > () = std::make_shared<meow::utils::sql_parser::SQLiteTable>();
        yylhs.value.as< meow::utils::sql_parser::SQLiteTablePtr > ()->setTemp(yystack_[7].value.as< bool > ());
        yylhs.value.as< meow::utils::sql_parser::SQLiteTablePtr > ()->setName(yystack_[5].value.as< std::string > ());
        yylhs.value.as< meow::utils::sql_parser::SQLiteTablePtr > ()->setColumns(yystack_[3].value.as< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > ());
        yylhs.value.as< meow::utils::sql_parser::SQLiteTablePtr > ()->setConstraints(yystack_[2].value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > ());
        yylhs.value.as< meow::utils::sql_parser::SQLiteTablePtr > ()->setWithoutRowID(yystack_[0].value.as< bool > ());

    }
#line 897 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 187 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 903 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 188 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 909 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 192 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteColumnPtr > ()); }
#line 915 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 193 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > () = yystack_[2].value.as< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > (); yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteColumnPtr > ()); }
#line 921 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 199 "sqlite_bison_parser.y" // lalr1.cc:859
    { 
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnPtr > () = std::make_shared<meow::utils::sql_parser::SQLiteColumn>();
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnPtr > ()->setName(yystack_[2].value.as< std::string > ());
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnPtr > ()->setType(yystack_[1].value.as< std::string > ());
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnPtr > ()->setConstraints(yystack_[0].value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > ());
    
    }
#line 933 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 209 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 939 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 210 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 945 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 214 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 951 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 215 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 957 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 220 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > () = {}; }
#line 963 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 221 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > () = yystack_[0].value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > (); }
#line 969 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 226 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > ()); }
#line 975 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 227 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > () = yystack_[1].value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > (); yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > ()); }
#line 981 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 231 "sqlite_bison_parser.y" // lalr1.cc:859
    {  
        using Constraint = meow::utils::sql_parser::SQLiteColumnConstraint;
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > () = std::make_shared<Constraint>(Constraint::Type::PrimaryKey);
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > ()->setIsAutoincrement(yystack_[0].value.as< bool > ());
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > ()->setOnConflict(yystack_[1].value.as< meow::utils::sql_parser::SQLiteDoOnConflict > ());
    }
#line 992 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 237 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        using Constraint = meow::utils::sql_parser::SQLiteColumnConstraint;
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > () = std::make_shared<Constraint>(Constraint::Type::NotNull);
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > ()->setOnConflict(yystack_[0].value.as< meow::utils::sql_parser::SQLiteDoOnConflict > ());
    }
#line 1002 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 242 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        using Constraint = meow::utils::sql_parser::SQLiteColumnConstraint;
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > () = std::make_shared<Constraint>(Constraint::Type::Unique);
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > ()->setOnConflict(yystack_[0].value.as< meow::utils::sql_parser::SQLiteDoOnConflict > ());
    }
#line 1012 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 247 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        auto DC = std::make_shared<meow::utils::sql_parser::SQLiteDefaultColumnConstraint>();
        DC->defaultValue.value = yystack_[0].value.as< std::string > ();
        DC->defaultValue.type = meow::utils::sql_parser::SQLiteLiteralValueType::Numeric;
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > () = DC;
    }
#line 1023 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 253 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        auto DC = std::make_shared<meow::utils::sql_parser::SQLiteDefaultColumnConstraint>();
        DC->defaultValue = yystack_[0].value.as< meow::utils::sql_parser::SQLiteLiteralValue > ();
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > () = DC;
    }
#line 1033 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 258 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        auto DC = std::make_shared<meow::utils::sql_parser::SQLiteDefaultColumnConstraint>();
        DC->defaultValue = yystack_[1].value.as< meow::utils::sql_parser::SQLiteLiteralValue > ();
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > () = DC;
    }
#line 1043 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 263 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        auto FK = std::make_shared<meow::utils::sql_parser::SQLiteForeignKeyColumnConstraint>();
        FK->foreignData = yystack_[0].value.as< meow::utils::sql_parser::SQLiteForeignDataPtr > ();
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > () = FK;
    }
#line 1053 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 272 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 1059 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 273 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 1065 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 277 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnConflict > () = meow::utils::sql_parser::SQLiteDoOnConflict::None; }
#line 1071 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 278 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnConflict > () = meow::utils::sql_parser::SQLiteDoOnConflict::Rollback; }
#line 1077 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 279 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnConflict > () = meow::utils::sql_parser::SQLiteDoOnConflict::Abort; }
#line 1083 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 280 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnConflict > () = meow::utils::sql_parser::SQLiteDoOnConflict::Fail; }
#line 1089 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 281 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnConflict > () = meow::utils::sql_parser::SQLiteDoOnConflict::Ignore; }
#line 1095 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 282 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnConflict > () = meow::utils::sql_parser::SQLiteDoOnConflict::Replace; }
#line 1101 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 286 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > () = {}; }
#line 1107 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 287 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > () = yystack_[0].value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > (); }
#line 1113 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 291 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteTableConstraintPtr > ()); }
#line 1119 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 292 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > () = yystack_[2].value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > (); yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteTableConstraintPtr > ());}
#line 1125 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 296 "sqlite_bison_parser.y" // lalr1.cc:859
    {

        auto FK = std::make_shared<meow::utils::sql_parser::SQLiteTableForeignKeyConstraint>();

        FK->name = yystack_[6].value.as< std::string > ();
        FK->columnNames = yystack_[2].value.as< std::vector<std::string> > ();
        FK->foreignData = yystack_[0].value.as< meow::utils::sql_parser::SQLiteForeignDataPtr > ();
        yylhs.value.as< meow::utils::sql_parser::SQLiteTableConstraintPtr > () = FK;
    }
#line 1139 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 305 "sqlite_bison_parser.y" // lalr1.cc:859
    {

        auto PK = std::make_shared<meow::utils::sql_parser::SQLiteTablePrimaryKeyConstraint>();

        PK->name = yystack_[6].value.as< std::string > ();
        PK->indexedColumnNames = yystack_[2].value.as< std::vector<std::string> > ();
        PK->conflict = yystack_[0].value.as< meow::utils::sql_parser::SQLiteDoOnConflict > ();
        yylhs.value.as< meow::utils::sql_parser::SQLiteTableConstraintPtr > () = PK;
    }
#line 1153 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 314 "sqlite_bison_parser.y" // lalr1.cc:859
    {

        auto UK = std::make_shared<meow::utils::sql_parser::SQLiteTableUniqueConstraint>();

        UK->name = yystack_[5].value.as< std::string > ();
        UK->indexedColumnNames = yystack_[2].value.as< std::vector<std::string> > ();
        UK->conflict = yystack_[0].value.as< meow::utils::sql_parser::SQLiteDoOnConflict > ();
        yylhs.value.as< meow::utils::sql_parser::SQLiteTableConstraintPtr > () = UK;
    }
#line 1167 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 326 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignDataPtr > () = std::make_shared<meow::utils::sql_parser::SQLiteForeignData>();
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignDataPtr > ()->foreignTableName = yystack_[2].value.as< std::string > ();
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignDataPtr > ()->foreignColumnNames = yystack_[1].value.as< std::vector<std::string> > ();
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignDataPtr > ()->actions = yystack_[0].value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > ();
    }
#line 1178 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 335 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > () = {}; }
#line 1184 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 336 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > () = yystack_[0].value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > (); }
#line 1190 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 340 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteForeignKeyAction > ()); }
#line 1196 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 341 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > () = yystack_[1].value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > (); yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteForeignKeyAction > ()); }
#line 1202 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 345 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignKeyAction > ().action = meow::utils::sql_parser::SQLiteAction::OnDelete;
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignKeyAction > ().doOnAction = yystack_[0].value.as< meow::utils::sql_parser::SQLiteDoOnAction > ();
    }
#line 1211 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 349 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignKeyAction > ().action = meow::utils::sql_parser::SQLiteAction::OnUpdate;
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignKeyAction > ().doOnAction = yystack_[0].value.as< meow::utils::sql_parser::SQLiteDoOnAction > ();
    }
#line 1220 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 356 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnAction > () = meow::utils::sql_parser::SQLiteDoOnAction::SetNull; }
#line 1226 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 357 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnAction > () = meow::utils::sql_parser::SQLiteDoOnAction::SetDefault; }
#line 1232 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 358 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnAction > () = meow::utils::sql_parser::SQLiteDoOnAction::Cascade; }
#line 1238 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 359 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnAction > () = meow::utils::sql_parser::SQLiteDoOnAction::Restrict; }
#line 1244 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 360 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnAction > () = meow::utils::sql_parser::SQLiteDoOnAction::NoAction; }
#line 1250 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 364 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1256 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 368 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = {}; }
#line 1262 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 369 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1268 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 373 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1274 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 374 "sqlite_bison_parser.y" // lalr1.cc:859
    {  // e.g. VARCHAR(255)
        yylhs.value.as< std::string > () = yystack_[3].value.as< std::string > () + "(" + yystack_[1].value.as< std::string > () + ")"; /* sqlite ignores length */ 
    }
#line 1282 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 377 "sqlite_bison_parser.y" // lalr1.cc:859
    { // e.g. DECIMAL(10,5)
        yylhs.value.as< std::string > () = yystack_[5].value.as< std::string > () + "(" + yystack_[3].value.as< std::string > () + "," + yystack_[1].value.as< std::string > () + ")"; 
    }
#line 1290 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 383 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1296 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 384 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > () + " " + yystack_[0].value.as< std::string > (); }
#line 1302 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 388 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<std::string> > () = {}; }
#line 1308 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 389 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<std::string> > () = yystack_[1].value.as< std::vector<std::string> > (); }
#line 1314 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 393 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<std::string> > ().push_back(yystack_[0].value.as< std::string > ()); }
#line 1320 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 394 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<std::string> > () = yystack_[2].value.as< std::vector<std::string> > (); yylhs.value.as< std::vector<std::string> > ().push_back(yystack_[0].value.as< std::string > ()); }
#line 1326 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 398 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1332 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 402 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1338 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 406 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = {}; }
#line 1344 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 407 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1350 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 411 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1356 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 412 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = "+" + yystack_[0].value.as< std::string > (); }
#line 1362 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 413 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = "-" + yystack_[0].value.as< std::string > (); }
#line 1368 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 417 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1374 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 418 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1380 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 422 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().type = meow::utils::sql_parser::SQLiteLiteralValueType::Numeric;
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().value = yystack_[0].value.as< std::string > ();
    }
#line 1389 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 426 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().type = meow::utils::sql_parser::SQLiteLiteralValueType::String;
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().value = yystack_[0].value.as< std::string > ();
    }
#line 1398 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 431 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().type = meow::utils::sql_parser::SQLiteLiteralValueType::Null;
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().value = yystack_[0].value.as< std::string > ();
    }
#line 1407 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 435 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().type = meow::utils::sql_parser::SQLiteLiteralValueType::True;
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().value = yystack_[0].value.as< std::string > ();
    }
#line 1416 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 439 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().type = meow::utils::sql_parser::SQLiteLiteralValueType::False;
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().value = yystack_[0].value.as< std::string > ();
    }
#line 1425 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 443 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().type = meow::utils::sql_parser::SQLiteLiteralValueType::CurrentTime;
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().value = yystack_[0].value.as< std::string > ();
    }
#line 1434 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 447 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().type = meow::utils::sql_parser::SQLiteLiteralValueType::CurrentDate;
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().value = yystack_[0].value.as< std::string > ();
    }
#line 1443 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 451 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().type = meow::utils::sql_parser::SQLiteLiteralValueType::CurrentTimestamp;
        yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > ().value = yystack_[0].value.as< std::string > ();
    }
#line 1452 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 82:
#line 458 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteLiteralValue > () = yystack_[0].value.as< meow::utils::sql_parser::SQLiteLiteralValue > (); }
#line 1458 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;


#line 1462 "sqlite_bison_parser.cpp" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -84;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
      -2,    -1,    52,    25,   -84,   -84,    42,   -84,   -84,    24,
      51,    68,    40,    41,   -84,   -84,    72,   -84,    43,   -14,
      75,   -84,    31,   -84,     1,   -84,    45,   -84,    76,   -84,
      34,    70,    65,    22,    22,    -3,    43,   -84,    31,   -84,
     -84,    48,   -84,   -84,    71,    73,    84,    74,    78,   -84,
      22,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -20,   -84,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -31,   -31,
     -84,   -84,   -84,    85,   -84,     8,   -84,    86,    58,    91,
     -84,    77,   -84,   -84,    92,   -84,   -84,    58,    -6,   -84,
      59,    58,   -84,    26,   -84,    58,   -84,   -84,   -84,    33,
      36,    36,   -84,    -6,   -84,    94,    35,    22,    58,    69,
     -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,   -84,
      22,   -84,   -84,    79,   -84,   -84
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,    11,     0,     2,     4,    12,     0,     1,     3,     0,
       6,     0,     0,     0,     7,    53,    34,     8,    54,    67,
       0,    65,    15,    55,    56,    59,     0,     9,    35,    36,
       0,    13,     0,    28,    28,     0,     0,    10,    16,    17,
      25,     0,    60,    68,    67,     0,     0,     0,     0,     5,
      28,    29,    30,    31,    32,    33,    20,    21,     0,    72,
      73,    75,    80,    79,    81,    78,    77,    76,     0,     0,
      22,    69,    23,    61,    18,     0,    37,     0,     0,     0,
      14,    26,    74,    82,     0,    70,    71,     0,    42,    57,
       0,     0,    66,     0,    63,     0,    27,    19,    24,     0,
       0,     0,    41,    43,    44,     0,     0,    28,     0,     0,
      62,    48,    49,    50,    51,    52,    46,    47,    45,    58,
      28,    40,    64,     0,    39,    38
  };

  const signed char
  parser::yypgoto_[] =
  {
     -84,   -84,   -84,   -84,   -84,   -84,    81,   -84,   -84,   -84,
     -84,    63,   -84,   -34,   -84,   -84,    60,   -18,   -84,   -84,
       0,     5,   -84,   -84,   -84,   -84,   -84,   -83,   -21,     2,
     -84,   -84,   -48,    44,   -84
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,     2,     3,     4,    11,    16,    17,     6,    49,    37,
      38,    39,    97,    56,    20,    28,    29,    40,   102,   103,
     104,   116,    18,    22,    23,    24,    88,    93,    25,    94,
      30,    70,    71,    72,    84
  };

  const unsigned char
  parser::yytable_[] =
  {
      57,    58,    26,    42,    99,    41,    59,    60,   106,     1,
      82,     5,   109,    89,    90,    73,    81,    59,    60,    61,
      85,    86,    15,   100,   101,    62,    63,    64,    65,    66,
      67,   107,   108,     8,    59,    60,    61,    21,   110,   108,
     120,   108,    62,    63,    64,    65,    66,    67,    32,    68,
      69,    45,     7,    33,    34,     9,    35,    46,    12,    36,
      10,    47,    51,    52,    53,    54,    55,   111,   112,   113,
     114,   115,    13,   121,   123,   108,    14,    15,    19,    21,
      31,    43,    44,    50,    48,    75,   124,    26,    78,    87,
      91,    77,    79,    80,    92,    95,   105,    98,    96,   119,
      27,    74,    83,   118,    76,   125,   117,    36,     0,     0,
     122
  };

  const signed char
  parser::yycheck_[] =
  {
      34,     4,    16,    24,    87,     4,    37,    38,    91,    11,
      58,    12,    95,     5,     6,    36,    50,    37,    38,    39,
      68,    69,    36,    29,    30,    45,    46,    47,    48,    49,
      50,     5,     6,     8,    37,    38,    39,    36,     5,     6,
       5,     6,    45,    46,    47,    48,    49,    50,    17,    52,
      53,    17,     0,    22,    23,    13,    25,    23,     7,    28,
      36,    27,    40,    41,    42,    43,    44,    31,    32,    33,
      34,    35,     4,   107,     5,     6,    36,    36,     6,    36,
       5,    36,     6,    18,    14,    37,   120,    16,     4,     4,
       4,    18,    18,    15,    36,     4,    37,     5,    21,     5,
      19,    38,    58,   103,    44,   123,   101,    28,    -1,    -1,
     108
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    11,    55,    56,    57,    12,    61,     0,     8,    13,
      36,    58,     7,     4,    36,    36,    59,    60,    76,     6,
      68,    36,    77,    78,    79,    82,    16,    60,    69,    70,
      84,     5,    17,    22,    23,    25,    28,    63,    64,    65,
      71,     4,    82,    36,     6,    17,    23,    27,    14,    62,
      18,    40,    41,    42,    43,    44,    67,    67,     4,    37,
      38,    39,    45,    46,    47,    48,    49,    50,    52,    53,
      85,    86,    87,    82,    65,    37,    70,    18,     4,    18,
      15,    67,    86,    87,    88,    86,    86,     4,    80,     5,
       6,     4,    36,    81,    83,     4,    21,    66,     5,    81,
      29,    30,    72,    73,    74,    37,    81,     5,     6,    81,
       5,    31,    32,    33,    34,    35,    75,    75,    74,     5,
       5,    67,    83,     5,    67,    71
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    54,    55,    55,    56,    57,    58,    58,    59,    59,
      60,    61,    61,    62,    62,    63,    63,    64,    64,    65,
      65,    65,    65,    65,    65,    65,    66,    66,    67,    67,
      67,    67,    67,    67,    68,    68,    69,    69,    70,    70,
      70,    71,    72,    72,    73,    73,    74,    74,    75,    75,
      75,    75,    75,    76,    77,    77,    78,    78,    78,    79,
      79,    80,    80,    81,    81,    82,    83,    84,    84,    85,
      85,    85,    86,    86,    87,    87,    87,    87,    87,    87,
      87,    87,    88
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     1,     2,     1,     9,     1,     3,     1,     3,
       3,     0,     1,     0,     2,     0,     1,     1,     2,     4,
       2,     2,     2,     2,     4,     1,     0,     1,     0,     1,
       1,     1,     1,     1,     0,     2,     1,     3,     7,     7,
       6,     4,     0,     1,     1,     2,     2,     2,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     4,     6,     1,
       2,     0,     3,     1,     3,     1,     1,     0,     2,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"End of file\"", "error", "$undefined", "\"*\"", "\"(\"", "\")\"",
  "\",\"", "\".\"", "\";\"", "\"[\"", "\"]\"", "\"CREATE\"", "\"TEMP\"",
  "\"TABLE\"", "\"WITHOUT\"", "\"ROWID\"", "\"CONSTRAINT\"", "\"PRIMARY\"",
  "\"KEY\"", "\"ASC\"", "\"DESC\"", "\"AUTOINCREMENT\"", "\"NOT_NULL\"",
  "\"UNIQUE\"", "\"CHECK\"", "\"DEFAULT\"", "\"COLLATE\"", "\"FOREIGN\"",
  "\"REFERENCES\"", "\"ON_DELETE\"", "\"ON_UPDATE\"", "\"SET_NULL\"",
  "\"SET_DEFAULT\"", "\"CASCADE\"", "\"RESTRICT\"", "\"NO_ACTION\"",
  "\"ID\"", "\"INTNUM\"", "\"FLOATNUM\"", "\"STRING\"",
  "\"ON_CONFLICT_ROLLBACK\"", "\"ON_CONFLICT_ABORT\"",
  "\"ON_CONFLICT_FAIL\"", "\"ON_CONFLICT_IGNORE\"",
  "\"ON_CONFLICT_REPLACE\"", "\"CURRENT_DATE\"", "\"CURRENT_TIME\"",
  "\"CURRENT_TIMESTAMP\"", "\"FALSE\"", "\"TRUE\"", "\"NULL\"", "ON",
  "\"+\"", "\"-\"", "$accept", "statements", "statement",
  "create_table_stmt", "name_with_schema", "create_column_list",
  "column_definition", "opt_temporary", "opt_without_rowid",
  "opt_column_constraint_list", "column_constraint_list",
  "column_constraint", "opt_autoincrement", "opt_conflict_clause",
  "opt_table_constraint_list", "table_constraint_list", "table_constraint",
  "foreign_key_clause", "opt_foreign_key_action_list",
  "foreign_key_action_list", "foreign_key_action",
  "foreign_key_do_on_action", "column_name", "opt_type_name", "type_name",
  "type_name_list", "opt_column_id_list", "column_id_list", "table_id",
  "column_id", "opt_constraint_name", "signed_number", "numeric_literal",
  "literal_value", "expr", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   164,   164,   165,   169,   173,   187,   188,   192,   193,
     199,   209,   210,   214,   215,   220,   221,   226,   227,   231,
     237,   242,   247,   253,   258,   263,   272,   273,   277,   278,
     279,   280,   281,   282,   286,   287,   291,   292,   296,   305,
     314,   326,   335,   336,   340,   341,   345,   349,   356,   357,
     358,   359,   360,   364,   368,   369,   373,   374,   377,   383,
     384,   388,   389,   393,   394,   398,   402,   406,   407,   411,
     412,   413,   417,   418,   422,   426,   431,   435,   439,   443,
     447,   451,   458
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 59 "sqlite_bison_parser.y" // lalr1.cc:1167
} } } //  meow::utils::sql_parser 
#line 1935 "sqlite_bison_parser.cpp" // lalr1.cc:1167
#line 462 "sqlite_bison_parser.y" // lalr1.cc:1168


void meow::utils::sql_parser::parser::error(const location_type& l, const std::string& m)
{
    std::stringstream ss;

    ss << l << ": " << m;

    drv._lastError = ss.str(); 
}

