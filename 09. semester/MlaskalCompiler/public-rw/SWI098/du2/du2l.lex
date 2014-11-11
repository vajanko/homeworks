%{
	#include <stdlib.h>
	#include <string>
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

	// true if line number is initialized already
	unsigned int line_init = 0;
	// increment current line number
	#define INC_LINE { ++(*l); }
	// return current line number
	#define LINE_NUM (*l)

	unsigned int bracket_count = 0 ;	
	#define INC_COMM { ++bracket_count; }
	#define DEC_COMM { --bracket_count; }
	#define ZERO_COMM (bracket_count == 0)

	void toupper(char *str)
	{
		int i = 0;
		while (str[i] != 0) {
			str[i] = (char)toupper(str[i]);
			++i;
		}
	}
	bool parse_int(const char *str, int &out)
	{
		int len = strlen(str);
		if (len > 10)
		{
			return false;
		}
		else if (len == 10)
		{
			const char *max_str = "2147483647";
			for (int i = 0; i < len; ++i)
			{
				if (str[i] > max_str[i])
					return false;
				else if (str[i] < max_str[i])
					break;
			}
		}

		out = atoi(str);
		return true;
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
     if (!line_init) 
	 {
		line_init = 1;
		(*l) = 1;
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

to		{ lv->dtge_ = DUTOKGE_TO; return DUTOKGE_FOR_DIRECTION; }
downto	{ lv->dtge_ = DUTOKGE_DOWNTO; return DUTOKGE_FOR_DIRECTION; }

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

{LETTER}({DIGIT}|{LETTER})*			{ 
	toupper(yytext);
	lv->id_ci_ = ctx->tab->ls_id().add(yytext); 
	return DUTOK_IDENTIFIER; 
}
{UINT}	{
	int val;
	if (!parse_int(yytext, val))
		error(DUERR_INTOUTRANGE, LINE_NUM, yytext);

	lv->int_ci_ = ctx->tab->ls_int().add(val); 
	return DUTOK_UINT; 
}
{UINT}(\.{DIGIT}+)?(e[+-]?{UINT})?	{
	double val = atof(yytext);
	if (val == HUGE_VAL)
		error(DUERR_REALOUTRANGE, LINE_NUM, *yytext, *yytext);

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
<INCOMMENT>\}		{ DEC_COMM; if (ZERO_COMM) { BEGIN(INITIAL); } }
\}					{ error(DUERR_UNEXPENDCMT, LINE_NUM); }

\'					{ BEGIN(INSTRING); }
<INSTRING><<EOF>>	{ error(DUERR_EOFINSTRCHR, LINE_NUM); BEGIN(INITIAL); }
<INSTRING>\n		{ error(DUERR_EOLINSTRCHR, LINE_NUM); BEGIN(INITIAL); }
<INSTRING>[^']		{ str_buff.append(yytext); }
<INSTRING>\'\'		{ str_buff.append("'"); }
<INSTRING>\'		{ BEGIN(INITIAL); lv->str_ci_ = ctx->tab->ls_str().add(str_buff); return DUTOK_STRING; }

.			error(DUERR_UNKCHAR, LINE_NUM, *yytext, *yytext);	/* -1 should be substituted by current line */

%%
