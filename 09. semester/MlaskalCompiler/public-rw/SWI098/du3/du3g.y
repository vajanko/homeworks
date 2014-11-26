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

/* Block P*/
program_block: block_label block_const block_type block_var block_proc_func block_begin_end
	;
block_proc_func: 
	| proc_func_defs DUTOK_SEMICOLON
	;
proc_func_def: proc DUTOK_SEMICOLON block
	| func DUTOK_SEMICOLON block
	;
proc_func_defs: proc_func_def
	| proc_func_defs DUTOK_SEMICOLON proc_func_def
	;
/* End of block P*/

/* Block */
block: block_label block_const block_type block_var block_begin_end
	;
block_label: /* empty */
	| DUTOK_LABEL uints DUTOK_SEMICOLON
	;
block_const: /* empty */
	| DUTOK_CONST id_assigns DUTOK_SEMICOLON
	;
id_assign: DUTOK_IDENTIFIER DUTOK_EQ const
	;
id_assigns: id_assign
	| id_assigns DUTOK_SEMICOLON id_assign
	;
block_type: /* empty */
	| DUTOK_TYPE type_assigns DUTOK_SEMICOLON
	;
type_assign: DUTOK_IDENTIFIER DUTOK_EQ type
	;
type_assigns: type_assign
	| type_assigns DUTOK_SEMICOLON type_assign
	;
block_var: /* empty */
	| DUTOK_VAR var_defs DUTOK_SEMICOLON
	;
var_def: identifiers DUTOK_COLON type
	;
var_defs: var_def
	| var_defs DUTOK_SEMICOLON var_def
	;
block_begin_end: DUTOK_BEGIN  DUTOK_END			/* !!!! TODO: add stmts here !!! */
/* End of block */

/* Statement */
stmt: m_stmt
	| u_stmt
	;
/* m_stmt only contains full conditions "if then else" */
m_stmt: DUTOK_IF expr DUTOK_THEN m_stmt DUTOK_ELSE m_stmt	/* --> boolean expression */
	| DUTOK_WHILE expr DUTOK_DO m_stmt						/* --> boolean expression */
	| DUTOK_UINT DUTOK_COLON stmt_rest						/* statement optionaly starts with 123: ... */ 	
	| stmt_rest
	;
u_stmt: DUTOK_IF expr DUTOK_THEN stmt						/* --> boolean expression */
	| DUTOK_IF expr DUTOK_THEN m_stmt DUTOK_ELSE u_stmt		/* --> boolean expression */
	| DUTOK_WHILE expr DUTOK_DO u_stmt						/* --> boolean expression */
	;
/* the rest of statement definition except "if" and "while" without leading label */
stmt_rest: DUTOK_IDENTIFIER DUTOK_ASSIGN stmt				/* --> variable, function identifier */
	| DUTOK_IDENTIFIER										/* --> procedure identifier */
	| DUTOK_IDENTIFIER DUTOK_LPAR real_params DUTOK_RPAR	/* --> procedure identifier */
	| DUTOK_GOTO DUTOK_UINT
	| DUTOK_BEGIN stmts DUTOK_END
	| DUTOK_REPEAT stmts DUTOK_UNTIL expr					/* --> boolean expression */
	| DUTOK_FOR DUTOK_IDENTIFIER DUTOK_ASSIGN expr DUTOK_FOR_DIRECTION expr DUTOK_DO stmt		/* --> ordinal variable identifier, ordinal expression 2x */
	;
/* non-empty list of statements separated by a semicolon */
stmts: stmt
	| stmts DUTOK_SEMICOLON stmt
	;
/* non-empty list of expressions separated by a comma */
real_params: expr
	| real_params DUTOK_COMMA expr
	;
/* End of statement */

/* Expression */
expr: simple_expr
	| simple_expr DUTOK_OPER_REL simple_expr
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
factor: DUTOK_IDENTIFIER	/* --> unsigned constant, variable, function identifier */
	| DUTOK_UINT			/* --> unsigned constant identifier */
	| DUTOK_REAL			/* --> unsigned constant identifier */
	| DUTOK_STRING			/* --> unsigned constant identifier */
	| DUTOK_IDENTIFIER DUTOK_LSBRA stmts DUTOK_RSBRA /* --> array identifier, ordinal statements */
	| DUTOK_IDENTIFIER DUTOK_LPAR expr DUTOK_RPAR /* --> function identifier */
	| DUTOK_LPAR expr DUTOK_RPAR
	| DUTOK_NOT factor
	;
/* non-empty list of factors separated by *,/,div,mod,and */
factors: factor
	| factors DUTOK_OPER_MUL factor
	;
/* End of expression */

/* Type */
type: DUTOK_IDENTIFIER	/* --> type, ordinal type, structural type, integer constant identifier */
	| ord_const DUTOK_DOTDOT ord_const
	| DUTOK_ARRAY DUTOK_LSBRA ord_type DUTOK_RSBRA DUTOK_OF type
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
/* End of procedure - function */

/*identifier: DUTOK_IDENTIFIER /* --> constant, function, procedure, variable identifier */

/* Constants */
const: DUTOK_IDENTIFIER	/* --> constant identifier */
	| DUTOK_UINT
	| DUTOK_OPER_SIGNADD DUTOK_UINT
	| DUTOK_REAL
	| DUTOK_OPER_SIGNADD DUTOK_REAL
	| DUTOK_STRING
	;
ord_const:
	| DUTOK_IDENTIFIER	
	| DUTOK_OPER_SIGNADD DUTOK_IDENTIFIER
	| DUTOK_UINT
	| DUTOK_OPER_SIGNADD DUTOK_UINT
	;
/* End of constants */

/* Helpers */

/* declaration */
decl: identifiers DUTOK_COLON type
	;
/* non-empty list of decalrations separated by semicolon */
decls: decl
	| decls DUTOK_SEMICOLON decl
	;
/* non-empty list of identifiers separated by comma */
identifiers: DUTOK_IDENTIFIER
	| identifiers DUTOK_COMMA DUTOK_IDENTIFIER
	;
/* non-empty list of uints separated by a comma */
uints: DUTOK_UINT
	| uints DUTOK_COMMA DUTOK_UINT
	;
/* End of helpers */

%%

static void yyerror(unsigned *line, MlaskalCtx *, const char *m)
{
    error(DUERR_SYNTAX, *line, m);
}
