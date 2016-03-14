// A Bison parser, made by GNU Bison 3.0.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

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
// //                    "%code top" blocks.
#line 34 "kappaParser.y" // lalr1.cc:392

    #include "KappaLexer.h"
    #include "KappaParser.hpp"
    #include "location.hh"
    
	#define yylex(x) x.getNextToken()

	using namespace yy;

#line 44 "KappaParser.cpp" // lalr1.cc:392


// First part of user declarations.

#line 49 "KappaParser.cpp" // lalr1.cc:399

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "KappaParser.hpp"

// User implementation prologue.

#line 63 "KappaParser.cpp" // lalr1.cc:407
// Unqualified %code blocks.
#line 44 "kappaParser.y" // lalr1.cc:408

    #include "KappaDriver.h"

#line 69 "KappaParser.cpp" // lalr1.cc:408


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
#define yyclearin       (yyempty = true)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 155 "KappaParser.cpp" // lalr1.cc:474

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
   KappaParser ::yytnamerr_ (const char *yystr)
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
   KappaParser :: KappaParser  (grammar::KappaDriver &driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

   KappaParser ::~ KappaParser  ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
   KappaParser ::by_state::by_state ()
    : state (empty)
  {}

  inline
   KappaParser ::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
   KappaParser ::by_state::move (by_state& that)
  {
    state = that.state;
    that.state = empty;
  }

  inline
   KappaParser ::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
   KappaParser ::symbol_number_type
   KappaParser ::by_state::type_get () const
  {
    return state == empty ? 0 : yystos_[state];
  }

  inline
   KappaParser ::stack_symbol_type::stack_symbol_type ()
  {}


  inline
   KappaParser ::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 134: // agent_expression
        value.move< ast::Agent > (that.value);
        break;

      case 127: // arrow
        value.move< ast::Arrow > (that.value);
        break;

      case 104: // comp_expr
        value.move< ast::CompExpression > (that.value);
        break;

      case 114: // variable_declaration
        value.move< ast::Declaration > (that.value);
        break;

      case 111: // effect
        value.move< ast::Effect > (that.value);
        break;

      case 115: // bool_expr
      case 118: // multiple
      case 119: // constant
      case 128: // variable
      case 129: // alg_expr
        value.move< ast::Expression > (that.value);
        break;

      case 106: // where_expr
        value.move< ast::Expression* > (that.value);
        break;

      case 120: // rule_label
        value.move< ast::Id > (that.value);
        break;

      case 138: // link_state
        value.move< ast::Link > (that.value);
        break;

      case 109: // perturbation_declaration
        value.move< ast::Perturbation > (that.value);
        break;

      case 131: // alg_with_radius
        value.move< ast::Radius > (that.value);
        break;

      case 130: // rate
        value.move< ast::Rate > (that.value);
        break;

      case 126: // rule_expression
        value.move< ast::Rule > (that.value);
        break;

      case 121: // lhs_rhs
        value.move< ast::RuleSide > (that.value);
        break;

      case 136: // port_expression
        value.move< ast::Site > (that.value);
        break;

      case 132: // multiple_mixture
        value.move< ast::mix_pair > (that.value);
        break;

      case 116: // opt_string
      case 117: // string_or_pr_expr
        value.move< ast::ptr_pair > (that.value);
        break;

      case 103: // join
      case 113: // boolean
      case 125: // rate_sep
        value.move< bool > (that.value);
        break;

      case 88: // FLOAT
        value.move< float > (that.value);
        break;

      case 83: // INT
        value.move< int > (that.value);
        break;

      case 124: // mixture
      case 133: // non_empty_mixture
        value.move< std::list<ast::Agent> > (that.value);
        break;

      case 108: // comp_list
        value.move< std::list<ast::CompExpression> > (that.value);
        break;

      case 110: // effect_list
        value.move< std::list<ast::Effect> > (that.value);
        break;

      case 105: // dimension
        value.move< std::list<ast::Expression> > (that.value);
        break;

      case 135: // interface_expression
        value.move< std::list<ast::Site> > (that.value);
        break;

      case 122: // token_expr
      case 123: // sum_token
        value.move< std::list<ast::Token> > (that.value);
        break;

      case 112: // print_expr
        value.move< std::list<ast::print_expr> > (that.value);
        break;

      case 107: // value_list
      case 137: // internal_state
        value.move< std::list<std::string> > (that.value);
        break;

      case 84: // ID
      case 85: // LABEL
      case 86: // KAPPA_MRK
      case 87: // NAME
      case 89: // STRING
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty;
  }

  inline
   KappaParser ::stack_symbol_type&
   KappaParser ::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 134: // agent_expression
        value.copy< ast::Agent > (that.value);
        break;

      case 127: // arrow
        value.copy< ast::Arrow > (that.value);
        break;

      case 104: // comp_expr
        value.copy< ast::CompExpression > (that.value);
        break;

      case 114: // variable_declaration
        value.copy< ast::Declaration > (that.value);
        break;

      case 111: // effect
        value.copy< ast::Effect > (that.value);
        break;

      case 115: // bool_expr
      case 118: // multiple
      case 119: // constant
      case 128: // variable
      case 129: // alg_expr
        value.copy< ast::Expression > (that.value);
        break;

      case 106: // where_expr
        value.copy< ast::Expression* > (that.value);
        break;

      case 120: // rule_label
        value.copy< ast::Id > (that.value);
        break;

      case 138: // link_state
        value.copy< ast::Link > (that.value);
        break;

      case 109: // perturbation_declaration
        value.copy< ast::Perturbation > (that.value);
        break;

      case 131: // alg_with_radius
        value.copy< ast::Radius > (that.value);
        break;

      case 130: // rate
        value.copy< ast::Rate > (that.value);
        break;

      case 126: // rule_expression
        value.copy< ast::Rule > (that.value);
        break;

      case 121: // lhs_rhs
        value.copy< ast::RuleSide > (that.value);
        break;

      case 136: // port_expression
        value.copy< ast::Site > (that.value);
        break;

      case 132: // multiple_mixture
        value.copy< ast::mix_pair > (that.value);
        break;

      case 116: // opt_string
      case 117: // string_or_pr_expr
        value.copy< ast::ptr_pair > (that.value);
        break;

      case 103: // join
      case 113: // boolean
      case 125: // rate_sep
        value.copy< bool > (that.value);
        break;

      case 88: // FLOAT
        value.copy< float > (that.value);
        break;

      case 83: // INT
        value.copy< int > (that.value);
        break;

      case 124: // mixture
      case 133: // non_empty_mixture
        value.copy< std::list<ast::Agent> > (that.value);
        break;

      case 108: // comp_list
        value.copy< std::list<ast::CompExpression> > (that.value);
        break;

      case 110: // effect_list
        value.copy< std::list<ast::Effect> > (that.value);
        break;

      case 105: // dimension
        value.copy< std::list<ast::Expression> > (that.value);
        break;

      case 135: // interface_expression
        value.copy< std::list<ast::Site> > (that.value);
        break;

      case 122: // token_expr
      case 123: // sum_token
        value.copy< std::list<ast::Token> > (that.value);
        break;

      case 112: // print_expr
        value.copy< std::list<ast::print_expr> > (that.value);
        break;

      case 107: // value_list
      case 137: // internal_state
        value.copy< std::list<std::string> > (that.value);
        break;

      case 84: // ID
      case 85: // LABEL
      case 86: // KAPPA_MRK
      case 87: // NAME
      case 89: // STRING
        value.copy< std::string > (that.value);
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
   KappaParser ::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
   KappaParser ::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
   KappaParser ::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
   KappaParser ::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
   KappaParser ::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
   KappaParser ::debug_stream () const
  {
    return *yycdebug_;
  }

  void
   KappaParser ::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


   KappaParser ::debug_level_type
   KappaParser ::debug_level () const
  {
    return yydebug_;
  }

  void
   KappaParser ::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline  KappaParser ::state_type
   KappaParser ::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
   KappaParser ::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
   KappaParser ::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
   KappaParser ::parse ()
  {
    /// Whether yyla contains a lookahead.
    bool yyempty = true;

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


    // User initialization code.
    #line 12 "kappaParser.y" // lalr1.cc:725
{
	// Initialize the initial location.
	//@$.initialize(driver.getCurrentFileName(),1,1);
	//yydebug_ = 1;
}

#line 679 "KappaParser.cpp" // lalr1.cc:725

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
    if (yyempty)
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (driver));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
        yyempty = false;
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

    // Discard the token being shifted.
    yyempty = true;

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
      case 134: // agent_expression
        yylhs.value.build< ast::Agent > ();
        break;

      case 127: // arrow
        yylhs.value.build< ast::Arrow > ();
        break;

      case 104: // comp_expr
        yylhs.value.build< ast::CompExpression > ();
        break;

      case 114: // variable_declaration
        yylhs.value.build< ast::Declaration > ();
        break;

      case 111: // effect
        yylhs.value.build< ast::Effect > ();
        break;

      case 115: // bool_expr
      case 118: // multiple
      case 119: // constant
      case 128: // variable
      case 129: // alg_expr
        yylhs.value.build< ast::Expression > ();
        break;

      case 106: // where_expr
        yylhs.value.build< ast::Expression* > ();
        break;

      case 120: // rule_label
        yylhs.value.build< ast::Id > ();
        break;

      case 138: // link_state
        yylhs.value.build< ast::Link > ();
        break;

      case 109: // perturbation_declaration
        yylhs.value.build< ast::Perturbation > ();
        break;

      case 131: // alg_with_radius
        yylhs.value.build< ast::Radius > ();
        break;

      case 130: // rate
        yylhs.value.build< ast::Rate > ();
        break;

      case 126: // rule_expression
        yylhs.value.build< ast::Rule > ();
        break;

      case 121: // lhs_rhs
        yylhs.value.build< ast::RuleSide > ();
        break;

      case 136: // port_expression
        yylhs.value.build< ast::Site > ();
        break;

      case 132: // multiple_mixture
        yylhs.value.build< ast::mix_pair > ();
        break;

      case 116: // opt_string
      case 117: // string_or_pr_expr
        yylhs.value.build< ast::ptr_pair > ();
        break;

      case 103: // join
      case 113: // boolean
      case 125: // rate_sep
        yylhs.value.build< bool > ();
        break;

      case 88: // FLOAT
        yylhs.value.build< float > ();
        break;

      case 83: // INT
        yylhs.value.build< int > ();
        break;

      case 124: // mixture
      case 133: // non_empty_mixture
        yylhs.value.build< std::list<ast::Agent> > ();
        break;

      case 108: // comp_list
        yylhs.value.build< std::list<ast::CompExpression> > ();
        break;

      case 110: // effect_list
        yylhs.value.build< std::list<ast::Effect> > ();
        break;

      case 105: // dimension
        yylhs.value.build< std::list<ast::Expression> > ();
        break;

      case 135: // interface_expression
        yylhs.value.build< std::list<ast::Site> > ();
        break;

      case 122: // token_expr
      case 123: // sum_token
        yylhs.value.build< std::list<ast::Token> > ();
        break;

      case 112: // print_expr
        yylhs.value.build< std::list<ast::print_expr> > ();
        break;

      case 107: // value_list
      case 137: // internal_state
        yylhs.value.build< std::list<std::string> > ();
        break;

      case 84: // ID
      case 85: // LABEL
      case 86: // KAPPA_MRK
      case 87: // NAME
      case 89: // STRING
        yylhs.value.build< std::string > ();
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
  case 3:
