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

/* Allow 'Program' as well as 'program' keywords */
%option case-insensitive

/* Keywords */
DUTOK_PROGRAM		program	
DUTOK_LABEL			label		
DUTOK_CONST			const		
DUTOK_TYPE			type		
DUTOK_VAR			var		
DUTOK_BEGIN			begin		
DUTOK_END			end		
DUTOK_PROCEDURE		procedure	
DUTOK_FUNCTION		function	
DUTOK_ARRAY			array		
DUTOK_OF			of		
DUTOK_GOTO			goto		
DUTOK_IF			if		
DUTOK_THEN			then		
DUTOK_ELSE			else		
DUTOK_WHILE			while		
DUTOK_DO			do		
DUTOK_REPEAT		repeat	
DUTOK_UNTIL			until		
DUTOK_FOR			for		
DUTOK_OR			or		
DUTOK_NOT			not		
DUTOK_RECORD		record	

/* Literals */
DIGIT				[0-9]
LETTER				[a-zA-Z]
DUTOK_IDENTIFIER	{LETTER}({DIGIT}|{LETTER})*
UINT				{DIGIT}*
DUTOK_REAL			({DUTOK_UINT}\.{DIGIT}*)?(e[+-]?{DUTOK_UINT})?

/* Other */
WHITESPACE		[ \r\n\t\f]

%%

{WHITESPACE}+		/* go out with whitespaces */

program				return DUTOK_PROGRAM;

{LETTER}({DIGIT}|{LETTER})*			return DUTOK_IDENTIFIER;
{UINT}								return DUTOK_UINT;
({UINT}\.{DIGIT}*)?(e[+-]?{UINT})?	return DUTOK_REAL;

;					return DUTOK_SEMICOLON;
\.					return DUTOK_DOT;

.			mlc::error(mlc::DUERR_UNKCHAR, 0, *yytext, *yytext);

%%

/* {DUTOK_IDENTIFIER}	return DUTOK_IDENTIFIER; */