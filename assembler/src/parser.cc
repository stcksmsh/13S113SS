// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

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

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.hh"


// Unqualified %code blocks.
#line 61 "src/parser.y"

   #include <iostream>
   #include <cstdint>
   #include <fstream>
   
   /* include for all driver functions */
   #include "driver.hpp"

   #undef yylex
   #define yylex scanner.yylex

#line 58 "src/parser.cc"


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


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
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
    while (false)
# endif


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
      *yycdebug_ << '\n';                       \
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
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 4 "src/parser.y"
namespace Assembler {
#line 151 "src/parser.cc"

  /// Build a parser object.
  Parser::Parser (Scanner  &scanner_yyarg, Driver  &driver_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      scanner (scanner_yyarg),
      driver (driver_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
    , location (that.location)
  {
    switch (this->kind ())
    {
      case symbol_kind::S_53_expression_: // "expression"
      case symbol_kind::S_expression: // expression
        value.copy< Assembler::EquStruct* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_CSR: // "control and status register"
        value.copy< Assembler::csr_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_52_operand_: // "operand"
      case symbol_kind::S_operand: // operand
        value.copy< Assembler::extended_instruction > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // "literal"
        value.copy< int32_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_SYMBOL: // "symbol"
      case symbol_kind::S_LABEL: // "label"
      case symbol_kind::S_STRING: // "ascii"
      case symbol_kind::S_54_label_type_: // "label_type"
      case symbol_kind::S_label: // label
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_50_symbol_list_: // "symbol_list"
      case symbol_kind::S_symbol_list: // symbol_list
        value.copy< std::vector<std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_51_symbol_or_literal_list_: // "symbol_or_literal_list"
      case symbol_kind::S_symbol_or_literal_list: // symbol_or_literal_list
        value.copy< std::vector<uint32_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_GPR: // "general purpose register"
        value.copy< uint8_t > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  Parser::symbol_kind_type
  Parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_53_expression_: // "expression"
      case symbol_kind::S_expression: // expression
        value.move< Assembler::EquStruct* > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_CSR: // "control and status register"
        value.move< Assembler::csr_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_52_operand_: // "operand"
      case symbol_kind::S_operand: // operand
        value.move< Assembler::extended_instruction > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_NUMBER: // "literal"
        value.move< int32_t > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_SYMBOL: // "symbol"
      case symbol_kind::S_LABEL: // "label"
      case symbol_kind::S_STRING: // "ascii"
      case symbol_kind::S_54_label_type_: // "label_type"
      case symbol_kind::S_label: // label
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_50_symbol_list_: // "symbol_list"
      case symbol_kind::S_symbol_list: // symbol_list
        value.move< std::vector<std::string> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_51_symbol_or_literal_list_: // "symbol_or_literal_list"
      case symbol_kind::S_symbol_or_literal_list: // symbol_or_literal_list
        value.move< std::vector<uint32_t> > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_GPR: // "general purpose register"
        value.move< uint8_t > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

    location = YY_MOVE (s.location);
  }

  // by_kind.
  Parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  Parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  Parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  Parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  Parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  Parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  Parser::symbol_kind_type
  Parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  Parser::symbol_kind_type
  Parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_kind_type
  Parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state), YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_53_expression_: // "expression"
      case symbol_kind::S_expression: // expression
        value.YY_MOVE_OR_COPY< Assembler::EquStruct* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_CSR: // "control and status register"
        value.YY_MOVE_OR_COPY< Assembler::csr_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_52_operand_: // "operand"
      case symbol_kind::S_operand: // operand
        value.YY_MOVE_OR_COPY< Assembler::extended_instruction > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // "literal"
        value.YY_MOVE_OR_COPY< int32_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_SYMBOL: // "symbol"
      case symbol_kind::S_LABEL: // "label"
      case symbol_kind::S_STRING: // "ascii"
      case symbol_kind::S_54_label_type_: // "label_type"
      case symbol_kind::S_label: // label
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_50_symbol_list_: // "symbol_list"
      case symbol_kind::S_symbol_list: // symbol_list
        value.YY_MOVE_OR_COPY< std::vector<std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_51_symbol_or_literal_list_: // "symbol_or_literal_list"
      case symbol_kind::S_symbol_or_literal_list: // symbol_or_literal_list
        value.YY_MOVE_OR_COPY< std::vector<uint32_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_GPR: // "general purpose register"
        value.YY_MOVE_OR_COPY< uint8_t > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s, YY_MOVE (that.location))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_53_expression_: // "expression"
      case symbol_kind::S_expression: // expression
        value.move< Assembler::EquStruct* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_CSR: // "control and status register"
        value.move< Assembler::csr_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_52_operand_: // "operand"
      case symbol_kind::S_operand: // operand
        value.move< Assembler::extended_instruction > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // "literal"
        value.move< int32_t > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_SYMBOL: // "symbol"
      case symbol_kind::S_LABEL: // "label"
      case symbol_kind::S_STRING: // "ascii"
      case symbol_kind::S_54_label_type_: // "label_type"
      case symbol_kind::S_label: // label
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_50_symbol_list_: // "symbol_list"
      case symbol_kind::S_symbol_list: // symbol_list
        value.move< std::vector<std::string> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_51_symbol_or_literal_list_: // "symbol_or_literal_list"
      case symbol_kind::S_symbol_or_literal_list: // symbol_or_literal_list
        value.move< std::vector<uint32_t> > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_GPR: // "general purpose register"
        value.move< uint8_t > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_53_expression_: // "expression"
      case symbol_kind::S_expression: // expression
        value.copy< Assembler::EquStruct* > (that.value);
        break;

      case symbol_kind::S_CSR: // "control and status register"
        value.copy< Assembler::csr_type > (that.value);
        break;

      case symbol_kind::S_52_operand_: // "operand"
      case symbol_kind::S_operand: // operand
        value.copy< Assembler::extended_instruction > (that.value);
        break;

      case symbol_kind::S_NUMBER: // "literal"
        value.copy< int32_t > (that.value);
        break;

      case symbol_kind::S_SYMBOL: // "symbol"
      case symbol_kind::S_LABEL: // "label"
      case symbol_kind::S_STRING: // "ascii"
      case symbol_kind::S_54_label_type_: // "label_type"
      case symbol_kind::S_label: // label
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_50_symbol_list_: // "symbol_list"
      case symbol_kind::S_symbol_list: // symbol_list
        value.copy< std::vector<std::string> > (that.value);
        break;

      case symbol_kind::S_51_symbol_or_literal_list_: // "symbol_or_literal_list"
      case symbol_kind::S_symbol_or_literal_list: // symbol_or_literal_list
        value.copy< std::vector<uint32_t> > (that.value);
        break;

      case symbol_kind::S_GPR: // "general purpose register"
        value.copy< uint8_t > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_53_expression_: // "expression"
      case symbol_kind::S_expression: // expression
        value.move< Assembler::EquStruct* > (that.value);
        break;

      case symbol_kind::S_CSR: // "control and status register"
        value.move< Assembler::csr_type > (that.value);
        break;

      case symbol_kind::S_52_operand_: // "operand"
      case symbol_kind::S_operand: // operand
        value.move< Assembler::extended_instruction > (that.value);
        break;

      case symbol_kind::S_NUMBER: // "literal"
        value.move< int32_t > (that.value);
        break;

      case symbol_kind::S_SYMBOL: // "symbol"
      case symbol_kind::S_LABEL: // "label"
      case symbol_kind::S_STRING: // "ascii"
      case symbol_kind::S_54_label_type_: // "label_type"
      case symbol_kind::S_label: // label
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_50_symbol_list_: // "symbol_list"
      case symbol_kind::S_symbol_list: // symbol_list
        value.move< std::vector<std::string> > (that.value);
        break;

      case symbol_kind::S_51_symbol_or_literal_list_: // "symbol_or_literal_list"
      case symbol_kind::S_symbol_or_literal_list: // symbol_or_literal_list
        value.move< std::vector<uint32_t> > (that.value);
        break;

      case symbol_kind::S_GPR: // "general purpose register"
        value.move< uint8_t > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " ("
            << yysym.location << ": ";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
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

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            yyla.kind_ = yytranslate_ (yylex (&yyla.value, &yyla.location));
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_53_expression_: // "expression"
      case symbol_kind::S_expression: // expression
        yylhs.value.emplace< Assembler::EquStruct* > ();
        break;

      case symbol_kind::S_CSR: // "control and status register"
        yylhs.value.emplace< Assembler::csr_type > ();
        break;

      case symbol_kind::S_52_operand_: // "operand"
      case symbol_kind::S_operand: // operand
        yylhs.value.emplace< Assembler::extended_instruction > ();
        break;

      case symbol_kind::S_NUMBER: // "literal"
        yylhs.value.emplace< int32_t > ();
        break;

      case symbol_kind::S_SYMBOL: // "symbol"
      case symbol_kind::S_LABEL: // "label"
      case symbol_kind::S_STRING: // "ascii"
      case symbol_kind::S_54_label_type_: // "label_type"
      case symbol_kind::S_label: // label
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_50_symbol_list_: // "symbol_list"
      case symbol_kind::S_symbol_list: // symbol_list
        yylhs.value.emplace< std::vector<std::string> > ();
        break;

      case symbol_kind::S_51_symbol_or_literal_list_: // "symbol_or_literal_list"
      case symbol_kind::S_symbol_or_literal_list: // symbol_or_literal_list
        yylhs.value.emplace< std::vector<uint32_t> > ();
        break;

      case symbol_kind::S_GPR: // "general purpose register"
        yylhs.value.emplace< uint8_t > ();
        break;

      default:
        break;
    }


