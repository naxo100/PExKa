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

    #include "ast/KappaAst.h"

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
   KappaParser :: KappaParser  (ast::KappaAst &driver_yyarg)
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
      case 114: // variable_declaration
        value.move< ast::Declaration > (that.value);
        break;

      case 127: // constant
      case 128: // variable
      case 129: // alg_expr
        value.move< ast::Expression > (that.value);
        break;

      case 88: // FLOAT
        value.move< float > (that.value);
        break;

      case 83: // INT
        value.move< int > (that.value);
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
      case 114: // variable_declaration
        value.copy< ast::Declaration > (that.value);
        break;

      case 127: // constant
      case 128: // variable
      case 129: // alg_expr
        value.copy< ast::Expression > (that.value);
        break;

      case 88: // FLOAT
        value.copy< float > (that.value);
        break;

      case 83: // INT
        value.copy< int > (that.value);
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
	yyla.location.initialize(new std::string("file"),1,1);
	cout << "inicializado" << endl;
}

#line 471 "KappaParser.cpp" // lalr1.cc:725

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
      case 114: // variable_declaration
        yylhs.value.build< ast::Declaration > ();
        break;

      case 127: // constant
      case 128: // variable
      case 129: // alg_expr
        yylhs.value.build< ast::Expression > ();
        break;

      case 88: // FLOAT
        yylhs.value.build< float > ();
        break;

      case 83: // INT
        yylhs.value.build< int > ();
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
#line 80 "kappaParser.y" // lalr1.cc:847
    {}
#line 611 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 5:
#line 85 "kappaParser.y" // lalr1.cc:847
    {}
#line 617 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 6:
#line 87 "kappaParser.y" // lalr1.cc:847
    {}
#line 623 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 7:
#line 89 "kappaParser.y" // lalr1.cc:847
    {}
#line 629 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 9:
#line 96 "kappaParser.y" // lalr1.cc:847
    {}
#line 635 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 10:
#line 98 "kappaParser.y" // lalr1.cc:847
    {}
#line 641 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 11:
#line 100 "kappaParser.y" // lalr1.cc:847
    {}
#line 647 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 12:
#line 102 "kappaParser.y" // lalr1.cc:847
    {}
#line 653 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 13:
#line 104 "kappaParser.y" // lalr1.cc:847
    {}
#line 659 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 14:
#line 106 "kappaParser.y" // lalr1.cc:847
    {}
#line 665 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 15:
#line 108 "kappaParser.y" // lalr1.cc:847
    {}
#line 671 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 16:
#line 110 "kappaParser.y" // lalr1.cc:847
    {}
#line 677 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 17:
#line 112 "kappaParser.y" // lalr1.cc:847
    {}
#line 683 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 18:
#line 114 "kappaParser.y" // lalr1.cc:847
    {driver.addDeclaration(yystack_[0].value.as< ast::Declaration > ());}
#line 689 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 19:
#line 116 "kappaParser.y" // lalr1.cc:847
    {}
#line 695 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 20:
#line 118 "kappaParser.y" // lalr1.cc:847
    {}
#line 701 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 21:
#line 120 "kappaParser.y" // lalr1.cc:847
    {}
#line 707 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 22:
#line 121 "kappaParser.y" // lalr1.cc:847
    {}
#line 713 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 23:
#line 123 "kappaParser.y" // lalr1.cc:847
    {}
#line 719 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 24:
#line 125 "kappaParser.y" // lalr1.cc:847
    {}
#line 725 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 25:
#line 127 "kappaParser.y" // lalr1.cc:847
    {}
#line 731 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 27:
#line 132 "kappaParser.y" // lalr1.cc:847
    {}
#line 737 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 28:
#line 133 "kappaParser.y" // lalr1.cc:847
    {}
#line 743 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 29:
#line 134 "kappaParser.y" // lalr1.cc:847
    {}
#line 749 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 30:
#line 141 "kappaParser.y" // lalr1.cc:847
    {}
#line 755 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 31:
#line 143 "kappaParser.y" // lalr1.cc:847
    {}
#line 761 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 32:
#line 145 "kappaParser.y" // lalr1.cc:847
    {}
#line 767 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 33:
#line 149 "kappaParser.y" // lalr1.cc:847
    {}
#line 773 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 34:
#line 154 "kappaParser.y" // lalr1.cc:847
    {}
#line 779 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 35:
#line 156 "kappaParser.y" // lalr1.cc:847
    {}
#line 785 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 36:
#line 161 "kappaParser.y" // lalr1.cc:847
    {}
#line 791 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 37:
#line 163 "kappaParser.y" // lalr1.cc:847
    {}
#line 797 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 38:
#line 165 "kappaParser.y" // lalr1.cc:847
    {}
#line 803 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 39:
#line 167 "kappaParser.y" // lalr1.cc:847
    {}
#line 809 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 40:
#line 169 "kappaParser.y" // lalr1.cc:847
    {}
#line 815 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 41:
#line 171 "kappaParser.y" // lalr1.cc:847
    {}
#line 821 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 42:
#line 173 "kappaParser.y" // lalr1.cc:847
    {}
#line 827 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 43:
#line 175 "kappaParser.y" // lalr1.cc:847
    {}
#line 833 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 44:
#line 177 "kappaParser.y" // lalr1.cc:847
    {}
#line 839 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 46:
#line 182 "kappaParser.y" // lalr1.cc:847
    {}
#line 845 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 47:
#line 184 "kappaParser.y" // lalr1.cc:847
    {}
#line 851 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 48:
#line 188 "kappaParser.y" // lalr1.cc:847
    {}
#line 857 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 49:
#line 190 "kappaParser.y" // lalr1.cc:847
    {}
#line 863 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 50:
#line 192 "kappaParser.y" // lalr1.cc:847
    {}
#line 869 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 51:
#line 194 "kappaParser.y" // lalr1.cc:847
    {}
#line 875 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 52:
#line 196 "kappaParser.y" // lalr1.cc:847
    {}
#line 881 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 53:
#line 201 "kappaParser.y" // lalr1.cc:847
    {}
#line 887 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 54:
#line 203 "kappaParser.y" // lalr1.cc:847
    {}
#line 893 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 56:
#line 208 "kappaParser.y" // lalr1.cc:847
    {}
#line 899 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 57:
#line 209 "kappaParser.y" // lalr1.cc:847
    {}
