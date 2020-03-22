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
#line 19 "sqlite_bison_parser.y" // lalr1.cc:404

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
#line 48 "sqlite_bison_parser.y" // lalr1.cc:413

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

#line 56 "sqlite_bison_parser.y" // lalr1.cc:479
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
      case 51: // opt_autoincrement
        value.move< bool > (that.value);
        break;

      case 50: // column_constraint
        value.move< meow::utils::sql_parser::SQLiteColumnConstraintPtr > (that.value);
        break;

      case 47: // column_definition
        value.move< meow::utils::sql_parser::SQLiteColumnPtr > (that.value);
        break;

      case 60: // foreign_key_do_on_action
        value.move< meow::utils::sql_parser::SQLiteDoOnAction > (that.value);
        break;

      case 56: // foreign_key_clause
        value.move< meow::utils::sql_parser::SQLiteForeignDataPtr > (that.value);
        break;

      case 59: // foreign_key_action
        value.move< meow::utils::sql_parser::SQLiteForeignKeyAction > (that.value);
        break;

      case 55: // table_constraint
        value.move< meow::utils::sql_parser::SQLiteTableConstraintPtr > (that.value);
        break;

      case 44: // create_table_stmt
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
      case 45: // name_with_schema
      case 52: // opt_conflict_clause
      case 61: // column_name
      case 62: // opt_type_name
      case 63: // type_name
      case 64: // type_name_list
      case 67: // table_id
      case 68: // column_id
        value.move< std::string > (that.value);
        break;

      case 48: // opt_column_constraint_list
      case 49: // column_constraint_list
        value.move< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > (that.value);
        break;

      case 46: // create_column_list
        value.move< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > (that.value);
        break;

      case 57: // opt_foreign_key_action_list
      case 58: // foreign_key_action_list
        value.move< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > (that.value);
        break;

      case 53: // opt_table_constraint_list
      case 54: // table_constraint_list
        value.move< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > (that.value);
        break;

      case 65: // opt_column_id_list
      case 66: // column_id_list
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
      case 51: // opt_autoincrement
        value.copy< bool > (that.value);
        break;

      case 50: // column_constraint
        value.copy< meow::utils::sql_parser::SQLiteColumnConstraintPtr > (that.value);
        break;

      case 47: // column_definition
        value.copy< meow::utils::sql_parser::SQLiteColumnPtr > (that.value);
        break;

      case 60: // foreign_key_do_on_action
        value.copy< meow::utils::sql_parser::SQLiteDoOnAction > (that.value);
        break;

      case 56: // foreign_key_clause
        value.copy< meow::utils::sql_parser::SQLiteForeignDataPtr > (that.value);
        break;

      case 59: // foreign_key_action
        value.copy< meow::utils::sql_parser::SQLiteForeignKeyAction > (that.value);
        break;

      case 55: // table_constraint
        value.copy< meow::utils::sql_parser::SQLiteTableConstraintPtr > (that.value);
        break;

      case 44: // create_table_stmt
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
      case 45: // name_with_schema
      case 52: // opt_conflict_clause
      case 61: // column_name
      case 62: // opt_type_name
      case 63: // type_name
      case 64: // type_name_list
      case 67: // table_id
      case 68: // column_id
        value.copy< std::string > (that.value);
        break;

      case 48: // opt_column_constraint_list
      case 49: // column_constraint_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > (that.value);
        break;

      case 46: // create_column_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > (that.value);
        break;

      case 57: // opt_foreign_key_action_list
      case 58: // foreign_key_action_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > (that.value);
        break;

      case 53: // opt_table_constraint_list
      case 54: // table_constraint_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > (that.value);
        break;

      case 65: // opt_column_id_list
      case 66: // column_id_list
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
      case 51: // opt_autoincrement
        yylhs.value.build< bool > ();
        break;

      case 50: // column_constraint
        yylhs.value.build< meow::utils::sql_parser::SQLiteColumnConstraintPtr > ();
        break;

      case 47: // column_definition
        yylhs.value.build< meow::utils::sql_parser::SQLiteColumnPtr > ();
        break;

      case 60: // foreign_key_do_on_action
        yylhs.value.build< meow::utils::sql_parser::SQLiteDoOnAction > ();
        break;

      case 56: // foreign_key_clause
        yylhs.value.build< meow::utils::sql_parser::SQLiteForeignDataPtr > ();
        break;

      case 59: // foreign_key_action
        yylhs.value.build< meow::utils::sql_parser::SQLiteForeignKeyAction > ();
        break;

      case 55: // table_constraint
        yylhs.value.build< meow::utils::sql_parser::SQLiteTableConstraintPtr > ();
        break;

      case 44: // create_table_stmt
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
      case 45: // name_with_schema
      case 52: // opt_conflict_clause
      case 61: // column_name
      case 62: // opt_type_name
      case 63: // type_name
      case 64: // type_name_list
      case 67: // table_id
      case 68: // column_id
        yylhs.value.build< std::string > ();
        break;

      case 48: // opt_column_constraint_list
      case 49: // column_constraint_list
        yylhs.value.build< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > ();
        break;

      case 46: // create_column_list
        yylhs.value.build< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > ();
        break;

      case 57: // opt_foreign_key_action_list
      case 58: // foreign_key_action_list
        yylhs.value.build< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > ();
        break;

      case 53: // opt_table_constraint_list
      case 54: // table_constraint_list
        yylhs.value.build< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > ();
        break;

      case 65: // opt_column_id_list
      case 66: // column_id_list
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
  case 5:
