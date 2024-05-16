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
#line 51 "src/parser.y"

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
      case symbol_kind::S_49_operand_: // "operand"
      case symbol_kind::S_operand: // operand
        value.copy<  Assembler::instruction  > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_48_symbol_list_: // "symbol_list"
      case symbol_kind::S_symbol_list: // symbol_list
        value.copy<  std::vector<std::string>  > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_CSR: // "control and status register"
        value.copy< Assembler::csr_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_GPR: // "general purpose register"
        value.copy< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_SYMBOL: // "symbol"
      case symbol_kind::S_LABEL: // "label"
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // "literal"
        value.copy< uint32_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_49_operand_: // "operand"
      case symbol_kind::S_operand: // operand
        value.move<  Assembler::instruction  > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_48_symbol_list_: // "symbol_list"
      case symbol_kind::S_symbol_list: // symbol_list
        value.move<  std::vector<std::string>  > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_CSR: // "control and status register"
        value.move< Assembler::csr_type > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_GPR: // "general purpose register"
        value.move< int > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_SYMBOL: // "symbol"
      case symbol_kind::S_LABEL: // "label"
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_NUMBER: // "literal"
        value.move< uint32_t > (YY_MOVE (s.value));
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
      case symbol_kind::S_49_operand_: // "operand"
      case symbol_kind::S_operand: // operand
        value.YY_MOVE_OR_COPY<  Assembler::instruction  > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_48_symbol_list_: // "symbol_list"
      case symbol_kind::S_symbol_list: // symbol_list
        value.YY_MOVE_OR_COPY<  std::vector<std::string>  > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_CSR: // "control and status register"
        value.YY_MOVE_OR_COPY< Assembler::csr_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_GPR: // "general purpose register"
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_SYMBOL: // "symbol"
      case symbol_kind::S_LABEL: // "label"
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // "literal"
        value.YY_MOVE_OR_COPY< uint32_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_49_operand_: // "operand"
      case symbol_kind::S_operand: // operand
        value.move<  Assembler::instruction  > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_48_symbol_list_: // "symbol_list"
      case symbol_kind::S_symbol_list: // symbol_list
        value.move<  std::vector<std::string>  > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_CSR: // "control and status register"
        value.move< Assembler::csr_type > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_GPR: // "general purpose register"
        value.move< int > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_SYMBOL: // "symbol"
      case symbol_kind::S_LABEL: // "label"
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_NUMBER: // "literal"
        value.move< uint32_t > (YY_MOVE (that.value));
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
      case symbol_kind::S_49_operand_: // "operand"
      case symbol_kind::S_operand: // operand
        value.copy<  Assembler::instruction  > (that.value);
        break;

      case symbol_kind::S_48_symbol_list_: // "symbol_list"
      case symbol_kind::S_symbol_list: // symbol_list
        value.copy<  std::vector<std::string>  > (that.value);
        break;

      case symbol_kind::S_CSR: // "control and status register"
        value.copy< Assembler::csr_type > (that.value);
        break;

      case symbol_kind::S_GPR: // "general purpose register"
        value.copy< int > (that.value);
        break;

      case symbol_kind::S_SYMBOL: // "symbol"
      case symbol_kind::S_LABEL: // "label"
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_NUMBER: // "literal"
        value.copy< uint32_t > (that.value);
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
      case symbol_kind::S_49_operand_: // "operand"
      case symbol_kind::S_operand: // operand
        value.move<  Assembler::instruction  > (that.value);
        break;

      case symbol_kind::S_48_symbol_list_: // "symbol_list"
      case symbol_kind::S_symbol_list: // symbol_list
        value.move<  std::vector<std::string>  > (that.value);
        break;

      case symbol_kind::S_CSR: // "control and status register"
        value.move< Assembler::csr_type > (that.value);
        break;

      case symbol_kind::S_GPR: // "general purpose register"
        value.move< int > (that.value);
        break;

      case symbol_kind::S_SYMBOL: // "symbol"
      case symbol_kind::S_LABEL: // "label"
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_NUMBER: // "literal"
        value.move< uint32_t > (that.value);
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
      case symbol_kind::S_49_operand_: // "operand"
      case symbol_kind::S_operand: // operand
        yylhs.value.emplace<  Assembler::instruction  > ();
        break;

      case symbol_kind::S_48_symbol_list_: // "symbol_list"
      case symbol_kind::S_symbol_list: // symbol_list
        yylhs.value.emplace<  std::vector<std::string>  > ();
        break;

      case symbol_kind::S_CSR: // "control and status register"
        yylhs.value.emplace< Assembler::csr_type > ();
        break;

      case symbol_kind::S_GPR: // "general purpose register"
        yylhs.value.emplace< int > ();
        break;

      case symbol_kind::S_SYMBOL: // "symbol"
      case symbol_kind::S_LABEL: // "label"
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_NUMBER: // "literal"
        yylhs.value.emplace< uint32_t > ();
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
#line 138 "src/parser.y"
                     { return 0; }
