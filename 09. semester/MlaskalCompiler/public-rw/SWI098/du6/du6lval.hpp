/*

DU6LVAL.H

*/

#ifndef DU6LVAL_H_
#define DU6LVAL_H_

#include "literal_storage.hpp"
#include "dutables.h"
#include "du6tok.h"

#define DUTOKGE_EQ (DUTOKGE_GT + 1)
#define DUTOKGE_OR (DUTOKGE_AND + 1)

namespace mlc {

	enum const_type
	{
		identifier,
		boolean,
		integer,
		real,
		string,
		none
	};

	enum block_type
	{
		program,
		subprogram
	};

	typedef std::vector<mlc::icblock_pointer> icblock_list;
	typedef std::vector<mlc::type_pointer> type_list;

		// compiler's semantic value
		struct MlaskalLval {
			int									dtge_;		// group element
			mlc::ls_int_type::const_pointer		int_ci_;	// const iterator into INT literal table
			mlc::ls_real_type::const_pointer	real_ci_;	// const iterator into REAL literal table
			mlc::ls_str_type::const_pointer		str_ci_;	// const iterator into STRING literal table
			mlc::ls_id_type::const_pointer		id_ci_;		// const iterator into identifier table

			// Sem prijdou dalsi polozky pro semantickou analyzu
			bool bool_val_;
			mlc::const_type const_type_;
			mlc::parameter_list *param_list_;
			std::vector<mlc::ls_id_type::const_pointer> identifiers_;
			// type pointer in the case of type identifier in the variable decalration or
			// parameter declaration, range declaration, ...
			// also used for storing type of expression
			mlc::type_pointer type_;
			std::vector<mlc::type_pointer> ranges_;
			// array indexer expressions
			icblock_list exprs_;

			mlc::icblock_pointer code_ = NULL;
		};

		/*************************************************************/

}

// lex interface
#define YY_DECL			int yylex(mlc::MlaskalLval *lv, unsigned *l, mlc::MlaskalCtx *ctx)
YY_DECL;

#endif
