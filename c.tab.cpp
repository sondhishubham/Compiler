/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "c.y"

#include <cstdio>
#include <iostream>
#include "ast.h"

using namespace std;

NODE* createFunction(NODE*,NODE*,NODE*);
NODE* createUnaryNode(SYMBOL, NODE*);
NODE* createBinaryNode(SYMBOL, NODE*, NODE*);
void addChild(NODE*, NODE*);
void printTree(NODE*);
// stuff from flex that bison needs to know about:
extern "C" int yylex();
int yyparse();
extern "C" FILE *yyin;

void yyerror(const char *s);


#line 91 "c.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_C_TAB_HPP_INCLUDED
# define YY_YY_C_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    I_CONSTANT = 258,
    STRING_LITERAL = 259,
    IDENTIFIER = 260,
    F_CONSTANT = 261,
    FUNC_NAME = 262,
    SIZEOF = 263,
    PTR_OP = 264,
    INC_OP = 265,
    DEC_OP = 266,
    LEFT_OP = 267,
    RIGHT_OP = 268,
    LE_OP = 269,
    GE_OP = 270,
    EQ_OP = 271,
    NE_OP = 272,
    AND_OP = 273,
    OR_OP = 274,
    MUL_ASSIGN = 275,
    DIV_ASSIGN = 276,
    MOD_ASSIGN = 277,
    ADD_ASSIGN = 278,
    SUB_ASSIGN = 279,
    LEFT_ASSIGN = 280,
    RIGHT_ASSIGN = 281,
    AND_ASSIGN = 282,
    XOR_ASSIGN = 283,
    OR_ASSIGN = 284,
    TYPEDEF_NAME = 285,
    ENUMERATION_CONSTANT = 286,
    TYPEDEF = 287,
    EXTERN = 288,
    STATIC = 289,
    AUTO = 290,
    REGISTER = 291,
    INLINE = 292,
    CONST = 293,
    RESTRICT = 294,
    VOLATILE = 295,
    BOOL = 296,
    CHAR = 297,
    SHORT = 298,
    INT = 299,
    LONG = 300,
    SIGNED = 301,
    UNSIGNED = 302,
    FLOAT = 303,
    DOUBLE = 304,
    VOID = 305,
    COMPLEX = 306,
    IMAGINARY = 307,
    STRUCT = 308,
    UNION = 309,
    ENUM = 310,
    ELLIPSIS = 311,
    CASE = 312,
    DEFAULT = 313,
    IF = 314,
    ELSE = 315,
    SWITCH = 316,
    WHILE = 317,
    DO = 318,
    FOR = 319,
    GOTO = 320,
    CONTINUE = 321,
    BREAK = 322,
    RETURN = 323,
    ALIGNAS = 324,
    ALIGNOF = 325,
    ATOMIC = 326,
    GENERIC = 327,
    NORETURN = 328,
    STATIC_ASSERT = 329,
    THREAD_LOCAL = 330
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 21 "c.y"

    int number;
    NODE* node_ptr;
    char* word;

#line 225 "c.tab.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_C_TAB_HPP_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  19
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   91

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  93
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  37
/* YYNRULES -- Number of rules.  */
#define YYNRULES  74
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  108

