// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton interface for Bison LALR(1) parsers in C++

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

/**
 ** \file sqlite_bison_parser.hpp
 ** Define the  meow::utils::sql_parser ::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_SQLITE_BISON_PARSER_HPP_INCLUDED
# define YY_YY_SQLITE_BISON_PARSER_HPP_INCLUDED
// //                    "%code requires" blocks.
#line 29 "sqlite_bison_parser.y" // lalr1.cc:377


    #include <string>
    #include "sqlite_types.h"
    
    namespace meow { namespace utils { namespace sql_parser { class SQLiteParser; } } }
    typedef void* yyscan_t;
    

#line 54 "sqlite_bison_parser.hpp" // lalr1.cc:377


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>
# include "stack.hh"
# include "location.hh"

#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
#endif


#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

#line 56 "sqlite_bison_parser.y" // lalr1.cc:377
namespace  meow { namespace utils { namespace sql_parser  {
#line 131 "sqlite_bison_parser.hpp" // lalr1.cc:377



  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {
    /// Type of *this.
    typedef variant<S> self_type;

    /// Empty construction.
    variant ()
    {}

    /// Construct and fill.
    template <typename T>
    variant (const T& t)
    {
      YYASSERT (sizeof (T) <= S);
      new (yyas_<T> ()) T (t);
    }

    /// Destruction, allowed only if empty.
    ~variant ()
    {}

    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    build ()
    {
      return *new (yyas_<T> ()) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    build (const T& t)
    {
      return *new (yyas_<T> ()) T (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as ()
    {
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const
    {
      return *yyas_<T> ();
    }

    /// Swap the content with \a other, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsability.
    /// Swapping between built and (possibly) non-built is done with
    /// variant::move ().
    template <typename T>
    void
    swap (self_type& other)
    {
      std::swap (as<T> (), other.as<T> ());
    }

    /// Move the content of \a other to this.
    ///
    /// Destroys \a other.
    template <typename T>
    void
    move (self_type& other)
    {
      build<T> ();
      swap<T> (other);
      other.destroy<T> ();
    }

    /// Copy the content of \a other to this.
    template <typename T>
    void
    copy (const self_type& other)
    {
      build<T> (other.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
    }

  private:
    /// Prohibit blind copies.
    self_type& operator=(const self_type&);
    variant (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ ()
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[S];
    } yybuffer_;
  };


  /// A Bison parser.
  class parser
  {
  public:
#ifndef YYSTYPE
    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // opt_autoincrement
      char dummy1[sizeof(bool)];

      // column_constraint
      char dummy2[sizeof(meow::utils::sql_parser::SQLiteColumnConstraintPtr)];

      // column_definition
      char dummy3[sizeof(meow::utils::sql_parser::SQLiteColumnPtr)];

      // foreign_key_do_on_action
      char dummy4[sizeof(meow::utils::sql_parser::SQLiteDoOnAction)];

      // foreign_key_clause
      char dummy5[sizeof(meow::utils::sql_parser::SQLiteForeignDataPtr)];

      // foreign_key_action
      char dummy6[sizeof(meow::utils::sql_parser::SQLiteForeignKeyAction)];

      // table_constraint
      char dummy7[sizeof(meow::utils::sql_parser::SQLiteTableConstraintPtr)];

      // create_table_stmt
      char dummy8[sizeof(meow::utils::sql_parser::SQLiteTablePtr)];

      // "CREATE"
      // "TEMP"
      // "TABLE"
      // "WITHOUT"
      // "ROWID"
      // "CONSTRAINT"
      // "PRIMARY"
      // "KEY"
      // "ASC"
      // "DESC"
      // "AUTOINCREMENT"
      // "NOT_NULL"
      // "UNIQUE"
      // "CHECK"
      // "DEFAULT"
      // "COLLATE"
      // "FOREIGN"
      // "REFERENCES"
      // "ON_DELETE"
      // "ON_UPDATE"
      // "SET_NULL"
      // "SET_DEFAULT"
      // "CASCADE"
      // "RESTRICT"
      // "NO_ACTION"
      // "ID"
      // "INTNUM"
      // "FLOATNUM"
      // "STRING"
      // name_with_schema
      // opt_conflict_clause
      // column_name
      // opt_type_name
      // type_name
      // type_name_list
      // table_id
      // column_id
      char dummy9[sizeof(std::string)];

      // opt_column_constraint_list
      // column_constraint_list
      char dummy10[sizeof(std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr>)];

      // create_column_list
      char dummy11[sizeof(std::vector<meow::utils::sql_parser::SQLiteColumnPtr>)];

      // opt_foreign_key_action_list
      // foreign_key_action_list
      char dummy12[sizeof(std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction>)];

      // opt_table_constraint_list
      // table_constraint_list
      char dummy13[sizeof(std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr>)];

      // opt_column_id_list
      // column_id_list
      char dummy14[sizeof(std::vector<std::string>)];
};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m);
      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        MSQL_EOF = 0,
        MSQL_STAR = 258,
        MSQL_LBRACK = 259,
        MSQL_RBRACK = 260,
        MSQL_COMMA = 261,
        MSQL_DOT = 262,
        MSQL_SEMICOLON = 263,
        MSQL_LBRACK_SQ = 264,
        MSQL_RBRACK_SQ = 265,
        MSQL_CREATE = 266,
        MSQL_TEMP = 267,
        MSQL_TABLE = 268,
        MSQL_WITHOUT = 269,
        MSQL_ROWID = 270,
        MSQL_CONSTRAINT = 271,
        MSQL_PRIMARY = 272,
        MSQL_KEY = 273,
        MSQL_ASC = 274,
        MSQL_DESC = 275,
        MSQL_AUTOINCREMENT = 276,
        MSQL_NOT_NULL = 277,
        MSQL_UNIQUE = 278,
        MSQL_CHECK = 279,
        MSQL_DEFAULT = 280,
        MSQL_COLLATE = 281,
        MSQL_FOREIGN = 282,
        MSQL_REFERENCES = 283,
        MSQL_ON_DELETE = 284,
        MSQL_ON_UPDATE = 285,
        MSQL_SET_NULL = 286,
        MSQL_SET_DEFAULT = 287,
        MSQL_CASCADE = 288,
        MSQL_RESTRICT = 289,
        MSQL_NO_ACTION = 290,
        MSQL_ID = 291,
        MSQL_INTNUM = 292,
        MSQL_FLOATNUM = 293,
        MSQL_STRING = 294,
        MSQL_ON = 295
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ();

      /// Copy constructor.
      basic_symbol (const basic_symbol& other);

      /// Constructor for valueless symbols, and symbols from each type.

  basic_symbol (typename Base::kind_type t, const location_type& l);

  basic_symbol (typename Base::kind_type t, const bool v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const meow::utils::sql_parser::SQLiteColumnConstraintPtr v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const meow::utils::sql_parser::SQLiteColumnPtr v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const meow::utils::sql_parser::SQLiteDoOnAction v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const meow::utils::sql_parser::SQLiteForeignDataPtr v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const meow::utils::sql_parser::SQLiteForeignKeyAction v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const meow::utils::sql_parser::SQLiteTableConstraintPtr v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const meow::utils::sql_parser::SQLiteTablePtr v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<meow::utils::sql_parser::SQLiteColumnPtr> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::vector<std::string> v, const location_type& l);


      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v,
                    const location_type& l);

      /// Destroy the symbol.
      ~basic_symbol ();

      /// Destroy contents, and record that is empty.
      void clear ();

      /// Whether empty.
      bool empty () const;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& other);
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

      /// Copy constructor.
      by_type (const by_type& other);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;

    // Symbol constructors declarations.
    static inline
    symbol_type
    make_EOF (const location_type& l);

    static inline
    symbol_type
    make_STAR (const location_type& l);

    static inline
    symbol_type
    make_LBRACK (const location_type& l);

    static inline
    symbol_type
    make_RBRACK (const location_type& l);

    static inline
    symbol_type
    make_COMMA (const location_type& l);

    static inline
    symbol_type
    make_DOT (const location_type& l);

    static inline
    symbol_type
    make_SEMICOLON (const location_type& l);

    static inline
    symbol_type
    make_LBRACK_SQ (const location_type& l);

    static inline
    symbol_type
    make_RBRACK_SQ (const location_type& l);

    static inline
    symbol_type
    make_CREATE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TEMP (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_TABLE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_WITHOUT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ROWID (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CONSTRAINT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_PRIMARY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_KEY (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ASC (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_DESC (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_AUTOINCREMENT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_NOT_NULL (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_UNIQUE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CHECK (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_DEFAULT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_COLLATE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_FOREIGN (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_REFERENCES (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ON_DELETE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ON_UPDATE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_SET_NULL (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_SET_DEFAULT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_CASCADE (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_RESTRICT (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_NO_ACTION (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ID (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_INTNUM (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_FLOATNUM (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_STRING (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_ON (const location_type& l);


    /// Build a parser object.
    parser (yyscan_t yyscanner_yyarg, SQLiteParser& drv_yyarg);
    virtual ~parser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

  private:
    /// This class is not copyable.
    parser (const parser&);
    parser& operator= (const parser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const signed char yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const signed char yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const signed char yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const unsigned char yytable_[];

  static const unsigned char yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    // Debugging.
    int yydebug_;
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state ();

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s);

      /// Copy constructor.
      by_state (const by_state& other);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const;

      /// The state number used to denote an empty symbol.
      enum { empty_state = -1 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, symbol_type& sym);
      /// Assignment, needed by push_back.
      stack_symbol_type& operator= (const stack_symbol_type& that);
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    void yypop_ (unsigned int n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 55,     ///< Last index in yytable_.
      yynnts_ = 28,  ///< Number of nonterminal symbols.
      yyfinal_ = 6, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 41  ///< Number of tokens.
    };


    // User arguments.
    yyscan_t yyscanner;
    SQLiteParser& drv;
  };

  // Symbol number corresponding to token number t.
  inline
  parser::token_number_type
  parser::yytranslate_ (token_type t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40
    };
    const unsigned int user_token_number_max_ = 295;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  inline
  parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
      switch (other.type_get ())
    {
      case 51: // opt_autoincrement
        value.copy< bool > (other.value);
        break;

      case 50: // column_constraint
        value.copy< meow::utils::sql_parser::SQLiteColumnConstraintPtr > (other.value);
        break;

      case 47: // column_definition
        value.copy< meow::utils::sql_parser::SQLiteColumnPtr > (other.value);
        break;

      case 60: // foreign_key_do_on_action
        value.copy< meow::utils::sql_parser::SQLiteDoOnAction > (other.value);
        break;

      case 56: // foreign_key_clause
        value.copy< meow::utils::sql_parser::SQLiteForeignDataPtr > (other.value);
        break;

      case 59: // foreign_key_action
        value.copy< meow::utils::sql_parser::SQLiteForeignKeyAction > (other.value);
        break;

      case 55: // table_constraint
        value.copy< meow::utils::sql_parser::SQLiteTableConstraintPtr > (other.value);
        break;

      case 44: // create_table_stmt
        value.copy< meow::utils::sql_parser::SQLiteTablePtr > (other.value);
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
        value.copy< std::string > (other.value);
        break;

      case 48: // opt_column_constraint_list
      case 49: // column_constraint_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > (other.value);
        break;

      case 46: // create_column_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > (other.value);
        break;

      case 57: // opt_foreign_key_action_list
      case 58: // foreign_key_action_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > (other.value);
        break;

      case 53: // opt_table_constraint_list
      case 54: // table_constraint_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > (other.value);
        break;

      case 65: // opt_column_id_list
      case 66: // column_id_list
        value.copy< std::vector<std::string> > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
    {
      case 51: // opt_autoincrement
        value.copy< bool > (v);
        break;

      case 50: // column_constraint
        value.copy< meow::utils::sql_parser::SQLiteColumnConstraintPtr > (v);
        break;

      case 47: // column_definition
        value.copy< meow::utils::sql_parser::SQLiteColumnPtr > (v);
        break;

      case 60: // foreign_key_do_on_action
        value.copy< meow::utils::sql_parser::SQLiteDoOnAction > (v);
        break;

      case 56: // foreign_key_clause
        value.copy< meow::utils::sql_parser::SQLiteForeignDataPtr > (v);
        break;

      case 59: // foreign_key_action
        value.copy< meow::utils::sql_parser::SQLiteForeignKeyAction > (v);
        break;

      case 55: // table_constraint
        value.copy< meow::utils::sql_parser::SQLiteTableConstraintPtr > (v);
        break;

      case 44: // create_table_stmt
        value.copy< meow::utils::sql_parser::SQLiteTablePtr > (v);
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
        value.copy< std::string > (v);
        break;

      case 48: // opt_column_constraint_list
      case 49: // column_constraint_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > (v);
        break;

      case 46: // create_column_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > (v);
        break;

      case 57: // opt_foreign_key_action_list
      case 58: // foreign_key_action_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > (v);
        break;

      case 53: // opt_table_constraint_list
      case 54: // table_constraint_list
        value.copy< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > (v);
        break;

      case 65: // opt_column_id_list
      case 66: // column_id_list
        value.copy< std::vector<std::string> > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const bool v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const meow::utils::sql_parser::SQLiteColumnConstraintPtr v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const meow::utils::sql_parser::SQLiteColumnPtr v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const meow::utils::sql_parser::SQLiteDoOnAction v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const meow::utils::sql_parser::SQLiteForeignDataPtr v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const meow::utils::sql_parser::SQLiteForeignKeyAction v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const meow::utils::sql_parser::SQLiteTableConstraintPtr v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const meow::utils::sql_parser::SQLiteTablePtr v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<meow::utils::sql_parser::SQLiteColumnPtr> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::vector<std::string> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
  parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  parser::basic_symbol<Base>::clear ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    basic_symbol<Base>& yysym = *this;
    (void) yysym;
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
    {
      case 51: // opt_autoincrement
        value.template destroy< bool > ();
        break;

      case 50: // column_constraint
        value.template destroy< meow::utils::sql_parser::SQLiteColumnConstraintPtr > ();
        break;

      case 47: // column_definition
        value.template destroy< meow::utils::sql_parser::SQLiteColumnPtr > ();
        break;

      case 60: // foreign_key_do_on_action
        value.template destroy< meow::utils::sql_parser::SQLiteDoOnAction > ();
        break;

      case 56: // foreign_key_clause
        value.template destroy< meow::utils::sql_parser::SQLiteForeignDataPtr > ();
        break;

      case 59: // foreign_key_action
        value.template destroy< meow::utils::sql_parser::SQLiteForeignKeyAction > ();
        break;

      case 55: // table_constraint
        value.template destroy< meow::utils::sql_parser::SQLiteTableConstraintPtr > ();
        break;

      case 44: // create_table_stmt
        value.template destroy< meow::utils::sql_parser::SQLiteTablePtr > ();
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
        value.template destroy< std::string > ();
        break;

      case 48: // opt_column_constraint_list
      case 49: // column_constraint_list
        value.template destroy< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > ();
        break;

      case 46: // create_column_list
        value.template destroy< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > ();
        break;

      case 57: // opt_foreign_key_action_list
      case 58: // foreign_key_action_list
        value.template destroy< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > ();
        break;

      case 53: // opt_table_constraint_list
      case 54: // table_constraint_list
        value.template destroy< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > ();
        break;

      case 65: // opt_column_id_list
      case 66: // column_id_list
        value.template destroy< std::vector<std::string> > ();
        break;

      default:
        break;
    }

    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
    {
      case 51: // opt_autoincrement
        value.move< bool > (s.value);
        break;

      case 50: // column_constraint
        value.move< meow::utils::sql_parser::SQLiteColumnConstraintPtr > (s.value);
        break;

      case 47: // column_definition
        value.move< meow::utils::sql_parser::SQLiteColumnPtr > (s.value);
        break;

      case 60: // foreign_key_do_on_action
        value.move< meow::utils::sql_parser::SQLiteDoOnAction > (s.value);
        break;

      case 56: // foreign_key_clause
        value.move< meow::utils::sql_parser::SQLiteForeignDataPtr > (s.value);
        break;

      case 59: // foreign_key_action
        value.move< meow::utils::sql_parser::SQLiteForeignKeyAction > (s.value);
        break;

      case 55: // table_constraint
        value.move< meow::utils::sql_parser::SQLiteTableConstraintPtr > (s.value);
        break;

      case 44: // create_table_stmt
        value.move< meow::utils::sql_parser::SQLiteTablePtr > (s.value);
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
        value.move< std::string > (s.value);
        break;

      case 48: // opt_column_constraint_list
      case 49: // column_constraint_list
        value.move< std::vector<meow::utils::sql_parser::SQLiteColumnConstraintPtr> > (s.value);
        break;

      case 46: // create_column_list
        value.move< std::vector<meow::utils::sql_parser::SQLiteColumnPtr> > (s.value);
        break;

      case 57: // opt_foreign_key_action_list
      case 58: // foreign_key_action_list
        value.move< std::vector<meow::utils::sql_parser::SQLiteForeignKeyAction> > (s.value);
        break;

      case 53: // opt_table_constraint_list
      case 54: // table_constraint_list
        value.move< std::vector<meow::utils::sql_parser::SQLiteTableConstraintPtr> > (s.value);
        break;

      case 65: // opt_column_id_list
      case 66: // column_id_list
        value.move< std::vector<std::string> > (s.value);
        break;

      default:
        break;
    }

    location = s.location;
  }

  // by_type.
  inline
  parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  parser::by_type::type_get () const
  {
    return type;
  }

  inline
  parser::token_type
  parser::by_type::token () const
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short int
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
  // Implementation of make_symbol for each symbol type.
  parser::symbol_type
  parser::make_EOF (const location_type& l)
  {
    return symbol_type (token::MSQL_EOF, l);
  }

  parser::symbol_type
  parser::make_STAR (const location_type& l)
  {
    return symbol_type (token::MSQL_STAR, l);
  }

  parser::symbol_type
  parser::make_LBRACK (const location_type& l)
  {
    return symbol_type (token::MSQL_LBRACK, l);
  }

  parser::symbol_type
  parser::make_RBRACK (const location_type& l)
  {
    return symbol_type (token::MSQL_RBRACK, l);
  }

  parser::symbol_type
  parser::make_COMMA (const location_type& l)
  {
    return symbol_type (token::MSQL_COMMA, l);
  }

  parser::symbol_type
  parser::make_DOT (const location_type& l)
  {
    return symbol_type (token::MSQL_DOT, l);
  }

  parser::symbol_type
  parser::make_SEMICOLON (const location_type& l)
  {
    return symbol_type (token::MSQL_SEMICOLON, l);
  }

  parser::symbol_type
  parser::make_LBRACK_SQ (const location_type& l)
  {
    return symbol_type (token::MSQL_LBRACK_SQ, l);
  }

  parser::symbol_type
  parser::make_RBRACK_SQ (const location_type& l)
  {
    return symbol_type (token::MSQL_RBRACK_SQ, l);
  }

  parser::symbol_type
  parser::make_CREATE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_CREATE, v, l);
  }

  parser::symbol_type
  parser::make_TEMP (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_TEMP, v, l);
  }

  parser::symbol_type
  parser::make_TABLE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_TABLE, v, l);
  }

  parser::symbol_type
  parser::make_WITHOUT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_WITHOUT, v, l);
  }

  parser::symbol_type
  parser::make_ROWID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_ROWID, v, l);
  }

  parser::symbol_type
  parser::make_CONSTRAINT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_CONSTRAINT, v, l);
  }

  parser::symbol_type
  parser::make_PRIMARY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_PRIMARY, v, l);
  }

  parser::symbol_type
  parser::make_KEY (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_KEY, v, l);
  }

  parser::symbol_type
  parser::make_ASC (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_ASC, v, l);
  }

  parser::symbol_type
  parser::make_DESC (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_DESC, v, l);
  }

  parser::symbol_type
  parser::make_AUTOINCREMENT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_AUTOINCREMENT, v, l);
  }

  parser::symbol_type
  parser::make_NOT_NULL (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_NOT_NULL, v, l);
  }

  parser::symbol_type
  parser::make_UNIQUE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_UNIQUE, v, l);
  }

  parser::symbol_type
  parser::make_CHECK (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_CHECK, v, l);
  }

  parser::symbol_type
  parser::make_DEFAULT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_DEFAULT, v, l);
  }

  parser::symbol_type
  parser::make_COLLATE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_COLLATE, v, l);
  }

  parser::symbol_type
  parser::make_FOREIGN (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_FOREIGN, v, l);
  }

  parser::symbol_type
  parser::make_REFERENCES (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_REFERENCES, v, l);
  }

  parser::symbol_type
  parser::make_ON_DELETE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_ON_DELETE, v, l);
  }

  parser::symbol_type
  parser::make_ON_UPDATE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_ON_UPDATE, v, l);
  }

  parser::symbol_type
  parser::make_SET_NULL (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_SET_NULL, v, l);
  }

  parser::symbol_type
  parser::make_SET_DEFAULT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_SET_DEFAULT, v, l);
  }

  parser::symbol_type
  parser::make_CASCADE (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_CASCADE, v, l);
  }

  parser::symbol_type
  parser::make_RESTRICT (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_RESTRICT, v, l);
  }

  parser::symbol_type
  parser::make_NO_ACTION (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_NO_ACTION, v, l);
  }

  parser::symbol_type
  parser::make_ID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_ID, v, l);
  }

  parser::symbol_type
  parser::make_INTNUM (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_INTNUM, v, l);
  }

  parser::symbol_type
  parser::make_FLOATNUM (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_FLOATNUM, v, l);
  }

  parser::symbol_type
  parser::make_STRING (const std::string& v, const location_type& l)
  {
    return symbol_type (token::MSQL_STRING, v, l);
  }

  parser::symbol_type
  parser::make_ON (const location_type& l)
  {
    return symbol_type (token::MSQL_ON, l);
  }


#line 56 "sqlite_bison_parser.y" // lalr1.cc:377
} } } //  meow::utils::sql_parser 
#line 1864 "sqlite_bison_parser.hpp" // lalr1.cc:377




#endif // !YY_YY_SQLITE_BISON_PARSER_HPP_INCLUDED
