/*

	DU2TOK.CPP

	JY

	Lexical tokens definition for DU2

*/

#include "du2tok.h"

using namespace mlc;
using namespace mlaskal;

namespace {

	DUTOK tokens_int_[] = {
			{ DUTOK_PROGRAM				, "program",				0 },
			{ DUTOK_LABEL				, "label",				0 },
			{ DUTOK_CONST				, "const",				0 },
			{ DUTOK_TYPE				, "type",				0 },
			{ DUTOK_VAR						, "var",				0 },
			{ DUTOK_BEGIN				, "begin",				0 },
			{ DUTOK_END						, "end",				0 },
			{ DUTOK_PROCEDURE				, "procedure",				0 },
			{ DUTOK_FUNCTION				, "function",				0 },
			{ DUTOK_ARRAY				, "array",				0 },
			{ DUTOK_OF						, "of",						0 },
			{ DUTOK_GOTO				, "goto",				0 },
			{ DUTOK_IF						, "if",						0 },
			{ DUTOK_THEN				, "then",				0 },
			{ DUTOK_ELSE				, "else",				0 },
			{ DUTOK_WHILE				, "while",				0 },
			{ DUTOK_DO						, "do",						0 },
			{ DUTOK_REPEAT				, "repeat",				0 },
			{ DUTOK_UNTIL				, "until",				0 },
			{ DUTOK_FOR						, "for",				0 },
			{ DUTOK_OR						, "or",						0 },
			{ DUTOK_NOT						, "not",				0 },
			{ DUTOK_RECORD				, "record",				0 },
			{ DUTOK_IDENTIFIER				, "identifier",				DUTOKF_ST_LIT_ID },
			{ DUTOK_UINT				, "unsigned integer",		DUTOKF_ST_LIT_UINT },
			{ DUTOK_REAL				, "real number",		DUTOKF_ST_LIT_REAL },
			{ DUTOK_STRING				, "string",				DUTOKF_ST_LIT_STRING },
			{ DUTOK_SEMICOLON				, ";",						0 },
			{ DUTOK_DOT						, ".",						0 },
			{ DUTOK_COMMA				, ",",						0 },
			{ DUTOK_EQ						, "=",						0 },
			{ DUTOK_COLON				, ":",						0 },
			{ DUTOK_LPAR				, "(",						0 },
			{ DUTOK_RPAR				, ")",						0 },
			{ DUTOK_DOTDOT				, "..",						0 },
			{ DUTOK_LSBRA				, "[",						0 },
			{ DUTOK_RSBRA				, "]",						0 },
			{ DUTOK_ASSIGN				, ":=",						0 },
			{ DUTOK_OPER_REL				, "rel oper",				DUTOKF_ST_HAS_GE },
			{ DUTOK_OPER_SIGNADD		, "sign/add oper",		DUTOKF_ST_HAS_GE },
			{ DUTOK_OPER_MUL				, "mul oper",				DUTOKF_ST_HAS_GE },
			{ DUTOK_FOR_DIRECTION		, "for direction",		DUTOKF_ST_HAS_GE }
	};

	DUTOKGE group_elements_int_[] = {
			{ DUTOKGE_LT				, "<" },
			{ DUTOKGE_LE				, "<=" },
			{ DUTOKGE_NE				, "<>" },
			{ DUTOKGE_GE				, ">=" },
			{ DUTOKGE_GT				, ">" },
			{ DUTOKGE_PLUS				, "+" },
			{ DUTOKGE_MINUS				, "-" },
			{ DUTOKGE_ASTERISK				, "*" },
			{ DUTOKGE_SOLIDUS				, "/" },
			{ DUTOKGE_DIV				, "div" },
			{ DUTOKGE_MOD				, "mod" },
			{ DUTOKGE_AND				, "and" },
			{ DUTOKGE_TO				, "to" },
			{ DUTOKGE_DOWNTO				, "downto" }
	};

}

namespace mlc {

	carray<DUTOK> tokens(tokens_int_);
	carray<DUTOKGE> group_elements(group_elements_int_);

}
