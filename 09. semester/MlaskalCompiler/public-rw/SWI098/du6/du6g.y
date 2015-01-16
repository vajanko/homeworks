%{
	#include <stdlib.h>

    #include "du6.h"
    #include "du6lval.hpp"
    #include "duerr.h"
	#include "du6tok.h"
    #include "du6sem.h"

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

mlaskal: DUTOK_PROGRAM DUTOK_IDENTIFIER DUTOK_SEMICOLON program_block DUTOK_DOT { set_block_code(ctx, $2, $4, block_type::program); }
	;

/* Block P*/
program_block: block_label block_const block_type block_var block_proc_func block_begin_end { append_code_block($$, $6); }
	;
block_proc_func: 
	| proc_func_defs DUTOK_SEMICOLON
	;
proc_func_def: proc DUTOK_SEMICOLON block { set_block_code(ctx, $1, $3, block_type::subprogram); }
	| func DUTOK_SEMICOLON block	{ set_block_code(ctx, $1, $3, block_type::subprogram); }
	;
proc_func_defs: proc_func_def
	| proc_func_defs DUTOK_SEMICOLON proc_func_def 
	;
/* End of block P*/

/* Block */
block: block_label block_const block_type block_var block_begin_end	{ append_code_block($$, $5); }
	;
block_label: /* empty */
	| DUTOK_LABEL uints DUTOK_SEMICOLON
	;
block_const: /* empty */
	| DUTOK_CONST id_assigns DUTOK_SEMICOLON
	;
id_assign: DUTOK_IDENTIFIER DUTOK_EQ const { const_declare(ctx, @1, $1, $3); }
	;
id_assigns: id_assign
	| id_assigns DUTOK_SEMICOLON id_assign
	;
block_type: /* empty */
	| DUTOK_TYPE type_assigns DUTOK_SEMICOLON
	;
type_assign: DUTOK_IDENTIFIER DUTOK_EQ type { type_assign(ctx, @1, $1, $3); }
	;
type_assigns: type_assign
	| type_assigns DUTOK_SEMICOLON type_assign
	;
block_var: /* empty */
	| DUTOK_VAR var_defs DUTOK_SEMICOLON
	;
var_def: identifiers DUTOK_COLON type { var_declare(ctx, $1, @3, $3); }
	;
var_defs: var_def
	| var_defs DUTOK_SEMICOLON var_def
	;
block_begin_end: DUTOK_BEGIN stmts DUTOK_END { append_code_block($$, $2); block_leave(ctx, @2); }
/* End of block */

/* Statement */
stmt: m_stmt
	| u_stmt
	;
/* m_stmt only contains full conditions "if then else" */
m_stmt: DUTOK_IF expr DUTOK_THEN m_stmt DUTOK_ELSE m_stmt	{ if_else_stmt(ctx, $$, @2, $2, $4, $6); } /* --> boolean expression */
	| DUTOK_WHILE expr DUTOK_DO m_stmt	{ while_stmt(ctx, $$, @2, $2, $4); }	/* --> boolean expression */
	| DUTOK_UINT DUTOK_COLON stmt_rest	{ label_target(ctx, $$, @1, $1); append_code_block($$, $3);  }	/* statement optionaly starts with 123: ... */ 	
	| stmt_rest
	;
u_stmt: DUTOK_IF expr DUTOK_THEN stmt	{ if_stmt(ctx, $$, @2, $2, $4); } /* --> boolean expression */
	| DUTOK_IF expr DUTOK_THEN m_stmt DUTOK_ELSE u_stmt		{ if_else_stmt(ctx, $$, @2, $2, $4, $6); } /* --> boolean expression */
	| DUTOK_WHILE expr DUTOK_DO u_stmt	{ while_stmt(ctx, $$, @2, $2, $4); }	/* --> boolean expression */
	| DUTOK_FOR DUTOK_IDENTIFIER DUTOK_ASSIGN expr DUTOK_FOR_DIRECTION expr DUTOK_DO u_stmt	{ for_stmt(ctx, $$, @2, $2, $4, $5, $6, $8); }	/* --> ordinal variable identifier, ordinal expression 2x */
	;
/* the rest of statement definition except "if" and "while" without leading label */
stmt_rest: /* empty */
	| DUTOK_IDENTIFIER DUTOK_ASSIGN expr	{ store_identifier(ctx, $$, @1, $1, $3); } /* --> variable, function identifier */
	| array_var DUTOK_ASSIGN expr			{ store_array(ctx, $$, @1, $1, $3); }	/* --> array variable */
	| DUTOK_IDENTIFIER						{ subprogram_call(ctx, $$, @1, $1, $1); }	/* --> procedure identifier */
	| DUTOK_IDENTIFIER DUTOK_LPAR real_params DUTOK_RPAR	{ subprogram_call(ctx, $$, @1, $1, $3); }	/* --> procedure identifier */
	| DUTOK_GOTO DUTOK_UINT					{ label_goto(ctx, $$, @2, $2); }
	| DUTOK_BEGIN stmts DUTOK_END			{ append_code_block($$, $2); }
	| DUTOK_REPEAT stmts DUTOK_UNTIL expr	{ repeat_stmt(ctx, $$, @4, $4, $2); }					/* --> boolean expression */
	| DUTOK_FOR DUTOK_IDENTIFIER DUTOK_ASSIGN expr DUTOK_FOR_DIRECTION expr DUTOK_DO m_stmt	{ for_stmt(ctx, $$, @2, $2, $4, $5, $6, $8); }	/* --> ordinal variable identifier, ordinal expression 2x */
	;
/* non-empty list of statements separated by a semicolon */
stmts: stmt
	| stmts DUTOK_SEMICOLON stmt		{ append_code_block($1, $3); }
	;
/* non-empty list of expressions separated by a comma */
real_params: expr { $$.exprs_.push_back($1.code_); $$.identifiers_.push_back($1.id_ci_); }
	| real_params DUTOK_COMMA expr	{ $$.exprs_.push_back($3.code_); $$.identifiers_.push_back($1.id_ci_); }
	;
/* End of statement */

/* Expression */
expr: simple_expr								
	| simple_expr DUTOK_OPER_REL simple_expr	{ binary_op(ctx, $$, $1, @2, $2, $3); }
	| simple_expr DUTOK_EQ simple_expr			{ $2.dtge_ = DUTOKGE_EQ; binary_op(ctx, $$, $1, @2, $2, $3); }
	;
simple_expr: terms								
	| DUTOK_OPER_SIGNADD terms					{ unary_op(ctx, $$, @1, $1, $2); }
	;
/* non-empty list of terms sepatated by +,-,or */
terms: term										
	| terms DUTOK_OPER_SIGNADD term				{ binary_op(ctx, $$, $1, @2, $2, $3); }
	| terms DUTOK_OR term						{ $2.dtge_ = DUTOKGE_OR; binary_op(ctx, $$, $1, @2, $2, $3); }
	;
term: factors
	;
/* non-empty list of factors separated by *,/,div,mod,and */
factors: factor
	| factors DUTOK_OPER_MUL factor				{ binary_op(ctx, $$, $1, @2, $2, $3); }
	;
factor:DUTOK_IDENTIFIER	{ load_identifier(ctx, $$, @1, $1); }						/* --> unsigned constant, variable, function identifier */
	| DUTOK_UINT		{ load_const_value(ctx, $$, $1, const_type::integer); }		/* --> unsigned constant identifier */
	| DUTOK_REAL		{ load_const_value(ctx, $$, $1, const_type::real);	}		/* --> unsigned constant identifier */
	| DUTOK_STRING		{ load_const_value(ctx, $$, $1, const_type::string); }		/* --> unsigned constant identifier */
	| array_var			{ load_element(ctx, $$, @1, $1); }							/* --> array identifier, ordinal expressions */
	| DUTOK_IDENTIFIER DUTOK_LPAR real_params DUTOK_RPAR	{ subprogram_call(ctx, $$, @1, $1, $3); } /* --> function identifier */
	| DUTOK_LPAR expr DUTOK_RPAR { $$.type_ = $2.type_; $$.code_ = $2.code_; }
	| DUTOK_NOT factor	{ unary_not(ctx, $$, @1, $1, $2); }
	;
array_var: DUTOK_IDENTIFIER idxs	{ array_element(ctx, $$, @1, $1, $2); }	/* --> array identifier, ordinal expressions */
	;
/* array indexer */
idxs: idx
	| idxs idx	{ $$.exprs_.insert($$.exprs_.end(), $2.exprs_.begin(), $2.exprs_.end()); }
	;
idx: DUTOK_LSBRA exprs DUTOK_RSBRA		{ $$.code_ = $2.code_; $$.type_ = $2.type_; $$.exprs_ = $2.exprs_; }	/* ordinal expressions */
	;
exprs: expr						{ array_index(ctx, $$, @1, $1); }
	| exprs DUTOK_COMMA expr	{ array_index(ctx, $$, @3, $3); }
	;
/* End of expression */

/* Type */
type: DUTOK_IDENTIFIER { type_declare(ctx, $$, @1, $1); }	/* --> type, ordinal type, structural type, integer constant identifier */
	| range { $$.type_ = $1.type_; }
	| DUTOK_ARRAY DUTOK_LSBRA ord_types DUTOK_RSBRA DUTOK_OF type { array_declare(ctx, $$, $3, $6); }
	;
ord_types: ord_type { range_add($$, $1); }
	| ord_types DUTOK_COMMA ord_type { range_add($$, $3); }
	;
ord_type: DUTOK_IDENTIFIER  { type_declare_ordinal(ctx, $$, @1, $1); }	/* --> ordinal type identifier (range) */
	| range { $$.type_ = $1.type_; }
	;
range: ord_const DUTOK_DOTDOT ord_const { range_declare(ctx, $$, $1, @3, $3); }
	;
/* End of type*/

/* Procedure - function */
/* procedure header */
proc: DUTOK_PROCEDURE DUTOK_IDENTIFIER params { procedure_declare(ctx, $$, @1, $2, $3); }
	;
/* function header */
func: DUTOK_FUNCTION DUTOK_IDENTIFIER params DUTOK_COLON DUTOK_IDENTIFIER { function_declare(ctx, $$, @1, $2, $3, @5, $5); } /* --> scalar type identifier */
	;
/* procedure or function parameters possibly without parentesis and any parameters */
params: { $$.param_list_ = create_parameter_list(); }	/* empty parameters without parentesis */
	| DUTOK_LPAR DUTOK_RPAR	{ $$.param_list_ = create_parameter_list(); }	/* empty parameters with parentesis */
	| DUTOK_LPAR formal_params DUTOK_RPAR { $$.param_list_ = $2.param_list_; }
	;
/* non-empty list of parameter sections separated by semicolon */
formal_params: params_section { $1.param_list_ = $$.param_list_; }
	| formal_params DUTOK_SEMICOLON params_section { $1.param_list_->append_and_kill($3.param_list_); $$.param_list_ = $1.param_list_; }
	;
/* declaration of possibly multiple parameters for one single type,  */
params_section: identifiers DUTOK_COLON DUTOK_IDENTIFIER { parameter_add(ctx, $$, $1, @3, $3, param_type::value); }		/* --> type identifier */
	| DUTOK_VAR identifiers DUTOK_COLON DUTOK_IDENTIFIER { parameter_add(ctx, $$, $2, @4, $4, param_type::reference); } /* --> type identifier */
	;
/* End of procedure - function */

/* Constants */
const:DUTOK_IDENTIFIER	{ const_load(ctx, $$, @1, $1); } /* --> constant identifier */
	| DUTOK_OPER_SIGNADD DUTOK_IDENTIFIER { const_load_and_calculate(ctx, $$, $1, @2, $2); }
	| DUTOK_UINT { $$.const_type_ = const_type::integer; }
	| DUTOK_OPER_SIGNADD DUTOK_UINT { const_calculate(ctx, $$, $1, $2, const_type::integer);  }
	| DUTOK_REAL { $$.const_type_ = const_type::real; }
	| DUTOK_OPER_SIGNADD DUTOK_REAL { const_calculate(ctx, $$, $1, $2, const_type::real); }
	| DUTOK_STRING { $$.const_type_ = const_type::string; }
	;
ord_const:
	| DUTOK_IDENTIFIER	{ const_load(ctx, $$, @1, $1); }
	| DUTOK_OPER_SIGNADD DUTOK_IDENTIFIER { const_load_and_calculate(ctx, $$, $1, @2, $2); }
	| DUTOK_UINT
	| DUTOK_OPER_SIGNADD DUTOK_UINT { const_calculate(ctx, $$, $1, $2, const_type::integer); }
	;
/* End of constants */

/* non-empty list of identifiers separated by comma */
identifiers: DUTOK_IDENTIFIER { identifier_add($$, $1); }
	| identifiers DUTOK_COMMA DUTOK_IDENTIFIER { identifier_add($$, $3); }
	;
/* non-empty list of uints separated by a comma - these are only used in block_label */
uints: DUTOK_UINT { ctx->tab->add_label_entry(@1, $1.int_ci_, new_label(ctx)); }
	| uints DUTOK_COMMA DUTOK_UINT { ctx->tab->add_label_entry(@3, $3.int_ci_, new_label(ctx)); }
	;

%%

static void yyerror(unsigned *line, MlaskalCtx *, const char *m)
{
    error(DUERR_SYNTAX, *line, m);
}