#line 106 "kappaParser.y" // lalr1.cc:847
    {}
#line 923 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 4:
#line 110 "kappaParser.y" // lalr1.cc:847
    {}
#line 929 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 5:
#line 111 "kappaParser.y" // lalr1.cc:847
    {return 0;}
#line 935 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 7:
#line 116 "kappaParser.y" // lalr1.cc:847
    {}
#line 941 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 8:
#line 118 "kappaParser.y" // lalr1.cc:847
    {}
#line 947 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 9:
#line 120 "kappaParser.y" // lalr1.cc:847
    {}
#line 953 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 11:
#line 127 "kappaParser.y" // lalr1.cc:847
    {}
#line 959 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 12:
#line 129 "kappaParser.y" // lalr1.cc:847
    {cout<<"Hola";}
#line 965 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 13:
#line 131 "kappaParser.y" // lalr1.cc:847
    {}
#line 971 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 14:
#line 133 "kappaParser.y" // lalr1.cc:847
    {}
#line 977 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 15:
#line 135 "kappaParser.y" // lalr1.cc:847
    {}
#line 983 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 16:
#line 137 "kappaParser.y" // lalr1.cc:847
    {}
#line 989 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 17:
#line 139 "kappaParser.y" // lalr1.cc:847
    {}
#line 995 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 18:
#line 141 "kappaParser.y" // lalr1.cc:847
    {}
#line 1001 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 19:
#line 143 "kappaParser.y" // lalr1.cc:847
    {}
#line 1007 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 20:
#line 145 "kappaParser.y" // lalr1.cc:847
    {driver.addDeclaration(yystack_[0].value.as< ast::Declaration > ());}
#line 1013 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 21:
#line 147 "kappaParser.y" // lalr1.cc:847
    {}
#line 1019 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 22:
#line 149 "kappaParser.y" // lalr1.cc:847
    {}
#line 1025 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 23:
#line 151 "kappaParser.y" // lalr1.cc:847
    {}
#line 1031 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 24:
#line 153 "kappaParser.y" // lalr1.cc:847
    {}
#line 1037 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 25:
#line 155 "kappaParser.y" // lalr1.cc:847
    {}
#line 1043 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 26:
#line 157 "kappaParser.y" // lalr1.cc:847
    {}
#line 1049 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 27:
#line 159 "kappaParser.y" // lalr1.cc:847
    {}
#line 1055 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 29:
#line 164 "kappaParser.y" // lalr1.cc:847
    {}
#line 1061 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 30:
#line 166 "kappaParser.y" // lalr1.cc:847
    {}
#line 1067 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 31:
#line 168 "kappaParser.y" // lalr1.cc:847
    {}
#line 1073 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 32:
#line 175 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< bool > ()=true;}
#line 1079 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 33:
#line 177 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< bool > ()=true;}
#line 1085 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 34:
#line 179 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< bool > ()=false;}
#line 1091 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 35:
#line 183 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::CompExpression > ()=ast::CompExpression(yystack_[2].value.as< std::string > (),std::list<ast::Expression>(),NULL,yystack_[2].location);}
#line 1097 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 36:
#line 188 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::Expression> > ()=std::list<ast::Expression>();}
#line 1103 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 37:
#line 190 "kappaParser.y" // lalr1.cc:847
    {
		yystack_[0].value.as< std::list<ast::Expression> > ().push_front(yystack_[2].value.as< ast::Expression > ());
 		yylhs.value.as< std::list<ast::Expression> > ()=yystack_[0].value.as< std::list<ast::Expression> > ();
	}
#line 1112 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 38:
#line 198 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression* > ()=NULL;}
#line 1118 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 39:
#line 200 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression* > ()=&yystack_[1].value.as< ast::Expression > ();}
#line 1124 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 41:
#line 226 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<std::string> > ()=std::list<std::string>(1,yystack_[0].value.as< std::string > ()); }
#line 1130 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 42:
#line 228 "kappaParser.y" // lalr1.cc:847
    {
		yystack_[0].value.as< std::list<std::string> > ().push_front(yystack_[1].value.as< std::string > ());
		yylhs.value.as< std::list<std::string> > ()=yystack_[0].value.as< std::list<std::string> > ();
	 }
#line 1139 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 43:
#line 235 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::CompExpression> > ()=std::list<ast::CompExpression>(); }
#line 1145 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 44:
#line 237 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::CompExpression> > ()=std::list<ast::CompExpression>(); }
#line 1151 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 45:
#line 239 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::CompExpression> > ()=std::list<ast::CompExpression>(1,yystack_[0].value.as< ast::CompExpression > ()); }
#line 1157 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 46:
#line 241 "kappaParser.y" // lalr1.cc:847
    { 
		yystack_[0].value.as< std::list<ast::CompExpression> > ().push_front(yystack_[1].value.as< ast::CompExpression > ());
 		yylhs.value.as< std::list<ast::CompExpression> > ()=yystack_[0].value.as< std::list<ast::CompExpression> > (); 
	}
#line 1166 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 47:
#line 246 "kappaParser.y" // lalr1.cc:847
    {	
		yystack_[0].value.as< std::list<ast::CompExpression> > ().push_front(yystack_[2].value.as< ast::CompExpression > ());
		yylhs.value.as< std::list<ast::CompExpression> > ()=yystack_[0].value.as< std::list<ast::CompExpression> > (); 
	}
#line 1175 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 49:
#line 255 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Perturbation > ()=yystack_[1].value.as< ast::Perturbation > ();}
#line 1181 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 50:
#line 256 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Perturbation > ()=ast::Perturbation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< std::list<ast::Effect> > (),yystack_[1].location);}
#line 1187 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 51:
#line 258 "kappaParser.y" // lalr1.cc:847
    {
		// temporal, preguntarle a naxo
		cout<<"Deprecated perturbation syntax: 'set' keyword is replaced by 'do'"<<endl;
		yylhs.value.as< ast::Perturbation > ()=ast::Perturbation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< std::list<ast::Effect> > (),yystack_[1].location);
	}