      // Default location.
      {
        stack_type::slice range (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, range, yylen);
        yyerror_range[1].location = yylhs.location;
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 4: // program: program "end_drct"
#line 155 "src/parser.y"
                     { return 0; }
#line 921 "src/parser.cc"
    break;

  case 5: // program: program label "end_drct"
#line 156 "src/parser.y"
                       { return 0; }
#line 927 "src/parser.cc"
    break;

  case 13: // instruction: "halt_inst"
#line 170 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::HALT; driver.append_TEXT(inst.raw); }
#line 933 "src/parser.cc"
    break;

  case 14: // instruction: "int_inst"
#line 171 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::INT; driver.append_TEXT(inst.raw); }
#line 939 "src/parser.cc"
    break;

  case 15: // instruction: "iret_inst"
#line 172 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::IRET; driver.append_TEXT(inst.raw); }
#line 945 "src/parser.cc"
    break;

  case 16: // instruction: "call_inst" operand
#line 173 "src/parser.y"
                                        { yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.OC = Opcode::CALL; 
                                          driver.append_TEXT(yystack_[0].value.as < Assembler::extended_instruction > ().inst.raw); 
                                          if((yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.MOD & 0b100) == 0){
                                             driver.append_TEXT(yystack_[0].value.as < Assembler::extended_instruction > ().extension);
                                          }
                                        }
#line 956 "src/parser.cc"
    break;

  case 17: // instruction: "ret_inst"
#line 179 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::RET; driver.append_TEXT(inst.raw); }
#line 962 "src/parser.cc"
    break;

  case 18: // instruction: "jmp_inst" operand
#line 180 "src/parser.y"
                                        { yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.OC = Opcode::JMP; 
                                          driver.append_TEXT(yystack_[0].value.as < Assembler::extended_instruction > ().inst.raw);
                                          if((yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.MOD & 0b100) == 0){
                                             driver.append_TEXT(yystack_[0].value.as < Assembler::extended_instruction > ().extension);
                                          }
                                        }
#line 973 "src/parser.cc"
    break;

  case 19: // instruction: "beq_inst" "general purpose register" "," "general purpose register" "," operand
#line 186 "src/parser.y"
                                        { yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.OC = Opcode::BEQ; yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.RegA = yystack_[4].value.as < uint8_t > (); yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.RegB = yystack_[2].value.as < uint8_t > (); 
                                          driver.append_TEXT(yystack_[0].value.as < Assembler::extended_instruction > ().inst.raw);
                                          if((yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.MOD & 0b100) == 0){
                                             driver.append_TEXT(yystack_[0].value.as < Assembler::extended_instruction > ().extension);
                                          }
                                        }
#line 984 "src/parser.cc"
    break;

  case 20: // instruction: "bne_inst" "general purpose register" "," "general purpose register" "," operand
#line 192 "src/parser.y"
                                        { yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.OC = Opcode::BNE; yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.RegA = yystack_[4].value.as < uint8_t > (); yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.RegB = yystack_[2].value.as < uint8_t > (); 
                                          driver.append_TEXT(yystack_[0].value.as < Assembler::extended_instruction > ().inst.raw);
                                          if((yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.MOD & 0b100) == 0){
                                             driver.append_TEXT(yystack_[0].value.as < Assembler::extended_instruction > ().extension);
                                          }
                                        }
#line 995 "src/parser.cc"
    break;

  case 21: // instruction: "bgt_inst" "general purpose register" "," "general purpose register" "," operand
#line 198 "src/parser.y"
                                        { yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.OC = Opcode::BGT; yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.RegA = yystack_[4].value.as < uint8_t > (); yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.RegB = yystack_[2].value.as < uint8_t > (); 
                                          driver.append_TEXT(yystack_[0].value.as < Assembler::extended_instruction > ().inst.raw);
                                          if((yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.MOD & 0b100) == 0){
                                             driver.append_TEXT(yystack_[0].value.as < Assembler::extended_instruction > ().extension);
                                          }
                                        }
#line 1006 "src/parser.cc"
    break;

  case 22: // instruction: "push_inst" "general purpose register"
#line 204 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::PUSH; inst.fields.RegA = yystack_[0].value.as < uint8_t > (); driver.append_TEXT(inst.raw); }
#line 1012 "src/parser.cc"
    break;

  case 23: // instruction: "pop_inst" "general purpose register"
#line 205 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::POP; inst.fields.RegA = yystack_[0].value.as < uint8_t > (); driver.append_TEXT(inst.raw); }
#line 1018 "src/parser.cc"
    break;

  case 24: // instruction: "xchg_inst" "general purpose register" "," "general purpose register"
#line 206 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::XCHG; inst.fields.RegA = yystack_[2].value.as < uint8_t > (); inst.fields.RegB = yystack_[0].value.as < uint8_t > (); driver.append_TEXT(inst.raw); }
#line 1024 "src/parser.cc"
    break;

  case 25: // instruction: "add_inst" "general purpose register" "," "general purpose register"
#line 207 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::ADD; inst.fields.RegA = yystack_[2].value.as < uint8_t > (); inst.fields.RegB = yystack_[0].value.as < uint8_t > (); driver.append_TEXT(inst.raw); }
#line 1030 "src/parser.cc"
    break;

  case 26: // instruction: "sub_inst" "general purpose register" "," "general purpose register"
#line 208 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::SUB; inst.fields.RegA = yystack_[2].value.as < uint8_t > (); inst.fields.RegB = yystack_[0].value.as < uint8_t > (); driver.append_TEXT(inst.raw); }
#line 1036 "src/parser.cc"
    break;

  case 27: // instruction: "mul_inst" "general purpose register" "," "general purpose register"
#line 209 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::MUL; inst.fields.RegA = yystack_[2].value.as < uint8_t > (); inst.fields.RegB = yystack_[0].value.as < uint8_t > (); driver.append_TEXT(inst.raw); }
#line 1042 "src/parser.cc"
    break;

  case 28: // instruction: "div_inst" "general purpose register" "," "general purpose register"
#line 210 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::DIV; inst.fields.RegA = yystack_[2].value.as < uint8_t > (); inst.fields.RegB = yystack_[0].value.as < uint8_t > (); driver.append_TEXT(inst.raw); }
#line 1048 "src/parser.cc"
    break;

  case 29: // instruction: "not_inst" "general purpose register"
#line 211 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::NOT; inst.fields.RegA = yystack_[0].value.as < uint8_t > (); driver.append_TEXT(inst.raw); }
#line 1054 "src/parser.cc"
    break;

  case 30: // instruction: "and_inst" "general purpose register" "," "general purpose register"
#line 212 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::AND; inst.fields.RegA = yystack_[2].value.as < uint8_t > (); inst.fields.RegB = yystack_[0].value.as < uint8_t > (); driver.append_TEXT(inst.raw); }
#line 1060 "src/parser.cc"
    break;

  case 31: // instruction: "or_inst" "general purpose register" "," "general purpose register"
#line 213 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::OR; inst.fields.RegA = yystack_[2].value.as < uint8_t > (); inst.fields.RegB = yystack_[0].value.as < uint8_t > (); driver.append_TEXT(inst.raw); }
#line 1066 "src/parser.cc"
    break;

  case 32: // instruction: "xor_inst" "general purpose register" "," "general purpose register"
#line 214 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::XOR; inst.fields.RegA = yystack_[2].value.as < uint8_t > (); inst.fields.RegB = yystack_[0].value.as < uint8_t > (); driver.append_TEXT(inst.raw); }
#line 1072 "src/parser.cc"
    break;

  case 33: // instruction: "shl_inst" "general purpose register" "," "general purpose register"
#line 215 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::SHL; inst.fields.RegA = yystack_[2].value.as < uint8_t > (); inst.fields.RegB = yystack_[0].value.as < uint8_t > (); driver.append_TEXT(inst.raw); }
#line 1078 "src/parser.cc"
    break;

  case 34: // instruction: "shr_inst" "general purpose register" "," "general purpose register"
#line 216 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::SHR; inst.fields.RegA = yystack_[2].value.as < uint8_t > (); inst.fields.RegB = yystack_[0].value.as < uint8_t > (); driver.append_TEXT(inst.raw); }
#line 1084 "src/parser.cc"
    break;

  case 35: // instruction: "ld_inst" operand "," "general purpose register"
#line 217 "src/parser.y"
                                        { yystack_[2].value.as < Assembler::extended_instruction > ().inst.fields.OC = Opcode::LD; yystack_[2].value.as < Assembler::extended_instruction > ().inst.fields.RegA = yystack_[0].value.as < uint8_t > (); 
                                          driver.append_TEXT(yystack_[2].value.as < Assembler::extended_instruction > ().inst.raw);
                                          if((yystack_[2].value.as < Assembler::extended_instruction > ().inst.fields.MOD & 0b100) == 0){
                                             driver.append_TEXT(yystack_[2].value.as < Assembler::extended_instruction > ().extension);
                                          }
                                        }
#line 1095 "src/parser.cc"
    break;

  case 36: // instruction: "st_inst" "general purpose register" "," operand
#line 223 "src/parser.y"
                                        { yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.OC = Opcode::ST; yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.RegA = yystack_[2].value.as < uint8_t > ();
                                          driver.append_TEXT(yystack_[0].value.as < Assembler::extended_instruction > ().inst.raw);
                                          if((yystack_[0].value.as < Assembler::extended_instruction > ().inst.fields.MOD & 0b100) == 0){
                                             driver.append_TEXT(yystack_[0].value.as < Assembler::extended_instruction > ().extension);
                                          }
                                        }
#line 1106 "src/parser.cc"
    break;

  case 37: // instruction: "csrrd_inst" "control and status register" "," "general purpose register"
#line 229 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::CSRRD; inst.fields.RegA = yystack_[2].value.as < Assembler::csr_type > (); inst.fields.RegB = yystack_[0].value.as < uint8_t > (); driver.append_TEXT(inst.raw); }
#line 1112 "src/parser.cc"
    break;

  case 38: // instruction: "csrwr_inst" "general purpose register" "," "control and status register"
#line 230 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = Opcode::CSRWR; inst.fields.RegA = yystack_[2].value.as < uint8_t > (); inst.fields.RegB = yystack_[0].value.as < Assembler::csr_type > (); driver.append_TEXT(inst.raw); }
#line 1118 "src/parser.cc"
    break;

  case 39: // operand: "$" "literal"
#line 234 "src/parser.y"
                                        { yylhs.value.as < Assembler::extended_instruction > ().inst.fields.MOD = Modifier::LIT_DIR; yylhs.value.as < Assembler::extended_instruction > ().extension = yystack_[0].value.as < int32_t > (); }
#line 1124 "src/parser.cc"
    break;

  case 40: // operand: "literal"
#line 235 "src/parser.y"
                                        { yylhs.value.as < Assembler::extended_instruction > ().inst.fields.MOD = Modifier::LIT_IND; yylhs.value.as < Assembler::extended_instruction > ().extension = yystack_[0].value.as < int32_t > (); }
#line 1130 "src/parser.cc"
    break;

  case 41: // operand: "$" "symbol"
#line 236 "src/parser.y"
                                        { yylhs.value.as < Assembler::extended_instruction > ().inst.fields.MOD = Modifier::SYM_DIR;
                                          Driver::STentry *entry = driver.get_symbol(yystack_[0].value.as < std::string > ());
                                          if(!entry || !entry->is_defined){
                                             driver.forward_reference(yystack_[0].value.as < std::string > ());
                                             yylhs.value.as < Assembler::extended_instruction > ().extension = 0;
                                          }else{
                                             if(!entry->is_const){
                                                driver.add_relocation(yystack_[0].value.as < std::string > ());
                                             }
                                             yylhs.value.as < Assembler::extended_instruction > ().extension = entry->offset;
                                          }
                                        }
#line 1147 "src/parser.cc"
    break;

  case 42: // operand: "symbol"
#line 248 "src/parser.y"
                                        { yylhs.value.as < Assembler::extended_instruction > ().inst.fields.MOD = Modifier::SYM_IND;
                                          Driver::STentry *entry = driver.get_symbol(yystack_[0].value.as < std::string > ());
                                          if(!entry || !entry->is_defined){
                                             driver.forward_reference(yystack_[0].value.as < std::string > ());
                                             yylhs.value.as < Assembler::extended_instruction > ().extension = 0;
                                          }else{
                                             if(!entry->is_const){
                                                driver.add_relocation(yystack_[0].value.as < std::string > ());
                                             } 
                                             yylhs.value.as < Assembler::extended_instruction > ().extension = entry->offset;
                                          }
                                        }
#line 1164 "src/parser.cc"
    break;

  case 43: // operand: "general purpose register"
#line 260 "src/parser.y"
                                        { yylhs.value.as < Assembler::extended_instruction > ().inst.fields.MOD = Modifier::REG_DIR; yylhs.value.as < Assembler::extended_instruction > ().inst.fields.RegC = yystack_[0].value.as < uint8_t > (); }
#line 1170 "src/parser.cc"
    break;

  case 44: // operand: "[" "general purpose register" "]"
#line 261 "src/parser.y"
                                        { yylhs.value.as < Assembler::extended_instruction > ().inst.fields.MOD = Modifier::REG_IND; yylhs.value.as < Assembler::extended_instruction > ().inst.fields.RegC = yystack_[1].value.as < uint8_t > (); }
#line 1176 "src/parser.cc"
    break;

  case 45: // operand: "[" "general purpose register" "+" "literal" "]"
#line 262 "src/parser.y"
                                        { yylhs.value.as < Assembler::extended_instruction > ().inst.fields.MOD = Modifier::REG_LIT_IND; yylhs.value.as < Assembler::extended_instruction > ().inst.fields.RegC = yystack_[3].value.as < uint8_t > ();
                                          if((yystack_[1].value.as < int32_t > () > (1<<12)-1) || yystack_[1].value.as < int32_t > () < 0-(1<<12)){
                                             driver.logger->logError("Literal value is out of range for REG_LIT_IND addressing", driver.filename, yystack_[2].location.begin.line);
                                          }
                                          else{
                                             yylhs.value.as < Assembler::extended_instruction > ().inst.fields.Disp = yystack_[1].value.as < int32_t > ();
                                          }
                                        }
#line 1189 "src/parser.cc"
    break;

  case 46: // operand: "[" "general purpose register" "+" "symbol" "]"
#line 270 "src/parser.y"
                                        { yylhs.value.as < Assembler::extended_instruction > ().inst.fields.MOD = Modifier::REG_SYM_IND; yylhs.value.as < Assembler::extended_instruction > ().inst.fields.RegC = yystack_[3].value.as < uint8_t > ();
                                          Driver::STentry *entry = driver.get_symbol(yystack_[1].value.as < std::string > ());
                                          if(!entry || !entry->is_defined){
                                             driver.logger->logError("Symbol '" + yystack_[1].value.as < std::string > () + "' is not defined at assembly time, which is required for REG_SYM_IND addressing", driver.filename, yystack_[2].location.begin.line);
                                          }
                                          else if(entry->offset > (1<<12)-1){
                                             driver.logger->logError("Symbol '" + yystack_[1].value.as < std::string > () + "' is out of range for REG_SYM_IND addressing", driver.filename, yystack_[2].location.begin.line);
                                          }
                                          else{

                                             if(!entry->is_const){
                                                driver.add_relocation(yystack_[1].value.as < std::string > ());
                                             }
                                             yylhs.value.as < Assembler::extended_instruction > ().inst.fields.Disp = entry->offset;
                                          }
                                        }
#line 1210 "src/parser.cc"
    break;

  case 47: // directive: "global_drct" symbol_list
#line 290 "src/parser.y"
                                        { driver.add_global(yystack_[0].value.as < std::vector<std::string> > ()); }
#line 1216 "src/parser.cc"
    break;

  case 48: // directive: "extern_drct" symbol_list
#line 291 "src/parser.y"
                                        { driver.add_extern(yystack_[0].value.as < std::vector<std::string> > ()); }
#line 1222 "src/parser.cc"
    break;

  case 49: // directive: "section_drct" "symbol"
#line 292 "src/parser.y"
                                        { driver.add_section(yystack_[0].value.as < std::string > ()); }
#line 1228 "src/parser.cc"
    break;

  case 50: // directive: "equ_drct" "symbol" "," expression
#line 293 "src/parser.y"
                                        { driver.add_equ(yystack_[2].value.as < std::string > (), yystack_[0].value.as < Assembler::EquStruct* > ()); }
#line 1234 "src/parser.cc"
    break;

  case 51: // data_directive: "word_drct" symbol_or_literal_list
#line 297 "src/parser.y"
                                        { for(uint32_t val : yystack_[0].value.as < std::vector<uint32_t> > ()) driver.append_DATA(val); }
#line 1240 "src/parser.cc"
    break;

  case 52: // data_directive: label "word_drct" symbol_or_literal_list
#line 298 "src/parser.y"
                                        { driver.update_symbol(yystack_[2].value.as < std::string > (), ".data");
                                          for(uint32_t val : yystack_[0].value.as < std::vector<uint32_t> > ()) driver.append_DATA(val);
                                        }
#line 1248 "src/parser.cc"
    break;

  case 53: // data_directive: "ascii_drct" "ascii"
#line 301 "src/parser.y"
                                        { for(char c : yystack_[0].value.as < std::string > ()) driver.DATA.push_back(c); }
#line 1254 "src/parser.cc"
    break;

  case 54: // data_directive: label "ascii_drct" "ascii"
#line 302 "src/parser.y"
                                        { driver.update_symbol(yystack_[2].value.as < std::string > (), ".data");
                                          for(char c : yystack_[0].value.as < std::string > ()) driver.DATA.push_back(c);
                                        }
#line 1262 "src/parser.cc"
    break;

  case 55: // bss_directive: "skip_drct" "literal"
#line 308 "src/parser.y"
                                        { driver.extend_BSS(yystack_[0].value.as < int32_t > ()); }
#line 1268 "src/parser.cc"
    break;

  case 56: // bss_directive: label "skip_drct" "literal"
#line 309 "src/parser.y"
                                        { driver.update_symbol(yystack_[2].value.as < std::string > (), ".bss");
                                          driver.extend_BSS(yystack_[0].value.as < int32_t > ());
                                        }
#line 1276 "src/parser.cc"
    break;

  case 57: // symbol_list: "symbol"
#line 315 "src/parser.y"
                                        { yylhs.value.as < std::vector<std::string> > () = std::vector<std::string>(); yylhs.value.as < std::vector<std::string> > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 1282 "src/parser.cc"
    break;

  case 58: // symbol_list: symbol_list "," "symbol"
#line 316 "src/parser.y"
                                        { yylhs.value.as < std::vector<std::string> > () = yystack_[2].value.as < std::vector<std::string> > (); yylhs.value.as < std::vector<std::string> > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 1288 "src/parser.cc"
    break;

  case 59: // symbol_or_literal_list: "literal"
#line 320 "src/parser.y"
                                        { yylhs.value.as < std::vector<uint32_t> > () = std::vector<uint32_t>(); yylhs.value.as < std::vector<uint32_t> > ().push_back(yystack_[0].value.as < int32_t > ()); }
#line 1294 "src/parser.cc"
    break;

  case 60: // symbol_or_literal_list: "symbol"
#line 321 "src/parser.y"
                                        { yylhs.value.as < std::vector<uint32_t> > () = std::vector<uint32_t>();
                                          yylhs.value.as < std::vector<uint32_t> > ().push_back(driver.get_symbol(yystack_[0].value.as < std::string > ())->offset);                                          
                                        }
#line 1302 "src/parser.cc"
    break;

  case 61: // symbol_or_literal_list: symbol_or_literal_list "," "literal"
#line 324 "src/parser.y"
                                        { yylhs.value.as < std::vector<uint32_t> > () = yystack_[2].value.as < std::vector<uint32_t> > (); yylhs.value.as < std::vector<uint32_t> > ().push_back(yystack_[0].value.as < int32_t > ()); }
#line 1308 "src/parser.cc"
    break;

  case 62: // symbol_or_literal_list: symbol_or_literal_list "," "symbol"
#line 325 "src/parser.y"
                                        { yylhs.value.as < std::vector<uint32_t> > () = yystack_[2].value.as < std::vector<uint32_t> > ();
                                          Driver::STentry *entry = driver.get_symbol(yystack_[0].value.as < std::string > ());
                                          if(!entry){
                                             driver.logger->logError("Undefined symbol", driver.filename, yystack_[0].location.begin.line);
                                          }
                                          yylhs.value.as < std::vector<uint32_t> > ().push_back(driver.get_symbol(yystack_[0].value.as < std::string > ())->offset); }
#line 1319 "src/parser.cc"
    break;

  case 63: // label: "label"
#line 333 "src/parser.y"
                                        { yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > (); driver.insert_symbol(yystack_[0].value.as < std::string > ()); }
#line 1325 "src/parser.cc"
    break;

  case 64: // label: label "eol"
#line 334 "src/parser.y"
     { yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > (); }
#line 1331 "src/parser.cc"
    break;

  case 65: // expression: "literal"
#line 338 "src/parser.y"
                                        { yylhs.value.as < Assembler::EquStruct* > () = new Assembler::EquStruct(); yylhs.value.as < Assembler::EquStruct* > ()->value = yystack_[0].value.as < int32_t > (); }
#line 1337 "src/parser.cc"
    break;

  case 66: // expression: "symbol"
#line 339 "src/parser.y"
                                        { yylhs.value.as < Assembler::EquStruct* > () = new Assembler::EquStruct(); yylhs.value.as < Assembler::EquStruct* > ()->name = yystack_[0].value.as < std::string > (); }
#line 1343 "src/parser.cc"
    break;

  case 67: // expression: expression "+" "literal"
#line 340 "src/parser.y"
                                    { yylhs.value.as < Assembler::EquStruct* > () = yystack_[2].value.as < Assembler::EquStruct* > (); yylhs.value.as < Assembler::EquStruct* > ()->operation = '+'; yylhs.value.as < Assembler::EquStruct* > ()->next = new Assembler::EquStruct(); yylhs.value.as < Assembler::EquStruct* > ()->next->value = yystack_[0].value.as < int32_t > (); }
#line 1349 "src/parser.cc"
    break;

  case 68: // expression: expression "-" "literal"
#line 341 "src/parser.y"
                                    { yylhs.value.as < Assembler::EquStruct* > () = yystack_[2].value.as < Assembler::EquStruct* > (); yylhs.value.as < Assembler::EquStruct* > ()->operation = '-'; yylhs.value.as < Assembler::EquStruct* > ()->next = new Assembler::EquStruct(); yylhs.value.as < Assembler::EquStruct* > ()->next->value = yystack_[0].value.as < int32_t > (); }
#line 1355 "src/parser.cc"
    break;

  case 69: // expression: expression "+" "symbol"
#line 342 "src/parser.y"
                                    { yylhs.value.as < Assembler::EquStruct* > () = yystack_[2].value.as < Assembler::EquStruct* > (); yylhs.value.as < Assembler::EquStruct* > ()->operation = '+'; yylhs.value.as < Assembler::EquStruct* > ()->next = new Assembler::EquStruct(); yylhs.value.as < Assembler::EquStruct* > ()->next->name = yystack_[0].value.as < std::string > (); }
#line 1361 "src/parser.cc"
    break;

  case 70: // expression: expression "-" "symbol"
#line 343 "src/parser.y"
                                    { yylhs.value.as < Assembler::EquStruct* > () = yystack_[2].value.as < Assembler::EquStruct* > (); yylhs.value.as < Assembler::EquStruct* > ()->operation = '-'; yylhs.value.as < Assembler::EquStruct* > ()->next = new Assembler::EquStruct(); yylhs.value.as < Assembler::EquStruct* > ()->next->name = yystack_[0].value.as < std::string > (); }
#line 1367 "src/parser.cc"
    break;


#line 1371 "src/parser.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
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
        std::string msg = YY_("syntax error");
        error (yyla.location, YY_MOVE (msg));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
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
    {
      stack_symbol_type error_token;

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
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


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  Parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const signed char Parser::yypact_ninf_ = -46;

  const signed char Parser::yytable_ninf_ = -1;

  const short
  Parser::yypact_[] =
  {
     -46,    15,   -46,   -45,   -45,   -35,   -36,   -30,   -33,    13,
     -46,   -46,   -46,   -46,   -38,   -46,   -38,    19,    23,    24,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,   -38,    39,    40,    42,   -46,   -46,   -46,
      47,    50,    51,    52,    95,   -46,    54,    54,   -46,   -46,
     -46,    55,   -46,   -46,    56,    49,   -34,   -46,   -46,   -46,
     -46,   -46,    58,    59,   100,   -46,   -46,   101,   102,   103,
     104,   105,   -46,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   -46,   -46,   -46,   -46,   -36,    88,    21,   -46,
     -46,   115,   116,   117,     8,    10,   -37,   -46,   -46,   118,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   -38,   133,   134,    55,   -46,   -46,   -46,
     -46,   -46,   -46,   -46,   -46,   -46,    25,   -46,    12,   119,
     142,   144,   -46,   -46,   -46,   -46,   -46,   -46,   -46,   -46,
     -46,   -46,   -46,   -46,   -46,   -46,    14,    18,   143,   145,
     -38,   -38,   -38,   -46,   -46,   -46,   -46,   -46,   -46,   -46,
     -46,   -46
  };

  const signed char
  Parser::yydefact_[] =
  {
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       4,    13,    14,    15,     0,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     6,    63,     3,
       0,     0,     0,     0,     0,    57,    47,    48,    49,    59,
      60,    51,    55,    53,     0,     0,     0,    40,    43,    42,
      16,    18,     0,     0,     0,    22,    23,     0,     0,     0,
       0,     0,    29,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     9,     7,    12,    11,     0,     0,     0,     5,
      64,     0,     0,     0,     0,     0,     0,    39,    41,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    52,    56,    54,    10,
       8,    58,    61,    62,    65,    66,    50,    44,     0,     0,
       0,     0,    24,    25,    26,    27,    28,    30,    31,    32,
      33,    34,    35,    36,    37,    38,     0,     0,     0,     0,
       0,     0,     0,    67,    69,    68,    70,    45,    46,    19,
      20,    21
  };

  const short
  Parser::yypgoto_[] =
  {
     -46,   -46,   -46,   138,   -16,   140,   -46,   -46,   148,    69,
     -46,   -46
  };

  const signed char
  Parser::yydefgoto_[] =
  {
       0,     1,    39,    40,    60,    41,    42,    43,    46,    51,
      44,   126
  };

  const unsigned char
  Parser::yytable_[] =
  {
      61,    55,    45,   127,   128,    56,    57,    58,    49,    59,
      97,    50,    48,    98,    52,     2,    53,    78,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,   122,    37,   124,   123,   148,   125,   153,   149,
      54,   154,   155,    38,    62,   156,   146,   147,    63,    64,
     118,    65,    66,    67,    68,    69,    70,    71,    72,    73,
      74,    75,    76,    77,    79,    82,    80,    81,    83,    84,
      85,    93,    94,    95,    96,    99,   100,   143,     3,     4,
       5,    86,    87,    88,     9,    89,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,   117,    90,   159,   160,   161,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,    47,   119,   120,   116,   150,     0,     0,     0,
       0,     0,     0,   129,   121,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   144,   151,
     145,   152,    91,   157,    92,   158
  };

  const short
  Parser::yycheck_[] =
  {
      16,    39,    47,    40,    41,    43,    44,    45,    44,    47,
      44,    47,    47,    47,    44,     0,    49,    33,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    44,    38,    44,    47,    44,    47,    44,    47,
      47,    47,    44,    48,    45,    47,    41,    42,    45,    45,
      49,    45,    45,    45,    45,    45,    45,    45,    45,    45,
      45,    45,    45,    45,    45,    38,    46,    45,    38,    38,
      38,    37,    37,    37,    45,    37,    37,   113,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    44,    38,   150,   151,   152,    37,    37,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,     4,    38,    38,    86,    37,    -1,    -1,    -1,
      -1,    -1,    -1,    45,    47,    45,    45,    45,    45,    45,
      45,    45,    45,    45,    45,    45,    45,    45,    45,    37,
      46,    37,    44,    40,    44,    40
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    56,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    38,    48,    57,
      58,    60,    61,    62,    65,    47,    63,    63,    47,    44,
      47,    64,    44,    49,    47,    39,    43,    44,    45,    47,
      59,    59,    45,    45,    45,    45,    45,    45,    45,    45,
      45,    45,    45,    45,    45,    45,    45,    45,    59,    45,
      46,    45,    38,    38,    38,    38,     6,     7,     8,    10,
      38,    58,    60,    37,    37,    37,    45,    44,    47,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    37,    37,    37,    37,    64,    44,    49,    38,
      38,    47,    44,    47,    44,    47,    66,    40,    41,    45,
      45,    45,    45,    45,    45,    45,    45,    45,    45,    45,
      45,    45,    45,    59,    45,    46,    41,    42,    44,    47,
      37,    37,    37,    44,    47,    44,    47,    40,    40,    59,
      59,    59
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    55,    56,    56,    56,    56,    57,    57,    57,    57,
      57,    57,    57,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    59,
      59,    59,    59,    59,    59,    59,    59,    60,    60,    60,
      60,    61,    61,    61,    61,    62,    62,    63,    63,    64,
      64,    64,    64,    65,    65,    66,    66,    66,    66,    66,
      66
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     0,     2,     2,     3,     1,     2,     3,     2,
       3,     2,     2,     1,     1,     1,     2,     1,     2,     6,
       6,     6,     2,     2,     4,     4,     4,     4,     4,     2,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     2,
       1,     2,     1,     1,     3,     5,     5,     2,     2,     2,
       4,     2,     3,     2,     3,     2,     3,     1,     3,     1,
       1,     3,     3,     1,     2,     1,     1,     3,     3,     3,
       3
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "\"global_drct\"",
  "\"extern_drct\"", "\"section_drct\"", "\"word_drct\"", "\"skip_drct\"",
  "\"ascii_drct\"", "\"equ_drct\"", "\"end_drct\"", "\"halt_inst\"",
  "\"int_inst\"", "\"iret_inst\"", "\"call_inst\"", "\"ret_inst\"",
  "\"jmp_inst\"", "\"beq_inst\"", "\"bne_inst\"", "\"bgt_inst\"",
  "\"push_inst\"", "\"pop_inst\"", "\"xchg_inst\"", "\"add_inst\"",
  "\"sub_inst\"", "\"mul_inst\"", "\"div_inst\"", "\"not_inst\"",
  "\"and_inst\"", "\"or_inst\"", "\"xor_inst\"", "\"shl_inst\"",
  "\"shr_inst\"", "\"ld_inst\"", "\"st_inst\"", "\"csrrd_inst\"",
  "\"csrwr_inst\"", "\",\"", "\"eol\"", "\"[\"", "\"]\"", "\"+\"", "\"-\"",
  "\"$\"", "\"literal\"", "\"general purpose register\"",
  "\"control and status register\"", "\"symbol\"", "\"label\"",
  "\"ascii\"", "\"symbol_list\"", "\"symbol_or_literal_list\"",
  "\"operand\"", "\"expression\"", "\"label_type\"", "$accept", "program",
  "line", "instruction", "operand", "directive", "data_directive",
  "bss_directive", "symbol_list", "symbol_or_literal_list", "label",
  "expression", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   152,   152,   154,   155,   156,   160,   161,   162,   163,
     164,   165,   166,   170,   171,   172,   173,   179,   180,   186,
     192,   198,   204,   205,   206,   207,   208,   209,   210,   211,
     212,   213,   214,   215,   216,   217,   223,   229,   230,   234,
     235,   236,   248,   260,   261,   262,   270,   290,   291,   292,
     293,   297,   298,   301,   302,   308,   309,   315,   316,   320,
     321,   324,   325,   333,   334,   338,   339,   340,   341,   342,
     343
  };

  void
  Parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  Parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  Parser::symbol_kind_type
  Parser::yytranslate_ (int t) YY_NOEXCEPT
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const signed char
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
    };
    // Last valid token kind.
    const int code_max = 309;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 4 "src/parser.y"
} // Assembler
#line 1845 "src/parser.cc"

#line 347 "src/parser.y"


void 
Assembler::Parser::error( const location_type &l, const std::string &err_message )
{
   driver.logger->logError(err_message, driver.filename, l.begin.line);
}
