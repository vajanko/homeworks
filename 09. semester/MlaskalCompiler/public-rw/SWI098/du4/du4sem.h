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

	void procedure_declare(MlaskalCtx *ctx, int line, MlaskalLval &lval);
	void function_declare(MlaskalCtx *ctx, int fnc_line, MlaskalLval &fnc_lval, int ret_line, MlaskalLval &ret_lval);

	/* This method must be called when procedure or function declaration is finised (after the body) */
	void block_leave(MlaskalCtx *ctx, int line);
}

#endif