#line 1197 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 53:
#line 266 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::Effect> > ()=yystack_[1].value.as< std::list<ast::Effect> > ();}
#line 1203 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 54:
#line 267 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::Effect> > ()=std::list<ast::Effect>(1,yystack_[0].value.as< ast::Effect > ());}
#line 1209 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 55:
#line 269 "kappaParser.y" // lalr1.cc:847
    {
		yystack_[0].value.as< std::list<ast::Effect> > ().push_front(yystack_[2].value.as< ast::Effect > ());
		yylhs.value.as< std::list<ast::Effect> > ()=yystack_[0].value.as< std::list<ast::Effect> > ();
	}
#line 1218 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 57:
#line 277 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::UPDATE,std::string(yystack_[2].value.as< std::string > ()),yystack_[0].value.as< ast::Expression > (),yystack_[1].location);}
#line 1224 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 58:
#line 279 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::UPDATE,std::string(yystack_[1].value.as< std::string > ()),yystack_[0].value.as< ast::Expression > (),yystack_[2].location);}
#line 1230 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 59:
#line 281 "kappaParser.y" // lalr1.cc:847
    {
		if(yystack_[0].value.as< bool > ())
			yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::CFLOW   ,std::string(yystack_[1].value.as< std::string > ()),yystack_[2].location);
		else 
			yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::CFLOWOFF,std::string(yystack_[1].value.as< std::string > ()),yystack_[2].location);
	}
#line 1241 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 60:
#line 289 "kappaParser.y" // lalr1.cc:847
    {
		ast::Effect::Action action= yystack_[0].value.as< bool > () ? ast::Effect::FLUX : ast::Effect::FLUXOFF;

		if(yystack_[1].value.as< ast::ptr_pair > ().str   != NULL) 
			yylhs.value.as< ast::Effect > ()=ast::Effect(action,std::list<ast::print_expr>(1,ast::print_expr(*(yystack_[1].value.as< ast::ptr_pair > ().str),yystack_[1].location)),yystack_[2].location);
		else if(yystack_[1].value.as< ast::ptr_pair > ().pexpr != NULL) 
			yylhs.value.as< ast::Effect > ()=ast::Effect(action,*(yystack_[1].value.as< ast::ptr_pair > ().pexpr),yystack_[2].location);
		else
			yylhs.value.as< ast::Effect > ()=ast::Effect(action,std::list<ast::print_expr>(),yystack_[2].location);
	}
#line 1256 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 61:
#line 300 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::INTRO,yystack_[0].value.as< ast::mix_pair > ().alg,yystack_[0].value.as< ast::mix_pair > ().mix,yystack_[1].location);}
#line 1262 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 62:
#line 302 "kappaParser.y" // lalr1.cc:847
    {}
#line 1268 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 63:
#line 304 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::DELETE,yystack_[0].value.as< ast::mix_pair > ().alg,yystack_[0].value.as< ast::mix_pair > ().mix,yystack_[1].location);}
#line 1274 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 64:
#line 306 "kappaParser.y" // lalr1.cc:847
    {}
#line 1280 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 65:
#line 308 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::UPDATE_TOK,yystack_[2].value.as< std::string > (),yystack_[0].value.as< ast::Expression > (),yystack_[1].location);}
#line 1286 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 66:
#line 310 "kappaParser.y" // lalr1.cc:847
    {	
		if(yystack_[0].value.as< ast::ptr_pair > ().str   != NULL) 
			yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::SNAPSHOT,std::list<ast::print_expr>(1,ast::print_expr(*(yystack_[0].value.as< ast::ptr_pair > ().str),yystack_[0].location)),yystack_[1].location);
		else if(yystack_[0].value.as< ast::ptr_pair > ().pexpr != NULL) 
			yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::SNAPSHOT,*(yystack_[0].value.as< ast::ptr_pair > ().pexpr),yystack_[1].location);
		else
			yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::SNAPSHOT,std::list<ast::print_expr>(),yystack_[1].location);
	}
#line 1299 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 67:
#line 319 "kappaParser.y" // lalr1.cc:847
    {
		if(yystack_[0].value.as< ast::ptr_pair > ().str   != NULL) 
			yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::STOP,std::list<ast::print_expr>(1,ast::print_expr(*(yystack_[0].value.as< ast::ptr_pair > ().str),yystack_[0].location)),yystack_[1].location);
		else if(yystack_[0].value.as< ast::ptr_pair > ().pexpr != NULL) 
			yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::STOP,*(yystack_[0].value.as< ast::ptr_pair > ().pexpr),yystack_[1].location);
		else
			yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::STOP,std::list<ast::print_expr>(),yystack_[1].location);

	}
#line 1313 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 68:
#line 329 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::PRINT,std::list<ast::print_expr>(),yystack_[1].value.as< std::list<ast::print_expr> > (),yystack_[3].location);}
#line 1319 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 69:
#line 331 "kappaParser.y" // lalr1.cc:847
    {
		if(yystack_[3].value.as< ast::ptr_pair > ().str   != NULL) 
			yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::PRINT,std::list<ast::print_expr>(1,ast::print_expr(*(yystack_[3].value.as< ast::ptr_pair > ().str),yystack_[3].location)),yystack_[1].value.as< std::list<ast::print_expr> > (),yystack_[4].location);
		else if(yystack_[3].value.as< ast::ptr_pair > ().pexpr != NULL) 
			yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::PRINT,*(yystack_[3].value.as< ast::ptr_pair > ().pexpr),yystack_[1].value.as< std::list<ast::print_expr> > (),yystack_[4].location);
		else
			yylhs.value.as< ast::Effect > ()=ast::Effect(ast::Effect::PRINT,std::list<ast::print_expr>(),yystack_[1].value.as< std::list<ast::print_expr> > (),yystack_[4].location);
	}
#line 1332 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 70:
#line 344 "kappaParser.y" // lalr1.cc:847
    { yylhs.value.as< std::list<ast::print_expr> > ()=std::list<ast::print_expr>(); }
#line 1338 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 71:
#line 346 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::print_expr> > ()=std::list<ast::print_expr>(1,ast::print_expr(yystack_[0].value.as< std::string > (),yystack_[0].location)); }
#line 1344 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 72:
#line 348 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::print_expr> > ()=std::list<ast::print_expr>(1,ast::print_expr(yystack_[0].value.as< ast::Expression > (),yystack_[0].location)); }
#line 1350 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 73:
#line 350 "kappaParser.y" // lalr1.cc:847
    {
		yystack_[0].value.as< std::list<ast::print_expr> > ().push_front(ast::print_expr(yystack_[2].value.as< std::string > (),yystack_[2].location));
		yylhs.value.as< std::list<ast::print_expr> > ()=yystack_[0].value.as< std::list<ast::print_expr> > ();
	}
#line 1359 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 74:
#line 355 "kappaParser.y" // lalr1.cc:847
    {
		yystack_[0].value.as< std::list<ast::print_expr> > ().push_front(ast::print_expr(yystack_[2].value.as< ast::Expression > (),yystack_[2].location));
		yylhs.value.as< std::list<ast::print_expr> > ()=yystack_[0].value.as< std::list<ast::print_expr> > ();
	}
#line 1368 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 76:
#line 362 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< bool > ()=true;}
#line 1374 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 77:
#line 363 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< bool > ()=false;}
#line 1380 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 78:
#line 368 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Declaration > () = ast::Declaration(ast::Id(yystack_[1].value.as< std::string > (),yystack_[1].location),yystack_[0].value.as< std::list<ast::Agent> > (),yylhs.location);}
#line 1386 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 79:
#line 370 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Declaration > () = ast::Declaration(ast::Id(yystack_[1].value.as< std::string > (),yystack_[1].location),yystack_[0].value.as< ast::Expression > (),yylhs.location);}
#line 1392 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 80:
#line 372 "kappaParser.y" // lalr1.cc:847
    {}