#define YYUNDEFTOK  2
#define YYMAXUTOK   330


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    80,    85,     2,
      76,    77,    78,    81,    89,    82,     2,    79,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    92,
      83,    88,    84,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    86,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    90,    87,    91,     2,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    51,    51,    52,    53,    54,    59,    69,    88,   106,
     125,   130,   131,   132,   133,   137,   138,   139,   143,   144,
     145,   149,   150,   151,   152,   153,   157,   158,   159,   163,
     164,   168,   169,   173,   174,   178,   179,   183,   184,   188,
     193,   194,   198,   212,   230,   259,   260,   261,   262,   263,
     264,   265,   268,   358,   362,   363,   373,   374,   393,   397,
     398,   402,   404,   481,   494,   495,   499,   500,   505,   510,
     537,   540,   541,   545,   551
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "I_CONSTANT", "STRING_LITERAL",
  "IDENTIFIER", "F_CONSTANT", "FUNC_NAME", "SIZEOF", "PTR_OP", "INC_OP",
  "DEC_OP", "LEFT_OP", "RIGHT_OP", "LE_OP", "GE_OP", "EQ_OP", "NE_OP",
  "AND_OP", "OR_OP", "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN",
  "ADD_ASSIGN", "SUB_ASSIGN", "LEFT_ASSIGN", "RIGHT_ASSIGN", "AND_ASSIGN",
  "XOR_ASSIGN", "OR_ASSIGN", "TYPEDEF_NAME", "ENUMERATION_CONSTANT",
  "TYPEDEF", "EXTERN", "STATIC", "AUTO", "REGISTER", "INLINE", "CONST",
  "RESTRICT", "VOLATILE", "BOOL", "CHAR", "SHORT", "INT", "LONG", "SIGNED",
  "UNSIGNED", "FLOAT", "DOUBLE", "VOID", "COMPLEX", "IMAGINARY", "STRUCT",
  "UNION", "ENUM", "ELLIPSIS", "CASE", "DEFAULT", "IF", "ELSE", "SWITCH",
  "WHILE", "DO", "FOR", "GOTO", "CONTINUE", "BREAK", "RETURN", "ALIGNAS",
  "ALIGNOF", "ATOMIC", "GENERIC", "NORETURN", "STATIC_ASSERT",
  "THREAD_LOCAL", "'('", "')'", "'*'", "'/'", "'%'", "'+'", "'-'", "'<'",
  "'>'", "'&'", "'^'", "'|'", "'='", "','", "'{'", "'}'", "';'", "$accept",
  "primary_expression", "constant", "string", "postfix_expression",
  "unary_expression", "cast_expression", "multiplicative_expression",
  "additive_expression", "shift_expression", "relational_expression",
  "equality_expression", "and_expression", "exclusive_or_expression",
  "inclusive_or_expression", "logical_and_expression",
  "logical_or_expression", "conditional_expression",
  "assignment_expression", "assignment_operator", "expression",
  "declaration_specifiers", "type_specifier", "declarator",
  "direct_declarator", "parameter_type_list", "parameter_list",
  "parameter_declaration", "statement", "compound_statement",
  "block_item_list", "block_item", "expression_statement", "start_unit",
  "translation_unit", "external_declaration", "function_definition", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,    40,    41,    42,    47,
      37,    43,    45,    60,    62,    38,    94,   124,    61,    44,
     123,   125,    59
};
# endif

