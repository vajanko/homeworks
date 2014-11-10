%{
	#include "duerr.h"
	#include "du2.h"
	#include "du2tok.h"
	
	/* portability stuff */
	#if defined _MSC_VER	    /* M$ VS2003 does not recognize long long, VS2005 has long long but no strtoull */
	    typedef unsigned __int64	UINT64;
	    #define C64(n)				n##i64
	    #define STRTOULL			_strtoui64
	#else						/* the rest of the world */
	    typedef unsigned long long	UINT64;
	    #define C64(n)				n##LL
	    #define STRTOULL			strtoull
	#endif
	
	using namespace std;
	using namespace mlc;
		
%}

%option noyywrap
%option nostdinit
%option never-interactive
%option 8bit
%option nounput
/* Allow 'Program' as well as 'program' keywords */
%option case-insensitive

/* Literals */
DIGIT				[0-9]
LETTER				[a-zA-Z]
UINT				{DIGIT}+

/* Other */
WHITESPACE		[ \r\n\t\f]

%%

{WHITESPACE}+		/* go out with whitespaces */

(<[>=]?)|(>=?)	return DUTOK_OPER_REL;			/* '<', '<>', '<=' or '>', '>=' */
(down)?to		return DUTOK_FOR_DIRECTION;		/* 'downto' or 'to' */
[+-]			return DUTOK_OPER_SIGNADD;		/* '+' or '-' */
([*/])|div|mod|and		return DUTOK_OPER_MUL;	/* '*', '/', div, mod, and */


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



.			error(DUERR_UNKCHAR, -1, *yytext, *yytext);	/* -1 should be substituted by current line */
