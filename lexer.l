/* terminates after a file reaches the end */
%option noyywrap
/* we're not using input & yyunput, don't generate code for them to avoid compiler warnings */
%option noinput
%option nounput
/* automatically track line number */
%option yylineno

%{

// Give Flex the prototype of yylex we want.
# define YY_DECL \
  yy::parser::symbol_type yylex()

#include <cstdlib>
#include <iostream>

#include "y.tab.h"

// Use LINENO for the actual line number in the source code
// yylineno has advanced to the next line when we reference it.
#define LINENO (yylineno - 1)
%}

integer [0-9]+
/* minus sign (-) must be the first or last one since it has special meaning
   when it's in the middle */
operator [-+*/]

%%

{integer} {
    return yy::parser::make_NUM(std::atoi(yytext));
  }

{operator} { return yytext[0]; }

[ \t\r]   {}

\n {}

. {
    std::cout << "Invalid input: " << yytext << std::endl;
    std::exit(-1);
  }

%%