#line 905 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 58:
#line 210 "kappaParser.y" // lalr1.cc:847
    {}
#line 911 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 60:
#line 214 "kappaParser.y" // lalr1.cc:847
    {}
#line 917 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 61:
#line 215 "kappaParser.y" // lalr1.cc:847
    {}
#line 923 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 62:
#line 216 "kappaParser.y" // lalr1.cc:847
    {}
#line 929 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 64:
#line 221 "kappaParser.y" // lalr1.cc:847
    {}
#line 935 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 65:
#line 223 "kappaParser.y" // lalr1.cc:847
    {}
#line 941 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 66:
#line 225 "kappaParser.y" // lalr1.cc:847
    {}
#line 947 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 67:
#line 227 "kappaParser.y" // lalr1.cc:847
    {}
#line 953 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 68:
#line 229 "kappaParser.y" // lalr1.cc:847
    {}
#line 959 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 69:
#line 231 "kappaParser.y" // lalr1.cc:847
    {}
#line 965 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 70:
#line 233 "kappaParser.y" // lalr1.cc:847
    {}
#line 971 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 71:
#line 235 "kappaParser.y" // lalr1.cc:847
    {}
#line 977 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 72:
#line 237 "kappaParser.y" // lalr1.cc:847
    {}
#line 983 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 73:
#line 239 "kappaParser.y" // lalr1.cc:847
    {}
#line 989 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 74:
#line 241 "kappaParser.y" // lalr1.cc:847
    {}
#line 995 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 75:
#line 242 "kappaParser.y" // lalr1.cc:847
    {}
#line 1001 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 76:
#line 244 "kappaParser.y" // lalr1.cc:847
    {}
#line 1007 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 77:
#line 249 "kappaParser.y" // lalr1.cc:847
    {}
#line 1013 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 78:
#line 250 "kappaParser.y" // lalr1.cc:847
    {}
#line 1019 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 79:
#line 251 "kappaParser.y" // lalr1.cc:847
    {}
#line 1025 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 80:
#line 252 "kappaParser.y" // lalr1.cc:847
    {}
#line 1031 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 81:
#line 253 "kappaParser.y" // lalr1.cc:847
    {}
#line 1037 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 83:
#line 257 "kappaParser.y" // lalr1.cc:847
    {}
#line 1043 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 84:
#line 258 "kappaParser.y" // lalr1.cc:847
    {}
#line 1049 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 85:
#line 263 "kappaParser.y" // lalr1.cc:847
    {}
#line 1055 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 86:
#line 265 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Declaration > () = ast::Declaration(ast::Id(yystack_[1].value.as< std::string > (),yystack_[1].location),yystack_[0].value.as< ast::Expression > (),yylhs.location);}
#line 1061 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 87:
#line 267 "kappaParser.y" // lalr1.cc:847
    {}
#line 1067 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 89:
#line 272 "kappaParser.y" // lalr1.cc:847
    {}
#line 1073 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 90:
#line 274 "kappaParser.y" // lalr1.cc:847
    {}
#line 1079 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 91:
#line 276 "kappaParser.y" // lalr1.cc:847
    {}
#line 1085 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 92:
#line 278 "kappaParser.y" // lalr1.cc:847
    {}
#line 1091 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 93:
#line 280 "kappaParser.y" // lalr1.cc:847
    {}
#line 1097 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 94:
#line 282 "kappaParser.y" // lalr1.cc:847
    {}
#line 1103 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 95:
#line 284 "kappaParser.y" // lalr1.cc:847
    {}
#line 1109 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 96:
#line 286 "kappaParser.y" // lalr1.cc:847
    {}
#line 1115 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 97:
#line 288 "kappaParser.y" // lalr1.cc:847
    {}
#line 1121 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 98:
#line 292 "kappaParser.y" // lalr1.cc:847
    {}
#line 1127 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 99:
#line 293 "kappaParser.y" // lalr1.cc:847
    {}
#line 1133 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 100:
#line 294 "kappaParser.y" // lalr1.cc:847
    {}
#line 1139 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 102:
#line 298 "kappaParser.y" // lalr1.cc:847
    {}
#line 1145 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 103:
#line 299 "kappaParser.y" // lalr1.cc:847
    {}
#line 1151 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 105:
#line 304 "kappaParser.y" // lalr1.cc:847
    {}
#line 1157 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 106:
#line 305 "kappaParser.y" // lalr1.cc:847
    {}
#line 1163 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 107:
#line 306 "kappaParser.y" // lalr1.cc:847
    {}
#line 1169 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 108:
#line 311 "kappaParser.y" // lalr1.cc:847
    {}
#line 1175 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 109:
#line 313 "kappaParser.y" // lalr1.cc:847
    {}
#line 1181 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 110:
#line 317 "kappaParser.y" // lalr1.cc:847
    {}
#line 1187 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 111:
#line 321 "kappaParser.y" // lalr1.cc:847
    {}
#line 1193 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 112:
#line 322 "kappaParser.y" // lalr1.cc:847
    {}
#line 1199 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 113:
#line 324 "kappaParser.y" // lalr1.cc:847
    {}
#line 1205 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 115:
#line 329 "kappaParser.y" // lalr1.cc:847
    {}
#line 1211 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 116:
#line 331 "kappaParser.y" // lalr1.cc:847
    {}
#line 1217 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 117:
#line 333 "kappaParser.y" // lalr1.cc:847
    {}
#line 1223 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 118:
#line 337 "kappaParser.y" // lalr1.cc:847
    {}
#line 1229 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 119:
#line 339 "kappaParser.y" // lalr1.cc:847
    {}
#line 1235 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 121:
#line 343 "kappaParser.y" // lalr1.cc:847
    {}
#line 1241 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 122:
#line 344 "kappaParser.y" // lalr1.cc:847
    {}
#line 1247 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 124:
#line 350 "kappaParser.y" // lalr1.cc:847
    { }
#line 1253 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 125:
#line 352 "kappaParser.y" // lalr1.cc:847
    {}
#line 1259 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 127:
#line 359 "kappaParser.y" // lalr1.cc:847
    {}
#line 1265 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 128:
#line 361 "kappaParser.y" // lalr1.cc:847
    {}
