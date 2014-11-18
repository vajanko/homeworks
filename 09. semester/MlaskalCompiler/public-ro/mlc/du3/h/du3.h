/*

	DU3.H

	JY

	Main interface for DU3

*/

#ifndef DU3_H_
#define DU3_H_

#include <string>
#include "literal_storage.hpp"

namespace mlc {

	// literal storages
	typedef mlaskal::lit_storage<int, false>			ls_int_type;
	typedef mlaskal::lit_storage<double, false>			ls_real_type;
	typedef mlaskal::lit_storage<std::string, false>	ls_str_type;
	typedef mlaskal::lit_storage<std::string, true>		ls_id_type;

	class fake_symbol_tables {
	public:
		ls_int_type		&ls_int()  { return ls_int_; }
		ls_real_type	&ls_real() { return ls_real_; }
		ls_str_type		&ls_str() { return ls_str_; }
		ls_id_type		&ls_id() { return ls_id_; }
	private:
		ls_int_type		ls_int_;
		ls_real_type	ls_real_;
		ls_str_type		ls_str_;
		ls_id_type		ls_id_;
	};

	struct MlaskalCtx {
		fake_symbol_tables		*tab;
	};

	// compiler's semantic value
	struct MlaskalLval {
		int								dtge_;		// group element
		ls_int_type::const_pointer		int_ci_;	// const iterator into INT literal table
		ls_real_type::const_pointer		real_ci_;	// const iterator into REAL literal table
		ls_str_type::const_pointer		str_ci_;	// const iterator into STRING literal table
		ls_id_type::const_pointer		id_ci_;		// const iterator into identifier table
	};

}

// lex interface
#define YY_DECL			int yylex(mlc::MlaskalLval *lv, unsigned *l, mlc::MlaskalCtx *ctx)
YY_DECL;

// parser interface
int yyparse(mlc::MlaskalCtx *ctx);

#endif
