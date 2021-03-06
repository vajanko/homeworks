
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 1



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"

	#include <stdlib.h>

    #include "du6.h"
    #include "du6lval.hpp"
    #include "duerr.h"
	#include "du6tok.h"
    #include "du6sem.h"

    /* semantic value */
    #define YYSTYPE mlc::MlaskalLval

    /* locations */
    #define YYLTYPE unsigned
    #define YYLLOC_DEFAULT(cur, rhs, n)	do { if(n) (cur)=(rhs)[1]; else (cur)=(rhs)[0]; } while(0)

    /* local stuff */
    using namespace mlc;

    /* error stuff */
    #define YYERROR_VERBOSE 1
    static void yyerror(YYLTYPE *line, mlc::MlaskalCtx *ctx, const char *m);

	/* malloc, free correct declarations */
	#define YYINCLUDED_STDLIB_H


/* Line 189 of yacc.c  */
#line 101 "du6g.cpp"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     DUTOK_PROGRAM = 258,
     DUTOK_LABEL = 259,
     DUTOK_CONST = 260,
     DUTOK_TYPE = 261,
     DUTOK_VAR = 262,
     DUTOK_BEGIN = 263,
     DUTOK_END = 264,
     DUTOK_PROCEDURE = 265,
     DUTOK_FUNCTION = 266,
     DUTOK_ARRAY = 267,
     DUTOK_OF = 268,
     DUTOK_GOTO = 269,
     DUTOK_IF = 270,
     DUTOK_THEN = 271,
     DUTOK_ELSE = 272,
     DUTOK_WHILE = 273,
     DUTOK_DO = 274,
     DUTOK_REPEAT = 275,
     DUTOK_UNTIL = 276,
     DUTOK_FOR = 277,
     DUTOK_OR = 278,
     DUTOK_NOT = 279,
     DUTOK_IDENTIFIER = 280,
     DUTOK_UINT = 281,
     DUTOK_REAL = 282,
     DUTOK_STRING = 283,
     DUTOK_SEMICOLON = 284,
     DUTOK_DOT = 285,
     DUTOK_COMMA = 286,
     DUTOK_EQ = 287,
     DUTOK_COLON = 288,
     DUTOK_LPAR = 289,
     DUTOK_RPAR = 290,
     DUTOK_DOTDOT = 291,
     DUTOK_LSBRA = 292,
     DUTOK_RSBRA = 293,
     DUTOK_ASSIGN = 294,
     DUTOK_OPER_REL = 295,
     DUTOK_OPER_SIGNADD = 296,
     DUTOK_OPER_MUL = 297,
     DUTOK_FOR_DIRECTION = 298
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 199 "du6g.cpp"

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
# if YYENABLE_NLS
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
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
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
	 || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
	     && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   245

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  47
/* YYNRULES -- Number of rules.  */
#define YYNRULES  109
/* YYNRULES -- Number of states.  */
#define YYNSTATES  222

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   298

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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     9,    16,    17,    20,    24,    28,    30,
      34,    40,    41,    45,    46,    50,    54,    56,    60,    61,
      65,    69,    71,    75,    76,    80,    84,    86,    90,    94,
      96,    98,   105,   110,   114,   116,   121,   128,   133,   142,
     143,   147,   151,   153,   158,   161,   165,   170,   179,   181,
     185,   187,   191,   193,   197,   201,   203,   206,   208,   212,
     216,   218,   220,   224,   226,   228,   230,   232,   234,   239,
     243,   246,   249,   251,   254,   258,   260,   264,   266,   268,
     275,   277,   281,   283,   285,   289,   293,   299,   300,   303,
     307,   309,   313,   317,   322,   324,   327,   329,   332,   334,
     337,   339,   340,   342,   345,   347,   350,   352,   356,   358
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      45,     0,    -1,     3,    25,    29,    46,    30,    -1,    51,
      52,    55,    58,    47,    61,    -1,    -1,    49,    29,    -1,
      82,    29,    50,    -1,    83,    29,    50,    -1,    48,    -1,
      49,    29,    48,    -1,    51,    52,    55,    58,    61,    -1,
      -1,     4,    90,    29,    -1,    -1,     5,    54,    29,    -1,
      25,    32,    87,    -1,    53,    -1,    54,    29,    53,    -1,
      -1,     6,    57,    29,    -1,    25,    32,    78,    -1,    56,
      -1,    57,    29,    56,    -1,    -1,     7,    60,    29,    -1,
      89,    33,    78,    -1,    59,    -1,    60,    29,    59,    -1,
       8,    66,     9,    -1,    63,    -1,    64,    -1,    15,    68,
      16,    63,    17,    63,    -1,    18,    68,    19,    63,    -1,
      26,    33,    65,    -1,    65,    -1,    15,    68,    16,    62,
      -1,    15,    68,    16,    63,    17,    64,    -1,    18,    68,
      19,    64,    -1,    22,    25,    39,    68,    43,    68,    19,
      64,    -1,    -1,    25,    39,    68,    -1,    74,    39,    68,
      -1,    25,    -1,    25,    34,    67,    35,    -1,    14,    26,
      -1,     8,    66,     9,    -1,    20,    66,    21,    68,    -1,
      22,    25,    39,    68,    43,    68,    19,    63,    -1,    62,
      -1,    66,    29,    62,    -1,    68,    -1,    67,    31,    68,
      -1,    69,    -1,    69,    40,    69,    -1,    69,    32,    69,
      -1,    70,    -1,    41,    70,    -1,    71,    -1,    70,    41,
      71,    -1,    70,    23,    71,    -1,    72,    -1,    73,    -1,
      72,    42,    73,    -1,    25,    -1,    26,    -1,    27,    -1,
      28,    -1,    74,    -1,    25,    34,    67,    35,    -1,    34,
      68,    35,    -1,    24,    73,    -1,    25,    75,    -1,    76,
      -1,    75,    76,    -1,    37,    77,    38,    -1,    68,    -1,
      77,    31,    68,    -1,    25,    -1,    81,    -1,    12,    37,
      79,    38,    13,    78,    -1,    80,    -1,    79,    31,    80,
      -1,    25,    -1,    81,    -1,    88,    36,    88,    -1,    10,
      25,    84,    -1,    11,    25,    84,    33,    25,    -1,    -1,
      34,    35,    -1,    34,    85,    35,    -1,    86,    -1,    85,
      29,    86,    -1,    89,    33,    25,    -1,     7,    89,    33,
      25,    -1,    25,    -1,    41,    25,    -1,    26,    -1,    41,
      26,    -1,    27,    -1,    41,    27,    -1,    28,    -1,    -1,
      25,    -1,    41,    25,    -1,    26,    -1,    41,    26,    -1,
      25,    -1,    89,    31,    25,    -1,    26,    -1,    90,    31,
      26,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    85,    85,    89,    91,    92,    94,    95,    97,    98,
     103,   105,   106,   108,   109,   111,   113,   114,   116,   117,
     119,   121,   122,   124,   125,   127,   129,   130,   132,   136,
     137,   140,   141,   142,   143,   145,   146,   147,   148,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   162,   163,
     166,   167,   172,   173,   174,   176,   177,   180,   181,   182,
     184,   187,   188,   190,   191,   192,   193,   194,   195,   196,
     197,   199,   202,   203,   205,   207,   208,   213,   214,   215,
     217,   218,   220,   221,   223,   229,   232,   235,   236,   237,
     240,   241,   244,   245,   250,   251,   252,   253,   254,   255,
     256,   258,   259,   260,   261,   262,   267,   268,   271,   272
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "DUTOK_PROGRAM", "DUTOK_LABEL",
  "DUTOK_CONST", "DUTOK_TYPE", "DUTOK_VAR", "DUTOK_BEGIN", "DUTOK_END",
  "DUTOK_PROCEDURE", "DUTOK_FUNCTION", "DUTOK_ARRAY", "DUTOK_OF",
  "DUTOK_GOTO", "DUTOK_IF", "DUTOK_THEN", "DUTOK_ELSE", "DUTOK_WHILE",
  "DUTOK_DO", "DUTOK_REPEAT", "DUTOK_UNTIL", "DUTOK_FOR", "DUTOK_OR",
  "DUTOK_NOT", "DUTOK_IDENTIFIER", "DUTOK_UINT", "DUTOK_REAL",
  "DUTOK_STRING", "DUTOK_SEMICOLON", "DUTOK_DOT", "DUTOK_COMMA",
  "DUTOK_EQ", "DUTOK_COLON", "DUTOK_LPAR", "DUTOK_RPAR", "DUTOK_DOTDOT",
  "DUTOK_LSBRA", "DUTOK_RSBRA", "DUTOK_ASSIGN", "DUTOK_OPER_REL",
  "DUTOK_OPER_SIGNADD", "DUTOK_OPER_MUL", "DUTOK_FOR_DIRECTION", "$accept",
  "mlaskal", "program_block", "block_proc_func", "proc_func_def",
  "proc_func_defs", "block", "block_label", "block_const", "id_assign",
  "id_assigns", "block_type", "type_assign", "type_assigns", "block_var",
  "var_def", "var_defs", "block_begin_end", "stmt", "m_stmt", "u_stmt",
  "stmt_rest", "stmts", "real_params", "expr", "simple_expr", "terms",
  "term", "factors", "factor", "array_var", "idxs", "idx", "exprs", "type",
  "ord_types", "ord_type", "range", "proc", "func", "params",
  "formal_params", "params_section", "const", "ord_const", "identifiers",
  "uints", 0
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    44,    45,    46,    47,    47,    48,    48,    49,    49,
      50,    51,    51,    52,    52,    53,    54,    54,    55,    55,
      56,    57,    57,    58,    58,    59,    60,    60,    61,    62,
      62,    63,    63,    63,    63,    64,    64,    64,    64,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    66,    66,
      67,    67,    68,    68,    68,    69,    69,    70,    70,    70,
      71,    72,    72,    73,    73,    73,    73,    73,    73,    73,
      73,    74,    75,    75,    76,    77,    77,    78,    78,    78,
      79,    79,    80,    80,    81,    82,    83,    84,    84,    84,
      85,    85,    86,    86,    87,    87,    87,    87,    87,    87,
      87,    88,    88,    88,    88,    88,    89,    89,    90,    90
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     5,     6,     0,     2,     3,     3,     1,     3,
       5,     0,     3,     0,     3,     3,     1,     3,     0,     3,
       3,     1,     3,     0,     3,     3,     1,     3,     3,     1,
       1,     6,     4,     3,     1,     4,     6,     4,     8,     0,
       3,     3,     1,     4,     2,     3,     4,     8,     1,     3,
       1,     3,     1,     3,     3,     1,     2,     1,     3,     3,
       1,     1,     3,     1,     1,     1,     1,     1,     4,     3,
       2,     2,     1,     2,     3,     1,     3,     1,     1,     6,
       1,     3,     1,     1,     3,     3,     5,     0,     2,     3,
       1,     3,     3,     4,     1,     2,     1,     2,     1,     2,
       1,     0,     1,     2,     1,     2,     1,     3,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,     0,     0,     1,    11,     0,     0,    13,   108,
       0,     2,     0,    18,    12,     0,     0,    16,     0,     0,
      23,   109,     0,    14,     0,    21,     0,     0,     4,    94,
      96,    98,   100,     0,    15,    17,   101,    19,   106,    26,
       0,     0,     0,     0,     0,     8,     0,     0,     0,    95,
      97,    99,     0,    77,   104,     0,    20,    78,     0,    22,
      24,     0,   101,    87,    87,    39,     3,     5,    11,    11,
     101,   103,   105,   101,    27,   107,    25,     0,    85,     0,
      39,     0,     0,     0,    39,     0,    42,     0,    48,    29,
      30,    34,     0,     0,     9,     6,    13,     7,    82,     0,
      80,    83,   102,    84,     0,    88,     0,    90,     0,     0,
       0,    44,     0,    63,    64,    65,    66,     0,     0,     0,
      52,    55,    57,    60,    61,    67,     0,     0,     0,     0,
       0,     0,    71,    72,    39,    28,    39,     0,    18,   101,
       0,     0,     0,    89,     0,    86,    45,    70,     0,     0,
      56,    39,     0,     0,     0,     0,     0,    39,     0,     0,
       0,    50,    75,     0,    40,    73,     0,    33,    49,    41,
      23,    81,   101,     0,    91,    92,     0,    69,    35,    29,
      54,    53,    59,    58,    62,    32,    37,    46,     0,     0,
      43,     0,    74,     0,     0,    79,    93,    68,    39,     0,
      51,    76,     0,    10,    31,    36,     0,     0,    39,     0,
      47,    38,     0,    39,     0,     0,     0,     0,    39,    39,
       0,    39
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     7,    44,    45,    46,    95,    96,    13,    17,
      18,    20,    25,    26,    28,    39,    40,    66,    88,    89,
      90,    91,    92,   160,   161,   120,   121,   122,   123,   124,
     125,   132,   133,   163,    56,    99,   100,    57,    47,    48,
      78,   106,   107,    34,    58,    41,    10
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -151
static const yytype_int16 yypact[] =
{
      13,    15,    56,    30,  -151,    64,    59,    46,    77,  -151,
      70,  -151,    68,    92,  -151,   101,   100,  -151,   118,   127,
     148,  -151,    90,    68,   130,  -151,   129,   140,   102,  -151,
    -151,  -151,  -151,   124,  -151,  -151,     0,   127,  -151,  -151,
     139,    -2,   144,   145,   163,  -151,   143,   146,   147,  -151,
    -151,  -151,   136,   138,  -151,   135,  -151,  -151,   141,  -151,
     140,   153,     0,   149,   149,    69,  -151,   102,    64,    64,
      39,  -151,  -151,    80,  -151,  -151,  -151,    -1,  -151,   151,
      69,   154,    19,    19,    69,   156,    86,   152,  -151,  -151,
    -151,  -151,     4,   150,  -151,  -151,    77,  -151,   138,    50,
    -151,  -151,  -151,  -151,   140,  -151,    -8,  -151,   111,   157,
       9,  -151,   112,     5,  -151,  -151,  -151,    19,   112,   170,
     -12,    -9,  -151,   137,  -151,  -151,   168,     1,   155,    19,
      19,    19,   158,  -151,    41,  -151,    69,    19,    92,    39,
     175,   126,    10,  -151,   165,  -151,  -151,  -151,    19,   161,
      -9,    69,    19,    19,   112,   112,   112,    69,    19,    19,
      93,  -151,  -151,    72,  -151,  -151,   166,  -151,  -151,  -151,
     148,  -151,     0,   167,  -151,  -151,    95,  -151,  -151,   176,
    -151,  -151,  -151,  -151,  -151,  -151,  -151,  -151,   159,    19,
    -151,    19,  -151,   160,   163,  -151,  -151,  -151,    69,    19,
    -151,  -151,    19,  -151,  -151,  -151,   178,   162,    69,    19,
    -151,  -151,   179,    82,    19,    19,   184,   182,    82,    82,
     186,    82
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -151,  -151,  -151,  -151,   142,  -151,   164,   199,   110,   185,
    -151,    73,   173,  -151,    37,   169,  -151,    18,  -128,  -146,
    -150,    79,    61,    66,   -80,    11,    97,    12,  -151,  -102,
     -65,  -151,    84,  -151,   -58,  -151,    78,   -69,  -151,  -151,
     171,  -151,    76,  -151,   172,   -68,  -151
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -103
static const yytype_int16 yytable[] =
{
      93,   101,   119,   126,    76,   179,   104,   186,   168,   108,
     147,   185,    52,   135,   154,    93,     1,   104,   146,    93,
     152,   142,   158,   178,    38,    53,    54,   143,   153,    61,
     136,    62,   155,   136,   105,    38,   141,   149,   136,   148,
       3,    55,   130,   112,   113,   114,   115,   116,   205,    80,
     162,   164,   204,   117,   184,    81,     4,   169,   211,     5,
     118,    84,   210,   166,    98,    54,    86,   210,     6,    93,
     101,    93,   220,   185,   108,   204,    11,    80,   187,   188,
      55,   139,    12,    81,    82,     9,    93,    83,   140,    84,
      80,    85,    93,    16,    86,    87,    81,   214,    19,    14,
     215,    15,    84,   191,   166,   102,    54,    86,    87,   200,
     192,   201,    42,    43,   195,    29,    30,    31,    32,   206,
     129,    55,   207,   130,   189,   131,   189,    21,   190,   212,
     197,    33,    22,    93,   216,   217,   112,   113,   114,   115,
     116,   110,    61,    93,   144,   127,   117,    23,    93,    49,
      50,    51,    24,    93,    93,    27,    93,    61,    37,   173,
      71,    72,    36,   180,   181,    38,   182,   183,    60,    63,
      64,    65,    67,    70,  -102,    68,    69,    73,    75,   156,
     111,   128,   145,    77,   109,   134,   151,   157,   172,   137,
     175,   193,   196,   198,   159,   130,   177,   208,   213,   202,
     218,   219,   199,   221,     8,   209,   138,   194,    35,    94,
      59,   170,   203,   167,   176,   150,   165,   171,   174,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    74,
       0,     0,     0,    97,     0,    79,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   103
};

static const yytype_int16 yycheck[] =
{
      65,    70,    82,    83,    62,   151,     7,   157,   136,    77,
     112,   157,    12,     9,    23,    80,     3,     7,     9,    84,
      32,    29,    21,   151,    25,    25,    26,    35,    40,    31,
      29,    33,    41,    29,    35,    25,   104,   117,    29,    34,
      25,    41,    37,    24,    25,    26,    27,    28,   198,     8,
     130,   131,   198,    34,   156,    14,     0,   137,   208,    29,
      41,    20,   208,    22,    25,    26,    25,   213,     4,   134,
     139,   136,   218,   219,   142,   221,    30,     8,   158,   159,
      41,    31,     5,    14,    15,    26,   151,    18,    38,    20,
       8,    22,   157,    25,    25,    26,    14,    15,     6,    29,
      18,    31,    20,    31,    22,    25,    26,    25,    26,   189,
      38,   191,    10,    11,   172,    25,    26,    27,    28,   199,
      34,    41,   202,    37,    31,    39,    31,    26,    35,   209,
      35,    41,    32,   198,   214,   215,    24,    25,    26,    27,
      28,    80,    31,   208,    33,    84,    34,    29,   213,    25,
      26,    27,    25,   218,   219,     7,   221,    31,    29,    33,
      25,    26,    32,   152,   153,    25,   154,   155,    29,    25,
      25,     8,    29,    37,    36,    29,    29,    36,    25,    42,
      26,    25,    25,    34,    33,    33,    16,    19,    13,    39,
      25,    25,    25,    17,    39,    37,    35,    19,    19,    39,
      16,    19,    43,    17,     5,    43,    96,   170,    23,    67,
      37,   138,   194,   134,   148,   118,   132,   139,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    60,
      -1,    -1,    -1,    69,    -1,    64,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    73
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,    45,    25,     0,    29,     4,    46,    51,    26,
      90,    30,     5,    52,    29,    31,    25,    53,    54,     6,
      55,    26,    32,    29,    25,    56,    57,     7,    58,    25,
      26,    27,    28,    41,    87,    53,    32,    29,    25,    59,
      60,    89,    10,    11,    47,    48,    49,    82,    83,    25,
      26,    27,    12,    25,    26,    41,    78,    81,    88,    56,
      29,    31,    33,    25,    25,     8,    61,    29,    29,    29,
      37,    25,    26,    36,    59,    25,    78,    34,    84,    84,
       8,    14,    15,    18,    20,    22,    25,    26,    62,    63,
      64,    65,    66,    74,    48,    50,    51,    50,    25,    79,
      80,    81,    25,    88,     7,    35,    85,    86,    89,    33,
      66,    26,    24,    25,    26,    27,    28,    34,    41,    68,
      69,    70,    71,    72,    73,    74,    68,    66,    25,    34,
      37,    39,    75,    76,    33,     9,    29,    39,    52,    31,
      38,    89,    29,    35,    33,    25,     9,    73,    34,    68,
      70,    16,    32,    40,    23,    41,    42,    19,    21,    39,
      67,    68,    68,    77,    68,    76,    22,    65,    62,    68,
      55,    80,    13,    33,    86,    25,    67,    35,    62,    63,
      69,    69,    71,    71,    73,    63,    64,    68,    68,    31,
      35,    31,    38,    25,    58,    78,    25,    35,    17,    43,
      68,    68,    39,    61,    63,    64,    68,    68,    19,    43,
      63,    64,    68,    19,    15,    18,    68,    68,    16,    19,
      63,    17
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
      yyerror (&yylloc, ctx, YY_("syntax error: cannot back up")); \
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
# if YYLTYPE_IS_TRIVIAL
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
# define YYLEX yylex (&yylval, &yylloc, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, &yylloc, ctx)
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
		  Type, Value, Location, ctx); \
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
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, mlc::MlaskalCtx *ctx)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, ctx)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    mlc::MlaskalCtx *ctx;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (yylocationp);
  YYUSE (ctx);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, mlc::MlaskalCtx *ctx)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, yylocationp, ctx)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    YYLTYPE const * const yylocationp;
    mlc::MlaskalCtx *ctx;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp, ctx);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
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
yy_reduce_print (YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule, mlc::MlaskalCtx *ctx)
#else
static void
yy_reduce_print (yyvsp, yylsp, yyrule, ctx)
    YYSTYPE *yyvsp;
    YYLTYPE *yylsp;
    int yyrule;
    mlc::MlaskalCtx *ctx;
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
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       , &(yylsp[(yyi + 1) - (yynrhs)])		       , ctx);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, yylsp, Rule, ctx); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, mlc::MlaskalCtx *ctx)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, yylocationp, ctx)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    YYLTYPE *yylocationp;
    mlc::MlaskalCtx *ctx;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  YYUSE (ctx);

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
int yyparse (mlc::MlaskalCtx *ctx);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

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
yyparse (mlc::MlaskalCtx *ctx)
#else
int
yyparse (ctx)
    mlc::MlaskalCtx *ctx;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Location data for the lookahead symbol.  */