#line 1271 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 129:
#line 366 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(ast::Const::INF,yystack_[0].location);}
#line 1277 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 130:
#line 368 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(yystack_[0].value.as< float > (),yystack_[0].location);}
#line 1283 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 131:
#line 370 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(yystack_[0].value.as< int > (),yystack_[0].location);}
#line 1289 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 132:
#line 372 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(ast::Const::EMAX,yystack_[0].location);}
#line 1295 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 133:
#line 374 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(ast::Const::TMAX,yystack_[0].location);}
#line 1301 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 134:
#line 379 "kappaParser.y" // lalr1.cc:847
    {ast::Var(yystack_[1].value.as< std::string > (),ast::Var::TOKEN,yylhs.location);}
#line 1307 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 135:
#line 381 "kappaParser.y" // lalr1.cc:847
    {ast::Var(yystack_[0].value.as< std::string > (),ast::Var::VAR,yylhs.location);}
#line 1313 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 136:
#line 383 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Var(ast::Var::TIME,yylhs.location);}
#line 1319 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 137:
#line 385 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Var(ast::Var::EVENT,yylhs.location);}
#line 1325 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 138:
#line 387 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Var(ast::Var::NULL_EVENT,yylhs.location);}
#line 1331 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 139:
#line 389 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Var(ast::Var::PROD_EVENT,yylhs.location);}
#line 1337 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 140:
#line 391 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Var(ast::Var::ACTIVITY,yylhs.location);}
#line 1343 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 141:
#line 393 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::Const(ast::Var::CPUTIME,yystack_[0].location);}
#line 1349 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 142:
#line 398 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = yystack_[1].value.as< ast::Expression > ();}
#line 1355 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 143:
#line 400 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = yystack_[0].value.as< ast::Expression > ();}
#line 1361 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 144:
#line 402 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = yystack_[0].value.as< ast::Expression > ();}
#line 1367 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 145:
#line 404 "kappaParser.y" // lalr1.cc:847
    {}
#line 1373 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 146:
#line 406 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::MULT,yystack_[1].location);}
#line 1379 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 147:
#line 408 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::SUM,yystack_[1].location);}
#line 1385 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 148:
#line 410 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::DIV,yystack_[1].location);}
#line 1391 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 149:
#line 412 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::MINUS,yystack_[1].location);}
#line 1397 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 150:
#line 414 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::POW,yystack_[1].location);}
#line 1403 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 151:
#line 416 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[2].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::MODULO,yystack_[1].location);}
#line 1409 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 152:
#line 418 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[1].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::MAX,yystack_[1].location);}
#line 1415 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 153:
#line 420 "kappaParser.y" // lalr1.cc:847
    {yylhs.value.as< ast::Expression > () = ast::BinaryOperation(yystack_[1].value.as< ast::Expression > (),yystack_[0].value.as< ast::Expression > (),ast::BinaryOperation::MIN,yystack_[1].location);}
#line 1421 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 154:
#line 422 "kappaParser.y" // lalr1.cc:847
    {}
#line 1427 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 155:
#line 424 "kappaParser.y" // lalr1.cc:847
    {}
#line 1433 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 156:
#line 426 "kappaParser.y" // lalr1.cc:847
    {}
#line 1439 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 157:
#line 428 "kappaParser.y" // lalr1.cc:847
    {}
#line 1445 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 158:
#line 430 "kappaParser.y" // lalr1.cc:847
    {}
#line 1451 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 159:
#line 432 "kappaParser.y" // lalr1.cc:847
    {}
#line 1457 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 160:
#line 434 "kappaParser.y" // lalr1.cc:847
    {}
#line 1463 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 161:
#line 437 "kappaParser.y" // lalr1.cc:847
    {}
#line 1469 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 162:
#line 439 "kappaParser.y" // lalr1.cc:847
    {}
#line 1475 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 163:
#line 441 "kappaParser.y" // lalr1.cc:847
    {}
#line 1481 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 164:
#line 443 "kappaParser.y" // lalr1.cc:847
    {}
#line 1487 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 166:
#line 448 "kappaParser.y" // lalr1.cc:847
    {}
#line 1493 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 167:
#line 450 "kappaParser.y" // lalr1.cc:847
    {}
#line 1499 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 168:
#line 452 "kappaParser.y" // lalr1.cc:847
    {}
#line 1505 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 170:
#line 456 "kappaParser.y" // lalr1.cc:847
    {}
#line 1511 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 171:
#line 457 "kappaParser.y" // lalr1.cc:847
    {}
#line 1517 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 173:
#line 462 "kappaParser.y" // lalr1.cc:847
    {}
#line 1523 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 174:
#line 464 "kappaParser.y" // lalr1.cc:847
    {}
#line 1529 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 176:
#line 469 "kappaParser.y" // lalr1.cc:847
    {}
#line 1535 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 177:
#line 471 "kappaParser.y" // lalr1.cc:847
    {}
#line 1541 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 178:
#line 473 "kappaParser.y" // lalr1.cc:847
    {}
#line 1547 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 179:
#line 478 "kappaParser.y" // lalr1.cc:847
    {}
#line 1553 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 180:
#line 480 "kappaParser.y" // lalr1.cc:847
    {}
#line 1559 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 181:
#line 485 "kappaParser.y" // lalr1.cc:847
    {}
#line 1565 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 182:
#line 487 "kappaParser.y" // lalr1.cc:847
    {}
#line 1571 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 184:
#line 492 "kappaParser.y" // lalr1.cc:847
    {}
#line 1577 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 185:
#line 494 "kappaParser.y" // lalr1.cc:847
    {}
#line 1583 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 187:
#line 500 "kappaParser.y" // lalr1.cc:847
    {}
#line 1589 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 188:
#line 504 "kappaParser.y" // lalr1.cc:847
    {}
#line 1595 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 189:
#line 506 "kappaParser.y" // lalr1.cc:847
    {}
#line 1601 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 190:
#line 508 "kappaParser.y" // lalr1.cc:847
    {}
#line 1607 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 191:
#line 513 "kappaParser.y" // lalr1.cc:847
    {}
#line 1613 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 192:
#line 515 "kappaParser.y" // lalr1.cc:847
    {}
#line 1619 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 193:
#line 517 "kappaParser.y" // lalr1.cc:847
    {}
#line 1625 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 194:
#line 519 "kappaParser.y" // lalr1.cc:847
    {}
#line 1631 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 195:
#line 521 "kappaParser.y" // lalr1.cc:847
    {}