#line 830 "src/parser.cc"
    break;

  case 11: // instruction: "halt_inst"
#line 151 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b00000; driver.append_TEXT(inst.raw); }
#line 836 "src/parser.cc"
    break;

  case 12: // instruction: "int_inst"
#line 152 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b00001; driver.append_TEXT(inst.raw); }
#line 842 "src/parser.cc"
    break;

  case 13: // instruction: "iret_inst"
#line 153 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b00010; driver.append_TEXT(inst.raw); }
#line 848 "src/parser.cc"
    break;

  case 14: // instruction: "call_inst" operand
#line 154 "src/parser.y"
                                        { yystack_[0].value.as <  Assembler::instruction  > ().fields.OC = 0b00011; driver.append_TEXT(yystack_[0].value.as <  Assembler::instruction  > ().raw); std::cout << std::hex << yystack_[0].value.as <  Assembler::instruction  > ().raw << std::endl; }
#line 854 "src/parser.cc"
    break;

  case 15: // instruction: "ret_inst"
#line 155 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b00100; driver.append_TEXT(inst.raw); }
#line 860 "src/parser.cc"
    break;

  case 16: // instruction: "jmp_inst" operand
#line 156 "src/parser.y"
                                        { yystack_[0].value.as <  Assembler::instruction  > ().fields.OC = 0b00101; driver.append_TEXT(yystack_[0].value.as <  Assembler::instruction  > ().raw); }
#line 866 "src/parser.cc"
    break;

  case 17: // instruction: "beq_inst" "general purpose register" "," "general purpose register" "," operand
#line 157 "src/parser.y"
                                        { yystack_[0].value.as <  Assembler::instruction  > ().fields.OC = 0b00110; yystack_[0].value.as <  Assembler::instruction  > ().fields.RegA = yystack_[4].value.as < int > (); yystack_[0].value.as <  Assembler::instruction  > ().fields.RegB = yystack_[2].value.as < int > (); driver.append_TEXT(yystack_[0].value.as <  Assembler::instruction  > ().raw); }
#line 872 "src/parser.cc"
    break;

  case 18: // instruction: "bne_inst" "general purpose register" "," "general purpose register" "," operand
#line 158 "src/parser.y"
                                        { yystack_[0].value.as <  Assembler::instruction  > ().fields.OC = 0b00111; yystack_[0].value.as <  Assembler::instruction  > ().fields.RegA = yystack_[4].value.as < int > (); yystack_[0].value.as <  Assembler::instruction  > ().fields.RegB = yystack_[2].value.as < int > (); driver.append_TEXT(yystack_[0].value.as <  Assembler::instruction  > ().raw); }
#line 878 "src/parser.cc"
    break;

  case 19: // instruction: "bgt_inst" "general purpose register" "," "general purpose register" "," operand
#line 159 "src/parser.y"
                                        { yystack_[0].value.as <  Assembler::instruction  > ().fields.OC = 0b01000; yystack_[0].value.as <  Assembler::instruction  > ().fields.RegA = yystack_[4].value.as < int > (); yystack_[0].value.as <  Assembler::instruction  > ().fields.RegB = yystack_[2].value.as < int > (); driver.append_TEXT(yystack_[0].value.as <  Assembler::instruction  > ().raw); }
#line 884 "src/parser.cc"
    break;

  case 20: // instruction: "push_inst" "general purpose register"
