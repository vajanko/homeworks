/*

	DU2TOK.H

	JY

	Lexical tokens definition for DU2

*/

#ifndef DU2TOK_H_
#define DU2TOK_H_

#include "carray.hpp"

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

/* keywords */
#define DUTOK_KEYWORD				100
#define DUTOK_PROGRAM				(DUTOK_KEYWORD+1)			/* program */
#define DUTOK_LABEL				(DUTOK_KEYWORD+2)			/* label */
#define DUTOK_CONST				(DUTOK_KEYWORD+3)			/* const */
#define DUTOK_TYPE				(DUTOK_KEYWORD+4)			/* type */
#define DUTOK_VAR				(DUTOK_KEYWORD+5)			/* var */
#define DUTOK_BEGIN				(DUTOK_KEYWORD+6)			/* begin */
#define DUTOK_END				(DUTOK_KEYWORD+7)			/* end */
#define DUTOK_PROCEDURE				(DUTOK_KEYWORD+8)			/* procedure */
#define DUTOK_FUNCTION				(DUTOK_KEYWORD+9)			/* function */
#define DUTOK_ARRAY				(DUTOK_KEYWORD+10)			/* array */
#define DUTOK_OF				(DUTOK_KEYWORD+11)			/* of */
#define DUTOK_GOTO				(DUTOK_KEYWORD+12)			/* goto */
#define DUTOK_IF				(DUTOK_KEYWORD+13)			/* if */
#define DUTOK_THEN				(DUTOK_KEYWORD+14)			/* then */
#define DUTOK_ELSE				(DUTOK_KEYWORD+15)			/* else */
#define DUTOK_WHILE				(DUTOK_KEYWORD+16)			/* while */
#define DUTOK_DO				(DUTOK_KEYWORD+17)			/* do */
#define DUTOK_REPEAT				(DUTOK_KEYWORD+18)			/* repeat */
#define DUTOK_UNTIL				(DUTOK_KEYWORD+19)			/* until */
#define DUTOK_FOR				(DUTOK_KEYWORD+20)			/* for */
#define DUTOK_OR				(DUTOK_KEYWORD+21)			/* or */
#define DUTOK_NOT				(DUTOK_KEYWORD+22)			/* not */
#define DUTOK_RECORD				(DUTOK_KEYWORD+23)			/* record */

/* literals */
#define DUTOK_LITERAL				300
#define DUTOK_IDENTIFIER		(DUTOK_LITERAL+1)			/* identifier */
#define DUTOK_UINT				(DUTOK_LITERAL+2)			/* unsigned integer */
#define DUTOK_REAL				(DUTOK_LITERAL+3)			/* real number */
#define DUTOK_STRING				(DUTOK_LITERAL+4)			/* string */

/* delimiters */
#define DUTOK_DELIMITER				500
#define DUTOK_SEMICOLON				(DUTOK_DELIMITER+1)			/* ; */
#define DUTOK_DOT				(DUTOK_DELIMITER+2)			/* . */
#define DUTOK_COMMA				(DUTOK_DELIMITER+3)			/* , */
#define DUTOK_EQ				(DUTOK_DELIMITER+4)			/* = */
#define DUTOK_COLON				(DUTOK_DELIMITER+5)			/* : */
#define DUTOK_LPAR				(DUTOK_DELIMITER+6)			/* ( */
#define DUTOK_RPAR				(DUTOK_DELIMITER+7)			/* ) */
#define DUTOK_DOTDOT				(DUTOK_DELIMITER+8)			/* .. */
#define DUTOK_LSBRA				(DUTOK_DELIMITER+9)			/* [ */
#define DUTOK_RSBRA				(DUTOK_DELIMITER+10)			/* ] */
#define DUTOK_ASSIGN				(DUTOK_DELIMITER+11)			/* := */

/* group tokens */
#define DUTOK_GROUP_TOKEN		700
#define DUTOK_OPER_REL				(DUTOK_GROUP_TOKEN+1)			/* < > <= >= <> */
#define DUTOK_OPER_SIGNADD		(DUTOK_GROUP_TOKEN+2)			/* + - */
#define DUTOK_OPER_MUL				(DUTOK_GROUP_TOKEN+3)			/* * / div mod and */
#define DUTOK_FOR_DIRECTION		(DUTOK_GROUP_TOKEN+4)			/* to downto */

/* flags for DUTOK */
#define DUTOKM_SUBTYPE				0x00000007
#define DUTOKF_ST_NOTHING		0x00000000
#define DUTOKF_ST_HAS_GE		0x00000001
#define DUTOKF_ST_LIT_ID		0x00000002
#define DUTOKF_ST_LIT_UINT		0x00000003
#define DUTOKF_ST_LIT_REAL		0x00000004
#define DUTOKF_ST_LIT_STRING		0x00000005

namespace mlc {

	/* DU token structure */
	typedef struct {
		int								tok;
		const char						*text;
		unsigned						flags;
	} DUTOK;

	typedef struct {
		int								tokge;
		const char						*text;
	} DUTOKGE;

	/* tokens & token groups */
	extern mlaskal::carray<DUTOK> tokens;
	extern mlaskal::carray<DUTOKGE> group_elements;
}

#endif