#line 146 "sqlite_bison_parser.y" // lalr1.cc:859
    { drv._parsedTable = yystack_[0].value.as< meow::utils::sql_parser::SQLiteTablePtr > (); }
#line 810 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 150 "sqlite_bison_parser.y" // lalr1.cc:859
    {

        yylhs.value.as< meow::utils::sql_parser::SQLiteTablePtr > () = std::make_shared<meow::utils::sql_parser::SQLiteTable>();
        yylhs.value.as< meow::utils::sql_parser::SQLiteTablePtr > ()->setName(yystack_[4].value.as< std::string > ());
        yylhs.value.as< meow::utils::sql_parser::SQLiteTablePtr > ()->setColumns(yystack_[2].value.as< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > ());
        yylhs.value.as< meow::utils::sql_parser::SQLiteTablePtr > ()->setConstraints(yystack_[1].value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > ());
        
        std::cout << yylhs.value.as< meow::utils::sql_parser::SQLiteTablePtr > ()->toString() << std::endl;
    }
#line 824 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 163 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 830 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 164 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 836 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 168 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteColumnPtr > ()); }
#line 842 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 169 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > () = yystack_[2].value.as< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > (); yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteColumnPtr > ()); }
#line 848 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 175 "sqlite_bison_parser.y" // lalr1.cc:859
    { 
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnPtr > () = std::make_shared<meow::utils::sql_parser::SQLiteColumn>();
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnPtr > ()->setName(yystack_[2].value.as< std::string > ());
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnPtr > ()->setType(yystack_[1].value.as< std::string > ());
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnPtr > ()->setConstraints(yystack_[0].value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > ());
    
    }
#line 860 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 185 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > () = {}; }
#line 866 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 186 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > () = yystack_[0].value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > (); }
#line 872 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 191 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > ()); }
#line 878 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 192 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > () = yystack_[1].value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > (); yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > ()); }
#line 884 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 196 "sqlite_bison_parser.y" // lalr1.cc:859
    {  
        using Constraint = meow::utils::sql_parser::SQLiteColumnConstraint;
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > () = std::make_shared<Constraint>(Constraint::Type::PrimaryKey);
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > ()->setIsAutoincrement(yystack_[0].value.as< bool > ());
    }
