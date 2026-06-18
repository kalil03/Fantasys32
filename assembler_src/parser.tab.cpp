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



// First part of user prologue.
#line 3 "parser.yy"

#include <iostream>
#include <string>
#include <vector>
#include "ast.h"

extern int current_stmt_line;
std::vector<AsmLine> programLines;

#line 51 "parser.tab.cpp"


#include "parser.tab.hpp"


// Unqualified %code blocks.
#line 13 "parser.yy"

    extern int yylex(yy::parser::semantic_type* yylval);

#line 62 "parser.tab.cpp"


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

namespace yy {
#line 135 "parser.tab.cpp"

  /// Build a parser object.
  parser::parser ()
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr)
#else

#endif
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/

  // basic_symbol.
  template <typename Base>
  parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
  {
    switch (this->kind ())
    {
      case symbol_kind::S_argument: // argument
        value.copy< Argument > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_instruction: // instruction
      case symbol_kind::S_directive: // directive
        value.copy< AsmLine > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TOK_LABEL_DEF: // TOK_LABEL_DEF
      case symbol_kind::S_TOK_IDENTIFIER: // TOK_IDENTIFIER
      case symbol_kind::S_TOK_REGISTER: // TOK_REGISTER
      case symbol_kind::S_TOK_HEX_NUMBER: // TOK_HEX_NUMBER
      case symbol_kind::S_TOK_DEC_NUMBER: // TOK_DEC_NUMBER
      case symbol_kind::S_TOK_STRING_LITERAL: // TOK_STRING_LITERAL
      case symbol_kind::S_TOK_LABEL_REF_H: // TOK_LABEL_REF_H
      case symbol_kind::S_TOK_LABEL_REF_L: // TOK_LABEL_REF_L
      case symbol_kind::S_label_decl: // label_decl
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_arg_list: // arg_list
        value.copy< std::vector<Argument> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }




  template <typename Base>
  parser::symbol_kind_type
  parser::basic_symbol<Base>::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }


  template <typename Base>
  bool
  parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return this->kind () == symbol_kind::S_YYEMPTY;
  }

  template <typename Base>
  void
  parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->kind ())
    {
      case symbol_kind::S_argument: // argument
        value.move< Argument > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_instruction: // instruction
      case symbol_kind::S_directive: // directive
        value.move< AsmLine > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_TOK_LABEL_DEF: // TOK_LABEL_DEF
      case symbol_kind::S_TOK_IDENTIFIER: // TOK_IDENTIFIER
      case symbol_kind::S_TOK_REGISTER: // TOK_REGISTER
      case symbol_kind::S_TOK_HEX_NUMBER: // TOK_HEX_NUMBER
      case symbol_kind::S_TOK_DEC_NUMBER: // TOK_DEC_NUMBER
      case symbol_kind::S_TOK_STRING_LITERAL: // TOK_STRING_LITERAL
      case symbol_kind::S_TOK_LABEL_REF_H: // TOK_LABEL_REF_H
      case symbol_kind::S_TOK_LABEL_REF_L: // TOK_LABEL_REF_L
      case symbol_kind::S_label_decl: // label_decl
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case symbol_kind::S_arg_list: // arg_list
        value.move< std::vector<Argument> > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

  }

  // by_kind.
  parser::by_kind::by_kind () YY_NOEXCEPT
    : kind_ (symbol_kind::S_YYEMPTY)
  {}