#line 160 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b01001; inst.fields.RegA = yystack_[0].value.as < int > (); driver.append_TEXT(inst.raw); }
#line 890 "src/parser.cc"
    break;

  case 21: // instruction: "pop_inst" "general purpose register"
#line 161 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b01010; inst.fields.RegA = yystack_[0].value.as < int > (); driver.append_TEXT(inst.raw); }
#line 896 "src/parser.cc"
    break;

  case 22: // instruction: "xchg_inst" "general purpose register" "," "general purpose register"
#line 162 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b01011; inst.fields.RegA = yystack_[2].value.as < int > (); inst.fields.RegB = yystack_[0].value.as < int > (); driver.append_TEXT(inst.raw); }
#line 902 "src/parser.cc"
    break;

  case 23: // instruction: "add_inst" "general purpose register" "," "general purpose register"
#line 163 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b01100; inst.fields.RegA = yystack_[2].value.as < int > (); inst.fields.RegB = yystack_[0].value.as < int > (); driver.append_TEXT(inst.raw); }
#line 908 "src/parser.cc"
    break;

  case 24: // instruction: "sub_inst" "general purpose register" "," "general purpose register"
#line 164 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b01101; inst.fields.RegA = yystack_[2].value.as < int > (); inst.fields.RegB = yystack_[0].value.as < int > (); driver.append_TEXT(inst.raw); }
#line 914 "src/parser.cc"
    break;

  case 25: // instruction: "mul_inst" "general purpose register" "," "general purpose register"
#line 165 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b01110; inst.fields.RegA = yystack_[2].value.as < int > (); inst.fields.RegB = yystack_[0].value.as < int > (); driver.append_TEXT(inst.raw); }
#line 920 "src/parser.cc"
    break;

  case 26: // instruction: "div_inst" "general purpose register" "," "general purpose register"
#line 166 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b01111; inst.fields.RegA = yystack_[2].value.as < int > (); inst.fields.RegB = yystack_[0].value.as < int > (); driver.append_TEXT(inst.raw); }
#line 926 "src/parser.cc"
    break;

  case 27: // instruction: "not_inst" "general purpose register"
#line 167 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b10000; inst.fields.RegA = yystack_[0].value.as < int > (); driver.append_TEXT(inst.raw); }
#line 932 "src/parser.cc"
    break;

  case 28: // instruction: "and_inst" "general purpose register" "," "general purpose register"
#line 168 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b10001; inst.fields.RegA = yystack_[2].value.as < int > (); inst.fields.RegB = yystack_[0].value.as < int > (); driver.append_TEXT(inst.raw); }
#line 938 "src/parser.cc"
    break;

  case 29: // instruction: "or_inst" "general purpose register" "," "general purpose register"
#line 169 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b10010; inst.fields.RegA = yystack_[2].value.as < int > (); inst.fields.RegB = yystack_[0].value.as < int > (); driver.append_TEXT(inst.raw); }
#line 944 "src/parser.cc"
    break;

  case 30: // instruction: "xor_inst" "general purpose register" "," "general purpose register"
#line 170 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b10011; inst.fields.RegA = yystack_[2].value.as < int > (); inst.fields.RegB = yystack_[0].value.as < int > (); driver.append_TEXT(inst.raw); }
#line 950 "src/parser.cc"
    break;

  case 31: // instruction: "shl_inst" "general purpose register" "," "general purpose register"
#line 171 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b10100; inst.fields.RegA = yystack_[2].value.as < int > (); inst.fields.RegB = yystack_[0].value.as < int > (); driver.append_TEXT(inst.raw); }
#line 956 "src/parser.cc"
    break;

  case 32: // instruction: "shr_inst" "general purpose register" "," "general purpose register"
#line 172 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b10101; inst.fields.RegA = yystack_[2].value.as < int > (); inst.fields.RegB = yystack_[0].value.as < int > (); driver.append_TEXT(inst.raw); }
#line 962 "src/parser.cc"
    break;

  case 33: // instruction: "ld_inst" operand "," "general purpose register"
