// A Bison parser, made by GNU Bison 3.0.2.

// Skeleton interface for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

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
 ** \file KappaParser.hpp
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_KAPPAPARSER_HPP_INCLUDED
# define YY_YY_KAPPAPARSER_HPP_INCLUDED
// //                    "%code requires" blocks.
#line 18 "kappaParser.y" // lalr1.cc:372

	#include <iostream>
	#include <string>
	#include "ast/AstStructs.h"
	
	using namespace std;
	
	namespace yy {
		class KappaLexer;
	}
	namespace grammar{
		class KappaDriver;
	}


#line 60 "KappaParser.hpp" // lalr1.cc:372

# include <cassert>
# include <vector>
# include <iostream>
# include <stdexcept>
# include <string>
# include "stack.hh"
# include "location.hh"
#include <typeinfo>
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


namespace yy {
#line 136 "KappaParser.hpp" // lalr1.cc:372



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
      : yytname_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    variant (const T& t)
      : yytname_ (typeid (T).name ())
    {
      YYASSERT (sizeof (T) <= S);
      new (yyas_<T> ()) T (t);
    }

    /// Destruction, allowed only if empty.
    ~variant ()
    {
      YYASSERT (!yytname_);
    }

    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    build ()
    {
      YYASSERT (!yytname_);
      YYASSERT (sizeof (T) <= S);
      yytname_ = typeid (T).name ();
      return *new (yyas_<T> ()) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    build (const T& t)
    {
      YYASSERT (!yytname_);
      YYASSERT (sizeof (T) <= S);
      yytname_ = typeid (T).name ();
      return *new (yyas_<T> ()) T (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as ()
    {
      YYASSERT (yytname_ == typeid (T).name ());
      YYASSERT (sizeof (T) <= S);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const
    {
      YYASSERT (yytname_ == typeid (T).name ());
      YYASSERT (sizeof (T) <= S);
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
      YYASSERT (yytname_);
      YYASSERT (yytname_ == other.yytname_);
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
      yytname_ = YY_NULLPTR;
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

    /// Whether the content is built: if defined, the name of the stored type.
    const char *yytname_;
  };


  /// A Bison parser.
  class  KappaParser 
  {
  public:
#ifndef YYSTYPE
    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // agent_expression
      char dummy1[sizeof(ast::Agent)];

      // arrow
      char dummy2[sizeof(ast::Arrow)];

      // comp_expr
      char dummy3[sizeof(ast::CompExpression)];

      // variable_declaration
      char dummy4[sizeof(ast::Declaration)];

      // effect
      char dummy5[sizeof(ast::Effect)];

      // bool_expr
      // multiple
      // constant
      // variable
      // alg_expr
      char dummy6[sizeof(ast::Expression)];

      // where_expr
      char dummy7[sizeof(ast::Expression*)];

      // rule_label
      char dummy8[sizeof(ast::Id)];

      // link_state
      char dummy9[sizeof(ast::Link)];

      // perturbation_declaration
      char dummy10[sizeof(ast::Perturbation)];

      // alg_with_radius
      char dummy11[sizeof(ast::Radius)];

      // rate
      char dummy12[sizeof(ast::Rate)];

      // rule_expression
      char dummy13[sizeof(ast::Rule)];

      // lhs_rhs
      char dummy14[sizeof(ast::RuleSide)];

      // port_expression
      char dummy15[sizeof(ast::Site)];

      // multiple_mixture
      char dummy16[sizeof(ast::mix_pair)];

      // opt_string
      // string_or_pr_expr
      char dummy17[sizeof(ast::ptr_pair)];

      // join
      // boolean
      // rate_sep
      char dummy18[sizeof(bool)];

      // FLOAT
      char dummy19[sizeof(float)];

      // INT
      char dummy20[sizeof(int)];

      // mixture
      // non_empty_mixture
      char dummy21[sizeof(std::list<ast::Agent>)];

      // comp_list
      char dummy22[sizeof(std::list<ast::CompExpression>)];

      // effect_list
      char dummy23[sizeof(std::list<ast::Effect>)];

      // dimension
      char dummy24[sizeof(std::list<ast::Expression>)];

      // interface_expression
      char dummy25[sizeof(std::list<ast::Site>)];

      // token_expr
      // sum_token
      char dummy26[sizeof(std::list<ast::Token>)];

      // print_expr
      char dummy27[sizeof(std::list<ast::print_expr>)];

      // value_list
      // internal_state
      char dummy28[sizeof(std::list<std::string>)];

      // ID
      // LABEL
      // KAPPA_MRK
      // NAME
      // STRING
      char dummy29[sizeof(std::string)];
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
        END = 258,
        NEWLINE = 259,
        SEMICOLON = 260,
        AT = 261,
        ATD = 262,
        FIX = 263,
        OP_PAR = 264,
        CL_PAR = 265,
        OP_BRA = 266,
        CL_BRA = 267,
        COMMA = 268,
        DOT = 269,
        TYPE = 270,
        LAR = 271,
        OP_CUR = 272,
        CL_CUR = 273,
        JOIN = 274,
        FREE = 275,
        LOG = 276,
        PLUS = 277,
        MULT = 278,
        MINUS = 279,
        AND = 280,
        OR = 281,
        GREATER = 282,
        SMALLER = 283,
        EQUAL = 284,
        PERT = 285,
        INTRO = 286,
        DELETE = 287,
        DO = 288,
        SET = 289,
        UNTIL = 290,
        TRUE = 291,
        FALSE = 292,
        OBS = 293,
        KAPPA_RAR = 294,
        TRACK = 295,
        CPUTIME = 296,
        CONFIG = 297,
        REPEAT = 298,
        DIFF = 299,
        KAPPA_WLD = 300,
        KAPPA_SEMI = 301,
        SIGNATURE = 302,
        INFINITY = 303,
        TIME = 304,
        EVENT = 305,
        ACTIVITY = 306,
        NULL_EVENT = 307,
        PROD_EVENT = 308,
        INIT = 309,
        LET = 310,
        DIV = 311,
        PLOT = 312,
        SINUS = 313,
        COSINUS = 314,
        TAN = 315,
        ATAN = 316,
        COIN = 317,
        RAND_N = 318,
        SQRT = 319,
        EXPONENT = 320,
        POW = 321,
        ABS = 322,
        MODULO = 323,
        EMAX = 324,
        TMAX = 325,
        RAND_1 = 326,
        FLUX = 327,
        ASSIGN = 328,
        ASSIGN2 = 329,
        TOKEN = 330,
        KAPPA_LNK = 331,
        PIPE = 332,
        KAPPA_LRAR = 333,
        PRINT = 334,
        PRINTF = 335,
        MAX = 336,
        MIN = 337,
        INT = 338,
        ID = 339,
        LABEL = 340,
        KAPPA_MRK = 341,
        NAME = 342,
        FLOAT = 343,
        STRING = 344,
        STOP = 345,
        SNAPSHOT = 346,
        COMPARTMENT = 347,
        C_LINK = 348,
        TRANSPORT = 349,
        USE = 350
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Internal symbol number.
    typedef int symbol_number_type;

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

  basic_symbol (typename Base::kind_type t, const ast::Agent v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::Arrow v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::CompExpression v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::Declaration v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::Effect v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::Expression v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::Expression* v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::Id v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::Link v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::Perturbation v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::Radius v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::Rate v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::Rule v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::RuleSide v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::Site v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::mix_pair v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ast::ptr_pair v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const bool v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const float v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const int v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::list<ast::Agent> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::list<ast::CompExpression> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::list<ast::Effect> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::list<ast::Expression> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::list<ast::Site> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::list<ast::Token> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::list<ast::print_expr> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::list<std::string> v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l);


      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v,
                    const location_type& l);

      ~basic_symbol ();

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

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// -1 when this symbol is empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      enum { empty = 0 };

      /// The symbol type.
      /// -1 when this symbol is empty.
      token_number_type type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;

    // Symbol constructors declarations.
    static inline
    symbol_type
    make_END (const location_type& l);

    static inline
    symbol_type
    make_NEWLINE (const location_type& l);

    static inline
    symbol_type
    make_SEMICOLON (const location_type& l);

    static inline
    symbol_type
    make_AT (const location_type& l);

    static inline
    symbol_type
    make_ATD (const location_type& l);

    static inline
    symbol_type
    make_FIX (const location_type& l);

    static inline
    symbol_type
    make_OP_PAR (const location_type& l);

    static inline
    symbol_type
    make_CL_PAR (const location_type& l);

    static inline
    symbol_type
    make_OP_BRA (const location_type& l);

    static inline
    symbol_type
    make_CL_BRA (const location_type& l);

    static inline
    symbol_type
    make_COMMA (const location_type& l);

    static inline
    symbol_type
    make_DOT (const location_type& l);

    static inline
    symbol_type
    make_TYPE (const location_type& l);

    static inline
    symbol_type
    make_LAR (const location_type& l);

    static inline
    symbol_type
    make_OP_CUR (const location_type& l);

    static inline
    symbol_type
    make_CL_CUR (const location_type& l);

    static inline
    symbol_type
    make_JOIN (const location_type& l);

    static inline
    symbol_type
    make_FREE (const location_type& l);

    static inline
    symbol_type
    make_LOG (const location_type& l);

    static inline
    symbol_type
    make_PLUS (const location_type& l);

    static inline
    symbol_type
    make_MULT (const location_type& l);

    static inline
    symbol_type
    make_MINUS (const location_type& l);

    static inline
    symbol_type
    make_AND (const location_type& l);

    static inline
    symbol_type
    make_OR (const location_type& l);

    static inline
    symbol_type
    make_GREATER (const location_type& l);

    static inline
    symbol_type
    make_SMALLER (const location_type& l);

    static inline
    symbol_type
    make_EQUAL (const location_type& l);

    static inline
    symbol_type
    make_PERT (const location_type& l);

    static inline
    symbol_type
    make_INTRO (const location_type& l);

    static inline
    symbol_type
    make_DELETE (const location_type& l);

    static inline
    symbol_type
    make_DO (const location_type& l);

    static inline
    symbol_type
    make_SET (const location_type& l);

    static inline
    symbol_type
    make_UNTIL (const location_type& l);

    static inline
    symbol_type
    make_TRUE (const location_type& l);

    static inline
    symbol_type
    make_FALSE (const location_type& l);

    static inline
    symbol_type
    make_OBS (const location_type& l);

    static inline
    symbol_type
    make_KAPPA_RAR (const location_type& l);

    static inline
    symbol_type
    make_TRACK (const location_type& l);

    static inline
    symbol_type
    make_CPUTIME (const location_type& l);

    static inline
    symbol_type
    make_CONFIG (const location_type& l);

    static inline
    symbol_type
    make_REPEAT (const location_type& l);

    static inline
    symbol_type
    make_DIFF (const location_type& l);

    static inline
    symbol_type
    make_KAPPA_WLD (const location_type& l);

    static inline
    symbol_type
    make_KAPPA_SEMI (const location_type& l);

    static inline
    symbol_type
    make_SIGNATURE (const location_type& l);

    static inline
    symbol_type
    make_INFINITY (const location_type& l);

    static inline
    symbol_type
    make_TIME (const location_type& l);

    static inline
    symbol_type
    make_EVENT (const location_type& l);

    static inline
    symbol_type
    make_ACTIVITY (const location_type& l);

    static inline
    symbol_type
    make_NULL_EVENT (const location_type& l);

    static inline
    symbol_type
    make_PROD_EVENT (const location_type& l);

    static inline
    symbol_type
    make_INIT (const location_type& l);

    static inline
    symbol_type
    make_LET (const location_type& l);

    static inline
    symbol_type
    make_DIV (const location_type& l);

    static inline
    symbol_type
    make_PLOT (const location_type& l);

    static inline
    symbol_type
    make_SINUS (const location_type& l);

    static inline
    symbol_type
    make_COSINUS (const location_type& l);

    static inline
    symbol_type
    make_TAN (const location_type& l);

    static inline
    symbol_type
    make_ATAN (const location_type& l);

    static inline
    symbol_type
    make_COIN (const location_type& l);

    static inline
    symbol_type
    make_RAND_N (const location_type& l);

    static inline
    symbol_type
    make_SQRT (const location_type& l);

    static inline
    symbol_type
    make_EXPONENT (const location_type& l);

    static inline
    symbol_type
    make_POW (const location_type& l);

    static inline
    symbol_type
    make_ABS (const location_type& l);

    static inline
    symbol_type
    make_MODULO (const location_type& l);

    static inline
    symbol_type
    make_EMAX (const location_type& l);

    static inline
    symbol_type
    make_TMAX (const location_type& l);

    static inline
    symbol_type
    make_RAND_1 (const location_type& l);

    static inline
    symbol_type
    make_FLUX (const location_type& l);

    static inline
    symbol_type
    make_ASSIGN (const location_type& l);

    static inline
    symbol_type
    make_ASSIGN2 (const location_type& l);

    static inline
    symbol_type
    make_TOKEN (const location_type& l);

    static inline
    symbol_type
    make_KAPPA_LNK (const location_type& l);

    static inline
    symbol_type
    make_PIPE (const location_type& l);

    static inline
    symbol_type
    make_KAPPA_LRAR (const location_type& l);

    static inline
    symbol_type
    make_PRINT (const location_type& l);

    static inline
    symbol_type
    make_PRINTF (const location_type& l);

    static inline
    symbol_type
    make_MAX (const location_type& l);

    static inline
    symbol_type
    make_MIN (const location_type& l);

    static inline
    symbol_type
    make_INT (const int& v, const location_type& l);

    static inline
    symbol_type
    make_ID (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_LABEL (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_KAPPA_MRK (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_NAME (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_FLOAT (const float& v, const location_type& l);

    static inline
    symbol_type
    make_STRING (const std::string& v, const location_type& l);

    static inline
    symbol_type
    make_STOP (const location_type& l);

    static inline
    symbol_type
    make_SNAPSHOT (const location_type& l);

    static inline
    symbol_type
    make_COMPARTMENT (const location_type& l);

    static inline
    symbol_type
    make_C_LINK (const location_type& l);

    static inline
    symbol_type
    make_TRANSPORT (const location_type& l);

    static inline
    symbol_type
    make_USE (const location_type& l);


    /// Build a parser object.
     KappaParser  (grammar::KappaDriver &driver_yyarg);
    virtual ~ KappaParser  ();

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
     KappaParser  (const  KappaParser &);
     KappaParser & operator= (const  KappaParser &);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yytoken   the lookahead token type, or yyempty_.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         symbol_number_type yytoken) const;

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

    static const short int yypact_ninf_;
    static const short int yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short int yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short int yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short int yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const short int yytable_[];

  static const short int yycheck_[];

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

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// "empty" when empty.
      symbol_number_type type_get () const;

      enum { empty = 0 };

      /// The state.
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

    // Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 1148,     ///< Last index in yytable_.
      yynnts_ = 42,  ///< Number of nonterminal symbols.
      yyempty_ = -2,
      yyfinal_ = 2, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 97  ///< Number of tokens.
    };


    // User arguments.
    grammar::KappaDriver &driver;
  };

  // Symbol number corresponding to token number t.
  inline
   KappaParser ::token_number_type
   KappaParser ::yytranslate_ (token_type t)
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
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96
    };
    const unsigned int user_token_number_max_ = 351;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  inline
   KappaParser ::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
   KappaParser ::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
   KappaParser ::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
      switch (other.type_get ())
    {
      case 134: // agent_expression
        value.copy< ast::Agent > (other.value);
        break;

      case 127: // arrow
        value.copy< ast::Arrow > (other.value);
        break;

      case 104: // comp_expr
        value.copy< ast::CompExpression > (other.value);
        break;

      case 114: // variable_declaration
        value.copy< ast::Declaration > (other.value);
        break;

      case 111: // effect
        value.copy< ast::Effect > (other.value);
        break;

      case 115: // bool_expr
      case 118: // multiple
      case 119: // constant
      case 128: // variable
      case 129: // alg_expr
        value.copy< ast::Expression > (other.value);
        break;

      case 106: // where_expr
        value.copy< ast::Expression* > (other.value);
        break;

      case 120: // rule_label
        value.copy< ast::Id > (other.value);
        break;

      case 138: // link_state
        value.copy< ast::Link > (other.value);
        break;

      case 109: // perturbation_declaration
        value.copy< ast::Perturbation > (other.value);
        break;

      case 131: // alg_with_radius
        value.copy< ast::Radius > (other.value);
        break;

      case 130: // rate
        value.copy< ast::Rate > (other.value);
        break;

      case 126: // rule_expression
        value.copy< ast::Rule > (other.value);
        break;

      case 121: // lhs_rhs
        value.copy< ast::RuleSide > (other.value);
        break;

      case 136: // port_expression
        value.copy< ast::Site > (other.value);
        break;

      case 132: // multiple_mixture
        value.copy< ast::mix_pair > (other.value);
        break;

      case 116: // opt_string
      case 117: // string_or_pr_expr
        value.copy< ast::ptr_pair > (other.value);
        break;

      case 103: // join
      case 113: // boolean
      case 125: // rate_sep
        value.copy< bool > (other.value);
        break;

      case 88: // FLOAT
        value.copy< float > (other.value);
        break;

      case 83: // INT
        value.copy< int > (other.value);
        break;

      case 124: // mixture
      case 133: // non_empty_mixture
        value.copy< std::list<ast::Agent> > (other.value);
        break;

      case 108: // comp_list
        value.copy< std::list<ast::CompExpression> > (other.value);
        break;

      case 110: // effect_list
        value.copy< std::list<ast::Effect> > (other.value);
        break;

      case 105: // dimension
        value.copy< std::list<ast::Expression> > (other.value);
        break;

      case 135: // interface_expression
        value.copy< std::list<ast::Site> > (other.value);
        break;

      case 122: // token_expr
      case 123: // sum_token
        value.copy< std::list<ast::Token> > (other.value);
        break;

      case 112: // print_expr
        value.copy< std::list<ast::print_expr> > (other.value);
        break;

      case 107: // value_list
      case 137: // internal_state
        value.copy< std::list<std::string> > (other.value);
        break;

      case 84: // ID
      case 85: // LABEL
      case 86: // KAPPA_MRK
      case 87: // NAME
      case 89: // STRING
        value.copy< std::string > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
    {
      case 134: // agent_expression
        value.copy< ast::Agent > (v);
        break;

      case 127: // arrow
        value.copy< ast::Arrow > (v);
        break;

      case 104: // comp_expr
        value.copy< ast::CompExpression > (v);
        break;

      case 114: // variable_declaration
        value.copy< ast::Declaration > (v);
        break;

      case 111: // effect
        value.copy< ast::Effect > (v);
        break;

      case 115: // bool_expr
      case 118: // multiple
      case 119: // constant
      case 128: // variable
      case 129: // alg_expr
        value.copy< ast::Expression > (v);
        break;

      case 106: // where_expr
        value.copy< ast::Expression* > (v);
        break;

      case 120: // rule_label
        value.copy< ast::Id > (v);
        break;

      case 138: // link_state
        value.copy< ast::Link > (v);
        break;

      case 109: // perturbation_declaration
        value.copy< ast::Perturbation > (v);
        break;

      case 131: // alg_with_radius
        value.copy< ast::Radius > (v);
        break;

      case 130: // rate
        value.copy< ast::Rate > (v);
        break;

      case 126: // rule_expression
        value.copy< ast::Rule > (v);
        break;

      case 121: // lhs_rhs
        value.copy< ast::RuleSide > (v);
        break;

      case 136: // port_expression
        value.copy< ast::Site > (v);
        break;

      case 132: // multiple_mixture
        value.copy< ast::mix_pair > (v);
        break;

      case 116: // opt_string
      case 117: // string_or_pr_expr
        value.copy< ast::ptr_pair > (v);
        break;

      case 103: // join
      case 113: // boolean
      case 125: // rate_sep
        value.copy< bool > (v);
        break;

      case 88: // FLOAT
        value.copy< float > (v);
        break;

      case 83: // INT
        value.copy< int > (v);
        break;

      case 124: // mixture
      case 133: // non_empty_mixture
        value.copy< std::list<ast::Agent> > (v);
        break;

      case 108: // comp_list
        value.copy< std::list<ast::CompExpression> > (v);
        break;

      case 110: // effect_list
        value.copy< std::list<ast::Effect> > (v);
        break;

      case 105: // dimension
        value.copy< std::list<ast::Expression> > (v);
        break;

      case 135: // interface_expression
        value.copy< std::list<ast::Site> > (v);
        break;

      case 122: // token_expr
      case 123: // sum_token
        value.copy< std::list<ast::Token> > (v);
        break;

      case 112: // print_expr
        value.copy< std::list<ast::print_expr> > (v);
        break;

      case 107: // value_list
      case 137: // internal_state
        value.copy< std::list<std::string> > (v);
        break;

      case 84: // ID
      case 85: // LABEL
      case 86: // KAPPA_MRK
      case 87: // NAME
      case 89: // STRING
        value.copy< std::string > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::Agent v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::Arrow v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::CompExpression v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::Declaration v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::Effect v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::Expression v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::Expression* v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::Id v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::Link v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::Perturbation v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::Radius v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::Rate v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::Rule v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::RuleSide v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::Site v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::mix_pair v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ast::ptr_pair v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const bool v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const float v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const int v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::list<ast::Agent> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::list<ast::CompExpression> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::list<ast::Effect> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::list<ast::Expression> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::list<ast::Site> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::list<ast::Token> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::list<ast::print_expr> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::list<std::string> v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
   KappaParser ::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const std::string v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
   KappaParser ::basic_symbol<Base>::~basic_symbol ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
    {
      case 134: // agent_expression
        value.template destroy< ast::Agent > ();
        break;

      case 127: // arrow
        value.template destroy< ast::Arrow > ();
        break;

      case 104: // comp_expr
        value.template destroy< ast::CompExpression > ();
        break;

      case 114: // variable_declaration
        value.template destroy< ast::Declaration > ();
        break;

      case 111: // effect
        value.template destroy< ast::Effect > ();
        break;

      case 115: // bool_expr
      case 118: // multiple
      case 119: // constant
      case 128: // variable
      case 129: // alg_expr
        value.template destroy< ast::Expression > ();
        break;

      case 106: // where_expr
        value.template destroy< ast::Expression* > ();
        break;

      case 120: // rule_label
        value.template destroy< ast::Id > ();
        break;

      case 138: // link_state
        value.template destroy< ast::Link > ();
        break;

      case 109: // perturbation_declaration
        value.template destroy< ast::Perturbation > ();
        break;

      case 131: // alg_with_radius
        value.template destroy< ast::Radius > ();
        break;

      case 130: // rate
        value.template destroy< ast::Rate > ();
        break;

      case 126: // rule_expression
        value.template destroy< ast::Rule > ();
        break;

      case 121: // lhs_rhs
        value.template destroy< ast::RuleSide > ();
        break;

      case 136: // port_expression
        value.template destroy< ast::Site > ();
        break;

      case 132: // multiple_mixture
        value.template destroy< ast::mix_pair > ();
        break;

      case 116: // opt_string
      case 117: // string_or_pr_expr
        value.template destroy< ast::ptr_pair > ();
        break;

      case 103: // join
      case 113: // boolean
      case 125: // rate_sep
        value.template destroy< bool > ();
        break;

      case 88: // FLOAT
        value.template destroy< float > ();
        break;

      case 83: // INT
        value.template destroy< int > ();
        break;

      case 124: // mixture
      case 133: // non_empty_mixture
        value.template destroy< std::list<ast::Agent> > ();
        break;

      case 108: // comp_list
        value.template destroy< std::list<ast::CompExpression> > ();
        break;

      case 110: // effect_list
        value.template destroy< std::list<ast::Effect> > ();
        break;

      case 105: // dimension
        value.template destroy< std::list<ast::Expression> > ();
        break;

      case 135: // interface_expression
        value.template destroy< std::list<ast::Site> > ();
        break;

      case 122: // token_expr
      case 123: // sum_token
        value.template destroy< std::list<ast::Token> > ();
        break;

      case 112: // print_expr
        value.template destroy< std::list<ast::print_expr> > ();
        break;

      case 107: // value_list
      case 137: // internal_state
        value.template destroy< std::list<std::string> > ();
        break;

      case 84: // ID
      case 85: // LABEL
      case 86: // KAPPA_MRK
      case 87: // NAME
      case 89: // STRING
        value.template destroy< std::string > ();
        break;

      default:
        break;
    }

  }

  template <typename Base>
  inline
  void
   KappaParser ::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
    {
      case 134: // agent_expression
        value.move< ast::Agent > (s.value);
        break;

      case 127: // arrow
        value.move< ast::Arrow > (s.value);
        break;

      case 104: // comp_expr
        value.move< ast::CompExpression > (s.value);
        break;

      case 114: // variable_declaration
        value.move< ast::Declaration > (s.value);
        break;

      case 111: // effect
        value.move< ast::Effect > (s.value);
        break;

      case 115: // bool_expr
      case 118: // multiple
      case 119: // constant
      case 128: // variable
      case 129: // alg_expr
        value.move< ast::Expression > (s.value);
        break;

      case 106: // where_expr
        value.move< ast::Expression* > (s.value);
        break;

      case 120: // rule_label
        value.move< ast::Id > (s.value);
        break;

      case 138: // link_state
        value.move< ast::Link > (s.value);
        break;

      case 109: // perturbation_declaration
        value.move< ast::Perturbation > (s.value);
        break;

      case 131: // alg_with_radius
        value.move< ast::Radius > (s.value);
        break;

      case 130: // rate
        value.move< ast::Rate > (s.value);
        break;

      case 126: // rule_expression
        value.move< ast::Rule > (s.value);
        break;

      case 121: // lhs_rhs
        value.move< ast::RuleSide > (s.value);
        break;

      case 136: // port_expression
        value.move< ast::Site > (s.value);
        break;

      case 132: // multiple_mixture
        value.move< ast::mix_pair > (s.value);
        break;

      case 116: // opt_string
      case 117: // string_or_pr_expr
        value.move< ast::ptr_pair > (s.value);
        break;

      case 103: // join
      case 113: // boolean
      case 125: // rate_sep
        value.move< bool > (s.value);
        break;

      case 88: // FLOAT
        value.move< float > (s.value);
        break;

      case 83: // INT
        value.move< int > (s.value);
        break;

      case 124: // mixture
      case 133: // non_empty_mixture
        value.move< std::list<ast::Agent> > (s.value);
        break;

      case 108: // comp_list
        value.move< std::list<ast::CompExpression> > (s.value);
        break;

      case 110: // effect_list
        value.move< std::list<ast::Effect> > (s.value);
        break;

      case 105: // dimension
        value.move< std::list<ast::Expression> > (s.value);
        break;

      case 135: // interface_expression
        value.move< std::list<ast::Site> > (s.value);
        break;

      case 122: // token_expr
      case 123: // sum_token
        value.move< std::list<ast::Token> > (s.value);
        break;

      case 112: // print_expr
        value.move< std::list<ast::print_expr> > (s.value);
        break;

      case 107: // value_list
      case 137: // internal_state
        value.move< std::list<std::string> > (s.value);
        break;

      case 84: // ID
      case 85: // LABEL
      case 86: // KAPPA_MRK
      case 87: // NAME
      case 89: // STRING
        value.move< std::string > (s.value);
        break;

      default:
        break;
    }

    location = s.location;
  }

  // by_type.
  inline
   KappaParser ::by_type::by_type ()
     : type (empty)
  {}

  inline
   KappaParser ::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
   KappaParser ::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
   KappaParser ::by_type::move (by_type& that)
  {
    type = that.type;
    that.type = empty;
  }

  inline
  int
   KappaParser ::by_type::type_get () const
  {
    return type;
  }

  inline
   KappaParser ::token_type
   KappaParser ::by_type::token () const
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
  // Implementation of make_symbol for each symbol type.
   KappaParser ::symbol_type
   KappaParser ::make_END (const location_type& l)
  {
    return symbol_type (token::END, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_NEWLINE (const location_type& l)
  {
    return symbol_type (token::NEWLINE, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_SEMICOLON (const location_type& l)
  {
    return symbol_type (token::SEMICOLON, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_AT (const location_type& l)
  {
    return symbol_type (token::AT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_ATD (const location_type& l)
  {
    return symbol_type (token::ATD, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_FIX (const location_type& l)
  {
    return symbol_type (token::FIX, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_OP_PAR (const location_type& l)
  {
    return symbol_type (token::OP_PAR, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_CL_PAR (const location_type& l)
  {
    return symbol_type (token::CL_PAR, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_OP_BRA (const location_type& l)
  {
    return symbol_type (token::OP_BRA, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_CL_BRA (const location_type& l)
  {
    return symbol_type (token::CL_BRA, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_COMMA (const location_type& l)
  {
    return symbol_type (token::COMMA, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_DOT (const location_type& l)
  {
    return symbol_type (token::DOT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_TYPE (const location_type& l)
  {
    return symbol_type (token::TYPE, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_LAR (const location_type& l)
  {
    return symbol_type (token::LAR, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_OP_CUR (const location_type& l)
  {
    return symbol_type (token::OP_CUR, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_CL_CUR (const location_type& l)
  {
    return symbol_type (token::CL_CUR, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_JOIN (const location_type& l)
  {
    return symbol_type (token::JOIN, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_FREE (const location_type& l)
  {
    return symbol_type (token::FREE, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_LOG (const location_type& l)
  {
    return symbol_type (token::LOG, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_PLUS (const location_type& l)
  {
    return symbol_type (token::PLUS, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_MULT (const location_type& l)
  {
    return symbol_type (token::MULT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_MINUS (const location_type& l)
  {
    return symbol_type (token::MINUS, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_AND (const location_type& l)
  {
    return symbol_type (token::AND, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_OR (const location_type& l)
  {
    return symbol_type (token::OR, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_GREATER (const location_type& l)
  {
    return symbol_type (token::GREATER, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_SMALLER (const location_type& l)
  {
    return symbol_type (token::SMALLER, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_EQUAL (const location_type& l)
  {
    return symbol_type (token::EQUAL, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_PERT (const location_type& l)
  {
    return symbol_type (token::PERT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_INTRO (const location_type& l)
  {
    return symbol_type (token::INTRO, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_DELETE (const location_type& l)
  {
    return symbol_type (token::DELETE, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_DO (const location_type& l)
  {
    return symbol_type (token::DO, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_SET (const location_type& l)
  {
    return symbol_type (token::SET, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_UNTIL (const location_type& l)
  {
    return symbol_type (token::UNTIL, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_TRUE (const location_type& l)
  {
    return symbol_type (token::TRUE, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_FALSE (const location_type& l)
  {
    return symbol_type (token::FALSE, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_OBS (const location_type& l)
  {
    return symbol_type (token::OBS, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_KAPPA_RAR (const location_type& l)
  {
    return symbol_type (token::KAPPA_RAR, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_TRACK (const location_type& l)
  {
    return symbol_type (token::TRACK, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_CPUTIME (const location_type& l)
  {
    return symbol_type (token::CPUTIME, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_CONFIG (const location_type& l)
  {
    return symbol_type (token::CONFIG, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_REPEAT (const location_type& l)
  {
    return symbol_type (token::REPEAT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_DIFF (const location_type& l)
  {
    return symbol_type (token::DIFF, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_KAPPA_WLD (const location_type& l)
  {
    return symbol_type (token::KAPPA_WLD, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_KAPPA_SEMI (const location_type& l)
  {
    return symbol_type (token::KAPPA_SEMI, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_SIGNATURE (const location_type& l)
  {
    return symbol_type (token::SIGNATURE, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_INFINITY (const location_type& l)
  {
    return symbol_type (token::INFINITY, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_TIME (const location_type& l)
  {
    return symbol_type (token::TIME, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_EVENT (const location_type& l)
  {
    return symbol_type (token::EVENT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_ACTIVITY (const location_type& l)
  {
    return symbol_type (token::ACTIVITY, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_NULL_EVENT (const location_type& l)
  {
    return symbol_type (token::NULL_EVENT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_PROD_EVENT (const location_type& l)
  {
    return symbol_type (token::PROD_EVENT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_INIT (const location_type& l)
  {
    return symbol_type (token::INIT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_LET (const location_type& l)
  {
    return symbol_type (token::LET, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_DIV (const location_type& l)
  {
    return symbol_type (token::DIV, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_PLOT (const location_type& l)
  {
    return symbol_type (token::PLOT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_SINUS (const location_type& l)
  {
    return symbol_type (token::SINUS, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_COSINUS (const location_type& l)
  {
    return symbol_type (token::COSINUS, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_TAN (const location_type& l)
  {
    return symbol_type (token::TAN, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_ATAN (const location_type& l)
  {
    return symbol_type (token::ATAN, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_COIN (const location_type& l)
  {
    return symbol_type (token::COIN, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_RAND_N (const location_type& l)
  {
    return symbol_type (token::RAND_N, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_SQRT (const location_type& l)
  {
    return symbol_type (token::SQRT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_EXPONENT (const location_type& l)
  {
    return symbol_type (token::EXPONENT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_POW (const location_type& l)
  {
    return symbol_type (token::POW, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_ABS (const location_type& l)
  {
    return symbol_type (token::ABS, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_MODULO (const location_type& l)
  {
    return symbol_type (token::MODULO, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_EMAX (const location_type& l)
  {
    return symbol_type (token::EMAX, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_TMAX (const location_type& l)
  {
    return symbol_type (token::TMAX, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_RAND_1 (const location_type& l)
  {
    return symbol_type (token::RAND_1, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_FLUX (const location_type& l)
  {
    return symbol_type (token::FLUX, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_ASSIGN (const location_type& l)
  {
    return symbol_type (token::ASSIGN, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_ASSIGN2 (const location_type& l)
  {
    return symbol_type (token::ASSIGN2, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_TOKEN (const location_type& l)
  {
    return symbol_type (token::TOKEN, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_KAPPA_LNK (const location_type& l)
  {
    return symbol_type (token::KAPPA_LNK, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_PIPE (const location_type& l)
  {
    return symbol_type (token::PIPE, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_KAPPA_LRAR (const location_type& l)
  {
    return symbol_type (token::KAPPA_LRAR, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_PRINT (const location_type& l)
  {
    return symbol_type (token::PRINT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_PRINTF (const location_type& l)
  {
    return symbol_type (token::PRINTF, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_MAX (const location_type& l)
  {
    return symbol_type (token::MAX, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_MIN (const location_type& l)
  {
    return symbol_type (token::MIN, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_INT (const int& v, const location_type& l)
  {
    return symbol_type (token::INT, v, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_ID (const std::string& v, const location_type& l)
  {
    return symbol_type (token::ID, v, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_LABEL (const std::string& v, const location_type& l)
  {
    return symbol_type (token::LABEL, v, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_KAPPA_MRK (const std::string& v, const location_type& l)
  {
    return symbol_type (token::KAPPA_MRK, v, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_NAME (const std::string& v, const location_type& l)
  {
    return symbol_type (token::NAME, v, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_FLOAT (const float& v, const location_type& l)
  {
    return symbol_type (token::FLOAT, v, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_STRING (const std::string& v, const location_type& l)
  {
    return symbol_type (token::STRING, v, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_STOP (const location_type& l)
  {
    return symbol_type (token::STOP, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_SNAPSHOT (const location_type& l)
  {
    return symbol_type (token::SNAPSHOT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_COMPARTMENT (const location_type& l)
  {
    return symbol_type (token::COMPARTMENT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_C_LINK (const location_type& l)
  {
    return symbol_type (token::C_LINK, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_TRANSPORT (const location_type& l)
  {
    return symbol_type (token::TRANSPORT, l);
  }

   KappaParser ::symbol_type
   KappaParser ::make_USE (const location_type& l)
  {
    return symbol_type (token::USE, l);
  }



} // yy
#line 2741 "KappaParser.hpp" // lalr1.cc:372




#endif // !YY_YY_KAPPAPARSER_HPP_INCLUDED
