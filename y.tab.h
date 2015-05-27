/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INT = 258,
     INTTYPE = 259,
     ID = 260,
     SEMI = 261,
     COMMA = 262,
     LC = 263,
     RC = 264,
     IF = 265,
     FOR = 266,
     IFX = 267,
     STRUCT = 268,
     RETURN = 269,
     ELSE = 270,
     BREAK = 271,
     CONT = 272,
     LP = 273,
     RP = 274,
     LB = 275,
     RB = 276,
     ASSIGN = 277,
     LOR = 278,
     LAND = 279,
     OR = 280,
     XOR = 281,
     AND = 282,
     RELATION = 283,
     SHIFT = 284,
     SUB = 285,
     ADD = 286,
     PRODUCT = 287,
     UNARYOP = 288,
     DOT = 289
   };
#endif
/* Tokens.  */
#define INT 258
#define INTTYPE 259
#define ID 260
#define SEMI 261
#define COMMA 262
#define LC 263
#define RC 264
#define IF 265
#define FOR 266
#define IFX 267
#define STRUCT 268
#define RETURN 269
#define ELSE 270
#define BREAK 271
#define CONT 272
#define LP 273
#define RP 274
#define LB 275
#define RB 276
#define ASSIGN 277
#define LOR 278
#define LAND 279
#define OR 280
#define XOR 281
#define AND 282
#define RELATION 283
#define SHIFT 284
#define SUB 285
#define ADD 286
#define PRODUCT 287
#define UNARYOP 288
#define DOT 289




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