#line 173 "src/parser.y"
                                        { yystack_[2].value.as <  Assembler::instruction  > ().fields.OC = 0b10110; yystack_[2].value.as <  Assembler::instruction  > ().fields.RegA = yystack_[0].value.as < int > (); driver.append_TEXT(yystack_[2].value.as <  Assembler::instruction  > ().raw); }
#line 968 "src/parser.cc"
    break;

  case 34: // instruction: "st_inst" "general purpose register" "," operand
#line 174 "src/parser.y"
                                        { yystack_[0].value.as <  Assembler::instruction  > ().fields.OC = 0b10111; yystack_[0].value.as <  Assembler::instruction  > ().fields.RegA = yystack_[2].value.as < int > (); driver.append_TEXT(yystack_[0].value.as <  Assembler::instruction  > ().raw); }
#line 974 "src/parser.cc"
    break;

  case 35: // instruction: "csrrd_inst" "control and status register" "," "general purpose register"
#line 175 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b11000; inst.fields.RegA = yystack_[2].value.as < Assembler::csr_type > (); driver.append_TEXT(inst.raw); }
#line 980 "src/parser.cc"
    break;

  case 36: // instruction: "csrwr_inst" "general purpose register" "," "control and status register"
#line 176 "src/parser.y"
                                        { Assembler::instruction inst; inst.fields.OC = 0b11001; inst.fields.RegA = yystack_[2].value.as < int > (); driver.append_TEXT(inst.raw); }
#line 986 "src/parser.cc"
    break;

  case 37: // operand: "$" "literal"
#line 180 "src/parser.y"
                                        { yylhs.value.as <  Assembler::instruction  > ().fields.MOD = 0b000; yylhs.value.as <  Assembler::instruction  > ().fields.Disp = yystack_[0].value.as < uint32_t > (); }
#line 992 "src/parser.cc"
    break;

  case 38: // operand: "literal"
#line 181 "src/parser.y"
                                        { yylhs.value.as <  Assembler::instruction  > ().fields.MOD = 0b001; yylhs.value.as <  Assembler::instruction  > ().fields.Disp = yystack_[0].value.as < uint32_t > (); }
#line 998 "src/parser.cc"
    break;

  case 39: // operand: "$" "symbol"
#line 182 "src/parser.y"
                                        { yylhs.value.as <  Assembler::instruction  > ().fields.MOD = 0b010; /* TODO: check if symbol is defined etc. */yylhs.value.as <  Assembler::instruction  > ().fields.Disp = driver.get_symbol(yystack_[0].value.as < std::string > (), driver.TEXT.size(), true)->offset; }
#line 1004 "src/parser.cc"
    break;

  case 40: // operand: "symbol"
#line 183 "src/parser.y"
                                        { yylhs.value.as <  Assembler::instruction  > ().fields.MOD = 0b011; /* TODO: check if symbol is defined etc. */yylhs.value.as <  Assembler::instruction  > ().fields.Disp = driver.get_symbol(yystack_[0].value.as < std::string > (), driver.TEXT.size(), true)->offset;}
#line 1010 "src/parser.cc"
    break;

  case 41: // operand: "general purpose register"
#line 184 "src/parser.y"
                                        { yylhs.value.as <  Assembler::instruction  > ().fields.MOD = 0b100; yylhs.value.as <  Assembler::instruction  > ().fields.RegC = yystack_[0].value.as < int > (); }
#line 1016 "src/parser.cc"
    break;

  case 42: // operand: "[" "general purpose register" "]"
#line 185 "src/parser.y"
                                        { yylhs.value.as <  Assembler::instruction  > ().fields.MOD = 0b101; yylhs.value.as <  Assembler::instruction  > ().fields.RegC = yystack_[1].value.as < int > (); }
#line 1022 "src/parser.cc"
    break;

  case 43: // operand: "[" "general purpose register" "+" "literal" "]"
#line 186 "src/parser.y"
                                        { yylhs.value.as <  Assembler::instruction  > ().fields.MOD = 0b110; yylhs.value.as <  Assembler::instruction  > ().fields.RegC = yystack_[3].value.as < int > (); yylhs.value.as <  Assembler::instruction  > ().fields.Disp = yystack_[1].value.as < uint32_t > (); }