#if 201103L <= YY_CPLUSPLUS
  parser::by_kind::by_kind (by_kind&& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {
    that.clear ();
  }
#endif

  parser::by_kind::by_kind (const by_kind& that) YY_NOEXCEPT
    : kind_ (that.kind_)
  {}

  parser::by_kind::by_kind (token_kind_type t) YY_NOEXCEPT
    : kind_ (yytranslate_ (t))
  {}



  void
  parser::by_kind::clear () YY_NOEXCEPT
  {
    kind_ = symbol_kind::S_YYEMPTY;
  }

  void
  parser::by_kind::move (by_kind& that)
  {
    kind_ = that.kind_;
    that.clear ();
  }

  parser::symbol_kind_type
  parser::by_kind::kind () const YY_NOEXCEPT
  {
    return kind_;
  }


  parser::symbol_kind_type
  parser::by_kind::type_get () const YY_NOEXCEPT
  {
    return this->kind ();
  }



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_argument: // argument
        value.YY_MOVE_OR_COPY< Argument > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_instruction: // instruction
      case symbol_kind::S_directive: // directive
        value.YY_MOVE_OR_COPY< AsmLine > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TOK_LABEL_DEF: // TOK_LABEL_DEF
      case symbol_kind::S_TOK_IDENTIFIER: // TOK_IDENTIFIER
      case symbol_kind::S_TOK_REGISTER: // TOK_REGISTER
      case symbol_kind::S_TOK_HEX_NUMBER: // TOK_HEX_NUMBER
      case symbol_kind::S_TOK_DEC_NUMBER: // TOK_DEC_NUMBER
      case symbol_kind::S_TOK_STRING_LITERAL: // TOK_STRING_LITERAL
      case symbol_kind::S_TOK_LABEL_REF_H: // TOK_LABEL_REF_H
      case symbol_kind::S_TOK_LABEL_REF_L: // TOK_LABEL_REF_L
      case symbol_kind::S_label_decl: // label_decl
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_arg_list: // arg_list
        value.YY_MOVE_OR_COPY< std::vector<Argument> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case symbol_kind::S_argument: // argument
        value.move< Argument > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_instruction: // instruction
      case symbol_kind::S_directive: // directive
        value.move< AsmLine > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_TOK_LABEL_DEF: // TOK_LABEL_DEF
      case symbol_kind::S_TOK_IDENTIFIER: // TOK_IDENTIFIER
      case symbol_kind::S_TOK_REGISTER: // TOK_REGISTER
      case symbol_kind::S_TOK_HEX_NUMBER: // TOK_HEX_NUMBER
      case symbol_kind::S_TOK_DEC_NUMBER: // TOK_DEC_NUMBER
      case symbol_kind::S_TOK_STRING_LITERAL: // TOK_STRING_LITERAL
      case symbol_kind::S_TOK_LABEL_REF_H: // TOK_LABEL_REF_H
      case symbol_kind::S_TOK_LABEL_REF_L: // TOK_LABEL_REF_L
      case symbol_kind::S_label_decl: // label_decl
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_arg_list: // arg_list
        value.move< std::vector<Argument> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_argument: // argument
        value.copy< Argument > (that.value);
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_instruction: // instruction
      case symbol_kind::S_directive: // directive
        value.copy< AsmLine > (that.value);
        break;

      case symbol_kind::S_TOK_LABEL_DEF: // TOK_LABEL_DEF
      case symbol_kind::S_TOK_IDENTIFIER: // TOK_IDENTIFIER
      case symbol_kind::S_TOK_REGISTER: // TOK_REGISTER
      case symbol_kind::S_TOK_HEX_NUMBER: // TOK_HEX_NUMBER
      case symbol_kind::S_TOK_DEC_NUMBER: // TOK_DEC_NUMBER
      case symbol_kind::S_TOK_STRING_LITERAL: // TOK_STRING_LITERAL
      case symbol_kind::S_TOK_LABEL_REF_H: // TOK_LABEL_REF_H
      case symbol_kind::S_TOK_LABEL_REF_L: // TOK_LABEL_REF_L
      case symbol_kind::S_label_decl: // label_decl
        value.copy< std::string > (that.value);
        break;

      case symbol_kind::S_arg_list: // arg_list
        value.copy< std::vector<Argument> > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_argument: // argument
        value.move< Argument > (that.value);
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_instruction: // instruction
      case symbol_kind::S_directive: // directive
        value.move< AsmLine > (that.value);
        break;

      case symbol_kind::S_TOK_LABEL_DEF: // TOK_LABEL_DEF
      case symbol_kind::S_TOK_IDENTIFIER: // TOK_IDENTIFIER
      case symbol_kind::S_TOK_REGISTER: // TOK_REGISTER
      case symbol_kind::S_TOK_HEX_NUMBER: // TOK_HEX_NUMBER
      case symbol_kind::S_TOK_DEC_NUMBER: // TOK_DEC_NUMBER
      case symbol_kind::S_TOK_STRING_LITERAL: // TOK_STRING_LITERAL
      case symbol_kind::S_TOK_LABEL_REF_H: // TOK_LABEL_REF_H
      case symbol_kind::S_TOK_LABEL_REF_L: // TOK_LABEL_REF_L
      case symbol_kind::S_label_decl: // label_decl
        value.move< std::string > (that.value);
        break;

      case symbol_kind::S_arg_list: // arg_list
        value.move< std::vector<Argument> > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

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
            yyla.kind_ = yytranslate_ (yylex (&yyla.value));
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
      case symbol_kind::S_argument: // argument
        yylhs.value.emplace< Argument > ();
        break;

      case symbol_kind::S_line: // line
      case symbol_kind::S_instruction: // instruction
      case symbol_kind::S_directive: // directive
        yylhs.value.emplace< AsmLine > ();
        break;

      case symbol_kind::S_TOK_LABEL_DEF: // TOK_LABEL_DEF
      case symbol_kind::S_TOK_IDENTIFIER: // TOK_IDENTIFIER
      case symbol_kind::S_TOK_REGISTER: // TOK_REGISTER
      case symbol_kind::S_TOK_HEX_NUMBER: // TOK_HEX_NUMBER
      case symbol_kind::S_TOK_DEC_NUMBER: // TOK_DEC_NUMBER
      case symbol_kind::S_TOK_STRING_LITERAL: // TOK_STRING_LITERAL
      case symbol_kind::S_TOK_LABEL_REF_H: // TOK_LABEL_REF_H
      case symbol_kind::S_TOK_LABEL_REF_L: // TOK_LABEL_REF_L
      case symbol_kind::S_label_decl: // label_decl
        yylhs.value.emplace< std::string > ();
        break;

      case symbol_kind::S_arg_list: // arg_list
        yylhs.value.emplace< std::vector<Argument> > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // program: lines TOK_EOF
#line 39 "parser.yy"
                  { return 0; }
#line 791 "parser.tab.cpp"
    break;

  case 5: // line_entry: line TOK_NEWLINE
#line 48 "parser.yy"
                     { programLines.push_back(yystack_[1].value.as < AsmLine > ()); }
#line 797 "parser.tab.cpp"
    break;

  case 6: // line_entry: TOK_NEWLINE
#line 49 "parser.yy"
                     { /* Ignora linhas em branco */ }
#line 803 "parser.tab.cpp"
    break;

  case 7: // label_decl: TOK_LABEL_DEF
#line 53 "parser.yy"
                  {
        yylhs.value.as < std::string > () = yystack_[0].value.as < std::string > ();
        yylhs.value.as < std::string > ().pop_back(); // Remove o ':'
    }
#line 812 "parser.tab.cpp"
    break;

  case 8: // label_decl: TOK_IDENTIFIER TOK_COLON
#line 57 "parser.yy"
                               {
        yylhs.value.as < std::string > () = yystack_[1].value.as < std::string > ();
    }
#line 820 "parser.tab.cpp"
    break;

  case 9: // line: label_decl instruction
#line 63 "parser.yy"
                           {
        yylhs.value.as < AsmLine > () = yystack_[0].value.as < AsmLine > ();
        yylhs.value.as < AsmLine > ().label = yystack_[1].value.as < std::string > ();
    }
#line 829 "parser.tab.cpp"
    break;

  case 10: // line: label_decl directive
#line 67 "parser.yy"
                           {
        yylhs.value.as < AsmLine > () = yystack_[0].value.as < AsmLine > ();
        yylhs.value.as < AsmLine > ().label = yystack_[1].value.as < std::string > ();
    }
#line 838 "parser.tab.cpp"
    break;

  case 11: // line: label_decl
#line 71 "parser.yy"
                 {
        yylhs.value.as < AsmLine > ().type = LINE_LABEL_ONLY;
        yylhs.value.as < AsmLine > ().label = yystack_[0].value.as < std::string > ();
        yylhs.value.as < AsmLine > ().lineNumber = current_stmt_line;
    }
#line 848 "parser.tab.cpp"
    break;

  case 12: // line: instruction
#line 76 "parser.yy"
                  { yylhs.value.as < AsmLine > () = yystack_[0].value.as < AsmLine > (); }
#line 854 "parser.tab.cpp"
    break;

  case 13: // line: directive
#line 77 "parser.yy"
                  { yylhs.value.as < AsmLine > () = yystack_[0].value.as < AsmLine > (); }
#line 860 "parser.tab.cpp"
    break;

  case 14: // instruction: TOK_IDENTIFIER arg_list
#line 81 "parser.yy"
                            {
        yylhs.value.as < AsmLine > ().type = LINE_INSTRUCTION;
        yylhs.value.as < AsmLine > ().mnemonic = yystack_[1].value.as < std::string > ();
        yylhs.value.as < AsmLine > ().args = yystack_[0].value.as < std::vector<Argument> > ();
        yylhs.value.as < AsmLine > ().lineNumber = current_stmt_line;
    }
#line 871 "parser.tab.cpp"
    break;

  case 15: // instruction: TOK_IDENTIFIER
#line 87 "parser.yy"
                     {
        yylhs.value.as < AsmLine > ().type = LINE_INSTRUCTION;
        yylhs.value.as < AsmLine > ().mnemonic = yystack_[0].value.as < std::string > ();
        yylhs.value.as < AsmLine > ().lineNumber = current_stmt_line;
    }
#line 881 "parser.tab.cpp"
    break;

  case 16: // directive: TOK_DIR_DATA
#line 95 "parser.yy"
                 {
        yylhs.value.as < AsmLine > ().type = LINE_DIRECTIVE;
        yylhs.value.as < AsmLine > ().mnemonic = ".data";
        yylhs.value.as < AsmLine > ().lineNumber = current_stmt_line;
    }
#line 891 "parser.tab.cpp"
    break;

  case 17: // directive: TOK_DIR_TEXT
#line 100 "parser.yy"
                   {
        yylhs.value.as < AsmLine > ().type = LINE_DIRECTIVE;
        yylhs.value.as < AsmLine > ().mnemonic = ".text";
        yylhs.value.as < AsmLine > ().lineNumber = current_stmt_line;
    }
#line 901 "parser.tab.cpp"
    break;

  case 18: // directive: TOK_DIR_EQU TOK_IDENTIFIER TOK_COMMA argument
#line 105 "parser.yy"
                                                    {
        yylhs.value.as < AsmLine > ().type = LINE_DIRECTIVE;
        yylhs.value.as < AsmLine > ().mnemonic = ".equ";
        yylhs.value.as < AsmLine > ().args.push_back(Argument{ARG_IDENTIFIER, 0, yystack_[2].value.as < std::string > ()}); 
        yylhs.value.as < AsmLine > ().args.push_back(yystack_[0].value.as < Argument > ());
        yylhs.value.as < AsmLine > ().lineNumber = current_stmt_line;
    }
#line 913 "parser.tab.cpp"
    break;

  case 19: // directive: TOK_DIR_VAR argument
#line 112 "parser.yy"
                           {
        yylhs.value.as < AsmLine > ().type = LINE_DIRECTIVE;
        yylhs.value.as < AsmLine > ().mnemonic = ".var";
        yylhs.value.as < AsmLine > ().args.push_back(yystack_[0].value.as < Argument > ()); 
        yylhs.value.as < AsmLine > ().lineNumber = current_stmt_line;
    }
#line 924 "parser.tab.cpp"
    break;

  case 20: // directive: TOK_DIR_ARRAY arg_list
#line 118 "parser.yy"
                             {
        yylhs.value.as < AsmLine > ().type = LINE_DIRECTIVE;
        yylhs.value.as < AsmLine > ().mnemonic = ".array";
        yylhs.value.as < AsmLine > ().args = yystack_[0].value.as < std::vector<Argument> > ();          
        yylhs.value.as < AsmLine > ().lineNumber = current_stmt_line;
    }
#line 935 "parser.tab.cpp"
    break;

  case 21: // directive: TOK_DIR_SPACE argument
#line 124 "parser.yy"
                             {
        yylhs.value.as < AsmLine > ().type = LINE_DIRECTIVE;
        yylhs.value.as < AsmLine > ().mnemonic = ".space";
        yylhs.value.as < AsmLine > ().args.push_back(yystack_[0].value.as < Argument > ()); 
        yylhs.value.as < AsmLine > ().lineNumber = current_stmt_line;
    }
#line 946 "parser.tab.cpp"
    break;

  case 22: // directive: TOK_DIR_STRING TOK_STRING_LITERAL
#line 130 "parser.yy"
                                        {
        yylhs.value.as < AsmLine > ().type = LINE_DIRECTIVE;
        yylhs.value.as < AsmLine > ().mnemonic = ".string";
        yylhs.value.as < AsmLine > ().args.push_back(Argument{ARG_STRING_LITERAL, 0, yystack_[0].value.as < std::string > ()});
        yylhs.value.as < AsmLine > ().lineNumber = current_stmt_line;
    }
#line 957 "parser.tab.cpp"
    break;

  case 23: // arg_list: argument
#line 139 "parser.yy"
             { yylhs.value.as < std::vector<Argument> > ().push_back(yystack_[0].value.as < Argument > ()); }
#line 963 "parser.tab.cpp"
    break;

  case 24: // arg_list: arg_list TOK_COMMA argument
#line 140 "parser.yy"
                                  {
        yylhs.value.as < std::vector<Argument> > () = yystack_[2].value.as < std::vector<Argument> > ();
        yylhs.value.as < std::vector<Argument> > ().push_back(yystack_[0].value.as < Argument > ());
    }
#line 972 "parser.tab.cpp"
    break;

  case 25: // argument: TOK_REGISTER
#line 147 "parser.yy"
                 { 
        uint32_t val = std::stoul(yystack_[0].value.as < std::string > ().substr(1));
        yylhs.value.as < Argument > () = Argument{ARG_REGISTER, val, ""}; 
    }
#line 981 "parser.tab.cpp"
    break;

  case 26: // argument: TOK_DEC_NUMBER
#line 151 "parser.yy"
                     { 
        uint32_t val = static_cast<uint32_t>(std::stoll(yystack_[0].value.as < std::string > ()));
        yylhs.value.as < Argument > () = Argument{ARG_DEC_NUMBER, val, ""}; 
    }
#line 990 "parser.tab.cpp"
    break;

  case 27: // argument: TOK_HEX_NUMBER
#line 155 "parser.yy"
                     { 
        uint32_t val = std::stoul(yystack_[0].value.as < std::string > (), nullptr, 16);
        yylhs.value.as < Argument > () = Argument{ARG_HEX_NUMBER, val, ""}; 
    }
#line 999 "parser.tab.cpp"
    break;

  case 28: // argument: TOK_IDENTIFIER
#line 159 "parser.yy"
                     { 
        yylhs.value.as < Argument > () = Argument{ARG_IDENTIFIER, 0, yystack_[0].value.as < std::string > ()}; 
    }
#line 1007 "parser.tab.cpp"
    break;

  case 29: // argument: TOK_LABEL_REF_H
#line 162 "parser.yy"
                      { 
        std::string x = yystack_[0].value.as < std::string > ().substr(0, yystack_[0].value.as < std::string > ().size() - 2); // Remove o ".h"
        yylhs.value.as < Argument > () = Argument{ARG_LABEL_REF_H, 0, x}; 
    }
#line 1016 "parser.tab.cpp"
    break;

  case 30: // argument: TOK_LABEL_REF_L
#line 166 "parser.yy"
                      { 
        std::string x = yystack_[0].value.as < std::string > ().substr(0, yystack_[0].value.as < std::string > ().size() - 2); // Remove o ".l"
        yylhs.value.as < Argument > () = Argument{ARG_LABEL_REF_L, 0, x}; 
    }
#line 1025 "parser.tab.cpp"
    break;


#line 1029 "parser.tab.cpp"

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
        error (YY_MOVE (msg));
      }


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

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


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
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

