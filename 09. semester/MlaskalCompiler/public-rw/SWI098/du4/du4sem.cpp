/*

DU4SEM.CPP

JY

Mlaskal's semantic interface for DU4

*/

#include "du4sem.h"
#include "du4tok.h"
#include "du4.h"
#include "duerr.h"

namespace mlc {

	void procedure_declare(MlaskalCtx *ctx, int line, MlaskalLval &lval)
	{
		ctx->tab->add_proc(line, lval.id_ci_, create_parameter_list());
		ctx->tab->enter(line, lval.id_ci_);
	}
	void function_declare(MlaskalCtx *ctx, int fnc_line, MlaskalLval &fnc_lval, int ret_line, MlaskalLval &ret_lval)
	{
		auto ret_type = ctx->tab->find_symbol(ret_lval.id_ci_);
		/* --> scalar type identifier */
		auto type = ret_type->access_type()->type();

		ctx->tab->add_fnc(fnc_line, fnc_lval.id_ci_, type, create_parameter_list());
		ctx->tab->enter(fnc_line, fnc_lval.id_ci_);
	}
	void block_leave(MlaskalCtx *ctx, int line)
	{
		// do not leave at the end of a program
		if (ctx->tab->nested())
			ctx->tab->leave(line);
	}
}