#line 1028 "src/parser.cc"
    break;

  case 44: // operand: "[" "general purpose register" "+" "symbol" "]"
#line 187 "src/parser.y"
                                        { yylhs.value.as <  Assembler::instruction  > ().fields.MOD = 0b111; yylhs.value.as <  Assembler::instruction  > ().fields.RegC = yystack_[3].value.as < int > (); /* TODO: check if symbol is defined etc. */yylhs.value.as <  Assembler::instruction  > ().fields.Disp = driver.get_symbol(yystack_[1].value.as < std::string > (), driver.TEXT.size(), true)->offset;}
#line 1034 "src/parser.cc"
    break;

  case 45: // directive: "global_drct" symbol_list
#line 192 "src/parser.y"
                                        {  for(std::string s : yystack_[0].value.as <  std::vector<std::string>  > ()){
                                              Assembler::Driver::STentry *entry = driver.get_symbol(s, driver.TEXT.size(), false);
                                              if(!entry){
                                                 entry = driver.insert_symbol(s, driver.current_section, driver.TEXT.size(), false);
                                              }
                                              entry->local = false;
                                           }
                                        }
#line 1047 "src/parser.cc"
    break;

  case 46: // directive: "extern_drct" symbol_list
#line 200 "src/parser.y"
                                        { driver.add_extern(yystack_[0].value.as <  std::vector<std::string>  > ()); }
#line 1053 "src/parser.cc"
    break;

  case 47: // directive: "section_drct" "symbol"
#line 201 "src/parser.y"
                                        { driver.current_section = yystack_[0].value.as < std::string > ();
                                          driver.insert_symbol(yystack_[0].value.as < std::string > (), driver.current_section, driver.TEXT.size());
                                        }
#line 1061 "src/parser.cc"
    break;

  case 48: // directive: "skip_drct" "literal"
#line 204 "src/parser.y"
                                        { for(int i = 0; i < yystack_[0].value.as < uint32_t > (); i++) driver.TEXT.push_back(0); }
#line 1067 "src/parser.cc"
    break;

  case 49: // symbol_list: "symbol"