#define YYPACT_NINF (-66)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -12,   -66,   -66,   -66,   -66,   -66,   -66,   -66,   -66,     4,
     -66,    14,   -12,   -66,   -66,   -66,     4,   -51,   -23,   -66,
     -66,   -18,    -3,   -66,   -22,   -66,   -66,   -66,   -66,     3,
     -66,   -66,   -66,   -66,   -66,   -26,   -66,   -33,   -65,    12,
      -2,    18,   -21,   -20,   -24,    47,    48,   -66,   -66,   -15,
     -66,     0,   -66,   -66,   -66,     4,    -9,   -19,   -66,    -8,
     -66,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
     -66,   -66,   -66,   -66,   -66,   -12,   -66,   -66,   -66,   -66,
     -66,   -66,   -33,   -33,   -65,   -65,    12,    12,    12,    12,
      -2,    -2,    18,   -21,   -20,   -24,    47,   -66
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    52,    46,    47,    48,    49,    50,    51,    45,     0,
      44,     0,    70,    71,    73,    54,     0,     0,    53,     1,
      72,     0,     0,    74,     0,    55,     6,     7,     2,     0,
      64,     8,     3,     4,     9,    10,    11,    15,    18,    21,
      26,    29,    31,    33,    35,    37,    39,    40,    43,     0,
      68,     0,    66,    63,    57,    62,     0,    58,    59,     0,
      42,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      69,    65,    67,    61,    56,     0,     5,    41,    10,    12,
      13,    14,    16,    17,    19,    20,    24,    25,    22,    23,
      27,    28,    30,    32,    34,    36,    38,    60
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -66,   -66,   -66,   -66,   -66,   -11,    -6,   -17,   -16,   -28,
     -13,     8,    -4,     1,     6,    -5,   -66,   -66,    24,   -66,
      46,   -14,   -66,    -1,   -66,   -66,   -66,     2,   -66,   -66,
     -66,    35,   -66,   -66,   -66,    77,   -66
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    31,    32,    33,    34,    88,    36,    37,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    61,
      49,     9,    10,    17,    18,    56,    57,    58,    50,    23,
      51,    52,    53,    11,    12,    13,    14
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      26,    27,    28,    26,    27,    28,    26,    27,    28,    15,
      55,    35,    69,    70,    19,    21,    65,    66,    35,     1,
       2,     3,     4,     5,    67,    68,     6,     7,     8,     1,
       2,     3,     4,     5,    73,    74,     6,     7,     8,    22,
      35,    96,    97,    98,    99,    62,    63,    64,    92,    93,
      35,    94,    95,    24,    83,    54,    89,    90,    91,    25,
     100,   101,    60,    77,    75,    78,    76,    79,    84,    86,
      85,    55,   103,    29,   106,    59,    29,    80,   104,    29,
      16,    71,    72,   102,   105,    87,    82,   107,    30,    20,
       0,    81
};

