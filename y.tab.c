/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 1 "small.y"

/*
 * By Xue Yinsong,Student No.5120309415
 * this file is the yacc source file , where all the syntax are being parsed here.
 * Mainly the idea was simple, we parse the program using the syntax rules in the document.
 * But in order to eliminate the bugs, I have done a few changes:
 * 1.the token BINARYOP is splited to several tokens like add/product.....
 * 2.the char "-" is singled out as token SUB, because it has 2 roles(a binaryop and a unaryop)
 * 3.the token ASSIGN which was not declared yet used in the tutorial is added.
 * 4.a token "IFX" was added , because we have to eliminate conflicts of if then / if then else, we set the priority of the latter one higher.
 * 5.solve the conflict of - as unary and binary operators by set priority to the former one.
 */ 
#include "def.h"

char code[MAX_CODE_NUM][MAX_CODE_LEN];

int has_main = 0;
int *merge_list(int *a,int *b){
	if (a == 0)
		return b;
	if (b == 0)
		return a;
	int la = a[0];
	int lb = b[0];
	int lc = la+lb;
	int *c = malloc(sizeof(int) * (lc+1));
	int i;
	for (i = 1;i <= la;i++) c[i] = a[i];
	for (i = 1;i <= lb;i++) c[i+la] = b[i];
	c[0] = lc;
	return c;
}

int *make_list(int x){
	int *c = malloc(sizeof(int)*2);
	c[0] = 1;c[1] = x;
	return c;
}
void backpatch(int *p,int pla){
	if (p == 0){
		return;
	}
	int i;
	for (i = 1;i <= p[0];i++){
		int tlen = strlen(code[p[i]]);
		if (code[p[i]][tlen-1] == ' ')	
			strcat(code[p[i]],get_label(pla));
		}
}
void ErrorMsg(char *s)
{
	fprintf(stderr,"%s",s);
	exit(0);
}
/*union
int for (intvalue)
char * for (ID)
TYPE for (anything)
array for (array)
ndim for (array)
offset for (array)
code for (anything)
place for (anything)
*/


typedef struct {
	int Intval;
	char * Strval;
	TYPE *Type;
	TYPE *array;
	int array_place;
	int ndim;
	int offset;
	int *true_list;
	int *false_list;
	int *next_list;
	int *break_list;
	int *continue_list;
	symbol_table *s_table;
	Place_type quad;
	int place;
	int num;
	queuetype *queue;
	int sdecs_dest;/*0-derived from struct declaration
					 1-derived from struct varible declaration	
						*/
}NODEtype;

char *get_place(int place_num){
	char *buf = malloc(sizeof(char)*20);
	if (place_num == -1){
	sprintf(buf,"$v0");
	return buf;
	}
	sprintf(buf,"t%d",place_num);
	return buf;
}

char *get_label(int quad_num){
	char *buf = malloc(sizeof(char)*20);
	sprintf(buf,"%d",quad_num+1);
	return buf;
}

symbol_table *Default;
void initial_set(){
	Default = malloc(sizeof(symbol_table));
	Default->next = 0;
	Default->name = malloc(sizeof(char));
	Default->name[0] = 0;
	Default->pos = 0;/* no meaning*/
	Default->type = 0;/* no meaning*/
}

int next_quad;

int newquad(){return ++next_quad;}

char genbuf[101];
void gen(char *buf){
	strcpy(code[next_quad],buf);
	next_quad++;
}

int next_place;
int newplace(){return ++next_place;}


#define YYSTYPE NODEtype
#define YYDEBUG 1
#include "lex.yy.c"
#define TTTT(rrrr) case rrrr:puts(#rrrr);break;
/* prototypes */
extern int yychar,curr_line;
extern int yydebug;
int yylex(void);

void yyerror(char *s);
/*this funcion is for debugging,when this function is called, 
 *it will print out the token of the point where error occurs
 */