#line 208 "src/parser.y"
                                        { yylhs.value.as <  std::vector<std::string>  > () = std::vector<std::string>(); yylhs.value.as <  std::vector<std::string>  > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 1073 "src/parser.cc"
    break;

  case 50: // symbol_list: symbol_list "," "symbol"
#line 209 "src/parser.y"
                                        { yylhs.value.as <  std::vector<std::string>  > () = yystack_[2].value.as <  std::vector<std::string>  > (); yylhs.value.as <  std::vector<std::string>  > ().push_back(yystack_[0].value.as < std::string > ()); }
#line 1079 "src/parser.cc"
    break;

  case 51: // label: "label"
#line 213 "src/parser.y"
                                        { /* TODO: change offset to section offset */driver.insert_symbol(yystack_[0].value.as < std::string > (), driver.current_section, driver.TEXT.size()); }
#line 1085 "src/parser.cc"
    break;


#line 1089 "src/parser.cc"

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









  const signed char Parser::yypact_ninf_ = -44;

  const signed char Parser::yytable_ninf_ = -1;

  const signed char
  Parser::yypact_[] =
  {
     -44,     8,   -44,   -43,   -43,   -36,   -29,   -44,   -44,   -44,
     -44,   -37,   -44,   -37,   -28,     1,     3,     7,    42,    44,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
     -37,    56,    40,    57,   -44,   -44,   -44,    64,    65,    45,
     -44,    67,    67,   -44,   -44,    61,   -42,   -44,   -44,   -44,
     -44,   -44,    69,    70,    71,   -44,   -44,    75,    76,    77,
      78,    79,   -44,    80,    81,    82,    83,    84,    85,    86,
      87,    88,   -44,   -44,   -44,    89,    90,    43,    13,   -44,
     -44,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   -37,   105,   106,   -44,   -44,
     -44,   -44,    41,   113,   115,   116,   -44,   -44,   -44,   -44,
     -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,   -44,
     114,   117,   -37,   -37,   -37,   -44,   -44,   -44,   -44,   -44
  };

  const signed char
  Parser::yydefact_[] =
  {
       2,     0,     1,     0,     0,     0,     0,     4,    11,    12,
      13,     0,    15,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     5,    51,     3,     0,     0,     0,
      49,    45,    46,    47,    48,     0,     0,    38,    41,    40,
      14,    16,     0,     0,     0,    20,    21,     0,     0,     0,
       0,     0,    27,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     9,     7,     6,     0,     0,     0,     0,    37,
      39,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    10,     8,
      50,    42,     0,     0,     0,     0,    22,    23,    24,    25,
      26,    28,    29,    30,    31,    32,    33,    34,    35,    36,
       0,     0,     0,     0,     0,    43,    44,    17,    18,    19
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -44,   -44,   -44,   119,   -13,   120,   122,   -44
  };

  const signed char
  Parser::yydefgoto_[] =
  {
       0,     1,    36,    37,    50,    38,    41,    39
  };

  const unsigned char
  Parser::yytable_[] =
  {
      51,    79,    45,    40,    80,    46,    47,    48,     2,    49,
      43,     3,     4,     5,    44,     6,    52,    68,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    53,    34,    54,     3,     4,
       5,    55,     6,   101,   102,    35,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,   117,    74,   120,    70,    56,   121,    57,   100,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      69,    71,    72,    73,    77,    78,    81,    82,    83,   127,
     128,   129,    84,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    42,    98,    99,     0,
       0,     0,     0,     0,     0,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   118,
     122,   119,   123,   124,   125,     0,     0,   126,    75,    76
  };

  const signed char
  Parser::yycheck_[] =
  {
      13,    43,    39,    46,    46,    42,    43,    44,     0,    46,
      46,     3,     4,     5,    43,     7,    44,    30,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    44,    38,    44,     3,     4,
       5,    44,     7,    40,    41,    47,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    95,    38,    43,    45,    44,    46,    44,    46,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    38,    38,    37,    44,    37,    37,    37,   122,
     123,   124,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    37,    37,    37,    37,     4,    38,    38,    -1,
      -1,    -1,    -1,    -1,    -1,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    44,    44,
      37,    45,    37,    37,    40,    -1,    -1,    40,    39,    39
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    51,     0,     3,     4,     5,     7,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    38,    47,    52,    53,    55,    57,
      46,    56,    56,    46,    43,    39,    42,    43,    44,    46,
      54,    54,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    44,    54,    44,
      45,    44,    38,    38,    38,    53,    55,    37,    44,    43,
      46,    37,    37,    37,    37,    37,    37,    37,    37,    37,
      37,    37,    37,    37,    37,    37,    37,    37,    38,    38,
      46,    40,    41,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,    44,    44,    54,    44,    45,
      43,    46,    37,    37,    37,    40,    40,    54,    54,    54
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    50,    51,    51,    51,    52,    52,    52,    52,    52,
      52,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    54,    54,    54,
      54,    54,    54,    54,    54,    55,    55,    55,    55,    56,
      56,    57
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     0,     2,     2,     1,     2,     2,     3,     2,
       3,     1,     1,     1,     2,     1,     2,     6,     6,     6,
       2,     2,     4,     4,     4,     4,     4,     2,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     2,     1,     2,
       1,     1,     3,     5,     5,     2,     2,     2,     2,     1,
       3,     1
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
  "\"csrwr_inst\"", "\",\"", "\"eol\"", "\"[\"", "\"]\"", "\"+\"", "\"$\"",
  "\"literal\"", "\"general purpose register\"",
  "\"control and status register\"", "\"symbol\"", "\"label\"",
  "\"symbol_list\"", "\"operand\"", "$accept", "program", "line",
  "instruction", "operand", "directive", "symbol_list", "label", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  Parser::yyrline_[] =
  {
       0,   135,   135,   137,   138,   142,   143,   144,   145,   146,
     147,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   180,   181,   182,
     183,   184,   185,   186,   187,   192,   200,   201,   204,   208,
     209,   213
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
      45,    46,    47,    48,    49
    };
    // Last valid token kind.
    const int code_max = 304;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

#line 4 "src/parser.y"
} // Assembler
#line 1534 "src/parser.cc"

#line 215 "src/parser.y"


void 
Assembler::Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << std::endl;
}
