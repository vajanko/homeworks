%{
	#include <stdlib.h>
	#include <string>
	#include "duerr.h"
	#include "du5.h"
	#include "du5lval.hpp"
	#include "du5tok.h"
	#include "du5g.hpp"
	
	/* portability stuff */
	#if defined _MSC_VER	    /* M$, VS2003 does not recognize long long */
	    typedef unsigned __int64	UINT64;
	    #define C64(n)				n##i64
	    #define STRTOULL			_strtoui64
	#else						/* the rest of the world; hmm, SunCC doesn't have strupr, hopefully no one will use it */
	    typedef unsigned long long	UINT64;
	    #define C64(n)				n##LL
	    #define STRTOULL			strtoull
	#endif
	
	using namespace std;
	using namespace mlc;
	
	// 0 - if line number needs to be initialized
	// 1 - if line number need to be incremented
	// 2 - otherwise
	unsigned int line_init = 0;
	// increment current line number
	#define INC_LINE { ++(*l); }
	// return current line number
	#define LINE_NUM (*l)
	// increment line number in the next call of yytext function
	#define INC_LINE_NEXT { line_init = 1; }

	// number of { brackets minus number of } brackets
	unsigned int bracket_count = 0;
	// increment current comment nesting
	#define INC_COMM { ++bracket_count; }
	// decrement current comment nesting
	#define DEC_COMM { --bracket_count; }
	// value indicating whether current current text position is outside comments
	#define IS_OUT_COMM (bracket_count == 0)
  #define OVERFLOW_TAG 1
  #define MALFORMED_TAG 2
  // value indicating whether result given from parse_* function means overflow
  #define IS_OVERFLOW(res) ((res & OVERFLOW_TAG) != 0)
  // value indicating whether result given from parse_* function means malformed number
  #define IS_MALFORMED(res) ((res & MALFORMED_TAG) != 0) 

void toupper(char *str)
{
	int i = 0;
	while (str[i] != 0) {
		str[i] = (char)toupper(str[i]);
		++i;
	}
}
int parse_int(const char *str, unsigned *out)
{
	int res = 0;
	UINT64 lval = 0;
	int i = 0;

	while (str[i] != '\0' && !isalpha(str[i]))
	{
		lval = lval * 10 + (str[i] - '0');
		if (lval > INT_MAX)
			res = OVERFLOW_TAG;
		lval &= 0x7fffffff;		// take only 31 bits
		++i;
	}
	if (isalpha(str[i]))
		res |= MALFORMED_TAG;

	*out = (unsigned)lval;
	return res;
}
int parse_double(char const *str, double *out) 
{
	errno = 0;
	int res = 0;
	char *endptr;
	double val = strtod(str, &endptr);

if (errno == ERANGE && val == HUGE_VAL)
	res |= OVERFLOW_TAG;
	if (isalpha(*endptr))
	res |= MALFORMED_TAG;

	*out = val;
	return res;
}
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
WHITESPACE		[ \r\t\f]

%x INSTRING
%x INCOMMENT

%%

%{
     if (line_init == 0)
	 {
		line_init = 2;
		(*l) = 1;
	 }
	 else if (line_init == 1)
	 {
		INC_LINE;
		line_init = 2;
	 }

	 std::string str_buff;
%}

{WHITESPACE}+		/* go out with whitespaces */

\<		{ lv->dtge_ = DUTOKGE_LT; return DUTOK_OPER_REL; }
\<=		{ lv->dtge_ = DUTOKGE_LE; return DUTOK_OPER_REL; }
\<\>	{ lv->dtge_ = DUTOKGE_NE; return DUTOK_OPER_REL; }
\>=		{ lv->dtge_ = DUTOKGE_GE; return DUTOK_OPER_REL; }
>		{ lv->dtge_ = DUTOKGE_GT; return DUTOK_OPER_REL; }

-	{ lv->dtge_ = DUTOKGE_MINUS; return DUTOK_OPER_SIGNADD; }
\+	{ lv->dtge_ = DUTOKGE_PLUS; return DUTOK_OPER_SIGNADD; }

\*	{ lv->dtge_ = DUTOKGE_ASTERISK; return DUTOK_OPER_MUL; }
\/	{ lv->dtge_ = DUTOKGE_SOLIDUS; return DUTOK_OPER_MUL; }
div	{ lv->dtge_ = DUTOKGE_DIV; return DUTOK_OPER_MUL; }
mod	{ lv->dtge_ = DUTOKGE_MOD; return DUTOK_OPER_MUL; }
and	{ lv->dtge_ = DUTOKGE_AND; return DUTOK_OPER_MUL; }

to		{ lv->dtge_ = DUTOKGE_TO; return DUTOK_FOR_DIRECTION; }
downto	{ lv->dtge_ = DUTOKGE_DOWNTO; return DUTOK_FOR_DIRECTION; }

program		return DUTOK_PROGRAM;	/* Keywords */
label			return DUTOK_LABEL;
const			return DUTOK_CONST;
type			return DUTOK_TYPE;
var				return DUTOK_VAR;
begin			return DUTOK_BEGIN;
end				return DUTOK_END;
procedure	return DUTOK_PROCEDURE;
function	return DUTOK_FUNCTION;
array			return DUTOK_ARRAY;
of				return DUTOK_OF;
goto			return DUTOK_GOTO;
if				return DUTOK_IF;
then			return DUTOK_THEN;
else			return DUTOK_ELSE;
while			return DUTOK_WHILE;
do				return DUTOK_DO;
repeat		return DUTOK_REPEAT;
until			return DUTOK_UNTIL;
for				return DUTOK_FOR;
or				return DUTOK_OR;
not				return DUTOK_NOT;

{LETTER}({DIGIT}|{LETTER})*			{ 
	toupper(yytext);
	lv->id_ci_ = ctx->tab->ls_id().add(yytext); 
	return DUTOK_IDENTIFIER; 
}
{UINT}{LETTER}*	{		/* Take also all imediatelly following letters (this will be malformed number) */
	unsigned val;
	int res = parse_int(yytext, &val);
  if (IS_MALFORMED(res))
    error( DUERR_BADINT, LINE_NUM, yytext );
  if (IS_OVERFLOW(res))
    error( DUERR_INTOUTRANGE, LINE_NUM, yytext );

	lv->int_ci_ = ctx->tab->ls_int().add(val); 
	return DUTOK_UINT; 
}
{UINT}(\.{DIGIT}+)?(e[+-]?{UINT})?{LETTER}*	{	/* Take also all imediatelly following letters (this will be malformed number) */
	double val;
	int res = parse_double(yytext, &val);	
  if (IS_MALFORMED(res))
    error( DUERR_BADREAL, LINE_NUM, yytext );
	if (IS_OVERFLOW(res))
		error(DUERR_REALOUTRANGE, LINE_NUM, yytext);

	lv->real_ci_ = ctx->tab->ls_real().add(val); 
	return DUTOK_REAL; 
}

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

\n		{ INC_LINE; }


\{					{ INC_COMM; BEGIN(INCOMMENT); }
<INCOMMENT>[^{}\n]+
<INCOMMENT>\n		{ INC_LINE; }
<INCOMMENT><<EOF>>	{ error(DUERR_EOFINCMT, LINE_NUM); BEGIN(INITIAL); }
<INCOMMENT>\{		{ INC_COMM; }
<INCOMMENT>\}		{ DEC_COMM; if (IS_OUT_COMM) { BEGIN(INITIAL); } }
\}					{ error(DUERR_UNEXPENDCMT, LINE_NUM); }

\'					{ BEGIN(INSTRING); }
<INSTRING><<EOF>>	{ error(DUERR_EOFINSTRCHR, LINE_NUM); BEGIN(INITIAL); return DUTOK_STRING; }
<INSTRING>\n		{ 
	error(DUERR_EOLINSTRCHR, LINE_NUM); 
	INC_LINE_NEXT;
	BEGIN(INITIAL);
	lv->str_ci_ = ctx->tab->ls_str().add(str_buff);
	return DUTOK_STRING; 
}
<INSTRING>[^']		{ str_buff.append(yytext); }
<INSTRING>\'\'		{ str_buff.append("'"); }
<INSTRING>\'		  { BEGIN(INITIAL); lv->str_ci_ = ctx->tab->ls_str().add(str_buff); return DUTOK_STRING; }

.			error(DUERR_UNKCHAR, LINE_NUM, *yytext, *yytext);

%%
