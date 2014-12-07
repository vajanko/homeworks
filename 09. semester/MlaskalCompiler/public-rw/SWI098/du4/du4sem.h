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

	void procedure_declare(MlaskalCtx *ctx, int proc_line, MlaskalLval &proc_lval, MlaskalLval &params_lval);
	void function_declare(MlaskalCtx *ctx, int fnc_line, MlaskalLval &fnc, MlaskalLval &params, int type_line, MlaskalLval &type);
	void parameter_add(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &ids, int type_line, MlaskalLval &type, param_type pt);

	/* This method must be called when procedure or function declaration is finised (after the body) */
	void block_leave(MlaskalCtx *ctx, int line);


	void identifier_add(MlaskalLval &out_lval, MlaskalLval &in_lval);
	void identifier_copy(MlaskalLval &out_lval, MlaskalLval &in_lval);

	void test(MlaskalCtx *ctx, MlaskalLval &lval);
}

#endif