YYLTYPE yylloc;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.
       `yyls': related to locations.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[2];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;
  yylsp = yyls;

#if YYLTYPE_IS_TRIVIAL
  /* Initialize the default location before parsing starts.  */
  yylloc.first_line   = yylloc.last_line   = 1;
  yylloc.first_column = yylloc.last_column = 1;
#endif

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
	YYLTYPE *yyls1 = yyls;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yyls1, yysize * sizeof (*yylsp),
		    &yystacksize);

	yyls = yyls1;
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
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
	YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
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

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;
  *++yylsp = yylloc;
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

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 85 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { set_block_code(ctx, (yyvsp[(2) - (5)]), (yyvsp[(4) - (5)]), block_type::program); ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 89 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { append_code_block((yyval), (yyvsp[(6) - (6)])); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 94 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { set_block_code(ctx, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), block_type::subprogram); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 95 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { set_block_code(ctx, (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)]), block_type::subprogram); ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 103 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { append_code_block((yyval), (yyvsp[(5) - (5)])); ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 111 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { const_declare(ctx, (yylsp[(1) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 119 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { type_assign(ctx, (yylsp[(1) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 127 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { var_declare(ctx, (yyvsp[(1) - (3)]), (yylsp[(3) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 132 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { append_code_block((yyval), (yyvsp[(2) - (3)])); block_leave(ctx, (yylsp[(2) - (3)])); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 140 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { if_else_stmt(ctx, (yyval), (yylsp[(2) - (6)]), (yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 141 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { while_stmt(ctx, (yyval), (yylsp[(2) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 142 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { label_target(ctx, (yyval), (yylsp[(1) - (3)]), (yyvsp[(1) - (3)])); append_code_block((yyval), (yyvsp[(3) - (3)]));  ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 145 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { if_stmt(ctx, (yyval), (yylsp[(2) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 146 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { if_else_stmt(ctx, (yyval), (yylsp[(2) - (6)]), (yyvsp[(2) - (6)]), (yyvsp[(4) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 147 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { while_stmt(ctx, (yyval), (yylsp[(2) - (4)]), (yyvsp[(2) - (4)]), (yyvsp[(4) - (4)])); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 148 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { for_stmt(ctx, (yyval), (yylsp[(2) - (8)]), (yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(8) - (8)])); ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 152 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { store_identifier(ctx, (yyval), (yylsp[(1) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 153 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { store_array(ctx, (yyval), (yylsp[(1) - (3)]), (yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 154 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { subprogram_call(ctx, (yyval), (yylsp[(1) - (1)]), (yyvsp[(1) - (1)]), (yyvsp[(1) - (1)])); ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 155 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { subprogram_call(ctx, (yyval), (yylsp[(1) - (4)]), (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 156 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { label_goto(ctx, (yyval), (yylsp[(2) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 157 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { append_code_block((yyval), (yyvsp[(2) - (3)])); ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 158 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { repeat_stmt(ctx, (yyval), (yylsp[(4) - (4)]), (yyvsp[(4) - (4)]), (yyvsp[(2) - (4)])); ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 159 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { for_stmt(ctx, (yyval), (yylsp[(2) - (8)]), (yyvsp[(2) - (8)]), (yyvsp[(4) - (8)]), (yyvsp[(5) - (8)]), (yyvsp[(6) - (8)]), (yyvsp[(8) - (8)])); ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 163 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { append_code_block((yyvsp[(1) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 166 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyval).exprs_.push_back((yyvsp[(1) - (1)]).code_); (yyval).identifiers_.push_back((yyvsp[(1) - (1)]).id_ci_); ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 167 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyval).exprs_.push_back((yyvsp[(3) - (3)]).code_); (yyval).identifiers_.push_back((yyvsp[(1) - (3)]).id_ci_); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 173 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { binary_op(ctx, (yyval), (yyvsp[(1) - (3)]), (yylsp[(2) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 174 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyvsp[(2) - (3)]).dtge_ = DUTOKGE_EQ; binary_op(ctx, (yyval), (yyvsp[(1) - (3)]), (yylsp[(2) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 177 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { unary_op(ctx, (yyval), (yylsp[(1) - (2)]), (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 181 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { binary_op(ctx, (yyval), (yyvsp[(1) - (3)]), (yylsp[(2) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 182 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyvsp[(2) - (3)]).dtge_ = DUTOKGE_OR; binary_op(ctx, (yyval), (yyvsp[(1) - (3)]), (yylsp[(2) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 188 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { binary_op(ctx, (yyval), (yyvsp[(1) - (3)]), (yylsp[(2) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 190 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { load_identifier(ctx, (yyval), (yylsp[(1) - (1)]), (yyvsp[(1) - (1)])); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 191 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { load_const_value(ctx, (yyval), (yyvsp[(1) - (1)]), const_type::integer); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 192 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { load_const_value(ctx, (yyval), (yyvsp[(1) - (1)]), const_type::real);	;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 193 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { load_const_value(ctx, (yyval), (yyvsp[(1) - (1)]), const_type::string); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 194 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { load_element(ctx, (yyval), (yylsp[(1) - (1)]), (yyvsp[(1) - (1)])); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 195 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { subprogram_call(ctx, (yyval), (yylsp[(1) - (4)]), (yyvsp[(1) - (4)]), (yyvsp[(3) - (4)])); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 196 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyval).type_ = (yyvsp[(2) - (3)]).type_; (yyval).code_ = (yyvsp[(2) - (3)]).code_; ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 197 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { unary_not(ctx, (yyval), (yylsp[(1) - (2)]), (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 199 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { array_element(ctx, (yyval), (yylsp[(1) - (2)]), (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 203 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyval).exprs_.insert((yyval).exprs_.end(), (yyvsp[(2) - (2)]).exprs_.begin(), (yyvsp[(2) - (2)]).exprs_.end()); ;}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 205 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyval).code_ = (yyvsp[(2) - (3)]).code_; (yyval).type_ = (yyvsp[(2) - (3)]).type_; (yyval).exprs_ = (yyvsp[(2) - (3)]).exprs_; ;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 207 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { array_index(ctx, (yyval), (yylsp[(1) - (1)]), (yyvsp[(1) - (1)])); ;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 208 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { array_index(ctx, (yyval), (yylsp[(3) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 213 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { type_declare(ctx, (yyval), (yylsp[(1) - (1)]), (yyvsp[(1) - (1)])); ;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 214 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyval).type_ = (yyvsp[(1) - (1)]).type_; ;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 215 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { array_declare(ctx, (yyval), (yyvsp[(3) - (6)]), (yyvsp[(6) - (6)])); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 217 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { range_add((yyval), (yyvsp[(1) - (1)])); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 218 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { range_add((yyval), (yyvsp[(3) - (3)])); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 220 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { type_declare_ordinal(ctx, (yyval), (yylsp[(1) - (1)]), (yyvsp[(1) - (1)])); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 221 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyval).type_ = (yyvsp[(1) - (1)]).type_; ;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 223 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { range_declare(ctx, (yyval), (yyvsp[(1) - (3)]), (yylsp[(3) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 229 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { procedure_declare(ctx, (yyval), (yylsp[(1) - (3)]), (yyvsp[(2) - (3)]), (yyvsp[(3) - (3)])); ;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 232 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { function_declare(ctx, (yyval), (yylsp[(1) - (5)]), (yyvsp[(2) - (5)]), (yyvsp[(3) - (5)]), (yylsp[(5) - (5)]), (yyvsp[(5) - (5)])); ;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 235 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyval).param_list_ = create_parameter_list(); ;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 236 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyval).param_list_ = create_parameter_list(); ;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 237 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyval).param_list_ = (yyvsp[(2) - (3)]).param_list_; ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 240 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyvsp[(1) - (1)]).param_list_ = (yyval).param_list_; ;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 241 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyvsp[(1) - (3)]).param_list_->append_and_kill((yyvsp[(3) - (3)]).param_list_); (yyval).param_list_ = (yyvsp[(1) - (3)]).param_list_; ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 244 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { parameter_add(ctx, (yyval), (yyvsp[(1) - (3)]), (yylsp[(3) - (3)]), (yyvsp[(3) - (3)]), param_type::value); ;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 245 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { parameter_add(ctx, (yyval), (yyvsp[(2) - (4)]), (yylsp[(4) - (4)]), (yyvsp[(4) - (4)]), param_type::reference); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 250 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { const_load(ctx, (yyval), (yylsp[(1) - (1)]), (yyvsp[(1) - (1)])); ;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 251 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { const_load_and_calculate(ctx, (yyval), (yyvsp[(1) - (2)]), (yylsp[(2) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 252 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyval).const_type_ = const_type::integer; ;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 253 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { const_calculate(ctx, (yyval), (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]), const_type::integer);  ;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 254 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyval).const_type_ = const_type::real; ;}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 255 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { const_calculate(ctx, (yyval), (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]), const_type::real); ;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 256 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { (yyval).const_type_ = const_type::string; ;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 259 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { const_load(ctx, (yyval), (yylsp[(1) - (1)]), (yyvsp[(1) - (1)])); ;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 260 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { const_load_and_calculate(ctx, (yyval), (yyvsp[(1) - (2)]), (yylsp[(2) - (2)]), (yyvsp[(2) - (2)])); ;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 262 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { const_calculate(ctx, (yyval), (yyvsp[(1) - (2)]), (yyvsp[(2) - (2)]), const_type::integer); ;}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 267 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { identifier_add((yyval), (yyvsp[(1) - (1)])); ;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 268 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { identifier_add((yyval), (yyvsp[(3) - (3)])); ;}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 271 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { ctx->tab->add_label_entry((yylsp[(1) - (1)]), (yyvsp[(1) - (1)]).int_ci_, new_label(ctx)); ;}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 272 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"
    { ctx->tab->add_label_entry((yylsp[(3) - (3)]), (yyvsp[(3) - (3)]).int_ci_, new_label(ctx)); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2180 "du6g.cpp"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

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
      yyerror (&yylloc, ctx, YY_("syntax error"));
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
	    yyerror (&yylloc, ctx, yymsg);
	  }
	else
	  {
	    yyerror (&yylloc, ctx, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }

  yyerror_range[0] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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
		      yytoken, &yylval, &yylloc, ctx);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
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

  yyerror_range[0] = yylsp[1-yylen];
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

      yyerror_range[0] = *yylsp;
      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, yylsp, ctx);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;

  yyerror_range[1] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, (yyerror_range - 1), 2);
  *++yylsp = yyloc;

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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (&yylloc, ctx, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, &yylloc, ctx);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, yylsp, ctx);
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



/* Line 1675 of yacc.c  */
#line 275 "c:\\Projects\\Homeworks\\09. semester\\MlaskalCompiler\\public-rw\\SWI098\\du6\\du6g.y"


static void yyerror(unsigned *line, MlaskalCtx *, const char *m)
{
    error(DUERR_SYNTAX, *line, m);
}