#line 1398 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 82:
#line 377 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > ()=yystack_[1].value.as< ast::Expression > ();}
#line 1404 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 83:
#line 379 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BoolOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BoolOperation::AND,yystack_[1].location);}
#line 1410 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 84:
#line 381 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BoolOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BoolOperation::OR,yystack_[1].location);}
#line 1416 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 85:
#line 383 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BoolOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BoolOperation::GREATER,yystack_[1].location);}
#line 1422 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 86:
#line 385 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BoolOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BoolOperation::SMALLER,yystack_[1].location);}
#line 1428 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 87:
#line 387 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BoolOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BoolOperation::EQUAL,yystack_[1].location);}
#line 1434 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 88:
#line 389 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BoolOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BoolOperation::DIFF,yystack_[1].location);}
#line 1440 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 89:
#line 391 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Bool(true,yystack_[0].location);}
#line 1446 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 90:
#line 393 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Bool(false,yystack_[0].location);}
#line 1452 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 91:
#line 397 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::ptr_pair > ()=(ast::ptr_pair(NULL,NULL));}
#line 1458 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 92:
#line 398 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::ptr_pair > ()=(ast::ptr_pair(&yystack_[0].value.as< std::string > (),NULL));}
#line 1464 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 93:
#line 399 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::ptr_pair > ()=(ast:: ptr_pair(NULL,&yystack_[1].value.as< std::list<ast::print_expr> > ()));}
#line 1470 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 95:
#line 403 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::ptr_pair > ()=(ast::ptr_pair(&yystack_[0].value.as< std::string > (),NULL));}
#line 1476 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 96:
#line 404 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::ptr_pair > ()=(ast::ptr_pair(NULL,&yystack_[1].value.as< std::list<ast::print_expr> > ()));}
#line 1482 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 98:
#line 409 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(yystack_[0].value.as< int > (),yystack_[0].location);}
#line 1488 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 99:
#line 410 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(yystack_[0].value.as< float > (),yystack_[0].location);}
#line 1494 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 100:
#line 411 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Var(yystack_[0].value.as< std::string > (),ast::Var::VAR,yystack_[0].location); }
#line 1500 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 101:
#line 416 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(ast::Const::INF,yystack_[0].location);}
#line 1506 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 102:
#line 418 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(yystack_[0].value.as< float > (),yystack_[0].location);}
#line 1512 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 103:
#line 420 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(yystack_[0].value.as< int > (),yystack_[0].location);}
#line 1518 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 104:
#line 422 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(ast::Const::EMAX,yystack_[0].location);}
#line 1524 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 106:
#line 427 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Id > ()=ast::Id();}
#line 1530 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 107:
#line 429 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Id > ()=ast::Id(yystack_[0].value.as< std::string > (),yystack_[0].location);}
#line 1536 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 108:
#line 433 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::RuleSide > ()=ast::RuleSide(yystack_[1].value.as< std::list<ast::Agent> > (),yystack_[0].value.as< std::list<ast::Token> > ());}
#line 1542 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 109:
#line 437 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::Token> > ()=std::list<ast::Token>();}
#line 1548 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 110:
#line 438 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::Token> > ()=yystack_[0].value.as< std::list<ast::Token> > ();}
#line 1554 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 111:
#line 440 "kappaParser.y" // lalr1.cc:847
    {}
#line 1560 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 113:
#line 445 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::Token> > ()=yystack_[1].value.as< std::list<ast::Token> > ();}
#line 1566 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 114:
#line 447 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::Token> > ()=std::list<ast::Token>(1,ast::Token(yystack_[2].value.as< ast::Expression > (),ast::Id(yystack_[0].value.as< std::string > ())));}
#line 1572 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 115:
#line 449 "kappaParser.y" // lalr1.cc:847
    {
		yystack_[0].value.as< std::list<ast::Token> > ().push_front(ast::Token(yystack_[4].value.as< ast::Expression > (),ast::Id(yystack_[2].value.as< std::string > ())));
		yylhs.value.as< std::list<ast::Token> > ()=yystack_[0].value.as< std::list<ast::Token> > ();
	}
#line 1581 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 116:
#line 456 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::Agent> > ()=std::list<ast::Agent>();}
#line 1587 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 117:
#line 458 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::Agent> > ()=yystack_[0].value.as< std::list<ast::Agent> > ();}
#line 1593 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 119:
#line 462 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< bool > ()=false;}
#line 1599 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 120:
#line 463 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< bool > ()=true;}
#line 1605 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 122:
#line 470 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Rule > ()=ast::Rule(yystack_[5].value.as< ast::Id > (),yystack_[4].value.as< ast::RuleSide > (),yystack_[2].value.as< ast::RuleSide > (),yystack_[3].value.as< ast::Arrow > (),yystack_[0].value.as< ast::Rate > (),yystack_[5].location);}
#line 1611 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 123:
#line 472 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Rule > ()=ast::Rule();}
#line 1617 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 124:
#line 479 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Arrow > ()=ast::Arrow(ast::Arrow::RIGHT,yystack_[0].location);}
#line 1623 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 125:
#line 481 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Arrow > ()=ast::Arrow(ast::Arrow::BI,yystack_[0].location);}
#line 1629 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 126:
#line 486 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(ast::Const::INF,yystack_[0].location);}
#line 1635 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 127:
#line 488 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(yystack_[0].value.as< float > (),yystack_[0].location);}
#line 1641 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 128:
#line 490 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(yystack_[0].value.as< int > (),yystack_[0].location);}
#line 1647 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 129:
#line 492 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(ast::Const::EMAX,yystack_[0].location);}
#line 1653 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 130:
#line 494 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(ast::Const::TMAX,yystack_[0].location);}
#line 1659 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 131:
#line 499 "kappaParser.y" // lalr1.cc:847
    {ast::Var(yystack_[1].value.as< std::string > (),ast::Var::TOKEN,yylhs.location);}
#line 1665 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 132:
#line 501 "kappaParser.y" // lalr1.cc:847
    {ast::Var(yystack_[0].value.as< std::string > (),ast::Var::VAR,yylhs.location);}
#line 1671 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 133:
#line 503 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Var(ast::Var::TIME,yylhs.location);}
#line 1677 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 134:
#line 505 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Var(ast::Var::EVENT,yylhs.location);}
#line 1683 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 135:
#line 507 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Var(ast::Var::NULL_EVENT,yylhs.location);}
#line 1689 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 136:
#line 509 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Var(ast::Var::PROD_EVENT,yylhs.location);}
#line 1695 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 137:
#line 511 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Var(ast::Var::ACTIVITY,yylhs.location);}
#line 1701 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 138:
#line 513 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(ast::Var::CPUTIME,yystack_[0].location);}
#line 1707 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 139:
#line 518 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = yystack_[1].value.as< ast::Expression > ();}
#line 1713 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 140:
#line 520 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = yystack_[0].value.as< ast::Expression > ();}
#line 1719 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 141:
#line 522 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = yystack_[0].value.as< ast::Expression > ();}
#line 1725 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 142:
#line 524 "kappaParser.y" // lalr1.cc:847
    {}
#line 1731 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 143:
#line 526 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::MULT,yystack_[1].location);}
#line 1737 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 144:
#line 528 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::SUM,yystack_[1].location);}
#line 1743 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 145:
#line 530 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::DIV,yystack_[1].location);}
#line 1749 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 146:
#line 532 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::MINUS,yystack_[1].location);}
#line 1755 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 147:
#line 534 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::POW,yystack_[1].location);}
#line 1761 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 148:
#line 536 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::MODULO,yystack_[1].location);}
#line 1767 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 149:
#line 538 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[1].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::MAX,yystack_[1].location);}
#line 1773 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 150:
#line 540 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[1].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::MIN,yystack_[1].location);}
#line 1779 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 151:
#line 542 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::UnaryOperation(yystack_[0].value.as< ast::Expression > (),ast::UnaryOperation::EXPONENT,yystack_[0].location);}
#line 1785 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 152:
#line 544 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::UnaryOperation(yystack_[0].value.as< ast::Expression > (),ast::UnaryOperation::SINUS,yystack_[0].location);}
#line 1791 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 153:
#line 546 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::UnaryOperation(yystack_[0].value.as< ast::Expression > (),ast::UnaryOperation::COSINUS,yystack_[0].location);}
#line 1797 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 154:
#line 548 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::UnaryOperation(yystack_[0].value.as< ast::Expression > (),ast::UnaryOperation::TAN,yystack_[0].location);}
#line 1803 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 155:
#line 550 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::UnaryOperation(yystack_[0].value.as< ast::Expression > (),ast::UnaryOperation::ABS,yystack_[0].location);}
#line 1809 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 156:
#line 552 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::UnaryOperation(yystack_[0].value.as< ast::Expression > (),ast::UnaryOperation::SQRT,yystack_[0].location);}
#line 1815 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 157:
#line 554 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::UnaryOperation(yystack_[0].value.as< ast::Expression > (),ast::UnaryOperation::LOG,yystack_[0].location);}
#line 1821 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 158:
#line 557 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::UnaryOperation(yystack_[0].value.as< ast::Expression > (),ast::UnaryOperation::ATAN,yystack_[0].location);}
#line 1827 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 159:
#line 559 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::UnaryOperation(yystack_[0].value.as< ast::Expression > (),ast::UnaryOperation::COIN,yystack_[0].location);}
#line 1833 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 160:
#line 561 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::UnaryOperation(yystack_[0].value.as< ast::Expression > (),ast::UnaryOperation::RAND_N,yystack_[0].location);}
#line 1839 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 161:
#line 563 "kappaParser.y" // lalr1.cc:847
    {}