static void print_tok()
{
if (yychar < 255) {
	fprintf(stderr,"%c",yychar);
	}
	else{
		switch (yychar){
			TTTT(INT);
			TTTT(INTTYPE);
			TTTT(ID);
			TTTT(SEMI);
			TTTT(COMMA);
			TTTT(LC);
			TTTT(RC);
			TTTT(IF);
			TTTT(FOR);
			TTTT(IFX);
			TTTT(STRUCT);
			TTTT(RETURN);
			TTTT(ELSE);
			TTTT(BREAK);
			TTTT(CONT);
			TTTT(ASSIGN);
			TTTT(LOR);
			TTTT(LAND);
			TTTT(OR);
			TTTT(XOR);
			TTTT(AND);
			TTTT(RELATION);
			TTTT(SHIFT);
			TTTT(ADD);
			TTTT(SUB);
			TTTT(PRODUCT);
			TTTT(UNARYOP);
			TTTT(LP);
			TTTT(RP);
			TTTT(LB);
			TTTT(RB);
			TTTT(DOT);
		}
	}
}


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 360 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   241

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  35
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  63
/* YYNRULES -- Number of rules.  */
#define YYNRULES  116
/* YYNRULES -- Number of states.  */
#define YYNSTATES  215

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     7,    11,    12,    13,    18,    23,
      29,    30,    31,    32,    34,    39,    40,    41,    43,    48,
      53,    61,    62,    63,    64,    65,    74,    81,    85,    86,
      87,    88,    89,    95,    96,   102,   105,   106,   107,   114,
     115,   116,   120,   121,   122,   125,   127,   132,   144,   152,
     167,   170,   173,   174,   175,   176,   177,   184,   191,   192,
     193,   194,   201,   202,   203,   204,   209,   211,   212,   214,
     219,   227,   232,   233,   234,   236,   239,   244,   248,   250,
     255,   256,   261,   263,   264,   266,   267,   268,   269,   270,
     274,   279,   284,   289,   294,   299,   304,   309,   314,   319,
     324,   329,   333,   338,   344,   346,   348,   350,   353,   357,
     358,   359,   365,   370,   376,   379,   380
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      36,     0,    -1,    37,    38,    -1,    -1,    40,    39,    38,
      -1,    -1,    -1,     4,    42,    46,     6,    -1,    50,    43,
      44,     6,    -1,     4,    42,    55,    41,    59,    -1,    -1,
      -1,    -1,     5,    -1,     5,     7,    45,    44,    -1,    -1,
      -1,    80,    -1,    80,    22,    49,    82,    -1,    80,     7,
      47,    46,    -1,    80,    22,    49,    82,     7,    48,    46,
      -1,    -1,    -1,    -1,    -1,    13,    51,     5,    52,     8,
      53,    72,     9,    -1,    13,    54,     8,    53,    72,     9,
      -1,    13,    51,     5,    -1,    -1,    -1,    -1,    -1,     5,
      18,    56,    57,    19,    -1,    -1,     4,     5,     7,    58,
      57,    -1,     4,     5,    -1,    -1,    -1,     8,    60,    69,
      61,    62,     9,    -1,    -1,    -1,    64,    63,    62,    -1,
      -1,    -1,    86,     6,    -1,    59,    -1,    14,    87,    86,
       6,    -1,    10,    18,    88,    86,    19,    67,    64,    65,
      15,    68,    64,    -1,    10,    18,    88,    86,    19,    67,
      64,    -1,    11,    18,    88,    86,     6,    67,    86,     6,
      67,    86,    66,    19,    67,    64,    -1,    17,     6,    -1,
      16,     6,    -1,    -1,    -1,    -1,    -1,     4,    42,    77,
       6,    70,    69,    -1,    50,    71,    75,     6,    70,    69,
      -1,    -1,    -1,    -1,     4,    73,    75,     6,    74,    72,
      -1,    -1,    -1,    -1,     5,     7,    76,    75,    -1,     5,
      -1,    -1,    80,    -1,    80,     7,    78,    77,    -1,    80,
      22,    49,    82,     7,    79,    77,    -1,    80,    22,    49,
      82,    -1,    -1,    -1,     5,    -1,    81,    21,    -1,    81,
      21,    20,     3,    -1,     5,    20,     3,    -1,    86,    -1,
       8,    83,    84,     9,    -1,    -1,    86,     7,    85,    84,
      -1,    86,    -1,    -1,    90,    -1,    -1,    -1,    -1,    -1,
      30,    87,    90,    -1,    90,    32,    88,    90,    -1,    90,
      31,    88,    90,    -1,    90,    30,    88,    90,    -1,    90,
      29,    88,    90,    -1,    90,    28,    88,    90,    -1,    90,
      27,    88,    90,    -1,    90,    26,    88,    90,    -1,    90,
      25,    88,    90,    -1,    90,    24,    92,    90,    -1,    90,
      23,    92,    90,    -1,    91,    22,    88,    90,    -1,    33,
      87,    90,    -1,    18,    87,    90,    19,    -1,     5,    18,
      93,    95,    19,    -1,    91,    -1,     3,    -1,     5,    -1,
      94,    21,    -1,     5,    34,     5,    -1,    -1,    -1,    94,
      21,    20,    89,    86,    -1,     5,    20,    88,    86,    -1,
      86,    97,     7,    96,    95,    -1,    86,    97,    -1,    -1,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   212,   212,   218,   222,   226,   231,   236,   239,   242,
     250,   255,   260,   266,   274,   282,   288,   293,   294,   297,
     301,   305,   310,   315,   319,   330,   334,   338,   345,   349,
     354,   357,   362,   380,   395,   400,   405,   411,   417,   425,
     430,   437,   444,   449,   455,   462,   468,   477,   485,   492,
     505,   512,   521,   526,   530,   534,   539,   542,   545,   550,
     555,   561,   566,   569,   575,   580,   594,   609,   615,   619,
     623,   627,   634,   640,   652,   661,   674,   683,   696,   709,
     721,   728,   735,   756,   776,   783,   789,   792,   795,   799,
     804,   819,   827,   835,   846,   871,   876,   884,   892,   897,
     902,   918,   942,   947,   969,   982,   989,   996,  1009,  1024,
    1029,  1041,  1053,  1066,  1069,  1074,  1078
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "INTTYPE", "ID", "SEMI", "COMMA",
  "LC", "RC", "IF", "FOR", "IFX", "STRUCT", "RETURN", "ELSE", "BREAK",
  "CONT", "LP", "RP", "LB", "RB", "ASSIGN", "LOR", "LAND", "OR", "XOR",
  "AND", "RELATION", "SHIFT", "SUB", "ADD", "PRODUCT", "UNARYOP", "DOT",
  "$accept", "PROGRAM", "M0", "EXTDEFS", "M1", "EXTDEF", "M18", "M369",
  "M3", "SEXTVARS", "M15", "EXTVARS", "M4", "M8", "M5", "STSPEC", "M9",
  "M10", "M111", "M11", "FUNC", "M16", "PARAS", "M17", "STMTBLOCK", "M19",
  "M22", "STMTS", "M41", "STMT", "N", "M66", "M26", "M27", "DEFS", "M20",
  "M21", "SDEFS", "M12", "M13", "SDECS", "M14", "DECS", "M23", "M24",
  "VAR", "ELISTDEF", "INIT", "M6", "ARRARGS", "M7", "EXP", "MF1", "MF2",
  "MF3", "EXPS", "LLL", "M31", "M32", "ALIST", "ARGS", "MG3", "M33", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    35,    36,    37,    38,    38,    39,    40,    40,    40,
      41,    42,    43,    44,    44,    44,    45,    46,    46,    46,
      46,    46,    47,    48,    49,    50,    50,    50,    51,    52,
      53,    54,    55,    56,    57,    57,    57,    58,    59,    60,
      61,    62,    62,    63,    64,    64,    64,    64,    64,    64,
      64,    64,    65,    66,    67,    68,    69,    69,    69,    70,
      71,    72,    72,    73,    74,    75,    75,    76,    77,    77,
      77,    77,    78,    79,    80,    80,    81,    81,    82,    82,
      83,    84,    84,    85,    86,    86,    87,    88,    89,    90,
      90,    90,    90,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    90,    90,    91,    91,    91,    92,
      93,    94,    94,    95,    95,    96,    97
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     0,     3,     0,     0,     4,     4,     5,
       0,     0,     0,     1,     4,     0,     0,     1,     4,     4,
       7,     0,     0,     0,     0,     8,     6,     3,     0,     0,
       0,     0,     5,     0,     5,     2,     0,     0,     6,     0,
       0,     3,     0,     0,     2,     1,     4,    11,     7,    14,
       2,     2,     0,     0,     0,     0,     6,     6,     0,     0,
       0,     6,     0,     0,     0,     4,     1,     0,     1,     4,
       7,     4,     0,     0,     1,     2,     4,     3,     1,     4,
       0,     4,     1,     0,     1,     0,     0,     0,     0,     3,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     3,     4,     5,     1,     1,     1,     2,     3,     0,
       0,     5,     4,     5,     2,     0,     0
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     5,     1,    11,    28,     2,     6,    12,    21,
       0,     0,     5,    15,    74,     0,    10,    17,     0,    27,
      30,     4,    13,     0,    33,     0,     7,     0,    22,    24,
      75,     0,    62,    16,     8,    36,    77,    39,     9,    21,
      85,     0,    30,    63,     0,    15,     0,     0,    58,    74,
      19,   105,   106,    80,    86,    86,    86,    18,    78,    84,
     104,     0,    76,    62,     0,    26,    14,    35,    32,    11,
      60,    40,   110,    87,     0,    85,     0,     0,     0,    23,
     109,   109,    87,    87,    87,    87,    87,    87,    87,    87,
      87,   107,     0,    66,     0,    37,     0,     0,    42,    85,
      85,   108,     0,    82,     0,    89,   101,    21,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    88,
      25,    67,    64,    36,     0,    68,     0,     0,     0,    86,
       0,     0,    45,     0,    43,     0,   116,     0,   112,    79,
      83,   102,    20,    99,    98,    97,    96,    95,    94,    93,
      92,    91,    90,   100,    85,     0,    62,    34,    59,    72,
      24,    59,    87,    87,    85,    51,    50,    38,    42,    44,
     114,   103,    85,   111,    65,    61,    58,     0,    85,    58,
      85,    85,     0,    41,   115,    81,    56,    69,    71,    57,
       0,     0,    46,    85,    73,    54,    54,   113,     0,    85,
      85,    70,    48,     0,     0,    54,    55,    85,    85,    53,
      47,     0,    54,    85,    49
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     6,    12,     7,    27,     9,    13,    23,
      45,    15,    39,   107,    40,    70,    10,    31,    32,    11,
      16,    35,    47,   123,   132,    48,    98,   133,   168,   134,
     204,   211,   199,   208,    71,   176,    97,    44,    64,   156,
      94,   155,   124,   177,   198,    17,    18,    57,    75,   102,
     172,   135,    76,   100,   154,    59,    60,   108,    99,    61,
     137,   193,   170
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -181
static const yytype_int16 yypact[] =
{
    -181,    28,    40,  -181,  -181,    31,  -181,  -181,  -181,    50,
      83,    32,    40,    84,    38,    44,  -181,    19,    69,    89,
    -181,  -181,    85,    94,  -181,    98,  -181,   108,  -181,  -181,
      99,   117,   123,  -181,  -181,   127,  -181,  -181,  -181,   132,
      15,   136,  -181,  -181,   134,    84,   139,   133,    74,   135,
    -181,  -181,     4,  -181,  -181,  -181,  -181,   147,  -181,   155,
     137,   140,  -181,   123,   151,  -181,  -181,   150,  -181,  -181,
    -181,  -181,  -181,  -181,   153,    16,    16,    16,    16,  -181,
    -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,  -181,
    -181,   142,   154,   158,   160,  -181,   132,   151,    88,    16,
      16,  -181,   196,   199,   145,  -181,  -181,   132,    16,    16,
      16,    16,    16,    16,    16,    16,    16,    16,    16,  -181,
    -181,  -181,  -181,   127,   201,    20,   202,   191,   192,  -181,
     205,   206,  -181,   204,  -181,   208,  -181,   197,  -181,  -181,
    -181,  -181,  -181,   164,   172,    81,   119,    45,   104,    49,
     183,   183,  -181,   155,    16,   151,   123,  -181,  -181,  -181,
    -181,  -181,  -181,  -181,    16,  -181,  -181,  -181,    88,  -181,
     210,  -181,    16,  -181,  -181,  -181,    74,   132,    15,    74,
      16,    16,   212,  -181,  -181,  -181,  -181,  -181,   213,  -181,
     200,   215,  -181,    16,  -181,  -181,  -181,  -181,   132,   112,
      16,  -181,  -181,   216,   209,  -181,  -181,    16,   112,  -181,
    -181,   207,  -181,   112,  -181
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -181,  -181,  -181,   211,  -181,  -181,  -181,   156,  -181,   182,
    -181,   -22,  -181,  -181,    68,    35,  -181,  -181,   187,  -181,
    -181,  -181,   107,  -181,   214,  -181,  -181,    63,  -181,  -156,
    -181,  -181,  -180,  -181,  -125,    71,  -181,   -61,  -181,  -181,
     -83,  -181,  -162,  -181,  -181,   -95,  -181,    55,  -181,    62,
    -181,   -40,   -43,   -79,  -181,   -47,  -181,   157,  -181,  -181,
      42,  -181,  -181
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -86
static const yytype_int16 yytable[] =
{
      58,   125,    92,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    77,    78,   126,   187,   200,    50,    51,    51,
      52,    52,    72,    53,    73,   207,    28,   159,     3,   104,
     105,   106,   213,    54,    54,   103,   201,     8,    74,   -31,
      20,    29,   160,   202,     4,    55,    55,     8,    56,    56,
      26,   186,   210,     5,   189,    14,    24,   214,    25,   136,
     138,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   174,    85,    86,    87,    88,    89,    69,    87,
      88,    89,   125,   180,   181,   142,   164,     5,    19,    22,
      30,    51,    33,    52,   -85,   175,    37,   -29,   127,   128,
      34,    36,   129,   125,   130,   131,    54,    83,    84,    85,
      86,    87,    88,    89,   173,    51,    37,    52,    55,    41,
      37,    56,   127,   128,   182,    42,   129,    43,   130,   131,
      54,    46,   103,    86,    87,    88,    89,    49,    58,    62,
     190,   191,    55,    65,    67,    56,    84,    85,    86,    87,
      88,    89,    68,   136,    79,    25,    93,    95,   101,    90,
     203,    91,   119,   120,   141,   121,   122,   209,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    80,    81,
      82,    83,    84,    85,    86,    87,    88,    89,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    82,    83,    84,
      85,    86,    87,    88,    89,   139,   140,   158,   161,   162,
     163,   165,   166,   167,   169,    89,   171,   184,   192,   195,
     194,   196,   205,    21,   206,    96,   212,    66,   178,    63,
     157,   183,   179,   188,   185,   197,     0,     0,   109,     0,
       0,    38
};

static const yytype_int16 yycheck[] =
{
      40,    96,    63,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    55,    56,    97,   177,   196,    39,     3,     3,
       5,     5,    18,     8,    20,   205,     7,     7,     0,    76,
      77,    78,   212,    18,    18,    75,   198,     2,    34,     8,
       8,    22,    22,   199,     4,    30,    30,    12,    33,    33,
       6,   176,   208,    13,   179,     5,    18,   213,    20,    99,
     100,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   155,    28,    29,    30,    31,    32,     4,    30,
      31,    32,   177,   162,   163,   107,   129,    13,     5,     5,
      21,     3,     7,     5,     6,   156,     8,     8,    10,    11,
       6,     3,    14,   198,    16,    17,    18,    26,    27,    28,
      29,    30,    31,    32,   154,     3,     8,     5,    30,    20,
       8,    33,    10,    11,   164,     8,    14,     4,    16,    17,
      18,     4,   172,    29,    30,    31,    32,     5,   178,     3,
     180,   181,    30,     9,     5,    33,    27,    28,    29,    30,
      31,    32,    19,   193,     7,    20,     5,     7,     5,    22,
     200,    21,    20,     9,    19,     7,     6,   207,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    25,    26,    27,
      28,    29,    30,    31,    32,     9,     7,     6,     6,    18,
      18,     6,     6,     9,     6,    32,    19,     7,     6,    19,
       7,     6,     6,    12,    15,    69,    19,    45,   160,    42,
     123,   168,   161,   178,   172,   193,    -1,    -1,    81,    -1,
      -1,    27
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    36,    37,     0,     4,    13,    38,    40,    50,    42,
      51,    54,    39,    43,     5,    46,    55,    80,    81,     5,
       8,    38,     5,    44,    18,    20,     6,    41,     7,    22,
      21,    52,    53,     7,     6,    56,     3,     8,    59,    47,
      49,    20,     8,     4,    72,    45,     4,    57,    60,     5,
      46,     3,     5,     8,    18,    30,    33,    82,    86,    90,
      91,    94,     3,    53,    73,     9,    44,     5,    19,     4,
      50,    69,    18,    20,    34,    83,    87,    87,    87,     7,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      22,    21,    72,     5,    75,     7,    42,    71,    61,    93,
      88,     5,    84,    86,    90,    90,    90,    48,    92,    92,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    20,
       9,     7,     6,    58,    77,    80,    75,    10,    11,    14,
      16,    17,    59,    62,    64,    86,    86,    95,    86,     9,
       7,    19,    46,    90,    90,    90,    90,    90,    90,    90,
      90,    90,    90,    90,    89,    76,    74,    57,     6,     7,
      22,     6,    18,    18,    87,     6,     6,     9,    63,     6,
      97,    19,    85,    86,    75,    72,    70,    78,    49,    70,
      88,    88,    86,    62,     7,    84,    69,    77,    82,    69,
      86,    86,     6,    96,     7,    19,     6,    95,    79,    67,
      67,    77,    64,    86,    65,     6,    15,    67,    68,    86,
      64,    66,    19,    67,    64
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 212 "small.y"
    {
	   		int i;
			backpatch((yyvsp[(2) - (2)]).next_list,next_quad);
	   }
    break;

  case 3:
#line 218 "small.y"
    {
  (yyval).s_table = Default;
  }
    break;

  case 4:
#line 222 "small.y"
    {
	   (yyval).s_table = (yyvsp[(3) - (3)]).s_table;
		backpatch((yyvsp[(2) - (3)]).next_list,next_quad);
	   }
    break;

  case 5:
#line 226 "small.y"
    {
	   (yyval).s_table = (yyvsp[(0) - (0)]).s_table;
	   }
    break;

  case 6:
#line 231 "small.y"
    {
  (yyval).s_table=(yyvsp[(0) - (0)]).s_table;
  }
    break;

  case 7:
#line 236 "small.y"
    {
	  (yyval).s_table = (yyvsp[(3) - (4)]).s_table;
	  }
    break;

  case 8:
#line 239 "small.y"
    {
	  (yyval).s_table = (yyvsp[(3) - (4)]).s_table;
	  }
    break;

  case 9:
#line 242 "small.y"
    {
	  (yyval).Type = (yyvsp[(3) - (5)]).Type;
	  (yyval).s_table = sym_table_of((yyvsp[(2) - (5)]).s_table,(yyvsp[(3) - (5)]).Strval,(yyvsp[(2) - (5)]).place,(yyvsp[(3) - (5)]).Type,0);
	  sprintf(genbuf,"endfunc %s",(yyvsp[(3) - (5)]).Strval);
	  gen(genbuf);
	  }
    break;

  case 10:
#line 250 "small.y"
    {
   (yyval).Type = (yyvsp[(0) - (0)]).Type;
   (yyval).s_table = (yyvsp[(0) - (0)]).s_table;
   }
    break;

  case 11:
#line 255 "small.y"
    {
	(yyval).s_table = (yyvsp[(-1) - (0)]).s_table;
	(yyval).Type = (yyvsp[(0) - (0)]).Type;
	(yyval).place = -1;
	}
    break;

  case 12:
#line 260 "small.y"
    {
  (yyval).s_table = (yyvsp[(0) - (0)]).s_table;
  (yyval).Type = (yyvsp[(0) - (0)]).Type;
  }
    break;

  case 13:
#line 266 "small.y"
    {
			int tp = newplace();
			(yyval).s_table = (yyvsp[(0) - (1)]).s_table;
			(yyval).Type = (yyvsp[(0) - (1)]).Type;
			sprintf(genbuf,"defarr %s %d",get_place(tp),get_size((yyvsp[(0) - (1)]).Type));
			gen(genbuf);
			(yyval).s_table = sym_table_of((yyval).s_table,(yyvsp[(1) - (1)]).Strval,tp,(yyvsp[(0) - (1)]).Type,0);
		}
    break;

  case 14:
#line 274 "small.y"
    {
			int tp = newplace();
			(yyval).s_table = (yyvsp[(4) - (4)]).s_table;
			(yyval).Type = (yyvsp[(0) - (4)]).Type;
			sprintf(genbuf,"defarr %s %d",get_place(tp),get_size((yyvsp[(0) - (4)]).Type));
			gen(genbuf);
			(yyval).s_table = sym_table_of((yyval).s_table,(yyvsp[(1) - (4)]).Strval,tp,(yyvsp[(0) - (4)]).Type,0);
		}
    break;

  case 15:
#line 282 "small.y"
    {
		(yyval).s_table = (yyvsp[(0) - (0)]).s_table;
		(yyval).Type = (yyvsp[(0) - (0)]).Type;
		}
    break;

  case 16:
#line 288 "small.y"
    {
   (yyval).Type = (yyvsp[(-2) - (0)]).Type;
   (yyval).s_table = (yyvsp[(-2) - (0)]).s_table;
   }
    break;

  case 17:
#line 293 "small.y"
    {(yyval).s_table=(yyvsp[(1) - (1)]).s_table;(yyval).Type=(yyvsp[(1) - (1)]).Type;}
    break;

  case 18:
#line 294 "small.y"
    {
	   		(yyval).s_table=(yyvsp[(1) - (4)]).s_table;(yyval).Type = (yyvsp[(1) - (4)]).Type;
	   }
    break;

  case 19:
#line 297 "small.y"
    {
	   		(yyval).s_table = (yyvsp[(4) - (4)]).s_table;
			(yyval).Type = (yyvsp[(4) - (4)]).Type;
	   }
    break;

  case 20:
#line 301 "small.y"
    {
	   		(yyval).s_table=(yyvsp[(7) - (7)]).s_table;(yyval).Type = (yyvsp[(1) - (7)]).Type;
	   }
    break;

  case 21:
#line 305 "small.y"
    {
	   (yyval).s_table=(yyvsp[(0) - (0)]).s_table;
	   }
    break;

  case 22:
#line 310 "small.y"
    {
  (yyval).s_table = (yyvsp[(-1) - (0)]).s_table;
  (yyval).Type = (yyvsp[(-1) - (0)]).Type;
  }
    break;

  case 23:
#line 315 "small.y"
    {
  (yyval).s_table = (yyvsp[(-1) - (0)]).s_table;
  (yyval).Type = (yyvsp[(-5) - (0)]).Type;
  }
    break;

  case 24:
#line 319 "small.y"
    {
  (yyval).s_table = (yyvsp[(-1) - (0)]).s_table;
  (yyval).Type = (yyvsp[(-1) - (0)]).Type;
  (yyval).Strval = (yyvsp[(-1) - (0)]).Strval;
  if (!((yyval).Type->t == 0 || ((yyval).Type->t == 1 && (yyval).Type->p->t == 0)))
  	ErrorMsg("Error: Type can't be initialized!\n");
  if (strcmp((yyvsp[(0) - (0)]).Strval,"=") != 0){
  	ErrorMsg("Error: Wrong assign operator used in initiating variables.\n");
  }
}
    break;

  case 25:
#line 330 "small.y"
    {
	  (yyval).Type = (yyvsp[(4) - (8)]).Type;
	  (yyval).s_table = (yyvsp[(4) - (8)]).s_table;
	  }
    break;

  case 26:
#line 334 "small.y"
    {
	  (yyval).Type = (yyvsp[(2) - (6)]).Type;
	  (yyval).s_table = (yyvsp[(2) - (6)]).s_table;
	  }
    break;

  case 27:
#line 338 "small.y"
    {
	  (yyval).s_table = (yyvsp[(2) - (3)]).s_table;
	  symbol_table *s = lookup((yyval).s_table,(yyvsp[(3) - (3)]).Strval);
	  (yyval).Type = s->type;
	  }
    break;

  case 28:
#line 345 "small.y"
    {
  (yyval).s_table = (yyvsp[(-1) - (0)]).s_table;
  }
    break;

  case 29:
#line 349 "small.y"
    {
   (yyval).Type = new_struct_type();
   (yyval).s_table = sym_table_of((yyvsp[(-3) - (0)]).s_table,(yyvsp[(0) - (0)]).Strval,0,(yyval).Type,1);
   }
    break;

  case 30:
#line 354 "small.y"
    {
	(yyval).Type = (yyvsp[(-1) - (0)]).Type;
	}
    break;

  case 31:
#line 357 "small.y"
    {
   (yyval).Type = new_struct_type();
   (yyval).s_table = (yyvsp[(-1) - (0)]).s_table;
   }
    break;

  case 32:
#line 362 "small.y"
    {
	int i;
	(yyval).Type = (yyvsp[(4) - (5)]).Type;
	(yyval).s_table = (yyvsp[(4) - (5)]).s_table;
	(yyval).Strval = (yyvsp[(1) - (5)]).Strval;
	sprintf(genbuf,"def %s %d",get_place((yyvsp[(0) - (5)]).place),1);
	gen(genbuf);
	if (!strcmp((yyvsp[(1) - (5)]).Strval,"main"))
		has_main = 1;
	sprintf(genbuf,"deffunc %s %d %s",(yyvsp[(1) - (5)]).Strval,(yyval).Type->num,get_place((yyvsp[(0) - (5)]).place));
	gen(genbuf);
	for (i = (yyval).Type->num-1;i >= 0;i--){
		sprintf(genbuf,"%s",get_place(lookup((yyval).s_table,(yyval).Type->strname[i])->pos));	
		gen(genbuf);
	}
	}
    break;

  case 33:
#line 380 "small.y"
    {
   (yyval).Type = new_function_type();
   (yyvsp[(-2) - (0)]).place = -1;
   /*
   $-2.s_table = sym_table_of($-2.s_table,"rtp",$-2.place,new_int_type(),0);
   */
   /*TODO:check pos*/
   (yyval).s_table = sym_table_of((yyvsp[(-2) - (0)]).s_table,(yyvsp[(-1) - (0)]).Strval,(yyvsp[(-2) - (0)]).place,(yyval).Type,1);
   (yyval).s_table = nosame_sym_table_of((yyval).s_table);
   (yyval).s_table = sym_table_of((yyval).s_table,(yyvsp[(-1) - (0)]).Strval,(yyvsp[(-2) - (0)]).place,(yyval).Type,1);
   (yyval).s_table = sym_table_of((yyval).s_table,"rtp",(yyvsp[(-2) - (0)]).place,new_int_type(),0);
   (yyval).Type->num = 0;
   }
    break;

  case 34:
#line 395 "small.y"
    {
	 	(yyval).Type = (yyvsp[(0) - (5)]).Type;
		(yyval).s_table = sym_table_of((yyvsp[(5) - (5)]).s_table,(yyvsp[(2) - (5)]).Strval,newplace(),new_int_type(),0);
		(yyval).Type->strname[(yyval).Type->num++] = (yyvsp[(2) - (5)]).Strval;
	 }
    break;

  case 35:
#line 400 "small.y"
    {
	 	(yyval).Type = (yyvsp[(0) - (2)]).Type;
		(yyval).s_table = sym_table_of((yyvsp[(0) - (2)]).s_table,(yyvsp[(2) - (2)]).Strval,newplace(),new_int_type(),0);
		(yyval).Type->strname[(yyval).Type->num++] = (yyvsp[(2) - (2)]).Strval;
	 }
    break;

  case 36:
#line 405 "small.y"
    {
	 (yyval).Type = (yyvsp[(0) - (0)]).Type;
	 (yyval).s_table = (yyvsp[(0) - (0)]).s_table;
	 }
    break;

  case 37:
#line 411 "small.y"
    {
   (yyval).Type = (yyvsp[(-3) - (0)]).Type;
   (yyval).s_table = (yyvsp[(-3) - (0)]).s_table;
   }
    break;

  case 38:
#line 417 "small.y"
    {
		 (yyval).s_table = (yyvsp[(3) - (6)]).s_table;
		 (yyval).next_list = (yyvsp[(5) - (6)]).next_list;
		 (yyval).break_list = (yyvsp[(5) - (6)]).break_list;
		 (yyval).continue_list = (yyvsp[(5) - (6)]).continue_list;
		 }
    break;

  case 39:
#line 425 "small.y"
    {
   (yyval).Type = new_unit_type();
   (yyval).s_table = nosame_sym_table_of((yyvsp[(-1) - (0)]).s_table);
   }
    break;

  case 40:
#line 430 "small.y"
    {
   (yyval).Type = new_unit_type();
   (yyval).s_table = (yyvsp[(0) - (0)]).s_table;
   (yyval).quad = next_quad;
   }
    break;

  case 41:
#line 437 "small.y"
    {
	 (yyval).s_table = (yyvsp[(0) - (3)]).s_table;
	 backpatch((yyvsp[(1) - (3)]).next_list,(yyvsp[(2) - (3)]).quad);
	 (yyval).next_list = (yyvsp[(3) - (3)]).next_list;
	 (yyval).break_list = merge_list((yyvsp[(1) - (3)]).break_list,(yyvsp[(3) - (3)]).break_list);
	 (yyval).continue_list = merge_list((yyvsp[(1) - (3)]).continue_list,(yyvsp[(3) - (3)]).continue_list);
	 }
    break;

  case 42:
#line 444 "small.y"
    {
	 (yyval).s_table = (yyvsp[(0) - (0)]).s_table;
	 }
    break;

  case 43:
#line 449 "small.y"
    {
   (yyval).quad = next_quad;
   (yyval).s_table = (yyvsp[(-1) - (0)]).s_table;
   }
    break;

  case 44:
#line 455 "small.y"
    {
	/*do nothing*/
		(yyval).Type = new_unit_type();
		(yyval).next_list = 0;
		(yyval).break_list = 0;
		(yyval).continue_list = 0;
	}
    break;

  case 45:
#line 462 "small.y"
    {
		(yyval).next_list = (yyvsp[(1) - (1)]).next_list;
		(yyval).Type = new_unit_type();
		(yyval).break_list = (yyvsp[(1) - (1)]).break_list;
		(yyval).continue_list = (yyvsp[(1) - (1)]).continue_list;
	}
    break;

  case 46:
#line 468 "small.y"
    {
		sprintf(genbuf,"move %s,%s",get_place(lookup((yyvsp[(0) - (4)]).s_table,"rtp")->pos),get_place((yyvsp[(3) - (4)]).place));
		gen(genbuf);
		sprintf(genbuf,"return ");
		gen(genbuf);
		(yyval).Type = new_unit_type();
		(yyval).break_list = 0;
		(yyval).continue_list = 0;
	}
    break;

  case 47:
#line 477 "small.y"
    {
		backpatch((yyvsp[(4) - (11)]).true_list,(yyvsp[(6) - (11)]).quad);
		backpatch((yyvsp[(4) - (11)]).false_list,(yyvsp[(10) - (11)]).quad);
		(yyval).next_list = merge_list(merge_list((yyvsp[(7) - (11)]).next_list,(yyvsp[(8) - (11)]).next_list),(yyvsp[(11) - (11)]).next_list);
		(yyval).Type = new_unit_type();
		(yyval).break_list = merge_list((yyvsp[(11) - (11)]).break_list,(yyvsp[(7) - (11)]).next_list);
		(yyval).continue_list = merge_list((yyvsp[(11) - (11)]).continue_list,(yyvsp[(7) - (11)]).continue_list);
	}
    break;

  case 48:
#line 485 "small.y"
    {
		backpatch((yyvsp[(4) - (7)]).true_list,(yyvsp[(6) - (7)]).quad);
		(yyval).next_list = merge_list((yyvsp[(4) - (7)]).false_list,(yyvsp[(7) - (7)]).next_list);
		(yyval).break_list = (yyvsp[(7) - (7)]).break_list;
		(yyval).continue_list = (yyvsp[(7) - (7)]).continue_list;
		(yyval).Type = new_unit_type();
	}
    break;

  case 49:
#line 492 "small.y"
    {
		backpatch((yyvsp[(7) - (14)]).true_list,(yyvsp[(13) - (14)]).quad);
		backpatch((yyvsp[(14) - (14)]).next_list,(yyvsp[(9) - (14)]).quad);
		backpatch((yyvsp[(10) - (14)]).next_list,(yyvsp[(6) - (14)]).quad);
		backpatch((yyvsp[(14) - (14)]).continue_list,(yyvsp[(9) - (14)]).quad);
		(yyval).next_list = merge_list((yyvsp[(14) - (14)]).break_list,(yyvsp[(7) - (14)]).false_list);
		(yyval).break_list = 0;
		(yyval).continue_list = 0;
		/*TODO:NOT SURE*/
		sprintf(genbuf,"goto %s",get_label((yyvsp[(9) - (14)]).quad));
		gen(genbuf);
		(yyval).Type = new_unit_type();
	}
    break;

  case 50:
#line 505 "small.y"
    {
	(yyval).Type = new_unit_type();
	(yyval).break_list = 0;
	(yyval).continue_list = make_list(next_quad);
	sprintf(genbuf,"goto ");
	gen(genbuf);
	}
    break;

  case 51:
#line 512 "small.y"
    {
	(yyval).Type = new_unit_type();
	(yyval).break_list = make_list(next_quad);
	(yyval).continue_list = 0;
	sprintf(genbuf,"goto ");
	gen(genbuf);
	}
    break;

  case 52:
#line 521 "small.y"
    {
 (yyval).next_list = make_list(next_quad);
 gen("goto ");
 }
    break;

  case 53:
#line 526 "small.y"
    {
   (yyvsp[(0) - (0)]).next_list = make_list(next_quad);
   gen("goto ");
   }
    break;

  case 54:
#line 530 "small.y"
    {
   (yyval).quad = next_quad;
   (yyval).s_table = (yyvsp[(-5) - (0)]).s_table;
   }
    break;

  case 55:
#line 534 "small.y"
    {
   (yyval).quad = next_quad;
   (yyval).s_table = (yyvsp[(-9) - (0)]).s_table;
   }
    break;

  case 56:
#line 539 "small.y"
    {
	(yyval).s_table = (yyvsp[(6) - (6)]).s_table;
	}
    break;

  case 57:
#line 542 "small.y"
    {
	(yyval).s_table = (yyvsp[(6) - (6)]).s_table;
	}
    break;

  case 58:
#line 545 "small.y"
    {
	(yyval).s_table = (yyvsp[(0) - (0)]).s_table;
	}
    break;

  case 59:
#line 550 "small.y"
    {
   (yyval).Type = new_unit_type();
   (yyval).s_table = (yyvsp[(-1) - (0)]).s_table;
   }
    break;

  case 60:
#line 555 "small.y"
    {
  (yyval).s_table = (yyvsp[(0) - (0)]).s_table;
  (yyval).Type = (yyvsp[(0) - (0)]).Type;
  (yyval).sdecs_dest = 1;
   }
    break;

  case 61:
#line 561 "small.y"
    {
	 /* TODO:may need check
	 The element is alredy added to the struct type
	 do nothing*/
	 }
    break;

  case 63:
#line 569 "small.y"
    {
   (yyval).Type = (yyvsp[(-1) - (0)]).Type;
   (yyval).s_table = (yyvsp[(0) - (0)]).s_table;
   (yyval).sdecs_dest = 0;
   }
    break;

  case 64:
#line 575 "small.y"
    {
   (yyval).Type = (yyvsp[(-4) - (0)]).Type;
   (yyval).s_table = (yyvsp[(-1) - (0)]).s_table;
   }
    break;

  case 65:
#line 580 "small.y"
    {
	 	(yyval).Type = (yyvsp[(0) - (4)]).Type;
		(yyval).sdecs_dest = (yyvsp[(0) - (4)]).sdecs_dest; 
		(yyval).s_table = (yyvsp[(0) - (4)]).s_table; 
		if (!(yyval).sdecs_dest) 
			TYPE_add_struct_ele((yyval).Type,(yyvsp[(1) - (4)]).Strval); 
		else{
			int tp = newplace();
			(yyval).s_table = (yyvsp[(0) - (4)]).s_table;
			sprintf(genbuf,"defarr %s %d",get_place(tp),get_size((yyvsp[(0) - (4)]).Type));
			gen(genbuf);
			(yyval).s_table = sym_table_of((yyval).s_table,(yyvsp[(1) - (4)]).Strval,tp,(yyvsp[(0) - (4)]).Type,0);
			}
	 }
    break;

  case 66:
#line 594 "small.y"
    {
	 	(yyval).Type = (yyvsp[(0) - (1)]).Type;
		(yyval).sdecs_dest = (yyvsp[(0) - (1)]).sdecs_dest;
		if (!(yyval).sdecs_dest)
			TYPE_add_struct_ele((yyval).Type,(yyvsp[(1) - (1)]).Strval);
		else{
			int tp = newplace();
			(yyval).s_table = (yyvsp[(0) - (1)]).s_table;
			sprintf(genbuf,"defarr %s %d",get_place(tp),get_size((yyvsp[(0) - (1)]).Type));
			gen(genbuf);
			(yyval).s_table = sym_table_of((yyval).s_table,(yyvsp[(1) - (1)]).Strval,tp,(yyvsp[(0) - (1)]).Type,0);
			}
	 }
    break;

  case 67:
#line 609 "small.y"
    {
   (yyval).Type = (yyvsp[(-2) - (0)]).Type;
	(yyval).sdecs_dest = (yyvsp[(-2) - (0)]).sdecs_dest;
	(yyval).s_table = (yyvsp[(-2) - (0)]).s_table;
   }
    break;

  case 68:
#line 615 "small.y"
    {
	(yyval).Type = (yyvsp[(0) - (1)]).Type;
	(yyval).s_table = (yyvsp[(1) - (1)]).s_table;
	}
    break;

  case 69:
#line 619 "small.y"
    {
	(yyval).Type = (yyvsp[(0) - (4)]).Type;
	(yyval).s_table = (yyvsp[(4) - (4)]).s_table;
	}
    break;

  case 70:
#line 623 "small.y"
    {
	   		(yyval).s_table=(yyvsp[(7) - (7)]).s_table;
			(yyval).Type = (yyvsp[(1) - (7)]).Type;
	   }
    break;

  case 71:
#line 627 "small.y"
    {
		(yyval).s_table=(yyvsp[(1) - (4)]).s_table;
		(yyval).Type = (yyvsp[(1) - (4)]).Type;
	}
    break;

  case 72:
#line 634 "small.y"
    {
   (yyval).s_table = (yyvsp[(-1) - (0)]).s_table;
   (yyval).Type = (yyvsp[(-2) - (0)]).Type;
   }
    break;

  case 73:
#line 640 "small.y"
    {
   (yyval).s_table = (yyvsp[(-4) - (0)]).s_table;
   (yyval).Type = (yyvsp[(-5) - (0)]).Type;
   }
    break;

  case 74:
#line 652 "small.y"
    {
   /* TODO:*/
   int tp = newplace();
   sprintf(genbuf,"def %s %d",get_place(tp),get_size((yyvsp[(0) - (1)]).Type));
   gen(genbuf);
   (yyval).s_table = sym_table_of((yyvsp[(0) - (1)]).s_table,(yyvsp[(1) - (1)]).Strval,tp,(yyvsp[(0) - (1)]).Type,0);
   (yyval).Type=(yyvsp[(0) - (1)]).Type;
   (yyval).Strval = (yyvsp[(1) - (1)]).Strval;
   }
    break;

  case 75:
#line 661 "small.y"
    {
   (yyval).s_table = (yyvsp[(1) - (2)]).s_table;
   (yyval).Type->p = (yyvsp[(0) - (2)]).Type;
   (yyval).Type = (yyvsp[(1) - (2)]).Type;
   TYPE_calsize((yyval).Type);
   int tp = newplace();
	(yyval).s_table = sym_table_of((yyvsp[(0) - (2)]).s_table,(yyvsp[(1) - (2)]).Strval,tp,(yyval).Type,0);
   sprintf(genbuf,"defarr %s %d",get_place(tp),get_size((yyval).Type));
   gen(genbuf);
   }
    break;

  case 76:
#line 674 "small.y"
    {
		(yyval).Type = new_array_type();
		(yyval).Type->num = (yyvsp[(4) - (4)]).Intval;
		(yyval).Strval = (yyvsp[(1) - (4)]).Strval;
		(yyvsp[(1) - (4)]).Type->p = (yyval).Type;
		(yyval).ndim = (yyvsp[(1) - (4)]).ndim+1;
		if ((yyval).ndim > 2)
			ErrorMsg("Error: Array dimension > 2!\n");
	}
    break;

  case 77:
#line 683 "small.y"
    {
		(yyval).Type = new_array_type();
		(yyval).Strval = (yyvsp[(1) - (3)]).Strval;
		(yyval).ndim = 1;
		(yyval).Type->num = (yyvsp[(3) - (3)]).Intval;
		/*
		$$.s_table = sym_table_of($0.s_table,$1.Strval,newplace(),$$.Type,0);
		*/
		(yyval).s_table = (yyvsp[(0) - (3)]).s_table;
	}
    break;

  case 78:
#line 696 "small.y"
    {
	(yyval).s_table=(yyvsp[(0) - (1)]).s_table;
	(yyval).Type = (yyvsp[(0) - (1)]).Type;
	symbol_table *ts = lookup((yyval).s_table,(yyvsp[(0) - (1)]).Strval);
	if (!TYPE_Same((yyvsp[(1) - (1)]).Type,(yyval).Type))
		ErrorMsg("Error: Initial Type not matched\n");
	if ((yyvsp[(1) - (1)]).Type->t == 0){
		sprintf(genbuf,"move %s,%s",get_place(ts->pos),get_place((yyvsp[(1) - (1)]).place));
		gen(genbuf);
		}
	else
		ErrorMsg("Error: Initial Type not matched\n");
	}
    break;

  case 79:
#line 709 "small.y"
    {
		(yyval).s_table = (yyvsp[(0) - (4)]).s_table;
		(yyval).Type = (yyvsp[(0) - (4)]).Type;
		symbol_table *s = lookup((yyval).s_table,(yyvsp[(0) - (4)]).Strval);
		if ((yyvsp[(3) - (4)]).num+1 < s->type->num){
			char buf[100];
			sprintf(buf,"Error: Initial Type not matched,too few initials,target %d,deed %d\n",s->type->num,(yyvsp[(3) - (4)]).num);
			ErrorMsg(buf);
			}
	}
    break;

  case 80:
#line 721 "small.y"
    {
  (yyval).s_table = (yyvsp[(-1) - (0)]).s_table;
  (yyval).Type = (yyvsp[(-1) - (0)]).Type;
  (yyval).Strval = (yyvsp[(-1) - (0)]).Strval;
  (yyval).num = -1;
  }
    break;

  case 81:
#line 728 "small.y"
    {
	(yyval).num = (yyvsp[(4) - (4)]).num-1;
	(yyval).s_table = (yyvsp[(3) - (4)]).s_table;
	(yyval).Type = (yyvsp[(3) - (4)]).Type;
	(yyval).num = (yyvsp[(4) - (4)]).num;
	}
    break;

  case 82:
#line 735 "small.y"
    {
	(yyval).Type = (yyvsp[(0) - (1)]).Type;
	(yyval).num = (yyvsp[(0) - (1)]).num+1;
	(yyval).s_table = (yyvsp[(0) - (1)]).s_table;
	(yyval).Strval = (yyvsp[(0) - (1)]).Strval;
	if ((yyvsp[(1) - (1)]).Type->t != 0)
		ErrorMsg("Error: Initial Type not matched\n");
	symbol_table *s = lookup((yyval).s_table,(yyval).Strval);
	if ((yyval).num > s->type->num)
		ErrorMsg("Error: Initial Type not matched,too many initials\n");
		/*
	sprintf(genbuf,"move %s[%d],,%s",get_place(s->pos),$$.num,get_place($1.place));
	*/
	int tp = newplace();
	sprintf(genbuf,"li %s %d",get_place(tp),(yyval).num);
	gen(genbuf);
	sprintf(genbuf,"sw %s,%s(%s)",get_place((yyvsp[(1) - (1)]).place),get_place(tp),get_place(s->pos));
	gen(genbuf);
	}
    break;

  case 83:
#line 756 "small.y"
    {
  (yyval).s_table = (yyvsp[(-2) - (0)]).s_table;
  (yyval).Type = (yyvsp[(-2) - (0)]).Type;
  (yyval).Strval = (yyvsp[(-2) - (0)]).Strval;
  (yyval).num = (yyvsp[(-2) - (0)]).num+1;
  if ((yyvsp[(-1) - (0)]).Type->t != 0)
		ErrorMsg("Error: Initial Type not matched,not int\n");
	symbol_table *s = lookup((yyval).s_table,(yyval).Strval);
	if ((yyval).num >= s->type->num)
		ErrorMsg("Error: Initial Type not matched,too many initials\n");
		/*
	sprintf(genbuf,"move %s[%d],,%s",get_place(s->pos),$$.num,get_place($-1.place));
	*/
	int tp = newplace();
	sprintf(genbuf,"li %s %d",get_place(tp),(yyval).num);
	gen(genbuf);
	sprintf(genbuf,"sw %s,%s(%s)",get_place((yyvsp[(-1) - (0)]).place),get_place(tp),get_place(s->pos));
	gen(genbuf);
  }
    break;

  case 84:
#line 776 "small.y"
    {
   (yyval).s_table = (yyvsp[(0) - (1)]).s_table;
   (yyval).Type = (yyvsp[(1) - (1)]).Type;
   (yyval).true_list = (yyvsp[(1) - (1)]).true_list;
   (yyval).false_list = (yyvsp[(1) - (1)]).false_list;
   (yyval).next_list = (yyvsp[(1) - (1)]).next_list;
   }
    break;

  case 85:
#line 783 "small.y"
    {
   (yyval).s_table = (yyvsp[(0) - (0)]).s_table;
   (yyval).Type = new_unit_type();
   }
    break;

  case 86:
#line 789 "small.y"
    {
   (yyval).s_table = (yyvsp[(-1) - (0)]).s_table;
   }
    break;

  case 87:
#line 792 "small.y"
    {
   (yyval).s_table = (yyvsp[(-2) - (0)]).s_table;
   }
    break;

  case 88:
#line 795 "small.y"
    {
   (yyval).s_table = (yyvsp[(-3) - (0)]).s_table;
   }
    break;

  case 89:
#line 799 "small.y"
    {
		(yyval).place = newplace();
		sprintf(genbuf,"neg %s,%s",get_place((yyval).place),get_place((yyvsp[(3) - (3)]).place));
		gen(genbuf);
	}
    break;

  case 90:
#line 804 "small.y"
    {
		(yyval).place = newplace();
		char buf[5];
		if (!strcmp((yyvsp[(2) - (4)]).Strval,"*"))
			strcpy(buf,"mul");
		if (!strcmp((yyvsp[(2) - (4)]).Strval,"%"))
			strcpy(buf,"rem");
		if (!strcmp((yyvsp[(2) - (4)]).Strval,"/"))
			strcpy(buf,"div");
		if ((yyvsp[(1) - (4)]).Type->t != 0 || (yyvsp[(4) - (4)]).Type->t != 0)
			ErrorMsg("product operands not int\n");
		(yyval).Type = new_int_type();;
		sprintf(genbuf,"%s %s,%s,%s",buf,get_place((yyval).place),get_place((yyvsp[(1) - (4)]).place),get_place((yyvsp[(4) - (4)]).place));
		gen(genbuf);
	}
    break;

  case 91:
#line 819 "small.y"
    {
		(yyval).place = newplace();
		if ((yyvsp[(1) - (4)]).Type->t != 0 || (yyvsp[(4) - (4)]).Type->t != 0)
			ErrorMsg("add operands not int\n");
		(yyval).Type = new_int_type();;
		sprintf(genbuf,"%s %s,%s,%s","add",get_place((yyval).place),get_place((yyvsp[(1) - (4)]).place),get_place((yyvsp[(4) - (4)]).place));
		gen(genbuf);
	}
    break;

  case 92:
#line 827 "small.y"
    {
		(yyval).place = newplace();
		if ((yyvsp[(1) - (4)]).Type->t != 0 || (yyvsp[(4) - (4)]).Type->t != 0)
			ErrorMsg("sub operands not int\n");
		(yyval).Type = new_int_type();;
		sprintf(genbuf,"%s %s,%s,%s","sub",get_place((yyval).place),get_place((yyvsp[(1) - (4)]).place),get_place((yyvsp[(4) - (4)]).place));
		gen(genbuf);
	}
    break;

  case 93:
#line 835 "small.y"
    {
		(yyval).place = newplace();
		if ((yyvsp[(1) - (4)]).Type->t != 0 || (yyvsp[(4) - (4)]).Type->t != 0)
			ErrorMsg("shift operands not int\n");
		(yyval).Type = new_int_type();;
		if ((yyvsp[(2) - (4)]).Strval[0] == '<')
			sprintf(genbuf,"%s %s,%s,%s","sll",get_place((yyval).place),get_place((yyvsp[(1) - (4)]).place),get_place((yyvsp[(4) - (4)]).place));
		else                                                      
			sprintf(genbuf,"%s %s,%s,%s","srl",get_place((yyval).place),get_place((yyvsp[(1) - (4)]).place),get_place((yyvsp[(4) - (4)]).place));
		gen(genbuf);
	}
    break;

  case 94:
#line 846 "small.y"
    {
		(yyval).place = newplace();
		char buf[5];
		if (!strcmp((yyvsp[(2) - (4)]).Strval,">"))
			strcpy(buf,"bgt");
		if (!strcmp((yyvsp[(2) - (4)]).Strval,">="))
			strcpy(buf,"bge");
		if (!strcmp((yyvsp[(2) - (4)]).Strval,"<"))
			strcpy(buf,"blt");
		if (!strcmp((yyvsp[(2) - (4)]).Strval,"<="))
			strcpy(buf,"ble");
		if (!strcmp((yyvsp[(2) - (4)]).Strval,"=="))
			strcpy(buf,"beq");
		if (!strcmp((yyvsp[(2) - (4)]).Strval,"!="))
			strcpy(buf,"bne");
		(yyval).true_list = make_list(next_quad);
		(yyval).false_list = make_list(next_quad+1);
		if ((yyvsp[(1) - (4)]).Type->t != 0 || (yyvsp[(4) - (4)]).Type->t != 0)
			ErrorMsg("shift operands not int\n");
		(yyval).Type = new_int_type();;
		sprintf(genbuf,"%s %s %s goto ",buf,get_place((yyvsp[(1) - (4)]).place),get_place((yyvsp[(4) - (4)]).place));
		gen(genbuf);
		sprintf(genbuf,"goto ");
		gen(genbuf);
	}
    break;

  case 95:
#line 871 "small.y"
    {
		(yyval).place = newplace();
		sprintf(genbuf,"%s %s,%s,%s","and",get_place((yyval).place),get_place((yyvsp[(1) - (4)]).place),get_place((yyvsp[(4) - (4)]).place));
		gen(genbuf);                                          
	}
    break;

  case 96:
#line 876 "small.y"
    {                                      
		(yyval).place = newplace();                                
		if ((yyvsp[(1) - (4)]).Type->t != 0 || (yyvsp[(4) - (4)]).Type->t != 0)
			ErrorMsg("shift operands not int\n");
		(yyval).Type = new_int_type();
		sprintf(genbuf,"%s %s,%s,%s","xor",get_place((yyval).place),get_place((yyvsp[(1) - (4)]).place),get_place((yyvsp[(4) - (4)]).place));
		gen(genbuf);
	}
    break;

  case 97:
#line 884 "small.y"
    {
		(yyval).place = newplace();
		if ((yyvsp[(1) - (4)]).Type->t != 0 || (yyvsp[(4) - (4)]).Type->t != 0)
			ErrorMsg("shift operands not int\n");
		(yyval).Type = new_int_type();;
		sprintf(genbuf,"%s %s,%s,%s","or",get_place((yyval).place),get_place((yyvsp[(1) - (4)]).place),get_place((yyvsp[(4) - (4)]).place));
		gen(genbuf);
	}
    break;

  case 98:
#line 892 "small.y"
    {
		backpatch((yyvsp[(1) - (4)]).true_list,(yyvsp[(3) - (4)]).quad);	
		(yyval).false_list = merge_list((yyvsp[(1) - (4)]).false_list,(yyvsp[(4) - (4)]).false_list);
		(yyval).true_list = (yyvsp[(4) - (4)]).true_list;
	}
    break;

  case 99:
#line 897 "small.y"
    {
		backpatch((yyvsp[(1) - (4)]).false_list,(yyvsp[(3) - (4)]).quad);
		(yyval).true_list = merge_list((yyvsp[(1) - (4)]).true_list,(yyvsp[(4) - (4)]).true_list);
		(yyval).false_list = (yyvsp[(4) - (4)]).false_list;
	}
    break;

  case 100:
#line 902 "small.y"
    {
		(yyval).place = newplace();
		if ((yyvsp[(1) - (4)]).offset == -1){
			sprintf(genbuf,"move %s,%s",get_place((yyvsp[(1) - (4)]).place),get_place((yyvsp[(4) - (4)]).place));
			gen(genbuf);
		}else{
		/*
			sprintf(genbuf,"move %s[%d],%s",get_place($1.place),$1.offset,get_place($4.place));
			*/
			int tp = newplace();
			sprintf(genbuf,"sw %s,%s(%s)",get_place((yyvsp[(4) - (4)]).place),get_place((yyvsp[(1) - (4)]).offset),get_place((yyvsp[(1) - (4)]).place));
			gen(genbuf);
		}
		sprintf(genbuf,"move %s,%s",get_place((yyval).place),get_place((yyvsp[(4) - (4)]).place));
		gen(genbuf);
	}
    break;

  case 101:
#line 918 "small.y"
    {
		(yyval).place = newplace();
		char buf[5];
		if (!strcmp((yyvsp[(1) - (3)]).Strval,"!")){
			(yyval).true_list = (yyvsp[(3) - (3)]).false_list;
			(yyval).false_list = (yyvsp[(3) - (3)]).true_list;
		}
		if (!strcmp((yyvsp[(1) - (3)]).Strval,"++")){
			sprintf(genbuf,"add %s,%s,1",get_place((yyvsp[(3) - (3)]).place),get_place((yyvsp[(3) - (3)]).place));
			gen(genbuf);
			sprintf(genbuf,"move %s,%s",get_place((yyval).place),get_place((yyvsp[(3) - (3)]).place));
			gen(genbuf);
		}
		if (!strcmp((yyvsp[(1) - (3)]).Strval,"--")){
			sprintf(genbuf,"sub %s,%s,1",get_place((yyvsp[(3) - (3)]).place),get_place((yyvsp[(3) - (3)]).place));
			gen(genbuf);
			sprintf(genbuf,"move %s,%s",get_place((yyval).place),get_place((yyvsp[(3) - (3)]).place));
			gen(genbuf);
		}
		if (!strcmp((yyvsp[(1) - (3)]).Strval,"~")){
			sprintf(genbuf,"not %s,%s",get_place((yyval).place),get_place((yyvsp[(3) - (3)]).place));
			gen(genbuf);
		}
	}
    break;

  case 102:
#line 942 "small.y"
    {
		(yyval).place = (yyvsp[(3) - (4)]).place;
		(yyval).true_list = (yyvsp[(3) - (4)]).true_list;
		(yyval).false_list = (yyvsp[(3) - (4)]).false_list;
	}
    break;

  case 103:
#line 947 "small.y"
    {
		int rtpos;
		int i;
		if (!strcmp((yyvsp[(1) - (5)]).Strval,"read") || !strcmp((yyvsp[(1) - (5)]).Strval,"write")){
			sprintf(genbuf,"%s %s",(yyvsp[(1) - (5)]).Strval,get_place((yyvsp[(3) - (5)]).queue->a[0]));		
			gen(genbuf);
		}else{
		(yyval).place = newplace();
		for (i = 0;i < (yyvsp[(3) - (5)]).queue->num;i++){
			sprintf(genbuf,"param %s",get_place((yyvsp[(3) - (5)]).queue->a[i]));
			gen(genbuf);
		}
		sprintf(genbuf,"call %s",(yyvsp[(1) - (5)]).Strval);
		gen(genbuf);
		}
		if (strcmp((yyvsp[(1) - (5)]).Strval,"read") && strcmp((yyvsp[(1) - (5)]).Strval,"write")){
			symbol_table *s = lookup((yyvsp[(0) - (5)]).s_table,(yyvsp[(1) - (5)]).Strval);
			rtpos = s->pos;
			sprintf(genbuf,"move %s,%s",get_place((yyval).place),get_place(rtpos));
			gen(genbuf);
		}
	}
    break;

  case 104:
#line 969 "small.y"
    {
		(yyval).false_list = (yyval).true_list;
		if ((yyvsp[(1) - (1)]).offset == -1)
			(yyval).place = (yyvsp[(1) - (1)]).place;
		else{
			(yyval).place = newplace();
			/*
			sprintf(genbuf,"move %s,%s[%d]",get_place($$.place),get_place($1.place),$1.offset);
			*/
			sprintf(genbuf,"lw %s,%s(%s)",get_place((yyval).place),get_place((yyvsp[(1) - (1)]).offset),get_place((yyvsp[(1) - (1)]).place));
			gen(genbuf);
			}
	}
    break;

  case 105:
#line 982 "small.y"
    {
		(yyval).place = newplace();
		sprintf(genbuf,"add %s,$0,%d",get_place((yyval).place),(yyvsp[(1) - (1)]).Intval);
		gen(genbuf);
	}
    break;

  case 106:
#line 989 "small.y"
    {
   (yyval).offset = -1;
   (yyval).s_table = (yyvsp[(0) - (1)]).s_table;
   symbol_table *s = lookup((yyval).s_table,(yyvsp[(1) - (1)]).Strval);
   (yyval).place = s->pos;
   (yyval).Type = s->type;	
   }
    break;

  case 107:
#line 996 "small.y"
    {
	(yyval).place = newplace();
	(yyval).offset = newplace();
	int xplace;
	sprintf(genbuf,"move %s,%s",get_place((yyval).place),get_place((yyvsp[(1) - (2)]).array_place));
	gen(genbuf);
	sprintf(genbuf,"mul %s,%s,%s",get_place((yyval).offset),get_place((yyvsp[(1) - (2)]).place),"1");
	gen(genbuf);
	(yyval).s_table = (yyvsp[(0) - (2)]).s_table;
	(yyval).Type = (yyvsp[(1) - (2)]).array->p;
	if ((yyval).Type->t == 1)
		ErrorMsg("Array element type error!\n");
   }
    break;

  case 108:
#line 1009 "small.y"
    {
	    int xx;
		(yyval).s_table = (yyvsp[(0) - (3)]).s_table;
		symbol_table *s = lookup((yyval).s_table,(yyvsp[(1) - (3)]).Strval);
		if (s->type->t != 2){
			sprintf(genbuf,"%s not struct type!\n",(yyvsp[(1) - (3)]).Strval);	
			ErrorMsg(genbuf);
		}
		(yyval).place = s->pos;
		xx = newplace();
		(yyval).offset = find_struct_index(s->type,(yyvsp[(3) - (3)]).Strval);
		sprintf(genbuf,"li %s %d",get_place(xx),(yyval).offset);
		gen(genbuf);
		(yyval).offset = xx;
	}
    break;

  case 109:
#line 1024 "small.y"
    {
	(yyval).quad = next_quad;
    (yyval).s_table = (yyvsp[(0) - (0)]).s_table;
}
    break;

  case 110:
#line 1029 "small.y"
    {
   (yyval).queue = init_queue();
   (yyval).s_table = (yyvsp[(-2) - (0)]).s_table;
   }
    break;

  case 111:
#line 1041 "small.y"
    {
	 	(yyval).array = (yyvsp[(1) - (5)]).array->p;
		(yyval).array_place = (yyvsp[(1) - (5)]).array_place;
		if ((yyval).array->t != 1)
			ErrorMsg("Error: index to non-array type!\n");
		(yyval).ndim = (yyvsp[(1) - (5)]).ndim + 1;
		(yyval).place = newplace();
		sprintf(genbuf,"mul %s,%s,%d",get_place((yyval).place),get_place((yyvsp[(1) - (5)]).place),(yyval).array->num);
		gen(genbuf);
		sprintf(genbuf,"add %s,%s,%s",get_place((yyval).place),get_place((yyval).place),get_place((yyvsp[(5) - (5)]).place));
		gen(genbuf);
	 }
    break;

  case 112:
#line 1053 "small.y"
    {
	 	(yyval).s_table = (yyvsp[(0) - (4)]).s_table;
		symbol_table *s = lookup((yyval).s_table,(yyvsp[(1) - (4)]).Strval);
		if (s->type->t != 1){
			ErrorMsg("Error: index to non-array type!\n");
		}
		(yyval).array = s->type;
		(yyval).array_place = s->pos;
		(yyval).ndim = 1;
		(yyval).place = (yyvsp[(4) - (4)]).place;
	 }
    break;

  case 113:
#line 1066 "small.y"
    {
	/*do nothing*/
	}
    break;

  case 114:
#line 1069 "small.y"
    {
	/*do nothing*/
	}
    break;

  case 115:
#line 1074 "small.y"
    {
   (yyval).s_table = (yyvsp[(-3) - (0)]).s_table;
   (yyval).queue = (yyvsp[(-3) - (0)]).queue;
   }
    break;

  case 116:
#line 1078 "small.y"
    {
   append_queue((yyvsp[(-1) - (0)]).queue,(yyvsp[(0) - (0)]).place);
   }
    break;


/* Line 1267 of yacc.c.  */
#line 2982 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 1084 "small.y"

void yyerror(char *s) {
    fprintf(stdout, "[line %d]%s : ",curr_line, s);
	print_tok();
}

int main(int argc,char *argv[]) {
	FILE *ffout;
	int i;
	if (argc != 2){
		fprintf(stderr,"wrong argument number!\n");
		return 0;
	}
	freopen(argv[1],"r",stdin);
	/*yydebug = 1;*/
    yyparse();
	if (!has_main)
		ErrorMsg("Error: Don't have function main!\n");
	ffout = fopen("InterCode.txt","w");
	fprintf(ffout,"%d %d\n",next_quad,next_place);
	for (i = 0;i < next_quad;i++)
		fprintf(ffout,"%s\n",code[i]);
	fclose(ffout);
    return 0;
}

