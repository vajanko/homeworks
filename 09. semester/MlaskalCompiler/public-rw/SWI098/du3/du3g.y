%{
	#include <stdlib.h>
    #include <assert.h>

    #include "du3.h"
    #include "duerr.h"

    /* semantic value */
    #define YYSTYPE mlc::MlaskalLval

	/* locations */
	#define YYLTYPE unsigned
	#define YYLLOC_DEFAULT(cur, rhs, n)	do { if(n) (cur)=(rhs)[1]; else (cur)=(rhs)[0]; } while(0)

    /* local stuff */
    using namespace mlc;
    
	/* error stuff */
    #define YYERROR_VERBOSE 1
    static void yyerror(YYLTYPE *line, mlc::MlaskalCtx *ctx, const char *m);
    
	/* malloc, free correct declarations */
	#define YYINCLUDED_STDLIB_H
%}

%token DUTOK_PROGRAM			/* program */
%token DUTOK_LABEL			    /* label */
%token DUTOK_CONST			    /* const */
%token DUTOK_TYPE			    /* type */
%token DUTOK_VAR			    /* var */
%token DUTOK_BEGIN			    /* begin */
%token DUTOK_END			    /* end */
%token DUTOK_PROCEDURE			/* procedure */
%token DUTOK_FUNCTION			/* function */
%token DUTOK_ARRAY			    /* array */
%token DUTOK_OF				    /* of */
%token DUTOK_GOTO			    /* goto */
%token DUTOK_IF				    /* if */
%token DUTOK_THEN			    /* then */
%token DUTOK_ELSE			    /* else */
%token DUTOK_WHILE			    /* while */
%token DUTOK_DO				    /* do */
%token DUTOK_REPEAT			    /* repeat */
%token DUTOK_UNTIL			    /* until */
%token DUTOK_FOR			    /* for */
%token DUTOK_OR				    /* or */
%token DUTOK_NOT			    /* not */
%token DUTOK_RECORD			    /* record */

/* literals */
%token DUTOK_IDENTIFIER			/* identifier */
%token DUTOK_UINT			    /* unsigned integer */
%token DUTOK_REAL			    /* real number */
%token DUTOK_STRING			    /* string */

/* delimiters */
%token DUTOK_SEMICOLON			/* ; */
%token DUTOK_DOT			    /* . */
%token DUTOK_COMMA			    /* , */
%token DUTOK_EQ				    /* = */
%token DUTOK_COLON			    /* : */
%token DUTOK_LPAR			    /* ( */
%token DUTOK_RPAR			    /* ) */
%token DUTOK_DOTDOT			    /* .. */
%token DUTOK_LSBRA			    /* [ */
%token DUTOK_RSBRA			    /* ] */
%token DUTOK_ASSIGN			    /* := */

/* grouped operators and keywords */
%token DUTOK_OPER_REL			    /* <, <=, <>, >=, > */
%token DUTOK_OPER_SIGNADD		    /* +, - */
%token DUTOK_OPER_MUL			    /* *, /, div, mod, and */
%token DUTOK_FOR_DIRECTION		    /* to, downto */

%start mlaskal
%error-verbose
%pure-parser
%parse-param {mlc::MlaskalCtx *ctx}
%lex-param {mlc::MlaskalCtx *ctx}
%locations

%%

mlaskal:	  DUTOK_PROGRAM DUTOK_IDENTIFIER DUTOK_SEMICOLON program_block DUTOK_DOT
		;

program_block: label
	| proc 
	| func
	;

/* Block */

/* End of block */

/* Variable */
variable: DUTOK_IDENTIFIER /* --> variable identifier */
	| 
	;
/* End of variable */

/* Label */
label: DUTOK_LABEL uints		/* one obligatory uint possibly followed by multiple ", uint" */
	;
/* non-empty list of uints separated by a comma */
uints: DUTOK_UINT
	| uints DUTOK_COMMA DUTOK_UINT
	;
/* End of label */

expr: simple_expr DUTOK_OPER_REL simple_expr
	| simple_expr DUTOK_EQ simple_expr
	;
simple_expr: terms 
	| DUTOK_OPER_SIGNADD terms
	;
term: factors
	;
/* non-empty list of terms sepatated by +,-,or */
terms: term
	| terms DUTOK_OPER_SIGNADD term
	| terms DUTOK_OR term
	;
factor: uconst
	| variable
	| DUTOK_LPAR expr DUTOK_RPAR
	;
/* non-empty list of factors separated by *,/,div,mod,and */
factors: factor
	| factors DUTOK_OPER_MUL factor
	;
/* Type */
type: DUTOK_IDENTIFIER	/* --> type identifier */
	| ord_type
	| struct_type
	;
ord_type: DUTOK_IDENTIFIER	/* --> ordinal type identifier */
	| ord_const DUTOK_DOTDOT ord_const
	;
struct_type: DUTOK_IDENTIFIER /* --> stuctural type identifier */
	| DUTOK_ARRAY DUTOK_LSBRA ord_types DUTOK_RSBRA DUTOK_OF type
	;
/* non-empty list of ordinal types separated by a comma */
ord_types: ord_type
	| ord_types DUTOK_COMMA ord_type
	;
/* End of type*/

/* Procedure - function */
/* procedure header */
proc: DUTOK_PROCEDURE DUTOK_IDENTIFIER params
	;
/* function header */
func: DUTOK_FUNCTION DUTOK_IDENTIFIER params DUTOK_COLON DUTOK_IDENTIFIER /* --> scalar type identifier */
	;
/* procedure or function parameters possibly without parentesis and any parameters */
params:	/* empty parameters without parentesis */
	| DUTOK_LPAR DUTOK_RPAR	/* empty parameters wiht parentesis */
	| DUTOK_LPAR formal_params DUTOK_RPAR
	;
/* non-empty list of parameter sections separated by semicolon */
formal_params: params_section
	| formal_params DUTOK_SEMICOLON params_section
	;
/* declaration of possibly multiple parameters for one single type,  */
params_section: identifiers DUTOK_COLON DUTOK_IDENTIFIER /* --> type identifier */
	| DUTOK_VAR identifiers DUTOK_COLON DUTOK_IDENTIFIER /* --> type identifier */
	;
/* non-empty list of identifiers separated by comma */
identifiers: DUTOK_IDENTIFIER
	| identifiers DUTOK_COMMA DUTOK_IDENTIFIER
	;
/* End of procedure - function */

/* Constants */
const:
	| uconst
	| DUTOK_OPER_SIGNADD DUTOK_UINT
	| DUTOK_OPER_SIGNADD DUTOK_REAL
	;
/* constant without sign */
uconst: literal
	;
literal:
	| DUTOK_IDENTIFIER
	| DUTOK_UINT
	| DUTOK_REAL
	| DUTOK_STRING
	;
ord_const:
	| DUTOK_IDENTIFIER	/* integer const identifier */
	| DUTOK_OPER_SIGNADD DUTOK_IDENTIFIER	/* integer const identifier */
	| DUTOK_OPER_SIGNADD DUTOK_UINT
	;
/* End of constants */

%%

static void yyerror(unsigned *line, MlaskalCtx *, const char *m)
{
    error(DUERR_SYNTAX, *line, m);
}