#line 894 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 201 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        using Constraint = meow::utils::sql_parser::SQLiteColumnConstraint;
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > () = std::make_shared<Constraint>(Constraint::Type::NotNull);
    }
#line 903 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 205 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        using Constraint = meow::utils::sql_parser::SQLiteColumnConstraint;
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > () = std::make_shared<Constraint>(Constraint::Type::Unique);
    }
#line 912 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 209 "sqlite_bison_parser.y" // lalr1.cc:859
    { // TODO
        using Constraint = meow::utils::sql_parser::SQLiteColumnConstraint;
        yylhs.value.as< meow::utils::sql_parser::SQLiteColumnConstraintPtr > () = std::make_shared<Constraint>(Constraint::Type::Default);
    }
#line 921 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 216 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; }
#line 927 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 217 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; }
#line 933 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 221 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 939 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 230 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > () = {}; }
#line 945 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 231 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > () = yystack_[0].value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > (); }
#line 951 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 235 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteTableConstraintPtr > ()); }
#line 957 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 236 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > () = yystack_[2].value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > (); yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteTableConstraintPtr > ());}
#line 963 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 240 "sqlite_bison_parser.y" // lalr1.cc:859
    {

        auto FK = std::make_shared<meow::utils::sql_parser::SQLiteTableForeignKeyConstraint>();

        FK->columnNames = yystack_[2].value.as< std::vector<std::string> > ();
        FK->foreignData = yystack_[0].value.as< meow::utils::sql_parser::SQLiteForeignDataPtr > ();
        yylhs.value.as< meow::utils::sql_parser::SQLiteTableConstraintPtr > () = FK;
    }
#line 976 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 251 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignDataPtr > () = std::make_shared<meow::utils::sql_parser::SQLiteForeignData>();
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignDataPtr > ()->foreignTableName = yystack_[2].value.as< std::string > ();
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignDataPtr > ()->foreignColumnNames = yystack_[1].value.as< std::vector<std::string> > ();
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignDataPtr > ()->actions = yystack_[0].value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > ();
    }
#line 987 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 260 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > () = {}; }
#line 993 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 261 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > () = yystack_[0].value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > (); }
#line 999 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 265 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteForeignKeyAction > ()); }
#line 1005 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 266 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > () = yystack_[1].value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > (); yylhs.value.as< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > ().push_back(yystack_[0].value.as< meow::utils::sql_parser::SQLiteForeignKeyAction > ()); }
#line 1011 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 270 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignKeyAction > ().action = meow::utils::sql_parser::SQLiteAction::OnDelete;
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignKeyAction > ().doOnAction = yystack_[0].value.as< meow::utils::sql_parser::SQLiteDoOnAction > ();
    }
#line 1020 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 274 "sqlite_bison_parser.y" // lalr1.cc:859
    {
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignKeyAction > ().action = meow::utils::sql_parser::SQLiteAction::OnUpdate;
        yylhs.value.as< meow::utils::sql_parser::SQLiteForeignKeyAction > ().doOnAction = yystack_[0].value.as< meow::utils::sql_parser::SQLiteDoOnAction > ();
    }
#line 1029 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 281 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnAction > () = meow::utils::sql_parser::SQLiteDoOnAction::SetNull; }
#line 1035 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 282 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnAction > () = meow::utils::sql_parser::SQLiteDoOnAction::SetDefault; }
#line 1041 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 283 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnAction > () = meow::utils::sql_parser::SQLiteDoOnAction::Cascade; }
#line 1047 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 284 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnAction > () = meow::utils::sql_parser::SQLiteDoOnAction::Restrict; }
#line 1053 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 285 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< meow::utils::sql_parser::SQLiteDoOnAction > () = meow::utils::sql_parser::SQLiteDoOnAction::NoAction; }
#line 1059 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 289 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1065 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 293 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = ""; }
#line 1071 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 294 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1077 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 298 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1083 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 299 "sqlite_bison_parser.y" // lalr1.cc:859
    {  // e.g. VARCHAR(255)
        yylhs.value.as< std::string > () = yystack_[3].value.as< std::string > () + "(" + yystack_[1].value.as< std::string > () + ")"; /* sqlite ignores length */ 
    }