#line 1637 "KappaParser.cpp" // lalr1.cc:847
    break;

  case 196:
#line 523 "kappaParser.y" // lalr1.cc:847
    {}
#line 1643 "KappaParser.cpp" // lalr1.cc:847
    break;


#line 1647 "KappaParser.cpp" // lalr1.cc:847
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


  const short int  KappaParser ::yypact_ninf_ = -209;

  const short int  KappaParser ::yytable_ninf_ = -189;

  const short int
   KappaParser ::yypact_[] =
  {
    -209,   260,  -209,  -209,   710,   -44,   -45,     2,    44,   -44,
     641,   -13,  -209,    15,    40,   115,   -50,    71,  -209,    -4,
    -209,   889,  1048,  -209,  -209,  -209,   946,  -209,  -209,  -209,
    -209,  -209,  -209,  1048,  1048,  1048,  1048,  1048,  1048,  1048,
    1048,  1048,  -209,  -209,  -209,    -3,  1048,  1048,  -209,  -209,
    -209,  -209,  -209,   134,  -209,  -209,  1189,   596,  -209,   -11,
    -209,    49,  -209,  -209,  -209,   156,  -209,  -209,  -209,    -4,
    -209,  -209,  1048,   327,  -209,   120,  1048,    15,  -209,  -209,
      63,  -209,    -6,  -209,  -209,    -4,    20,    81,  -209,   148,
     169,   116,   450,  -209,   157,   245,  -209,  -209,  -209,   327,
     327,   327,  -209,  -209,  -209,   106,   824,   824,  1003,  1003,
      90,    90,  1048,  1048,  1048,  1048,  1048,  1048,  1048,  1048,
    1048,  1048,  -209,  1093,   181,   327,  -209,   -11,  -209,  -209,
       0,   -18,    55,  -209,   535,    34,   170,   327,    20,    -4,
     -50,  -209,   185,  -209,  -209,    -4,   355,  -209,    -4,  -209,
    -209,    90,  -209,  1003,  -209,    89,    89,  1003,  -209,   210,
      90,   440,   525,   125,    -7,   151,   217,     5,   222,   180,
      -7,    -7,   213,   246,  -209,    89,    -5,    89,   327,   327,
     327,   327,    -5,   188,   188,  -209,     7,   247,  -209,   242,
    -209,   238,    34,  -209,  -209,   184,  1003,  -209,    15,   257,
    -209,  -209,   138,  -209,  1138,  -209,   608,  -209,  -209,   152,
      47,   263,  -209,   175,  -209,  -209,  -209,  -209,   190,   755,
    -209,   190,  1048,   755,   755,  -209,   239,  1048,  1048,  -209,
    -209,  1003,    90,  -209,     7,   -27,  -209,     0,  -209,   558,
     120,    34,    34,    34,    34,    34,    34,    14,   268,  1048,
    -209,  -209,  1048,   266,   178,   193,  -209,  -209,  -209,  -209,
    -209,   270,   253,   343,  -209,   327,   255,   259,   755,   327,
     327,   152,  -209,  -209,  -209,     8,  -209,  -209,  -209,  -209,
      97,    88,    97,    88,   221,   221,  -209,   -14,   327,   206,
    -209,  -209,   267,   755,  -209,   755,  -209,  -209,   261,  -209,
    -209,  -209,   277,  -209,  1048,  1048,  1138,  -209,  -209,  -209,
     208,   733,   283,   327,  -209,  -209,  1048,  -209,   327
  };

  const unsigned char
   KappaParser ::yydefact_[] =
  {
       2,     0,     1,     7,    88,     0,     0,     0,     0,     0,
       0,     0,   109,     0,     0,    30,    48,     0,     6,   118,
       5,    88,     0,    96,    97,   141,    88,   129,   136,   137,
     140,   138,   139,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   132,   133,   164,     0,     0,     0,   131,   145,
     135,   130,    22,     0,   143,   144,     0,     0,    19,    45,
      15,     0,    13,    17,   105,     0,   107,   106,    16,   175,
      18,    21,     0,    20,    14,    34,     0,     0,    31,    32,
       0,    49,    48,    12,     3,   175,   126,   111,   119,   178,
       0,     0,     0,   160,     0,     0,   155,   156,   157,   161,
     162,   163,   159,   154,   158,     0,     0,     0,    88,    88,
      59,    59,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    87,   175,     0,    86,    85,    45,    24,   180,
     181,   104,    26,    27,     0,     0,    53,     9,   126,   118,
      48,    51,     0,   127,   128,   118,     0,   110,   175,    56,
      89,    59,   142,    88,   134,   152,   153,    88,    90,    91,
      59,     0,     0,     0,    98,     0,     0,     0,     0,     0,
      98,    98,    57,    61,    58,   147,   146,   149,    92,    93,
      94,    95,   148,   150,   151,    47,     0,     0,   182,   185,
      28,     0,     0,    36,    37,     0,    88,    33,     0,     0,
      52,   176,   120,   113,   114,   112,     0,   177,    57,    23,
       0,     0,    69,   175,    68,   174,    71,    70,    82,    77,
      99,    82,     0,    77,    77,   102,     0,     0,     0,    74,
      73,    88,    59,   190,     0,   191,   179,   183,    29,     0,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     121,   122,   165,     0,     0,     0,    60,   173,    83,    84,
      66,    78,     0,    79,    67,    65,     0,     0,    77,    72,
      64,    25,    62,   189,   195,     0,   187,   184,    38,    35,
      40,    39,    42,    41,    43,    44,    54,     0,    11,   167,
     124,   115,   116,    77,   100,    77,    75,   103,     0,   196,
     193,   192,     0,    10,   169,     0,   114,    80,    81,    76,
       0,   170,     0,   168,   117,   194,     0,   166,   171
  };

  const short int
   KappaParser ::yypgoto_[] =
  {
    -209,  -209,  -209,  -209,   165,  -209,    -9,    60,   -21,   176,
     -63,  -209,    98,  -109,  -209,  -208,    83,   296,   -20,    62,
    -209,   195,  -209,   171,  -209,  -190,   189,  -209,  -209,   194,
      46,  -209,   -10,  -209,  -209,   172,   -47,   329,  -209,   102,
    -209,   112,  -209
  };

  const short int
   KappaParser ::yydefgoto_[] =
  {
      -1,     1,    17,    18,    68,    80,    82,   136,   195,   128,
      83,   197,    52,   172,   173,   262,   260,    58,    53,   221,
     226,    69,    19,    86,   147,   205,    87,   252,    20,   145,
      54,    55,    56,   290,   312,   214,    88,    89,   187,   188,
     189,   235,   276
  };

  const short int
   KappaParser ::yytable_[] =
  {
      73,    91,   174,    60,    76,    85,    95,   140,   233,   299,
     126,    92,    93,  -186,   253,   266,   267,  -188,   274,   141,
    -188,   219,   133,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   286,   224,    27,    75,   106,   107,   142,   108,
     109,    57,   208,   192,    59,    63,    81,   125,   -26,   275,
     129,   211,  -188,  -104,   300,    42,    43,   150,   130,   143,
     298,   120,   134,   121,  -104,    64,   137,    66,   138,    48,
      67,    74,   108,   109,    51,    84,   142,   200,   127,    75,
      61,   105,   220,  -188,   186,   307,    61,   308,   158,   159,
      81,   301,   302,   234,   225,   -63,   155,   156,   144,   160,
      75,   207,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   113,   134,   215,   215,   314,   193,   194,    90,
     242,   161,   162,   272,    94,    77,   150,    64,    65,    66,
     163,   135,    67,   209,    78,    79,   206,   210,    64,    65,
      66,   108,   109,    67,   250,   119,   251,    92,   139,   151,
     111,   213,   213,   244,   245,   120,   246,   121,   146,   108,
     109,   148,   164,   245,   165,   246,   257,   110,   111,   166,
     167,   239,   131,   132,   168,   169,   247,   108,   109,   149,
     170,   171,   129,   154,    85,  -145,  -145,   196,   152,   248,
     130,  -145,   153,   255,   254,   201,   240,   112,   113,   114,
     112,   113,   114,  -145,  -145,  -145,   241,   242,   243,   263,
     218,   271,   265,   263,   263,   304,  -145,   269,   270,   305,
     280,   281,   282,   283,   284,   285,   258,   259,   112,   113,
     114,   119,   229,   230,   119,   108,   222,  -145,   227,   288,
     244,   120,   289,   121,   120,   223,   121,  -145,   231,  -145,
     245,   232,   246,   228,   120,   237,   238,   236,   263,    61,
       2,     3,   119,   249,    -4,  -145,  -108,   268,  -108,  -108,
     108,   109,   120,   256,   121,   287,   291,   292,   151,   111,
     294,  -108,   296,   263,   293,   263,   297,   245,   309,   306,
       4,   310,   315,   317,   311,   313,   206,   191,     5,  -108,
     279,  -108,     6,   185,   264,    70,   318,     7,  -108,  -108,
    -108,  -108,  -108,  -108,     8,     9,   202,    10,  -108,  -108,
    -108,  -108,  -108,  -108,  -108,  -108,   190,  -108,   199,  -108,
    -108,  -108,   198,   303,   217,    11,    62,  -108,  -108,   277,
       0,  -108,  -108,  -108,  -108,    12,   273,     0,  -108,   112,
     113,   114,    13,    14,    15,    16,   203,   295,     0,  -114,
       0,  -114,     0,  -114,   204,   112,   113,   114,     0,     0,
       0,     0,     0,     0,     0,     0,    22,     0,     0,     0,
       0,     0,     0,   119,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   120,  -114,   121,    25,     0,     0,   119,
       0,     0,     0,    27,    28,    29,    30,    31,    32,   120,
       0,   121,     0,    33,    34,    35,    36,    37,    38,    39,
      40,     0,    41,     0,    42,    43,    44,     0,     0,     0,
       0,     0,    45,  -114,     0,     0,    46,    47,    48,    49,
      50,   212,     0,    51,  -172,  -172,     0,     0,     0,   123,
    -172,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     152,    22,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   112,   113,   114,  -172,     0,   115,   116,   117,
       0,    25,     0,     0,     0,     0,     0,     0,    27,    28,
      29,    30,    31,    32,   118,     0,     0,     0,    33,    34,
      35,    36,    37,    38,    39,    40,   119,    41,     0,    42,
      43,    44,     0,     0,     0,     0,   120,    45,   121,     0,
       0,    46,    47,    48,   124,    50,   216,     0,    51,  -172,
    -172,     0,     0,     0,   123,  -172,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   152,    22,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   112,   113,   114,
    -172,     0,     0,     0,     0,     0,    25,     0,   278,     0,
       0,     0,     0,    27,    28,    29,    30,    31,    32,     0,
     241,   242,   243,    33,    34,    35,    36,    37,    38,    39,
      40,   119,    41,     0,    42,    43,    44,   122,     0,     0,
    -175,   120,    45,   121,     0,   123,    46,    47,    48,   124,
      50,     0,     0,    51,   244,     0,     0,    22,     0,     0,
       0,     0,     0,   255,   245,     0,   246,     0,     0,     0,
     112,   113,   114,     0,     0,     0,     0,    25,     0,     0,
       0,     0,    71,     0,    27,    28,    29,    30,    31,    32,
      72,     0,     0,     0,    33,    34,    35,    36,    37,    38,
      39,    40,    22,    41,   119,    42,    43,    44,     0,     0,
       0,     0,     0,    45,   120,     0,   121,    46,    47,    48,
     124,    50,    25,     0,    51,     0,     0,     0,     0,    27,
      28,    29,    30,    31,    32,     0,     0,     0,     0,    33,
      34,    35,    36,    37,    38,    39,    40,     0,    41,     0,
      42,    43,    44,     0,   -55,     0,     0,     0,    45,    21,
       0,     0,    46,    47,    48,    49,    50,     0,     0,    51,
       0,    22,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    23,    24,   316,     0,
       0,    25,     0,    26,     0,   112,   113,   114,    27,    28,
      29,    30,    31,    32,    72,     0,     0,     0,    33,    34,
      35,    36,    37,    38,    39,    40,    22,    41,     0,    42,
      43,    44,     0,     0,     0,     0,     0,    45,     0,   119,
       0,    46,    47,    48,    49,    50,    25,     0,    51,   120,
       0,   121,     0,    27,    28,    29,    30,    31,    32,     0,
       0,     0,     0,    33,    34,    35,    36,    37,    38,    39,
      40,     0,    41,     0,    42,    43,    44,     0,     0,     0,
       0,     0,    45,    72,     0,     0,    46,    47,    48,    49,
      50,     0,     0,    51,   261,    22,   112,   113,   114,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    25,     0,     0,     0,     0,
       0,     0,    27,    28,    29,    30,    31,    32,     0,     0,
     119,     0,    33,    34,    35,    36,    37,    38,    39,    40,
     120,    41,   121,    42,    43,    44,     0,     0,    21,   -55,
       0,    45,     0,     0,     0,    46,    47,    48,    49,    50,
      22,     0,    51,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    23,    24,     0,     0,     0,
      25,     0,     0,     0,     0,     0,     0,    27,    28,    29,
      30,    31,    32,     0,     0,     0,     0,    33,    34,    35,
      36,    37,    38,    39,    40,    21,    41,     0,    42,    43,
      44,     0,     0,     0,     0,     0,    45,    22,     0,     0,
      46,    47,    48,    49,    50,     0,     0,    51,     0,     0,
       0,   -55,    23,    24,     0,     0,     0,    25,     0,     0,
       0,     0,     0,     0,    27,    28,    29,    30,    31,    32,
       0,     0,     0,     0,    33,    34,    35,    36,    37,    38,
      39,    40,   157,    41,     0,    42,    43,    44,     0,     0,
       0,     0,     0,    45,    22,     0,     0,    46,    47,    48,
      49,    50,     0,     0,    51,     0,     0,     0,     0,    23,
      24,     0,     0,     0,    25,     0,     0,     0,     0,     0,
       0,    27,    28,    29,    30,    31,    32,    72,     0,     0,
       0,    33,    34,    35,    36,    37,    38,    39,    40,    22,
      41,     0,    42,    43,    44,     0,     0,     0,     0,     0,
      45,     0,     0,     0,    46,    47,    48,    49,    50,    25,
       0,    51,     0,     0,     0,     0,    27,    28,    29,    30,
      31,    32,   123,     0,     0,     0,    33,    34,    35,    36,
      37,    38,    39,    40,    22,    41,     0,    42,    43,    44,
       0,     0,     0,     0,     0,    45,     0,     0,     0,    46,
      47,    48,    49,    50,    25,     0,    51,     0,     0,     0,
       0,    27,    28,    29,    30,    31,    32,   204,     0,     0,
       0,    33,    34,    35,    36,    37,    38,    39,    40,    22,
      41,     0,    42,    43,    44,     0,     0,     0,     0,     0,
      45,     0,     0,     0,    46,    47,    48,   124,    50,    25,
       0,    51,     0,     0,     0,     0,    27,    28,    29,    30,
      31,    32,     0,     0,     0,     0,    33,    34,    35,    36,
      37,    38,    39,    40,     0,    41,     0,    42,    43,    44,
       0,   112,   113,   114,     0,    45,   115,   116,   117,    46,
      47,    48,    49,    50,     0,     0,    51,     0,     0,     0,
       0,     0,     0,   118,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   119,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   120,     0,   121
  };

  const short int
   KappaParser ::yycheck_[] =
  {
      10,    21,   111,     1,    13,     9,    26,    13,     1,     1,
      57,    21,    22,    13,   204,   223,   224,    10,    45,    82,
      13,    28,    69,    33,    34,    35,    36,    37,    38,    39,
      40,    41,    18,    28,    48,    85,    46,    47,    85,    25,
      26,    85,   151,     9,    89,     1,    96,    57,     4,    76,
       1,   160,    45,     9,    46,    69,    70,    10,     9,    39,
     268,    66,    72,    68,     9,    83,    76,    85,    77,    83,
      88,    84,    25,    26,    88,     4,   123,   140,    89,    85,
      84,    84,    89,    76,    84,   293,    84,   295,   108,   109,
      96,    83,    84,    86,    89,     5,   106,   107,    78,     9,
      85,   148,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,    23,   123,   161,   162,   306,    83,    84,    21,
      23,    31,    32,   232,    26,    85,    10,    83,    84,    85,
      40,    11,    88,   153,    19,    20,   146,   157,    83,    84,
      85,    25,    26,    88,     6,    56,     8,   157,    85,    33,
      34,   161,   162,    56,    66,    66,    68,    68,    77,    25,
      26,    13,    72,    66,    74,    68,   213,    33,    34,    79,
      80,   192,    16,    17,    84,    85,   196,    25,    26,    10,
      90,    91,     1,    77,     9,     4,     5,    17,    10,   198,
       9,    10,    35,    15,   204,    10,    12,    22,    23,    24,
      22,    23,    24,    22,    23,    24,    22,    23,    24,   219,
      85,   231,   222,   223,   224,     9,    35,   227,   228,    13,
     241,   242,   243,   244,   245,   246,    36,    37,    22,    23,
      24,    56,   170,   171,    56,    25,    85,    56,    16,   249,
      56,    66,   252,    68,    66,    28,    68,    66,    35,    68,
      66,     5,    68,    73,    66,    13,    18,    10,   268,    84,
       0,     1,    56,     6,     4,    84,     6,    28,     8,     9,
      25,    26,    66,    10,    68,     7,    10,    84,    33,    34,
      27,    21,    27,   293,    14,   295,    27,    66,    27,    22,
      30,    14,    84,    10,   304,   305,   306,   132,    38,    39,
     240,    41,    42,   127,   221,     9,   316,    47,    48,    49,
      50,    51,    52,    53,    54,    55,   145,    57,    58,    59,
      60,    61,    62,    63,    64,    65,   131,    67,   139,    69,
      70,    71,   138,   287,   162,    75,     7,    77,    78,   237,
      -1,    81,    82,    83,    84,    85,   234,    -1,    88,    22,
      23,    24,    92,    93,    94,    95,     1,    14,    -1,     4,
      -1,     6,    -1,     8,     9,    22,    23,    24,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    66,    39,    68,    41,    -1,    -1,    56,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    66,
      -1,    68,    -1,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    67,    -1,    69,    70,    71,    -1,    -1,    -1,
      -1,    -1,    77,    78,    -1,    -1,    81,    82,    83,    84,
      85,     1,    -1,    88,     4,     5,    -1,    -1,    -1,     9,
      10,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      10,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    22,    23,    24,    35,    -1,    27,    28,    29,
      -1,    41,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    44,    -1,    -1,    -1,    58,    59,
      60,    61,    62,    63,    64,    65,    56,    67,    -1,    69,
      70,    71,    -1,    -1,    -1,    -1,    66,    77,    68,    -1,
      -1,    81,    82,    83,    84,    85,     1,    -1,    88,     4,
       5,    -1,    -1,    -1,     9,    10,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    10,    21,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,
      35,    -1,    -1,    -1,    -1,    -1,    41,    -1,    10,    -1,
      -1,    -1,    -1,    48,    49,    50,    51,    52,    53,    -1,
      22,    23,    24,    58,    59,    60,    61,    62,    63,    64,
      65,    56,    67,    -1,    69,    70,    71,     1,    -1,    -1,
       4,    66,    77,    68,    -1,     9,    81,    82,    83,    84,
      85,    -1,    -1,    88,    56,    -1,    -1,    21,    -1,    -1,
      -1,    -1,    -1,    15,    66,    -1,    68,    -1,    -1,    -1,
      22,    23,    24,    -1,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,     1,    -1,    48,    49,    50,    51,    52,    53,
       9,    -1,    -1,    -1,    58,    59,    60,    61,    62,    63,
      64,    65,    21,    67,    56,    69,    70,    71,    -1,    -1,
      -1,    -1,    -1,    77,    66,    -1,    68,    81,    82,    83,
      84,    85,    41,    -1,    88,    -1,    -1,    -1,    -1,    48,
      49,    50,    51,    52,    53,    -1,    -1,    -1,    -1,    58,
      59,    60,    61,    62,    63,    64,    65,    -1,    67,    -1,
      69,    70,    71,    -1,     4,    -1,    -1,    -1,    77,     9,
      -1,    -1,    81,    82,    83,    84,    85,    -1,    -1,    88,
      -1,    21,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    37,    15,    -1,
      -1,    41,    -1,    43,    -1,    22,    23,    24,    48,    49,
      50,    51,    52,    53,     9,    -1,    -1,    -1,    58,    59,
      60,    61,    62,    63,    64,    65,    21,    67,    -1,    69,
      70,    71,    -1,    -1,    -1,    -1,    -1,    77,    -1,    56,
      -1,    81,    82,    83,    84,    85,    41,    -1,    88,    66,
      -1,    68,    -1,    48,    49,    50,    51,    52,    53,    -1,
      -1,    -1,    -1,    58,    59,    60,    61,    62,    63,    64,
      65,    -1,    67,    -1,    69,    70,    71,    -1,    -1,    -1,
      -1,    -1,    77,     9,    -1,    -1,    81,    82,    83,    84,
      85,    -1,    -1,    88,    89,    21,    22,    23,    24,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    48,    49,    50,    51,    52,    53,    -1,    -1,
      56,    -1,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    -1,    -1,     9,    10,
      -1,    77,    -1,    -1,    -1,    81,    82,    83,    84,    85,
      21,    -1,    88,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    36,    37,    -1,    -1,    -1,
      41,    -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    -1,    -1,    -1,    -1,    58,    59,    60,
      61,    62,    63,    64,    65,     9,    67,    -1,    69,    70,
      71,    -1,    -1,    -1,    -1,    -1,    77,    21,    -1,    -1,
      81,    82,    83,    84,    85,    -1,    -1,    88,    -1,    -1,
      -1,    35,    36,    37,    -1,    -1,    -1,    41,    -1,    -1,
      -1,    -1,    -1,    -1,    48,    49,    50,    51,    52,    53,
      -1,    -1,    -1,    -1,    58,    59,    60,    61,    62,    63,
      64,    65,     9,    67,    -1,    69,    70,    71,    -1,    -1,
      -1,    -1,    -1,    77,    21,    -1,    -1,    81,    82,    83,
      84,    85,    -1,    -1,    88,    -1,    -1,    -1,    -1,    36,
      37,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,     9,    -1,    -1,
      -1,    58,    59,    60,    61,    62,    63,    64,    65,    21,
      67,    -1,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    81,    82,    83,    84,    85,    41,
      -1,    88,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,     9,    -1,    -1,    -1,    58,    59,    60,    61,
      62,    63,    64,    65,    21,    67,    -1,    69,    70,    71,
      -1,    -1,    -1,    -1,    -1,    77,    -1,    -1,    -1,    81,
      82,    83,    84,    85,    41,    -1,    88,    -1,    -1,    -1,
      -1,    48,    49,    50,    51,    52,    53,     9,    -1,    -1,
      -1,    58,    59,    60,    61,    62,    63,    64,    65,    21,
      67,    -1,    69,    70,    71,    -1,    -1,    -1,    -1,    -1,
      77,    -1,    -1,    -1,    81,    82,    83,    84,    85,    41,
      -1,    88,    -1,    -1,    -1,    -1,    48,    49,    50,    51,
      52,    53,    -1,    -1,    -1,    -1,    58,    59,    60,    61,
      62,    63,    64,    65,    -1,    67,    -1,    69,    70,    71,
      -1,    22,    23,    24,    -1,    77,    27,    28,    29,    81,
      82,    83,    84,    85,    -1,    -1,    88,    -1,    -1,    -1,
      -1,    -1,    -1,    44,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    66,    -1,    68
  };

  const unsigned char
   KappaParser ::yystos_[] =
  {
       0,    98,     0,     1,    30,    38,    42,    47,    54,    55,
      57,    75,    85,    92,    93,    94,    95,    99,   100,   119,
     125,     9,    21,    36,    37,    41,    43,    48,    49,    50,
      51,    52,    53,    58,    59,    60,    61,    62,    63,    64,
      65,    67,    69,    70,    71,    77,    81,    82,    83,    84,
      85,    88,   109,   115,   127,   128,   129,    85,   114,    89,
       1,    84,   134,     1,    83,    84,    85,    88,   101,   118,
     114,     1,     9,   129,    84,    85,   103,    85,    19,    20,
     102,    96,   103,   107,     4,     9,   120,   123,   133,   134,
     109,   115,   129,   129,   109,   115,   129,   129,   129,   129,
     129,   129,   129,   129,   129,    84,   129,   129,    25,    26,
      33,    34,    22,    23,    24,    27,    28,    29,    44,    56,
      66,    68,     1,     9,    84,   129,   133,    89,   106,     1,
       9,    16,    17,   133,   129,    11,   104,   129,   103,    85,
      13,   107,   133,    39,    78,   126,    77,   121,    13,    10,
      10,    33,    10,    35,    77,   129,   129,     9,   115,   115,
       9,    31,    32,    40,    72,    74,    79,    80,    84,    85,
      90,    91,   110,   111,   110,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   106,    84,   135,   136,   137,
     118,   101,     9,    83,    84,   105,    17,   108,   126,   123,
     107,    10,   120,     1,     9,   122,   129,   133,   110,   115,
     115,   110,     1,   129,   132,   133,     1,   132,    85,    28,
      89,   116,    85,    28,    28,    89,   117,    16,    73,   116,
     116,    35,     5,     1,    86,   138,    10,    13,    18,   105,
      12,    22,    23,    24,    56,    66,    68,   115,   103,     6,
       6,     8,   124,   122,   129,    15,    10,   133,    36,    37,
     113,    89,   112,   129,   113,   129,   112,   112,    28,   129,
     129,   115,   110,   138,    45,    76,   139,   136,    10,   104,
     105,   105,   105,   105,   105,   105,    18,     7,   129,   129,
     130,    10,    84,    14,    27,    14,    27,    27,   112,     1,
      46,    83,    84,   127,     9,    13,    22,   112,   112,    27,
      14,   129,   131,   129,   122,    84,    15,    10,   129
  };

  const unsigned char
   KappaParser ::yyr1_[] =
  {
       0,    97,    98,    98,    99,    99,    99,    99,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   100,   100,
     100,   100,   100,   100,   100,   100,   101,   101,   101,   101,
     102,   102,   102,   103,   104,   104,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   106,   106,   106,   107,   107,
     107,   107,   107,   108,   108,   109,   109,   109,   109,   110,
     110,   110,   110,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   112,   112,   112,
     112,   112,   113,   113,   113,   114,   114,   114,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   116,   116,
     116,   117,   117,   117,   118,   118,   118,   118,   119,   119,
     120,   121,   121,   121,   122,   122,   122,   122,   123,   123,
     124,   124,   124,   125,   125,   125,   126,   126,   126,   127,
     127,   127,   127,   127,   128,   128,   128,   128,   128,   128,
     128,   128,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   129,
     129,   129,   129,   129,   129,   130,   130,   130,   130,   131,
     131,   131,   132,   132,   132,   133,   133,   133,   133,   134,
     134,   135,   135,   136,   136,   136,   137,   137,   138,   138,
     138,   139,   139,   139,   139,   139,   139
  };

  const unsigned char
   KappaParser ::yyr2_[] =
  {
       0,     2,     0,     3,     0,     1,     1,     1,     0,     3,
       7,     6,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     5,     3,     6,     0,     2,     3,     4,
       0,     1,     1,     3,     0,     4,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     0,     1,     2,     0,     1,
       1,     2,     3,     0,     3,     0,     3,     3,     3,     0,
       3,     1,     3,     0,     3,     3,     3,     3,     2,     2,
       2,     2,     3,     2,     2,     4,     5,     0,     1,     1,
       3,     3,     0,     1,     1,     2,     2,     2,     0,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     0,     1,
       3,     0,     1,     3,     0,     1,     1,     1,     0,     1,
       2,     0,     2,     2,     0,     3,     3,     5,     0,     1,
       0,     1,     1,     0,     6,     4,     0,     1,     1,     1,
       1,     1,     1,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     1,     0,     4,     1,     3,     0,
       1,     3,     0,     2,     1,     0,     3,     3,     1,     4,
       2,     0,     1,     0,     3,     1,     0,     3,     0,     2,
       1,     0,     2,     2,     4,     1,     2
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
  "\"$ALL\"", "$accept", "statements", "statement", "instruction",
  "init_declaration", "join", "comp_expr", "dimension", "index_expr",
  "value_list", "comp_list", "where_expr", "perturbation_declaration",
  "effect_list", "effect", "print_expr", "boolean", "variable_declaration",
  "bool_expr", "opt_string", "string_or_pr_expr", "multiple", "rule_label",
  "lhs_rhs", "token_expr", "sum_token", "mixture", "rate_sep",
  "rule_expression", "arrow", "constant", "variable", "alg_expr", "rate",
  "alg_with_radius", "multiple_mixture", "non_empty_mixture",
  "agent_expression", "interface_expression", "ne_interface_expression",
  "port_expression", "internal_state", "link_state", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
   KappaParser ::yyrline_[] =
  {
       0,    78,    78,    79,    83,    84,    86,    88,    93,    95,
      97,    99,   101,   103,   105,   107,   109,   111,   113,   115,
     117,   119,   121,   122,   124,   126,   130,   131,   133,   134,
     141,   142,   144,   148,   154,   155,   160,   162,   164,   166,
     168,   170,   172,   174,   176,   180,   181,   183,   188,   189,
     191,   193,   195,   201,   202,   207,   208,   209,   210,   213,
     214,   215,   216,   219,   220,   222,   224,   226,   228,   230,
     232,   234,   236,   238,   240,   242,   243,   249,   250,   251,
     252,   253,   256,   257,   258,   262,   264,   266,   270,   271,
     273,   275,   277,   279,   281,   283,   285,   287,   292,   293,
     294,   297,   298,   299,   303,   304,   305,   306,   311,   312,
     317,   321,   322,   323,   327,   328,   330,   332,   337,   338,
     342,   343,   344,   348,   349,   351,   357,   358,   360,   365,
     367,   369,   371,   373,   378,   380,   382,   384,   386,   388,
     390,   392,   397,   399,   401,   403,   405,   407,   409,   411,
     413,   415,   417,   419,   421,   423,   425,   427,   429,   431,
     433,   436,   438,   440,   442,   446,   447,   449,   451,   455,
     456,   457,   460,   461,   463,   467,   468,   470,   472,   477,
     479,   485,   486,   490,   491,   493,   498,   499,   504,   505,
     507,   513,   514,   516,   518,   520,   522
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
#line 2439 "KappaParser.cpp" // lalr1.cc:1155
#line 526 "kappaParser.y" // lalr1.cc:1156


void yy::KappaParser::error(const location &loc , const std::string &message) {
        
        // Location should be initialized inside scanner action, but is not in this example.
        // Let's grab location directly from driver class.
	// cout << "Error: " << message << endl << "Location: " << loc << endl;
	
        cout << "Error: " << message << endl << "Error location: " << loc << endl;
}