#line 1845 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 163:
#line 568 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Rate > ()=ast::Rate(yystack_[3].value.as< ast::Expression > (),&yystack_[1].value.as< ast::Radius > (),NULL);}
#line 1851 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 164:
#line 570 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Rate > ()=ast::Rate(yystack_[0].value.as< ast::Expression > (),NULL,NULL);}
#line 1857 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 165:
#line 572 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Rate > ()=ast::Rate(yystack_[2].value.as< ast::Expression > (),NULL,&yystack_[0].value.as< ast::Expression > ());}
#line 1863 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 167:
#line 576 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Radius > ()=ast::Radius(yystack_[0].value.as< ast::Expression > ());}
#line 1869 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 168:
#line 577 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Radius > ()=ast::Radius(yystack_[2].value.as< ast::Expression > (),&yystack_[0].value.as< ast::Expression > ());}
#line 1875 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 170:
#line 582 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::mix_pair > ()=ast::mix_pair(yystack_[1].value.as< ast::Expression > (),yystack_[0].value.as< std::list<ast::Agent> > ());}
#line 1881 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 171:
#line 584 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::mix_pair > ()=ast::mix_pair(ast::Const(1.0f,yystack_[0].location),yystack_[0].value.as< std::list<ast::Agent> > ());}
#line 1887 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 173:
#line 589 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::Agent> > ()=yystack_[1].value.as< std::list<ast::Agent> > ();}
#line 1893 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 174:
#line 591 "kappaParser.y" // lalr1.cc:847
    {
		yystack_[0].value.as< std::list<ast::Agent> > ().push_front(yystack_[2].value.as< ast::Agent > ());
		yylhs.value.as< std::list<ast::Agent> > ()=yystack_[0].value.as< std::list<ast::Agent> > ();
	}
#line 1902 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 175:
#line 596 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::Agent> > ()=std::list<ast::Agent>(1,yystack_[0].value.as< ast::Agent > ());}
#line 1908 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 176:
#line 601 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Agent > ()=ast::Agent(yystack_[3].value.as< std::string > (),yystack_[1].value.as< std::list<ast::Site> > (),yystack_[3].location);}
#line 1914 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 177:
#line 603 "kappaParser.y" // lalr1.cc:847
    {yy::KappaParser::error(yystack_[1].location,std::string("Malformed agent ")+yystack_[1].value.as< std::string > ());}
#line 1920 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 178:
#line 608 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::Site> > ()=std::list<ast::Site>();}
#line 1926 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 179:
#line 610 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<ast::Site> > ()=std::list<ast::Site>(1,yystack_[0].value.as< ast::Site > ());}
#line 1932 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 180:
#line 612 "kappaParser.y" // lalr1.cc:847
    {
	yystack_[0].value.as< std::list<ast::Site> > ().push_front(yystack_[2].value.as< ast::Site > ());
	yylhs.value.as< std::list<ast::Site> > ()=yystack_[0].value.as< std::list<ast::Site> > ();
}
#line 1941 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 182:
#line 633 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Site > ()=ast::Site(yystack_[2].value.as< std::string > (),yystack_[1].value.as< std::list<std::string> > (),yystack_[0].value.as< ast::Link > (),yystack_[2].location);}
#line 1947 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 183:
#line 638 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< std::list<std::string> > ()=std::list<std::string>(); }
#line 1953 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 184:
#line 640 "kappaParser.y" // lalr1.cc:847
    {
		yystack_[0].value.as< std::list<std::string> > ().push_front(yystack_[1].value.as< std::string > ());
		yylhs.value.as< std::list<std::string> > ()=yystack_[0].value.as< std::list<std::string> > ();
	 }
#line 1962 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 185:
#line 645 "kappaParser.y" // lalr1.cc:847
    {yy::KappaParser::error(yystack_[0].location,"Invalid internal state");}
#line 1968 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 186:
#line 650 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Link > () = ast::Link(ast::Link::FREE,yylhs.location);}
#line 1974 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 187:
#line 652 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Link > () = ast::Link(ast::Link::VALUE,yystack_[0].value.as< int > (),yystack_[0].location);}
#line 1980 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 188:
#line 654 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Link > () = ast::Link(ast::Link::SOME,yystack_[0].location);}
#line 1986 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 189:
#line 656 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Link > () = ast::Link(ast::Link::TYPE,ast::Id(yystack_[2].value.as< std::string > (),yystack_[2].location),ast::Id(yystack_[0].value.as< std::string > (),yystack_[0].location),yystack_[3].location);}
#line 1992 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 190:
#line 658 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Link > () = ast::Link(ast::Link::ANY,yystack_[0].location);}
#line 1998 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 191:
#line 660 "kappaParser.y" // lalr1.cc:847
    {yy::KappaParser::error(yystack_[1].location,"Invalid link state");}
#line 2004 "KappaParser.cpp" // lalr1.cc:847
    break;


