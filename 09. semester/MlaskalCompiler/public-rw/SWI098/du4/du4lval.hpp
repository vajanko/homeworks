/*

DU4LVAL.H

JY

Main interface for DU4

*/

#ifndef DU4LVAL_H_
#define DU4LVAL_H_

#include <string>
#include "literal_storage.hpp"
#include "flat_icblock.hpp"
#include "dutables.h"
#include "abstract_instr.hpp"
#include "gen_ainstr.hpp"

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

		// compiler's semantic value
		struct MlaskalLval {
				int										dtge_;			// group element
				mlc::ls_int_type::const_pointer			int_ci_;		// const iterator into INT literal table
				mlc::ls_real_type::const_pointer		real_ci_;		// const iterator into REAL literal table
				mlc::ls_str_type::const_pointer			str_ci_;		// const iterator into STRING literal table
				mlc::ls_id_type::const_pointer			id_ci_;			// const iterator into identifier table

				bool bool_val_;
				mlc::const_type const_type_;
				mlc::parameter_list *param_list_;
				std::vector<mlc::ls_id_type::const_pointer> identifiers_;
				mlc::type_pointer type_;
				std::vector<mlc::type_pointer> ranges_;
		};

		/*************************************************************/

}

// lex interface
#define YY_DECL			int yylex(mlc::MlaskalLval *lv, unsigned *l, mlc::MlaskalCtx *ctx)
YY_DECL;

#endif
