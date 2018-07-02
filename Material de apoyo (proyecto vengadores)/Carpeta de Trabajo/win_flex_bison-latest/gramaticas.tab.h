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

#ifndef YY_YY_GRAMATICAS_TAB_H_INCLUDED
# define YY_YY_GRAMATICAS_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 583 "gramaticas.cpp" /* yacc.c:1909  */

	#include <list>
	#include <string>
	using namespace std;

#line 50 "gramaticas.tab.h" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    HILERA = 258,
    FOR = 259,
    WHILE = 260,
    IF = 261,
    ELSE = 262,
    COMPARACION = 263,
    AND_OR = 264,
    BOOL = 265,
    NUM = 266,
    INI = 267,
    FIN = 268,
    RET = 269,
    MAS = 270,
    MENOS = 271,
    POR = 272,
    ENTRE = 273,
    MOD = 274,
    ENTRADA = 275,
    SALIDA = 276,
    ID = 277,
    ERROR = 278,
    EJECUCION = 279,
    PARD = 280,
    CORD = 281,
    PARI = 282,
    CORI = 283,
    IGUAL = 284,
    DOSP = 285,
    COM = 286,
    PUNTO = 287,
    POINTER = 288,
    PYC = 289,
    LINK = 290,
    JAAH = 291
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 588 "gramaticas.cpp" /* yacc.c:1909  */

	string* hilera;
	int intVal;
	struct Caja* nodo;
	struct Simbolos* simbolo;
	std::list<Caja*>* params;

#line 107 "gramaticas.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAMATICAS_TAB_H_INCLUDED  */
