/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_HINT_PARSER_EXPORT_HOME_PB2_BUILD_SB_3_38464617_1584985015_96_DIST_GPL_SQL_SQL_HINTS_YY_H_INCLUDED
# define YY_HINT_PARSER_EXPORT_HOME_PB2_BUILD_SB_3_38464617_1584985015_96_DIST_GPL_SQL_SQL_HINTS_YY_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int HINT_PARSER_debug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    MAX_EXECUTION_TIME_HINT = 258,
    BKA_HINT = 259,
    BNL_HINT = 260,
    DUPSWEEDOUT_HINT = 261,
    FIRSTMATCH_HINT = 262,
    INTOEXISTS_HINT = 263,
    LOOSESCAN_HINT = 264,
    MATERIALIZATION_HINT = 265,
    NO_BKA_HINT = 266,
    NO_BNL_HINT = 267,
    NO_ICP_HINT = 268,
    NO_MRR_HINT = 269,
    NO_RANGE_OPTIMIZATION_HINT = 270,
    NO_SEMIJOIN_HINT = 271,
    MRR_HINT = 272,
    QB_NAME_HINT = 273,
    SEMIJOIN_HINT = 274,
    SUBQUERY_HINT = 275,
    HINT_ARG_NUMBER = 276,
    HINT_ARG_IDENT = 277,
    HINT_ARG_QB_NAME = 278,
    HINT_CLOSE = 279,
    HINT_ERROR = 280
  };
#endif
/* Tokens.  */
#define MAX_EXECUTION_TIME_HINT 258
#define BKA_HINT 259
#define BNL_HINT 260
#define DUPSWEEDOUT_HINT 261
#define FIRSTMATCH_HINT 262
#define INTOEXISTS_HINT 263
#define LOOSESCAN_HINT 264
#define MATERIALIZATION_HINT 265
#define NO_BKA_HINT 266
#define NO_BNL_HINT 267
#define NO_ICP_HINT 268
#define NO_MRR_HINT 269
#define NO_RANGE_OPTIMIZATION_HINT 270
#define NO_SEMIJOIN_HINT 271
#define MRR_HINT 272
#define QB_NAME_HINT 273
#define SEMIJOIN_HINT 274
#define SUBQUERY_HINT 275
#define HINT_ARG_NUMBER 276
#define HINT_ARG_IDENT 277
#define HINT_ARG_QB_NAME 278
#define HINT_CLOSE 279
#define HINT_ERROR 280

/* Value type.  */



int HINT_PARSER_parse (class THD *thd, class Hint_scanner *scanner, class PT_hint_list **ret);

#endif /* !YY_HINT_PARSER_EXPORT_HOME_PB2_BUILD_SB_3_38464617_1584985015_96_DIST_GPL_SQL_SQL_HINTS_YY_H_INCLUDED  */
