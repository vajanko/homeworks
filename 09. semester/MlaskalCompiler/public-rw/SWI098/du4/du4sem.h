/*

	DU4SEM.H

	DB

	Mlaskal's semantic interface for DU4

*/

#ifndef __DU4SEM_H
#define __DU4SEM_H

#include "du4.h"
#include "du4lval.hpp"

namespace mlc {

	enum param_type
	{
		value,
		reference
	};

	void type_assign(MlaskalCtx *ctx, int name_line, MlaskalLval &name, MlaskalLval &type);
	void type_declare(MlaskalCtx *ctx, MlaskalLval &out, int type_line, MlaskalLval &type);
	void type_declare_ordinal(MlaskalCtx *ctx, MlaskalLval &out, int type_line, MlaskalLval &type);
	void range_declare(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &low, int high_line, MlaskalLval &high);
	void range_add(MlaskalLval &out, MlaskalLval &in);
	void array_declare(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &range, MlaskalLval &type);

	void procedure_declare(MlaskalCtx *ctx, int proc_line, MlaskalLval &proc_lval, MlaskalLval &params_lval);
	void function_declare(MlaskalCtx *ctx, int fnc_line, MlaskalLval &fnc, MlaskalLval &params, int type_line, MlaskalLval &type);
	void parameter_add(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &ids, int type_line, MlaskalLval &type, param_type pt);

	/* This method must be called when procedure or function declaration is finised (after the body) */
	void block_leave(MlaskalCtx *ctx, int line);

	void var_declare(MlaskalCtx *ctx, MlaskalLval &ids, int type_line, MlaskalLval &type);

	void identifier_add(MlaskalLval &out, MlaskalLval &in);

	void const_declare(MlaskalCtx *ctx, int line, MlaskalLval &name, MlaskalLval &value);
	void const_calculate(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &sig, MlaskalLval &value, const_type type);

	void test(MlaskalCtx *ctx, MlaskalLval &lval);
}

#endif