#if YYDEBUG || 0
  const char *
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytname_[yysymbol];
  }
#endif // #if YYDEBUG || 0









  const signed char parser::yypact_ninf_ = -14;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
      15,   -14,    32,   -14,   -14,    -3,    55,    55,    55,    -2,
     -14,     5,     0,   -14,    41,   -13,   -14,   -14,   -14,   -14,
     -14,   -14,   -14,   -14,   -14,   -10,   -14,    -9,   -14,   -10,
     -14,   -14,   -14,   -14,   -14,    55,   -14,   -14,   -14,    55,
      55,   -14,   -14
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     7,    15,    16,    17,     0,     0,     0,     0,     0,
       6,     0,     0,     4,    11,     0,    12,    13,    28,    25,
      27,    26,    29,    30,     8,    14,    23,     0,    19,    20,
      21,    22,     1,     2,     3,    15,     9,    10,     5,     0,
       0,    24,    18
  };

  const signed char
  parser::yypgoto_[] =
  {
     -14,   -14,   -14,    10,   -14,   -14,    -4,     9,    17,    -6
  };

  const signed char
  parser::yydefgoto_[] =
  {
       0,    11,    12,    13,    14,    15,    16,    17,    25,    26
  };

  const signed char
  parser::yytable_[] =
  {
      28,    27,    30,     1,     2,    32,    31,    38,    39,    40,
      36,     3,     4,     5,     6,     7,     8,     9,     1,     2,
      10,    33,    34,    37,    29,     0,     3,     4,     5,     6,
       7,     8,     9,    41,    42,    10,    18,    19,    20,    21,
       0,    22,    23,     0,     0,    35,     0,     0,     0,     0,
       0,    24,     3,     4,     5,     6,     7,     8,     9,    18,
      19,    20,    21,     0,    22,    23
  };

  const signed char
  parser::yycheck_[] =
  {
       6,     4,     8,     3,     4,     0,     8,    20,    18,    18,
      14,    11,    12,    13,    14,    15,    16,    17,     3,     4,
      20,    21,    12,    14,     7,    -1,    11,    12,    13,    14,
      15,    16,    17,    39,    40,    20,     4,     5,     6,     7,
      -1,     9,    10,    -1,    -1,     4,    -1,    -1,    -1,    -1,
      -1,    19,    11,    12,    13,    14,    15,    16,    17,     4,
       5,     6,     7,    -1,     9,    10
  };

  const signed char
  parser::yystos_[] =
  {
       0,     3,     4,    11,    12,    13,    14,    15,    16,    17,
      20,    23,    24,    25,    26,    27,    28,    29,     4,     5,
       6,     7,     9,    10,    19,    30,    31,     4,    31,    30,
      31,     8,     0,    21,    25,     4,    28,    29,    20,    18,
      18,    31,    31
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    22,    23,    24,    24,    25,    25,    26,    26,    27,
      27,    27,    27,    27,    28,    28,    29,    29,    29,    29,
      29,    29,    29,    30,    30,    31,    31,    31,    31,    31,
      31
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     2,     1,     2,     1,     1,     2,     2,
       2,     1,     1,     1,     2,     1,     1,     1,     4,     2,
       2,     2,     2,     1,     3,     1,     1,     1,     1,     1,
       1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "TOK_LABEL_DEF",
  "TOK_IDENTIFIER", "TOK_REGISTER", "TOK_HEX_NUMBER", "TOK_DEC_NUMBER",
  "TOK_STRING_LITERAL", "TOK_LABEL_REF_H", "TOK_LABEL_REF_L",
  "TOK_DIR_DATA", "TOK_DIR_TEXT", "TOK_DIR_EQU", "TOK_DIR_VAR",
  "TOK_DIR_ARRAY", "TOK_DIR_SPACE", "TOK_DIR_STRING", "TOK_COMMA",
  "TOK_COLON", "TOK_NEWLINE", "TOK_EOF", "$accept", "program", "lines",
  "line_entry", "label_decl", "line", "instruction", "directive",
  "arg_list", "argument", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const unsigned char
  parser::yyrline_[] =
  {
       0,    39,    39,    43,    44,    48,    49,    53,    57,    63,
      67,    71,    76,    77,    81,    87,    95,   100,   105,   112,
     118,   124,   130,   139,   140,   147,   151,   155,   159,   162,
     166
  };

  void
  parser::yy_stack_print_ () const
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
  parser::yy_reduce_print_ (int yyrule) const
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

  parser::symbol_kind_type
  parser::yytranslate_ (int t) YY_NOEXCEPT
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
      15,    16,    17,    18,    19,    20,    21
    };
    // Last valid token kind.
    const int code_max = 276;

    if (t <= 0)
      return symbol_kind::S_YYEOF;
    else if (t <= code_max)
      return static_cast <symbol_kind_type> (translate_table[t]);
    else
      return symbol_kind::S_YYUNDEF;
  }

} // yy
#line 1412 "parser.tab.cpp"

#line 172 "parser.yy"


void yy::parser::error(const std::string& msg) {
    std::cerr << "Erro sintatico na linha " << current_stmt_line << ": " << msg << std::endl;
}
