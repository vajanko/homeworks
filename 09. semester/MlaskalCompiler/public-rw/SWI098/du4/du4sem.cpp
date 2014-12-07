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

	type_pointer get_type_pointer(MlaskalCtx *ctx, int line, mlc::ls_id_type::const_pointer type_id)
	{
		auto sp = ctx->tab->find_symbol(type_id);
		type_pointer tp = sp->access_typed()->type();
		if (sp->kind() != SKIND_TYPE)
		{	// type does not exit
			error(DUERR_NOTTYPE, line, *type_id);
		}

		return tp;
	}

	void procedure_declare(MlaskalCtx *ctx, int proc_line, MlaskalLval &proc, MlaskalLval &params)
	{
		ctx->tab->add_proc(proc_line, proc.id_ci_, params.param_list_);
		ctx->tab->enter(proc_line, proc.id_ci_);
	}
	void function_declare(MlaskalCtx *ctx, int fnc_line, MlaskalLval &fnc, MlaskalLval &params, int type_line, MlaskalLval &type)
	{
		/* --> scalar type identifier */
		auto sp = ctx->tab->find_symbol(type.id_ci_);
		type_pointer tp = sp->access_typed()->type();
		if (sp->kind() != SKIND_TYPE) 
		{	// return type does not exit
			error(DUERR_NOTTYPE, type_line, *type.id_ci_);
		}
		else if (tp->cat() == TCAT_ARRAY || tp->cat() == TCAT_UNDEF)
		{	// return type is not an scalar type
			error(DUERR_NOTSCALAR, type_line, *type.id_ci_);
		}	

		ctx->tab->add_fnc(fnc_line, fnc.id_ci_, tp, params.param_list_);
		ctx->tab->enter(fnc_line, fnc.id_ci_);
	}
	void block_leave(MlaskalCtx *ctx, int line)
	{
		// do not leave at the end of a program
		if (ctx->tab->nested())
			ctx->tab->leave(line);
	}

	void type_declare(MlaskalCtx *ctx, int line, MlaskalLval &lval)
	{
		//DUERR_BADRANGE
	}

	void parameter_add(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &ids, int type_line, MlaskalLval &type, param_type pt)
	{
		out.param_list_ = mlc::create_parameter_list();

		/* --> type identifier */
		type_pointer tp = get_type_pointer(ctx, type_line, type.id_ci_);

		if (pt == param_type::value)
		{
			for (auto id : ids.identifiers_)
				out.param_list_->append_parameter_by_value(id, tp);
		}
		else
		{
			for (auto id : ids.identifiers_)
				out.param_list_->append_parameter_by_reference(id, tp);
		}
	}

	void identifier_add(MlaskalLval &out_lval, MlaskalLval &in_lval)
	{
		out_lval.identifiers_.push_back(in_lval.id_ci_);
	}
	void identifier_copy(MlaskalLval &out_lval, MlaskalLval &in_lval)
	{
		out_lval.identifiers_.insert(out_lval.identifiers_.end(), in_lval.identifiers_.begin(), in_lval.identifiers_.end());
	}

	void test(MlaskalCtx *ctx, MlaskalLval &lval)
	{

	}
}
