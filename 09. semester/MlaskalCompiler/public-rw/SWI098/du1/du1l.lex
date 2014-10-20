%{
	#include "du1.h"
	#include "du1tok.h"
	#include "duerr.h"
%}

%option noyywrap
%option nostdinit
%option never-interactive
%option 8bit
%option nounput

DIGIT			[0-9]
WHITESPACE		[ \r\n\t\f]

%%

{DIGIT}*			return DUTOK_UINT;

{WHITESPACE}+		/* go out with whitespaces */

.			mlc::error(mlc::DUERR_UNKCHAR, 0, *yytext, *yytext);
