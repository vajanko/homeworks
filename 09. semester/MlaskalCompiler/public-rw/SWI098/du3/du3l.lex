%{
	
	#include "duerr.h"
	#include "du3.h"
	#include "du3tok.h"
	#include "du3g.hpp"
	
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
		
%}

%option noyywrap
%option nostdinit
%option never-interactive
%option 8bit
%option nounput

/* basic defines */
DIGIT			[0-9]
WHITESPACE		[ \r\t\f]
UINT			{DIGIT}+

%%
			
{UINT}		{
			    *l = -1;
			    return DUTOK_UINT;
			}
			
{WHITESPACE}+		/* go out with whitespaces */

.			error(DUERR_UNKCHAR, -1, *yytext, *yytext);
