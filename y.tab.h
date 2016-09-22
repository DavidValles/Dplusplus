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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 2 "syn.y" /* yacc.c:1915  */

    #include <string>
    using namespace std;

#line 49 "y.tab.h" /* yacc.c:1915  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INCLUDE = 258,
    MAIN = 259,
    VAR = 260,
    FUNC = 261,
    INT = 262,
    DECIMAL = 263,
    TEXT = 264,
    CHARACTER = 265,
    FLAG = 266,
    ARRAY = 267,
    MATRIX = 268,
    IF = 269,
    ELSE = 270,
    ELSEIF = 271,
    WHILE = 272,
    DO = 273,
    CLASS = 274,
    PRIVATE = 275,
    PUBLIC = 276,
    PRINT = 277,
    READ = 278,
    NOTEQUALTO = 279,
    EQUALTO = 280,
    LESSTHANOREQUALTO = 281,
    GREATERTHANOREQUALTO = 282,
    AND = 283,
    OR = 284,
    NOT = 285,
    ID = 286,
    ICONSTANT = 287,
    DCONSTANT = 288,
    SCONSTANT = 289
  };
#endif
/* Tokens.  */
#define INCLUDE 258
#define MAIN 259
#define VAR 260
#define FUNC 261
#define INT 262
#define DECIMAL 263
#define TEXT 264
#define CHARACTER 265
#define FLAG 266
#define ARRAY 267
#define MATRIX 268
#define IF 269
#define ELSE 270
#define ELSEIF 271
#define WHILE 272
#define DO 273
#define CLASS 274
#define PRIVATE 275
#define PUBLIC 276
#define PRINT 277
#define READ 278
#define NOTEQUALTO 279
#define EQUALTO 280
#define LESSTHANOREQUALTO 281
#define GREATERTHANOREQUALTO 282
#define AND 283
#define OR 284
#define NOT 285
#define ID 286
#define ICONSTANT 287
#define DCONSTANT 288
#define SCONSTANT 289

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 76 "syn.y" /* yacc.c:1915  */

  string* stringValue;
  float floatValue;
  int intValue;

#line 135 "y.tab.h" /* yacc.c:1915  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
