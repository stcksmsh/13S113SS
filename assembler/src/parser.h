/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
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
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_SRC_PARSER_H_INCLUDED
# define YY_YY_SRC_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    GLOBAL = 258,                  /* GLOBAL  */
    EXTERN = 259,                  /* EXTERN  */
    SECTION = 260,                 /* SECTION  */
    WORD = 261,                    /* WORD  */
    SKIP = 262,                    /* SKIP  */
    ASCII = 263,                   /* ASCII  */
    EQU = 264,                     /* EQU  */
    END = 265,                     /* END  */
    HALT = 266,                    /* HALT  */
    INT = 267,                     /* INT  */
    IRET = 268,                    /* IRET  */
    CALL = 269,                    /* CALL  */
    RET = 270,                     /* RET  */
    JMP = 271,                     /* JMP  */
    BEQ = 272,                     /* BEQ  */
    BNE = 273,                     /* BNE  */
    BGE = 274,                     /* BGE  */
    BLE = 275,                     /* BLE  */
    BGT = 276,                     /* BGT  */
    PUSH = 277,                    /* PUSH  */
    POP = 278,                     /* POP  */
    XCHG = 279,                    /* XCHG  */
    ADD = 280,                     /* ADD  */
    SUB = 281,                     /* SUB  */
    MUL = 282,                     /* MUL  */
    DIV = 283,                     /* DIV  */
    NOT = 284,                     /* NOT  */
    AND = 285,                     /* AND  */
    OR = 286,                      /* OR  */
    XOR = 287,                     /* XOR  */
    SHL = 288,                     /* SHL  */
    SHR = 289,                     /* SHR  */
    LD = 290,                      /* LD  */
    ST = 291,                      /* ST  */
    CSRRD = 292,                   /* CSRRD  */
    LABEL = 293,                   /* LABEL  */
    REGISTER = 294,                /* REGISTER  */
    BIN_LITERAL = 295,             /* BIN_LITERAL  */
    OCT_LITERAL = 296,             /* OCT_LITERAL  */
    HEX_LITERAL = 297,             /* HEX_LITERAL  */
    DEC_LITERAL = 298,             /* DEC_LITERAL  */
    SYMBOL = 299,                  /* SYMBOL  */
    COMMA = 300                    /* COMMA  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_SRC_PARSER_H_INCLUDED  */
