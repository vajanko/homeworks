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
UINT				{DIGIT}+

/* Other */
WHITESPACE		[ \r\n\t\f]

%%

{WHITESPACE}+		/* go out with whitespaces */

([\<>][>=]?)	return DUTOK_OPER_REL;
(down)?to		return DUTOK_FOR_DIRECTION;
[+-]			return DUTOK_OPER_SIGNADD;
([*/])|div|mod|and		return DUTOK_OPER_MUL;


program			return DUTOK_PROGRAM;	/* Keywords */
label			return DUTOK_LABEL;
const			return DUTOK_CONST;
type			return DUTOK_TYPE;
var				return DUTOK_VAR;
begin			return DUTOK_BEGIN;
end				return DUTOK_END;
procedure		return DUTOK_PROCEDURE;
function		return DUTOK_FUNCTION;
array			return DUTOK_ARRAY;
of				return DUTOK_OF;
goto			return DUTOK_GOTO;
if				return DUTOK_IF;
then			return DUTOK_THEN;
else			return DUTOK_ELSE;
while			return DUTOK_WHILE;
do				return DUTOK_DO;
repeat			return DUTOK_REPEAT;
until			return DUTOK_UNTIL;
for				return DUTOK_FOR;
or				return DUTOK_OR;
not				return DUTOK_NOT;
record			return DUTOK_RECORD;

{LETTER}({DIGIT}|{LETTER})*			return DUTOK_IDENTIFIER;
{UINT}								return DUTOK_UINT;
{UINT}(\.{DIGIT}*)?(e[+-]?{UINT})?	return DUTOK_REAL;

;	return DUTOK_SEMICOLON;		/* Delimiters */
\.	return DUTOK_DOT;
,	return DUTOK_COMMA;
=	return DUTOK_EQ;
:	return DUTOK_COLON;
\(	return DUTOK_LPAR;
\)	return DUTOK_RPAR;
\.\.	return DUTOK_DOTDOT;
\[	return DUTOK_LSBRA;
\]	return DUTOK_RSBRA;
:=	return DUTOK_ASSIGN;

.			mlc::error(mlc::DUERR_UNKCHAR, 0, *yytext, *yytext);

%%