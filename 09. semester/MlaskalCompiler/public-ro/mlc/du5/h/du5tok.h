/*

	DU5TOK.H

	JY

	Lexical tokens definition for DU5

*/

#ifndef DU5TOK_H_
#define DU5TOK_H_

/* group elements */
#define DUTOKGE_OPER_REL		10
#define DUTOKGE_LT				(DUTOKGE_OPER_REL+1)			/* < */
#define DUTOKGE_LE				(DUTOKGE_OPER_REL+2)			/* <= */
#define DUTOKGE_NE				(DUTOKGE_OPER_REL+3)			/* <> */
#define DUTOKGE_GE				(DUTOKGE_OPER_REL+4)			/* >= */
#define DUTOKGE_GT				(DUTOKGE_OPER_REL+5)			/* > */

#define DUTOKGE_OPER_SIGNADD		20
#define DUTOKGE_PLUS				(DUTOKGE_OPER_SIGNADD+1)	/* + */
#define DUTOKGE_MINUS				(DUTOKGE_OPER_SIGNADD+2)	/* - */

#define DUTOKGE_OPER_MUL		30
#define DUTOKGE_ASTERISK		(DUTOKGE_OPER_MUL+1)			/* * */
#define DUTOKGE_SOLIDUS				(DUTOKGE_OPER_MUL+2)			/* / */
#define DUTOKGE_DIV				(DUTOKGE_OPER_MUL+3)			/* div */
#define DUTOKGE_MOD				(DUTOKGE_OPER_MUL+4)			/* mod */
#define DUTOKGE_AND				(DUTOKGE_OPER_MUL+5)			/* and */

#define DUTOKGE_FOR_DIRECTION		40
#define DUTOKGE_TO				(DUTOKGE_FOR_DIRECTION+1)	/* to */
#define DUTOKGE_DOWNTO				(DUTOKGE_FOR_DIRECTION+2)	/* downto */

#endif