static const yytype_int8 yycheck[] =
{
       3,     4,     5,     3,     4,     5,     3,     4,     5,     5,
      24,    22,    14,    15,     0,    16,    81,    82,    29,    41,
      42,    43,    44,    45,    12,    13,    48,    49,    50,    41,
      42,    43,    44,    45,    16,    17,    48,    49,    50,    90,
      51,    69,    70,    71,    72,    78,    79,    80,    65,    66,
      61,    67,    68,    76,    55,    77,    62,    63,    64,    77,
      73,    74,    88,    87,    85,    18,    86,    19,    77,    77,
      89,    85,    76,    76,    79,    29,    76,    92,    77,    76,
      76,    83,    84,    75,    78,    61,    51,    85,    91,    12,
      -1,    91
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    41,    42,    43,    44,    45,    48,    49,    50,   114,
     115,   126,   127,   128,   129,     5,    76,   116,   117,     0,
     128,   116,    90,   122,    76,    77,     3,     4,     5,    76,
      91,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   113,
     121,   123,   124,   125,    77,   114,   118,   119,   120,   113,
      88,   112,    78,    79,    80,    81,    82,    12,    13,    14,
      15,    83,    84,    16,    17,    85,    86,    87,    18,    19,
      92,    91,   124,   116,    77,    89,    77,   111,    98,    99,
      99,    99,   100,   100,   101,   101,   102,   102,   102,   102,
     103,   103,   104,   105,   106,   107,   108,   120
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    93,    94,    94,    94,    94,    95,    96,    97,    98,
      99,   100,   100,   100,   100,   101,   101,   101,   102,   102,
     102,   103,   103,   103,   103,   103,   104,   104,   104,   105,
     105,   106,   106,   107,   107,   108,   108,   109,   109,   110,
     111,   111,   112,   113,   114,   115,   115,   115,   115,   115,
     115,   115,   115,   116,   117,   117,   117,   117,   118,   119,
     119,   120,   120,   121,   122,   122,   123,   123,   124,   125,
     126,   127,   127,   128,   129
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     1,     3,     1,     1,     1,     1,
       1,     1,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     4,     3,     1,     1,
       3,     2,     1,     1,     2,     3,     1,     2,     1,     2,
       1,     1,     2,     1,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

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
#line 51 "c.y"
                                {char* val = (yyvsp[0].word);(yyval.node_ptr)= new NODE(IDENT,(void*)val,0);}
#line 1503 "c.tab.cpp"
    break;

  case 3:
#line 52 "c.y"
                                        {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1509 "c.tab.cpp"
    break;

  case 4:
#line 53 "c.y"
                            {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1515 "c.tab.cpp"
    break;

  case 5:
#line 54 "c.y"
                            {(yyval.node_ptr) = (yyvsp[-1].node_ptr);}
#line 1521 "c.tab.cpp"
    break;

  case 6:
#line 59 "c.y"
                                {int* val = (int*)malloc(sizeof(int));*val = (yyvsp[0].number);(yyval.node_ptr)= new NODE(INTEGER,(void*)val,0);}
#line 1527 "c.tab.cpp"
    break;

  case 7:
#line 69 "c.y"
                            {char* val = (yyvsp[0].word);(yyval.node_ptr)= new NODE(STRING,(void*)val,0);}
#line 1533 "c.tab.cpp"
    break;

  case 8:
#line 88 "c.y"
                                                                                                                {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1539 "c.tab.cpp"
    break;

  case 9:
#line 106 "c.y"
                                                                                                                {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1545 "c.tab.cpp"
    break;

  case 10:
#line 125 "c.y"
                                                                                                                        {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1551 "c.tab.cpp"
    break;

  case 11:
#line 130 "c.y"
                                                                                                                        {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1557 "c.tab.cpp"
    break;

  case 12:
#line 131 "c.y"
                                                                                        {(yyval.node_ptr) = createBinaryNode(MULT, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1563 "c.tab.cpp"
    break;

  case 13:
#line 132 "c.y"
                                                                                        {(yyval.node_ptr) = createBinaryNode(DIVIDE, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1569 "c.tab.cpp"
    break;

  case 14:
#line 133 "c.y"
                                                                                        {(yyval.node_ptr) = createBinaryNode(REMAINDER, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1575 "c.tab.cpp"
    break;

  case 15:
#line 137 "c.y"
                                                                                                                {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1581 "c.tab.cpp"
    break;

  case 16:
#line 138 "c.y"
                                                                                {(yyval.node_ptr) = createBinaryNode(PLUS, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1587 "c.tab.cpp"
    break;

  case 17:
#line 139 "c.y"
                                                                                        {(yyval.node_ptr) = createBinaryNode(SUB, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1593 "c.tab.cpp"
    break;

  case 18:
#line 143 "c.y"
                                                                                                                {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1599 "c.tab.cpp"
    break;

  case 19:
#line 144 "c.y"
                                                                                        {(yyval.node_ptr) = createBinaryNode(LEFT_SHIFT, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1605 "c.tab.cpp"
    break;

  case 20:
#line 145 "c.y"
                                                                                        {(yyval.node_ptr) = createBinaryNode(RIGHT_SHIFT, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1611 "c.tab.cpp"
    break;

  case 21:
#line 149 "c.y"
                                                                        {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1617 "c.tab.cpp"
    break;

  case 22:
#line 150 "c.y"
                                                                                        {(yyval.node_ptr) = createBinaryNode(LESS_THAN, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1623 "c.tab.cpp"
    break;

  case 23:
#line 151 "c.y"
                                                                                        {(yyval.node_ptr) = createBinaryNode(GREATER_THAN, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1629 "c.tab.cpp"
    break;

  case 24:
#line 152 "c.y"
                                                                                        {(yyval.node_ptr) = createBinaryNode(LESS_THAN_EQUAL_TO, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1635 "c.tab.cpp"
    break;

  case 25:
#line 153 "c.y"
                                                                                        {(yyval.node_ptr) = createBinaryNode(GREATER_THAN_EQUAL_TO, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1641 "c.tab.cpp"
    break;

  case 26:
#line 157 "c.y"
                                                                        {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1647 "c.tab.cpp"
    break;

  case 27:
#line 158 "c.y"
                                                                                        {(yyval.node_ptr) = createBinaryNode(EQUAL_TO, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1653 "c.tab.cpp"
    break;

  case 28:
#line 159 "c.y"
                                                                                        {(yyval.node_ptr) = createBinaryNode(NOT_EQUAL_TO, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1659 "c.tab.cpp"
    break;

  case 29:
#line 163 "c.y"
                                                                        {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1665 "c.tab.cpp"
    break;

  case 30:
#line 164 "c.y"
                                                                                                {(yyval.node_ptr) = createBinaryNode(AND, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1671 "c.tab.cpp"
    break;

  case 31:
#line 168 "c.y"
                                                                        {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1677 "c.tab.cpp"
    break;

  case 32:
#line 169 "c.y"
                                                                                        {(yyval.node_ptr) = createBinaryNode(EXCLUSIVE_OR, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1683 "c.tab.cpp"
    break;

  case 33:
#line 173 "c.y"
                                                                        {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1689 "c.tab.cpp"
    break;

  case 34:
#line 174 "c.y"
                                                                                {(yyval.node_ptr) = createBinaryNode(INCLUSIVE_OR, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1695 "c.tab.cpp"
    break;

  case 35:
#line 178 "c.y"
                                                                        {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1701 "c.tab.cpp"
    break;

  case 36:
#line 179 "c.y"
                                                                                {(yyval.node_ptr) = createBinaryNode(LOGICAL_AND, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1707 "c.tab.cpp"
    break;

  case 37:
#line 183 "c.y"
                                                                        {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1713 "c.tab.cpp"
    break;

  case 38:
#line 184 "c.y"
                                                                                {(yyval.node_ptr) = createBinaryNode(LOGICAL_OR, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1719 "c.tab.cpp"
    break;

  case 39:
#line 188 "c.y"
                                                                                    {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1725 "c.tab.cpp"
    break;

  case 40:
#line 193 "c.y"
                                                                                    {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1731 "c.tab.cpp"
    break;

  case 41:
#line 194 "c.y"
                                                                                    {(yyval.node_ptr) = createBinaryNode(ASSIGN, (yyvsp[-2].node_ptr), (yyvsp[0].node_ptr));}
#line 1737 "c.tab.cpp"
    break;

  case 43:
#line 212 "c.y"
                                                                                                                                        {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1743 "c.tab.cpp"
    break;

  case 44:
#line 230 "c.y"
                                                                                                        {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1749 "c.tab.cpp"
    break;

  case 45:
#line 259 "c.y"
                                                                                                                {(yyval.node_ptr) = new NODE(TYPE_VOID,NULL,0);}
#line 1755 "c.tab.cpp"
    break;

  case 46:
#line 260 "c.y"
                                                                                                                {(yyval.node_ptr) = new NODE(TYPE_CHAR,NULL,0);}
#line 1761 "c.tab.cpp"
    break;

  case 47:
#line 261 "c.y"
                                                                                                                {(yyval.node_ptr) = new NODE(TYPE_SHORT,NULL,0);}
#line 1767 "c.tab.cpp"
    break;

  case 48:
#line 262 "c.y"
                                                                                                                {(yyval.node_ptr) = new NODE(TYPE_INT,NULL,0);}
#line 1773 "c.tab.cpp"
    break;

  case 49:
#line 263 "c.y"
                                                                                                                {(yyval.node_ptr) = new NODE(TYPE_LONG,NULL,0);}
#line 1779 "c.tab.cpp"
    break;

  case 50:
#line 264 "c.y"
                                                                                                                {(yyval.node_ptr) = new NODE(TYPE_FLOAT,NULL,0);}
#line 1785 "c.tab.cpp"
    break;

  case 51:
#line 265 "c.y"
                                                                                                                {(yyval.node_ptr) = new NODE(TYPE_DOUBLE,NULL,0);}
#line 1791 "c.tab.cpp"
    break;

  case 52:
#line 268 "c.y"
                                                                                                                {(yyval.node_ptr) = new NODE(TYPE_BOOL,NULL,0);}
#line 1797 "c.tab.cpp"
    break;

  case 53:
#line 358 "c.y"
                                                                {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1803 "c.tab.cpp"
    break;

  case 54:
#line 362 "c.y"
                                                                                                                                        {char* val = (yyvsp[0].word);(yyval.node_ptr)= new NODE(IDENT,(void*)val,0);}
#line 1809 "c.tab.cpp"
    break;

  case 55:
#line 363 "c.y"
                                                                                                                                {(yyval.node_ptr) = (yyvsp[-1].node_ptr);}
#line 1815 "c.tab.cpp"
    break;

  case 56:
#line 373 "c.y"
                                                                                                        {(yyval.node_ptr) = createBinaryNode(FUNC_DECLARATOR, (yyvsp[-3].node_ptr), (yyvsp[-1].node_ptr));}
#line 1821 "c.tab.cpp"
    break;

  case 57:
#line 374 "c.y"
                                                                                                                                {(yyval.node_ptr) = createUnaryNode(FUNC_DECLARATOR,(yyvsp[-2].node_ptr));}
#line 1827 "c.tab.cpp"
    break;

  case 58:
#line 393 "c.y"
                                                                                                        {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1833 "c.tab.cpp"
    break;

  case 59:
#line 397 "c.y"
                                                                                                {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1839 "c.tab.cpp"
    break;

  case 60:
#line 398 "c.y"
                                                                                {(yyval.node_ptr)=(yyvsp[-2].node_ptr);addChild((yyval.node_ptr),(yyvsp[0].node_ptr));}
#line 1845 "c.tab.cpp"
    break;

  case 61:
#line 402 "c.y"
                                                                                        {NODE* m = createBinaryNode(DECLARATION, (yyvsp[-1].node_ptr), (yyvsp[0].node_ptr)); (yyval.node_ptr) = createUnaryNode(PARAMETERS,m);}
#line 1851 "c.tab.cpp"
    break;

  case 62:
#line 404 "c.y"
                                                                                                {(yyval.node_ptr) = createUnaryNode(PARAMETERS, (yyvsp[0].node_ptr));}
#line 1857 "c.tab.cpp"
    break;

  case 63:
#line 481 "c.y"
                                                                                                {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1863 "c.tab.cpp"
    break;

  case 64:
#line 494 "c.y"
                                                                                                                        {(yyval.node_ptr) = new NODE(BLOCK, NULL, 0);}
#line 1869 "c.tab.cpp"
    break;

  case 65:
#line 495 "c.y"
                                                                                                {(yyval.node_ptr) = (yyvsp[-1].node_ptr);}
#line 1875 "c.tab.cpp"
    break;

  case 66:
#line 499 "c.y"
                                                                                                                {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1881 "c.tab.cpp"
    break;

  case 67:
#line 500 "c.y"
                                                                                                {(yyval.node_ptr) = (yyvsp[-1].node_ptr); addChild((yyval.node_ptr),(yyvsp[0].node_ptr));}
#line 1887 "c.tab.cpp"
    break;

  case 68:
#line 505 "c.y"
                                                                                                                        {(yyval.node_ptr) = createUnaryNode(BLOCK, (yyvsp[0].node_ptr));}
#line 1893 "c.tab.cpp"
    break;

  case 69:
#line 510 "c.y"
                                                                                                                {(yyval.node_ptr) = (yyvsp[-1].node_ptr);}
#line 1899 "c.tab.cpp"
    break;

  case 70:
#line 537 "c.y"
                                                                                                                                                {printTree((yyvsp[0].node_ptr));}
#line 1905 "c.tab.cpp"
    break;

  case 71:
#line 540 "c.y"
                                                                                                {(yyval.node_ptr) = (yyvsp[0].node_ptr);}
#line 1911 "c.tab.cpp"
    break;

  case 72:
#line 541 "c.y"
                                                                                                                        {(yyval.node_ptr) = (yyvsp[-1].node_ptr);addChild((yyval.node_ptr),(yyvsp[0].node_ptr));}
#line 1917 "c.tab.cpp"
    break;

  case 73:
#line 545 "c.y"
                                                                                                                                        {(yyval.node_ptr) = createUnaryNode(CODE_SECTIONS,(yyvsp[0].node_ptr));}
#line 1923 "c.tab.cpp"
    break;

  case 74:
#line 551 "c.y"
                                                                                                        {(yyval.node_ptr) = createFunction((yyvsp[-2].node_ptr),(yyvsp[-1].node_ptr),(yyvsp[0].node_ptr));}
#line 1929 "c.tab.cpp"
    break;


#line 1933 "c.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



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
                      yytoken, &yylval);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp);
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
  return yyresult;
}
#line 559 "c.y"

#include <stdio.h>


NODE* createFunction(NODE* specifier, NODE* func_dec, NODE* function_body){
	NODE* m = new NODE(FUNC_DEF, NULL, 4);
	*(m->children++) = *specifier;
	*(m->children++) = *func_dec;
	*(m->children++) = *function_body;
	return m;
}

NODE* createUnaryNode(SYMBOL sym, NODE* child){
	NODE* m = new NODE(sym, NULL, 1);
	*(m->children++) = *child;
	return m;
}

NODE* createBinaryNode(SYMBOL sym, NODE* left, NODE* right){
	NODE* m = new NODE(sym, NULL, 2);
	*(m->children++) = *left;
	*(m->children++) = *right;
	return m;
}

void addChild(NODE* parent, NODE* child){
	int totalChildren = (parent->children - parent->bp);
//	cout<<"The total number of children assigned are: "<<totalChildren<<endl;
//	cout<<"The total number of children allowed are: "<<parent->numChildren<<endl;
	if(totalChildren == parent->numChildren){
		//increase the memory by 2
//		cout<<"Initial bp is "<<parent->bp<<endl;
//		cout<<"Initial children is "<<parent->children<<endl;
		NODE* new_arr = (NODE*)malloc(2*(totalChildren+1)*sizeof(NODE)); // +1 to avoid infinite loop in totalChildren=0 case.
//		cout << "Value of new_arr initially is: "<<new_arr<<endl;
		parent->numChildren = 2*(totalChildren+1);
		NODE* new_bp = new_arr;
		NODE* itr = parent->bp;
		while(itr!=parent->children){
			*(new_arr++) = *(itr++);
		}
		free(parent->bp);
		parent->children = new_arr;
		parent->bp = new_bp;
		*(parent->children++) = *(child->bp);
//		printTree();
		//increase the memory by 2
//		cout<<"Final bp is "<<parent->bp<<endl;
//		cout<<"Final children is "<<parent->children<<endl;
//		cout<<"The difference is: "<<parent->children - parent->bp<<endl;
	}
	else if(totalChildren < parent->numChildren){
		*(parent->children++) = *(child->bp);
	}
	else{
		cout<<"The number of children assigned are greater than the number of children allowed!!!\n";
	}
	return;
}

void printTree(NODE* p){
    switch(p->symbol){   
    	case INTEGER:
    		cout << "INTEGER("<<*((int*)p->value)<<")";	
    		break;
    	case IDENT:
    		cout << "IDENT("<<(char*)p->value<<")";
    		break;
    	case STRING:
    		cout << "STRING("<<(char*)p->value<<")";
    		break;    		    
    	case TYPE_VOID:
    		cout << "TYPE_VOID";
    		break;
    	case TYPE_INT:
    		cout << "TYPE_INT";
    		break;
    	case TYPE_SHORT:
    		cout << "TYPE_SHORT";
    		break;
    	case TYPE_LONG:
    		cout << "TYPE_LONG";
    		break;
    	case TYPE_DOUBLE:
    		cout << "TYPE_DOUBLE";
    		break;
    	case TYPE_BOOL:
    		cout << "TYPE_BOOL";
    		break;
    	case TYPE_CHAR:
    		cout << "TYPE_CHAR";
    		break;
    	case TYPE_FLOAT:
    		cout << "TYPE_FLOAT";
    		break;
    
    		
    	//Write all the binary rules here
    	case PLUS:
    		cout << "[PLUS[";
    	case ASSIGN:
    		if(p->symbol == ASSIGN)cout << "[ASSIGN[";
    	case SUB:
    		if(p->symbol == SUB)cout << "[SUBTRACT[";
    	case DIVIDE:
    		if(p->symbol == DIVIDE)cout << "[DIVIDE[";
    	case REMAINDER:
    		if(p->symbol == REMAINDER)cout << "[REMAINDER[";
    	case LEFT_SHIFT:
    		if(p->symbol == LEFT_SHIFT)cout << "[LEFT_SHIFT[";
    	case RIGHT_SHIFT:
    		if(p->symbol == RIGHT_SHIFT)cout << "[RIGHT_SHIFT[";
    	case LESS_THAN_EQUAL_TO:
    		if(p->symbol == LESS_THAN_EQUAL_TO)cout << "[LESS_THAN_EQUAL_TO[";
    	case GREATER_THAN_EQUAL_TO:
    		if(p->symbol == GREATER_THAN_EQUAL_TO)cout << "[GREATER_THAN_EQUAL_TO[";
	    case LESS_THAN:
    		if(p->symbol == LESS_THAN)cout << "[LESS_THAN[";
    	case GREATER_THAN:
    		if(p->symbol == GREATER_THAN)cout << "[GREATER_THAN[";
		case EQUAL_TO:
    		if(p->symbol == EQUAL_TO)cout << "[EQUAL_TO[";
	    case NOT_EQUAL_TO:
    		if(p->symbol == NOT_EQUAL_TO)cout << "[NOT_EQUAL_TO[";
    	case AND:
    		if(p->symbol == AND)cout << "[AND[";
    	case EXCLUSIVE_OR:
    		if(p->symbol == EXCLUSIVE_OR)cout << "[EXCLUSIVE_OR[";
    	case INCLUSIVE_OR:
    		if(p->symbol == INCLUSIVE_OR)cout << "[INCLUSIVE_OR[";
    	case LOGICAL_AND:
    		if(p->symbol == LOGICAL_AND)cout << "[LOGICAL_AND[";
    	case LOGICAL_OR:
    		if(p->symbol == LOGICAL_OR)cout << "[LOGICAL_OR[";
    	case MULT:
    		if(p->symbol == MULT)cout << "[MULT[";
    	case DECLARATION:
    		if(p->symbol == DECLARATION)cout << "[DECLARATION[";
    		printTree(p->bp++);
    		cout << "],[";
    		printTree(p->bp++);
    		cout << "]]";
    		break;
    		
    	case BLOCK:
    		cout<< "BLOCK_LIST[";
    	case FUNC_DEF:
    		if(p->symbol == FUNC_DEF)cout<< "FUNC_DEF[";
    	case FUNC_DECLARATOR:
    		if(p->symbol == FUNC_DECLARATOR)cout<< "FUNC_DECLARATOR[";
    	case PARAMETERS:
    		if(p->symbol == PARAMETERS)cout<< "PARAMETERS[";
    	case CODE_SECTIONS:
    		if(p->symbol == CODE_SECTIONS)cout<< "CODE_SECTIONS[";
    		while(p->bp != p->children){
    			cout<<"|[";
    			printTree(p->bp++);
    			cout<<"]";
    		}
    		cout<<"]";
    		break;
    	DEFAULT:
    		cout<<"No prining rule for "<<p->symbol<<endl;
    }
    return;
}


void yyerror(const char *s)
{
	fflush(stdout);
	fprintf(stderr, "*** %s\n", s);
}