#line 2008 "KappaParser.cpp" // lalr1.cc:847
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
        error (yyla.location, yysyntax_error_ (yystack_[0].state,
                                           yyempty ? yyempty_ : yyla.type_get ()));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyempty)
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyempty = true;
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
    if (!yyempty)
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
        if (!yyempty)
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
   KappaParser ::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
   KappaParser ::yysyntax_error_ (state_type yystate, symbol_number_type yytoken) const
  {
    std::string yyres;
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
       - The only way there can be no lookahead present (in yytoken) is
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
    if (yytoken != yyempty_)
      {
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


  const short int  KappaParser ::yypact_ninf_ = -198;

  const short int  KappaParser ::yytable_ninf_ = -184;

  const short int
   KappaParser ::yypact_[] =
  {
    -198,   142,  -198,  -198,   601,   -53,   -54,    17,    12,   -53,
     532,   -46,  -198,   -31,   -24,   150,   -51,   179,  -198,    -2,
    -198,   780,   939,  -198,  -198,  -198,   837,  -198,  -198,  -198,
    -198,  -198,  -198,   939,   939,   939,   939,   939,   939,   939,
     939,   939,  -198,  -198,  -198,   -15,   939,   939,  -198,  -198,
    -198,  -198,  -198,   237,  -198,  -198,  1080,   487,  -198,   -25,
    -198,    58,  -198,  -198,  -198,   197,  -198,  -198,  -198,    -2,
    -198,  -198,   939,   555,  -198,    62,   939,   -31,  -198,  -198,
       3,  -198,    -4,  -198,  -198,  -198,  -198,    -2,    63,    -1,
    -198,    70,   104,   198,   232,  -198,    45,   321,  -198,  -198,
    -198,   555,   555,   555,  -198,  -198,  -198,    46,   715,   715,
     894,   894,   431,   431,   939,   939,   939,   939,   939,   939,
     939,   939,   939,   939,  -198,   984,   340,   555,  -198,   -25,
    -198,  -198,    -5,    67,    37,  -198,   520,   939,   109,   555,
      63,    -2,   -51,  -198,   130,  -198,  -198,    -2,   244,  -198,
      -2,  -198,  -198,   431,  -198,   894,  -198,    -3,    -3,   894,
    -198,   131,   431,   330,   416,    78,   -14,    80,   139,     5,
     155,   100,   -14,   -14,   140,   172,  -198,    -3,    -8,    -3,
     555,   555,   555,   555,    -8,   113,   113,  -198,    42,   175,
     177,  -198,   180,   216,   894,  -198,   -31,   187,  -198,  -198,
      66,  -198,  1029,  -198,   426,  -198,  -198,   218,    31,   185,
    -198,   138,  -198,  -198,  -198,  -198,   221,   646,  -198,   221,
     939,   646,   646,  -198,   174,   939,   939,  -198,  -198,   894,
     431,  -198,    42,   -26,  -198,    -5,  -198,    62,   111,   193,
     939,  -198,  -198,   939,   195,   624,   134,  -198,  -198,  -198,
    -198,  -198,   207,   202,   110,  -198,   555,   214,   219,   646,
     555,   555,   218,  -198,  -198,  -198,     2,  -198,  -198,  -198,
    -198,     1,   555,   135,  -198,  -198,   203,   646,  -198,   646,
    -198,  -198,   224,  -198,  -198,  -198,   250,  -198,   939,   939,
    1029,  -198,  -198,  -198,   182,   738,   258,   555,  -198,  -198,
     939,  -198,   555
  };

  const unsigned char
   KappaParser ::yydefact_[] =
  {
       2,     0,     1,     9,    81,     0,     0,     0,     0,     0,
       0,     0,   107,     0,     0,    32,    43,     0,     8,   116,
       7,    81,     0,    89,    90,   138,    81,   101,   133,   134,
     137,   135,   136,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   104,   105,   161,     0,     0,     0,   103,   142,
     132,   102,    24,     0,   140,   141,     0,     0,    21,    40,
      17,     0,    15,    19,    98,     0,   100,    99,    18,   172,
      20,    23,     0,    22,    16,    36,     0,     0,    33,    34,
       0,    44,    43,    14,     5,     4,     3,   172,     0,   109,
     117,   175,     0,     0,     0,   157,     0,     0,   152,   153,
     154,   158,   159,   160,   156,   151,   155,     0,     0,     0,
      81,    81,    52,    52,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    80,   172,     0,    79,    78,    40,
      26,   177,   178,    97,    28,    29,     0,     0,    38,    11,
       0,   116,    43,    46,     0,   124,   125,   116,     0,   108,
     172,    49,    82,    52,   139,    81,   131,   149,   150,    81,
      83,    84,    52,     0,     0,     0,    91,     0,     0,     0,
       0,     0,    91,    91,    50,    54,    51,   144,   143,   146,
      85,    86,    87,    88,   145,   147,   148,    42,     0,     0,
     179,    30,     0,     0,    81,    35,     0,     0,    47,   173,
     118,   111,   112,   110,     0,   174,    50,    25,     0,     0,
      62,   172,    61,   171,    64,    63,    75,    70,    92,    75,
       0,    70,    70,    95,     0,     0,     0,    67,    66,    81,
      52,   185,     0,   186,   176,   178,    31,    36,     0,     0,
       0,   119,   120,   162,     0,     0,     0,    53,   170,    76,
      77,    59,    71,     0,    72,    60,    58,     0,     0,    70,
      65,    57,    27,    55,   184,   190,     0,   182,   180,    37,
      39,     0,    13,   164,   122,   113,   114,    70,    93,    70,
      68,    96,     0,   191,   188,   187,     0,    12,   166,     0,
     112,    73,    74,    69,     0,   167,     0,   165,   115,   189,
       0,   163,   168
  };

  const short int
   KappaParser ::yypgoto_[] =
  {
    -198,  -198,  -198,  -198,  -198,   141,  -198,    -9,    40,  -198,
     152,   -65,    18,  -111,  -198,   -91,    68,   277,   -20,   101,
    -198,   156,    20,  -198,   154,  -198,  -197,   158,  -198,  -198,
     170,  -198,   -10,  -198,  -198,   148,   -47,   309,    82,  -198,
      86,  -198
  };

  const short int
   KappaParser ::yydefgoto_[] =
  {
      -1,     1,    86,    17,    18,    68,    80,    82,   138,   195,
     130,    83,    52,   174,   175,   253,   251,    58,    53,   219,
     224,    69,    54,    19,    88,   149,   203,    89,   243,    20,
     147,    55,    56,   274,   296,   212,    90,    91,   189,   190,
     233,   267
  };

  const short int
   KappaParser ::yytable_[] =
  {
      73,    93,   176,   283,    76,   244,    97,    87,  -181,   142,
     128,    94,    95,    63,   217,   -28,   -28,   143,    60,   265,
     115,   -97,   135,    98,    99,   100,   101,   102,   103,   104,
     105,   106,    57,   222,    75,    59,   108,   109,    74,    92,
     144,   152,   206,   231,    96,    81,   -97,   127,   284,    27,
     266,   209,  -183,   121,    75,  -183,   110,   111,   122,   131,
     123,    77,   136,   122,   129,   123,   139,   132,   140,   107,
      42,    43,   241,   137,   242,   218,   148,   198,   144,   188,
     155,    75,    61,   150,    48,   285,   286,  -183,   141,    51,
     160,   161,    81,   298,   223,    64,    65,    66,   157,   158,
      67,    61,   145,   205,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   151,   136,   213,   213,  -183,   263,
      64,    65,    66,   156,   279,    67,   194,   193,   232,   270,
     257,   258,   114,   115,   116,   207,   110,   111,   204,   208,
     199,   146,     2,     3,   288,    -6,    -6,    87,   289,    94,
      64,  -106,    66,   211,   211,    67,   110,   114,   115,   116,
     114,   115,   116,   216,   248,   220,   121,   221,   282,    78,
      79,   225,     4,   226,   238,   229,   122,   230,   123,   122,
       5,  -106,    84,    85,     6,   234,   291,   239,   292,     7,
     235,   121,   245,   240,   121,   247,     8,     9,   236,    10,
     271,   122,   259,   123,   122,   275,   123,   254,   152,   262,
     256,   254,   254,   133,   134,   260,   261,    11,   276,  -106,
    -106,   277,    61,   110,   111,   290,  -106,    12,   237,   278,
     272,   153,   113,   273,    13,    14,    15,    16,   114,   115,
     116,   280,   154,   110,   111,   201,   281,  -112,  -112,   254,
    -112,   293,  -112,   202,   114,   115,   116,   249,   250,   117,
     118,   119,   110,   111,   294,    22,   299,   254,   301,   254,
     112,   113,   121,   227,   228,   192,   120,   269,   295,   297,
     204,   187,   122,  -112,   123,    25,    70,   255,   121,   191,
     302,   287,    27,    28,    29,    30,    31,    32,   122,   197,
     123,   200,    33,    34,    35,    36,    37,    38,    39,    40,
     196,    41,   215,    42,    43,    44,    62,   268,   264,     0,
       0,    45,  -112,     0,     0,    46,    47,    48,    49,    50,
       0,   210,    51,  -169,  -169,  -169,     0,     0,     0,   125,
    -169,   131,     0,  -142,  -142,  -142,   110,   111,     0,   132,
    -142,    22,     0,     0,   153,   113,     0,     0,     0,     0,
       0,     0,  -142,  -142,  -142,  -169,     0,     0,     0,     0,
       0,    25,     0,     0,     0,  -142,     0,     0,    27,    28,
      29,    30,    31,    32,     0,     0,     0,     0,    33,    34,
      35,    36,    37,    38,    39,    40,  -142,    41,     0,    42,
      43,    44,     0,     0,     0,     0,  -142,    45,  -142,     0,
       0,    46,    47,    48,   126,    50,     0,   214,    51,  -169,
    -169,  -169,     0,     0,  -142,   125,  -169,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   -56,    22,     0,     0,
     162,   246,     0,     0,     0,     0,     0,     0,   114,   115,
     116,  -169,     0,     0,     0,     0,     0,    25,     0,     0,
       0,     0,   163,   164,    27,    28,    29,    30,    31,    32,
       0,   165,     0,     0,    33,    34,    35,    36,    37,    38,
      39,    40,   121,    41,     0,    42,    43,    44,   124,     0,
    -172,  -172,   122,    45,   123,     0,   125,    46,    47,    48,
     126,    50,     0,   166,    51,   167,     0,     0,    22,     0,
     168,   169,     0,     0,     0,   170,   171,     0,     0,     0,
       0,   172,   173,     0,     0,     0,     0,     0,    25,     0,
     154,     0,     0,    71,     0,    27,    28,    29,    30,    31,
      32,    72,   114,   115,   116,    33,    34,    35,    36,    37,
      38,    39,    40,    22,    41,     0,    42,    43,    44,     0,
       0,     0,     0,     0,    45,     0,     0,     0,    46,    47,
      48,   126,    50,    25,     0,    51,   121,   114,   115,   116,
      27,    28,    29,    30,    31,    32,   122,     0,   123,     0,
      33,    34,    35,    36,    37,    38,    39,    40,     0,    41,
       0,    42,    43,    44,   -48,   -48,     0,     0,     0,    45,
      21,   121,     0,    46,    47,    48,    49,    50,     0,     0,
      51,   122,    22,   123,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   154,     0,     0,    23,    24,   246,
       0,     0,    25,     0,    26,     0,   114,   115,   116,    27,
      28,    29,    30,    31,    32,    72,     0,     0,     0,    33,
      34,    35,    36,    37,    38,    39,    40,    22,    41,     0,
      42,    43,    44,     0,     0,     0,     0,     0,    45,     0,
     121,     0,    46,    47,    48,    49,    50,    25,     0,    51,
     122,     0,   123,     0,    27,    28,    29,    30,    31,    32,
       0,     0,     0,     0,    33,    34,    35,    36,    37,    38,
      39,    40,     0,    41,     0,    42,    43,    44,     0,     0,
       0,     0,     0,    45,    72,     0,     0,    46,    47,    48,
      49,    50,     0,     0,    51,   252,    22,   114,   115,   116,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   300,     0,     0,    25,     0,     0,     0,
     114,   115,   116,    27,    28,    29,    30,    31,    32,     0,
       0,   121,     0,    33,    34,    35,    36,    37,    38,    39,
      40,   122,    41,   123,    42,    43,    44,     0,     0,    21,
     -48,     0,    45,     0,   121,     0,    46,    47,    48,    49,
      50,    22,     0,    51,   122,     0,   123,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,    24,     0,     0,
       0,    25,     0,     0,     0,     0,     0,     0,    27,    28,
      29,    30,    31,    32,     0,     0,     0,     0,    33,    34,
      35,    36,    37,    38,    39,    40,    21,    41,     0,    42,
      43,    44,     0,     0,     0,     0,     0,    45,    22,     0,
       0,    46,    47,    48,    49,    50,     0,     0,    51,     0,
       0,     0,   -48,    23,    24,     0,     0,     0,    25,     0,
       0,     0,     0,     0,     0,    27,    28,    29,    30,    31,
      32,     0,     0,     0,     0,    33,    34,    35,    36,    37,
      38,    39,    40,   159,    41,     0,    42,    43,    44,     0,
       0,     0,     0,     0,    45,    22,     0,     0,    46,    47,
      48,    49,    50,     0,     0,    51,     0,     0,     0,     0,
      23,    24,     0,     0,     0,    25,     0,     0,     0,     0,
       0,     0,    27,    28,    29,    30,    31,    32,    72,     0,
       0,     0,    33,    34,    35,    36,    37,    38,    39,    40,
      22,    41,     0,    42,    43,    44,     0,     0,     0,     0,
       0,    45,     0,     0,     0,    46,    47,    48,    49,    50,
      25,     0,    51,     0,     0,     0,     0,    27,    28,    29,
      30,    31,    32,   125,     0,     0,     0,    33,    34,    35,
      36,    37,    38,    39,    40,    22,    41,     0,    42,    43,
      44,     0,     0,     0,     0,     0,    45,     0,     0,     0,
      46,    47,    48,    49,    50,    25,     0,    51,     0,     0,
       0,     0,    27,    28,    29,    30,    31,    32,   202,     0,
       0,     0,    33,    34,    35,    36,    37,    38,    39,    40,
      22,    41,     0,    42,    43,    44,     0,     0,     0,     0,
       0,    45,     0,     0,     0,    46,    47,    48,   126,    50,
      25,     0,    51,     0,     0,     0,     0,    27,    28,    29,
      30,    31,    32,     0,     0,     0,     0,    33,    34,    35,
      36,    37,    38,    39,    40,     0,    41,     0,    42,    43,
      44,     0,   114,   115,   116,     0,    45,   117,   118,   119,
      46,    47,    48,    49,    50,     0,     0,    51,     0,     0,
       0,     0,     0,     0,   120,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   121,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   122,     0,   123
  };

  const short int
   KappaParser ::yycheck_[] =
  {
      10,    21,   113,     1,    13,   202,    26,     9,    13,    13,
      57,    21,    22,     1,    28,     3,     4,    82,     1,    45,
      23,     9,    69,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    85,    28,    85,    89,    46,    47,    84,    21,
      87,    10,   153,     1,    26,    96,     9,    57,    46,    48,
      76,   162,    10,    56,    85,    13,    25,    26,    66,     1,
      68,    85,    72,    66,    89,    68,    76,     9,    77,    84,
      69,    70,     6,    11,     8,    89,    77,   142,   125,    84,
      35,    85,    84,    13,    83,    83,    84,    45,    85,    88,
     110,   111,    96,   290,    89,    83,    84,    85,   108,   109,
      88,    84,    39,   150,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,    10,   125,   163,   164,    76,   230,
      83,    84,    85,    77,    14,    88,    17,   137,    86,    18,
     221,   222,    22,    23,    24,   155,    25,    26,   148,   159,
      10,    78,     0,     1,     9,     3,     4,     9,    13,   159,
      83,     9,    85,   163,   164,    88,    25,    22,    23,    24,
      22,    23,    24,    85,   211,    85,    56,    28,   259,    19,
      20,    16,    30,    73,   194,    35,    66,     5,    68,    66,
      38,    39,     3,     4,    42,    10,   277,   196,   279,    47,
      13,    56,   202,     6,    56,    10,    54,    55,    18,    57,
       7,    66,    28,    68,    66,    10,    68,   217,    10,   229,
     220,   221,   222,    16,    17,   225,   226,    75,    84,    77,
      78,    14,    84,    25,    26,    22,    84,    85,    12,    27,
     240,    33,    34,   243,    92,    93,    94,    95,    22,    23,
      24,    27,    10,    25,    26,     1,    27,     3,     4,   259,
       6,    27,     8,     9,    22,    23,    24,    36,    37,    27,
      28,    29,    25,    26,    14,    21,    84,   277,    10,   279,
      33,    34,    56,   172,   173,   134,    44,   237,   288,   289,
     290,   129,    66,    39,    68,    41,     9,   219,    56,   133,
     300,   271,    48,    49,    50,    51,    52,    53,    66,   141,
      68,   147,    58,    59,    60,    61,    62,    63,    64,    65,
     140,    67,   164,    69,    70,    71,     7,   235,   232,    -1,
      -1,    77,    78,    -1,    -1,    81,    82,    83,    84,    85,
      -1,     1,    88,     3,     4,     5,    -1,    -1,    -1,     9,
      10,     1,    -1,     3,     4,     5,    25,    26,    -1,     9,
      10,    21,    -1,    -1,    33,    34,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    23,    24,    35,    -1,    -1,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    35,    -1,    -1,    48,    49,
      50,    51,    52,    53,    -1,    -1,    -1,    -1,    58,    59,
      60,    61,    62,    63,    64,    65,    56,    67,    -1,    69,
      70,    71,    -1,    -1,    -1,    -1,    66,    77,    68,    -1,
      -1,    81,    82,    83,    84,    85,    -1,     1,    88,     3,
       4,     5,    -1,    -1,    84,     9,    10,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,     5,    21,    -1,    -1,
       9,    15,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,
      24,    35,    -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    31,    32,    48,    49,    50,    51,    52,    53,
      -1,    40,    -1,    -1,    58,    59,    60,    61,    62,    63,
      64,    65,    56,    67,    -1,    69,    70,    71,     1,    -1,
       3,     4,    66,    77,    68,    -1,     9,    81,    82,    83,
      84,    85,    -1,    72,    88,    74,    -1,    -1,    21,    -1,
      79,    80,    -1,    -1,    -1,    84,    85,    -1,    -1,    -1,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    41,    -1,
      10,    -1,    -1,     1,    -1,    48,    49,    50,    51,    52,
      53,     9,    22,    23,    24,    58,    59,    60,    61,    62,
      63,    64,    65,    21,    67,    -1,    69,    70,    71,    -1,
      -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    81,    82,
      83,    84,    85,    41,    -1,    88,    56,    22,    23,    24,
      48,    49,    50,    51,    52,    53,    66,    -1,    68,    -1,
      58,    59,    60,    61,    62,    63,    64,    65,    -1,    67,
      -1,    69,    70,    71,     3,     4,    -1,    -1,    -1,    77,
       9,    56,    -1,    81,    82,    83,    84,    85,    -1,    -1,
      88,    66,    21,    68,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    10,    -1,    -1,    36,    37,    15,
      -1,    -1,    41,    -1,    43,    -1,    22,    23,    24,    48,
      49,    50,    51,    52,    53,     9,    -1,    -1,    -1,    58,
      59,    60,    61,    62,    63,    64,    65,    21,    67,    -1,
      69,    70,    71,    -1,    -1,    -1,    -1,    -1,    77,    -1,
      56,    -1,    81,    82,    83,    84,    85,    41,    -1,    88,
      66,    -1,    68,    -1,    48,    49,    50,    51,    52,    53,
      -1,    -1,    -1,    -1,    58,    59,    60,    61,    62,    63,
      64,    65,    -1,    67,    -1,    69,    70,    71,    -1,    -1,
      -1,    -1,    -1,    77,     9,    -1,    -1,    81,    82,    83,
      84,    85,    -1,    -1,    88,    89,    21,    22,    23,    24,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    15,    -1,    -1,    41,    -1,    -1,    -1,
      22,    23,    24,    48,    49,    50,    51,    52,    53,    -1,
      -1,    56,    -1,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    -1,    -1,     9,
      10,    -1,    77,    -1,    56,    -1,    81,    82,    83,    84,
      85,    21,    -1,    88,    66,    -1,    68,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    -1,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    -1,    -1,    -1,    -1,    58,    59,
      60,    61,    62,    63,    64,    65,     9,    67,    -1,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    77,    21,    -1,
      -1,    81,    82,    83,    84,    85,    -1,    -1,    88,    -1,
      -1,    -1,    35,    36,    37,    -1,    -1,    -1,    41,    -1,
      -1,    -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,
      53,    -1,    -1,    -1,    -1,    58,    59,    60,    61,    62,
      63,    64,    65,     9,    67,    -1,    69,    70,    71,    -1,
      -1,    -1,    -1,    -1,    77,    21,    -1,    -1,    81,    82,
      83,    84,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,
      36,    37,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,     9,    -1,
      -1,    -1,    58,    59,    60,    61,    62,    63,    64,    65,
      21,    67,    -1,    69,    70,    71,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      41,    -1,    88,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,     9,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    63,    64,    65,    21,    67,    -1,    69,    70,
      71,    -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,
      81,    82,    83,    84,    85,    41,    -1,    88,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,     9,    -1,
      -1,    -1,    58,    59,    60,    61,    62,    63,    64,    65,
      21,    67,    -1,    69,    70,    71,    -1,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      41,    -1,    88,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    -1,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    63,    64,    65,    -1,    67,    -1,    69,    70,
      71,    -1,    22,    23,    24,    -1,    77,    27,    28,    29,
      81,    82,    83,    84,    85,    -1,    -1,    88,    -1,    -1,
      -1,    -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    68
  };

  const unsigned char
   KappaParser ::yystos_[] =
  {
       0,    98,     0,     1,    30,    38,    42,    47,    54,    55,
      57,    75,    85,    92,    93,    94,    95,   100,   101,   120,
     126,     9,    21,    36,    37,    41,    43,    48,    49,    50,
      51,    52,    53,    58,    59,    60,    61,    62,    63,    64,
      65,    67,    69,    70,    71,    77,    81,    82,    83,    84,
      85,    88,   109,   115,   119,   128,   129,    85,   114,    89,
       1,    84,   134,     1,    83,    84,    85,    88,   102,   118,
     114,     1,     9,   129,    84,    85,   104,    85,    19,    20,
     103,    96,   104,   108,     3,     4,    99,     9,   121,   124,
     133,   134,   109,   115,   129,   129,   109,   115,   129,   129,
     129,   129,   129,   129,   129,   129,   129,    84,   129,   129,
      25,    26,    33,    34,    22,    23,    24,    27,    28,    29,
      44,    56,    66,    68,     1,     9,    84,   129,   133,    89,
     107,     1,     9,    16,    17,   133,   129,    11,   105,   129,
     104,    85,    13,   108,   133,    39,    78,   127,    77,   122,
      13,    10,    10,    33,    10,    35,    77,   129,   129,     9,
     115,   115,     9,    31,    32,    40,    72,    74,    79,    80,
      84,    85,    90,    91,   110,   111,   110,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   107,    84,   135,
     136,   118,   102,   129,    17,   106,   127,   124,   108,    10,
     121,     1,     9,   123,   129,   133,   110,   115,   115,   110,
       1,   129,   132,   133,     1,   132,    85,    28,    89,   116,
      85,    28,    28,    89,   117,    16,    73,   116,   116,    35,
       5,     1,    86,   137,    10,    13,    18,    12,   115,   104,
       6,     6,     8,   125,   123,   129,    15,    10,   133,    36,
      37,   113,    89,   112,   129,   113,   129,   112,   112,    28,
     129,   129,   115,   110,   137,    45,    76,   138,   135,   105,
      18,     7,   129,   129,   130,    10,    84,    14,    27,    14,
      27,    27,   112,     1,    46,    83,    84,   119,     9,    13,
      22,   112,   112,    27,    14,   129,   131,   129,   123,    84,
      15,    10,   129
  };

  const unsigned char
   KappaParser ::yyr1_[] =
  {
       0,    97,    98,    98,    99,    99,   100,   100,   100,   100,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   102,   102,
     102,   102,   103,   103,   103,   104,   105,   105,   106,   106,
     107,   107,   107,   108,   108,   108,   108,   108,   109,   109,
     109,   109,   110,   110,   110,   110,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     112,   112,   112,   112,   112,   113,   113,   113,   114,   114,
     114,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   116,   116,   116,   117,   117,   117,   118,   118,   118,
     118,   119,   119,   119,   119,   119,   120,   120,   121,   122,
     122,   122,   123,   123,   123,   123,   124,   124,   125,   125,
     125,   126,   126,   126,   127,   127,   119,   119,   119,   119,
     119,   128,   128,   128,   128,   128,   128,   128,   128,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   130,   130,   130,   130,   131,   131,   131,   132,
     132,   132,   133,   133,   133,   133,   134,   134,   135,   135,
     135,   136,   136,   137,   137,   137,   138,   138,   138,   138,
     138,   138
  };

  const unsigned char
   KappaParser ::yyr2_[] =
  {
       0,     2,     0,     3,     1,     1,     0,     1,     1,     1,
       0,     3,     7,     6,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     5,     3,     6,     0,     2,
       3,     4,     0,     1,     1,     3,     0,     4,     0,     3,
       0,     1,     2,     0,     1,     1,     2,     3,     0,     3,
       3,     3,     0,     3,     1,     3,     0,     3,     3,     3,
       3,     2,     2,     2,     2,     3,     2,     2,     4,     5,
       0,     1,     1,     3,     3,     0,     1,     1,     2,     2,
       2,     0,     3,     3,     3,     3,     3,     3,     3,     1,
       1,     0,     1,     3,     0,     1,     3,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     2,     0,
       2,     2,     0,     3,     3,     5,     0,     1,     0,     1,
       1,     0,     6,     4,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     3,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     1,     0,     4,     1,     3,     0,     1,     3,     0,
       2,     1,     0,     3,     3,     1,     4,     2,     0,     1,
       3,     0,     3,     0,     2,     1,     0,     2,     2,     4,
       1,     2
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const  KappaParser ::yytname_[] =
  {
  "$end", "error", "$undefined", "END", "NEWLINE", "SEMICOLON", "AT",
  "ATD", "FIX", "OP_PAR", "CL_PAR", "OP_BRA", "CL_BRA", "COMMA", "DOT",
  "TYPE", "LAR", "OP_CUR", "CL_CUR", "JOIN", "FREE", "LOG", "PLUS", "MULT",
  "MINUS", "AND", "OR", "GREATER", "SMALLER", "EQUAL", "PERT", "INTRO",
  "DELETE", "DO", "SET", "UNTIL", "TRUE", "FALSE", "OBS", "KAPPA_RAR",
  "TRACK", "CPUTIME", "CONFIG", "REPEAT", "DIFF", "KAPPA_WLD",
  "KAPPA_SEMI", "SIGNATURE", "INFINITY", "TIME", "EVENT", "ACTIVITY",
  "NULL_EVENT", "PROD_EVENT", "INIT", "LET", "DIV", "PLOT", "SINUS",
  "COSINUS", "TAN", "ATAN", "COIN", "RAND_N", "SQRT", "EXPONENT", "POW",
  "ABS", "MODULO", "EMAX", "TMAX", "RAND_1", "FLUX", "ASSIGN", "ASSIGN2",
  "TOKEN", "KAPPA_LNK", "PIPE", "KAPPA_LRAR", "PRINT", "PRINTF", "MAX",
  "MIN", "INT", "ID", "LABEL", "KAPPA_MRK", "NAME", "FLOAT", "STRING",
  "STOP", "SNAPSHOT", "COMPARTMENT", "C_LINK", "TRANSPORT", "USE",
  "\"$ALL\"", "$accept", "statements", "newline", "statement",
  "instruction", "init_declaration", "join", "comp_expr", "dimension",
  "where_expr", "value_list", "comp_list", "perturbation_declaration",
  "effect_list", "effect", "print_expr", "boolean", "variable_declaration",
  "bool_expr", "opt_string", "string_or_pr_expr", "multiple", "constant",
  "rule_label", "lhs_rhs", "token_expr", "sum_token", "mixture",
  "rate_sep", "rule_expression", "arrow", "variable", "alg_expr", "rate",
  "alg_with_radius", "multiple_mixture", "non_empty_mixture",
  "agent_expression", "interface_expression", "port_expression",
  "internal_state", "link_state", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
   KappaParser ::yyrline_[] =
  {
       0,   104,   104,   105,   110,   111,   114,   115,   117,   119,
     124,   126,   128,   130,   132,   134,   136,   138,   140,   142,
     144,   146,   148,   150,   152,   154,   156,   158,   162,   163,
     165,   167,   175,   176,   178,   182,   188,   189,   198,   199,
     224,   225,   227,   235,   236,   238,   240,   245,   254,   255,
     256,   257,   265,   266,   267,   268,   275,   276,   278,   280,
     288,   299,   301,   303,   305,   307,   309,   318,   328,   330,
     344,   345,   347,   349,   354,   361,   362,   363,   367,   369,
     371,   375,   376,   378,   380,   382,   384,   386,   388,   390,
     392,   397,   398,   399,   402,   403,   404,   408,   409,   410,
     411,   415,   417,   419,   421,   423,   427,   428,   433,   437,
     438,   439,   443,   444,   446,   448,   456,   457,   461,   462,
     463,   468,   469,   471,   478,   480,   485,   487,   489,   491,
     493,   498,   500,   502,   504,   506,   508,   510,   512,   517,
     519,   521,   523,   525,   527,   529,   531,   533,   535,   537,
     539,   541,   543,   545,   547,   549,   551,   553,   556,   558,
     560,   562,   566,   567,   569,   571,   575,   576,   577,   580,
     581,   583,   587,   588,   590,   595,   600,   602,   608,   609,
     611,   631,   632,   638,   639,   644,   650,   651,   653,   655,
     657,   659
  };

  // Print the state stack on the debug stream.
  void
   KappaParser ::yystack_print_ ()
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
   KappaParser ::yy_reduce_print_ (int yyrule)
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



} // yy
#line 2775 "KappaParser.cpp" // lalr1.cc:1155
#line 663 "kappaParser.y" // lalr1.cc:1156


void yy::KappaParser::error(const location &loc , const std::string &message) {
        
        // Location should be initialized inside scanner action, but is not in this example.
        // Let's grab location directly from driver class.
	// cout << "Error: " << message << endl << "Location: " << loc << endl;
	
        cout << "Error: " << message << endl << "Error location: " << loc << endl;
}