#line 1091 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 302 "sqlite_bison_parser.y" // lalr1.cc:859
    { // e.g. DECIMAL(10,5)
        yylhs.value.as< std::string > () = yystack_[5].value.as< std::string > () + "(" + yystack_[3].value.as< std::string > () + "," + yystack_[1].value.as< std::string > () + ")"; 
    }
#line 1099 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 308 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1105 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 309 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[1].value.as< std::string > () + " " + yystack_[0].value.as< std::string > (); }
#line 1111 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 313 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<std::string> > () = {}; }
#line 1117 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 314 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<std::string> > () = yystack_[1].value.as< std::vector<std::string> > (); }
#line 1123 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 318 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<std::string> > ().push_back(yystack_[0].value.as< std::string > ()); }
#line 1129 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 319 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::vector<std::string> > () = yystack_[2].value.as< std::vector<std::string> > (); yylhs.value.as< std::vector<std::string> > ().push_back(yystack_[0].value.as< std::string > ()); }
#line 1135 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 323 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1141 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 327 "sqlite_bison_parser.y" // lalr1.cc:859
    { yylhs.value.as< std::string > () = yystack_[0].value.as< std::string > (); }
#line 1147 "sqlite_bison_parser.cpp" // lalr1.cc:859
    break;


#line 1151 "sqlite_bison_parser.cpp" // lalr1.cc:859
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


  const signed char parser::yypact_ninf_ = -29;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
       4,    11,    25,    18,   -29,    -8,   -29,   -29,    20,    26,
      -7,    -5,   -29,   -29,    28,   -29,    -4,   -25,    30,   -29,
      -9,   -29,    -3,   -29,    21,   -29,    32,   -29,   -29,    22,
     -29,   -29,   -29,   -29,    -9,   -29,     5,   -29,    37,     9,
     -29,   -29,   -29,   -29,    -1,     7,   -29,    23,   -29,     8,
     -29,     1,   -29,   -29,   -29,    41,    19,     7,   -29,    -4,
     -29,   -29,    44,     7,   -20,    17,   -14,   -14,   -29,   -20,
     -29,   -29,   -29,   -29,   -29,   -29,   -29,   -29,   -29,   -29
  };

  const unsigned char
  parser::yydefact_[] =
  {
       4,     0,     0,     2,     5,     0,     1,     3,     7,     0,
       0,     0,     8,    40,    23,     9,    41,     0,     0,    52,
      12,    42,    43,    46,     0,    10,    24,    25,     6,     0,
      22,    22,    19,    11,    13,    14,     0,    47,     0,     0,
      22,    17,    18,    15,     0,     0,    26,    20,    44,     0,
      53,     0,    50,    21,    16,     0,     0,     0,    45,     0,
      27,    51,    48,     0,    29,     0,     0,     0,    28,    30,
      31,    49,    35,    36,    37,    38,    39,    33,    34,    32
  };

  const signed char
  parser::yypgoto_[] =
  {
     -29,   -29,   -29,   -29,   -29,   -29,    33,   -29,   -29,    15,
     -29,   -28,   -29,   -29,    12,   -29,   -29,   -29,   -17,   -13,
     -29,   -29,   -29,   -29,   -29,   -10,   -22,    -2
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,     2,     3,     4,     9,    14,    15,    33,    34,    35,
      54,    41,    18,    26,    27,    60,    68,    69,    70,    77,
      16,    20,    21,    22,    64,    51,    23,    52
  };

  const unsigned char
  parser::yytable_[] =
  {
      37,    36,    24,    42,    48,    49,    56,    57,    29,    66,
      67,    13,    47,    30,    31,     1,    32,    72,    73,    74,
      75,    76,    71,    57,     5,     6,     7,    10,     8,    12,
      11,    13,    19,    19,    17,    28,    24,    62,    39,    38,
      40,    45,    44,    50,    53,    55,    58,    59,    63,    43,
      25,    46,    79,    65,    78,    61
  };

  const unsigned char
  parser::yycheck_[] =
  {
      22,     4,    27,    31,     5,     6,     5,     6,    17,    29,
      30,    36,    40,    22,    23,    11,    25,    31,    32,    33,
      34,    35,     5,     6,    13,     0,     8,     7,    36,    36,
       4,    36,    36,    36,     6,     5,    27,    59,     6,    18,
      18,     4,    37,    36,    21,    37,     5,    28,     4,    34,
      17,    39,    69,    63,    67,    57
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    11,    42,    43,    44,    13,     0,     8,    36,    45,
       7,     4,    36,    36,    46,    47,    61,     6,    53,    36,
      62,    63,    64,    67,    27,    47,    54,    55,     5,    17,
      22,    23,    25,    48,    49,    50,     4,    67,    18,     6,
      18,    52,    52,    50,    37,     4,    55,    52,     5,     6,
      36,    66,    68,    21,    51,    37,     5,     6,     5,    28,
      56,    68,    67,     4,    65,    66,    29,    30,    57,    58,
      59,     5,    31,    32,    33,    34,    35,    60,    60,    59
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    41,    42,    42,    43,    43,    44,    45,    45,    46,
      46,    47,    48,    48,    49,    49,    50,    50,    50,    50,
      51,    51,    52,    53,    53,    54,    54,    55,    56,    57,
      57,    58,    58,    59,    59,    60,    60,    60,    60,    60,
      61,    62,    62,    63,    63,    63,    64,    64,    65,    65,
      66,    66,    67,    68
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     1,     2,     0,     1,     7,     1,     3,     1,
       3,     3,     0,     1,     1,     2,     4,     2,     2,     1,
       0,     1,     0,     0,     2,     1,     3,     6,     4,     0,
       1,     1,     2,     2,     2,     1,     1,     1,     1,     1,
       1,     0,     1,     1,     4,     6,     1,     2,     0,     3,
       1,     3,     1,     1
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
  "\"ID\"", "\"INTNUM\"", "\"FLOATNUM\"", "\"STRING\"", "ON", "$accept",
  "statements", "statement", "create_table_stmt", "name_with_schema",
  "create_column_list", "column_definition", "opt_column_constraint_list",
  "column_constraint_list", "column_constraint", "opt_autoincrement",
  "opt_conflict_clause", "opt_table_constraint_list",
  "table_constraint_list", "table_constraint", "foreign_key_clause",
  "opt_foreign_key_action_list", "foreign_key_action_list",
  "foreign_key_action", "foreign_key_do_on_action", "column_name",
  "opt_type_name", "type_name", "type_name_list", "opt_column_id_list",
  "column_id_list", "table_id", "column_id", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  parser::yyrline_[] =
  {
       0,   141,   141,   142,   145,   146,   150,   163,   164,   168,
     169,   175,   185,   186,   191,   192,   196,   201,   205,   209,
     216,   217,   221,   230,   231,   235,   236,   240,   251,   260,
     261,   265,   266,   270,   274,   281,   282,   283,   284,   285,
     289,   293,   294,   298,   299,   302,   308,   309,   313,   314,
     318,   319,   323,   327
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


#line 56 "sqlite_bison_parser.y" // lalr1.cc:1167
} } } //  meow::utils::sql_parser 
#line 1579 "sqlite_bison_parser.cpp" // lalr1.cc:1167
#line 330 "sqlite_bison_parser.y" // lalr1.cc:1168


void meow::utils::sql_parser::parser::error(const location_type& l, const std::string& m)
{
    std::cerr << l << ": " << m << std::endl;
}

