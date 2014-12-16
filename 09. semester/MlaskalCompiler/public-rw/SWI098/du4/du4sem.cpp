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
		else if (tp->cat() == type_category::TCAT_ARRAY || tp->cat() == type_category::TCAT_UNDEF
			|| tp->cat() == type_category::TCAT_RANGE || tp->cat() == type_category::TCAT_RECORD)
		{	// return type is not an scalar type
			error(DUERR_NOTSCALAR, type_line, *type.id_ci_);
		}

		ctx->tab->add_fnc(fnc_line, fnc.id_ci_, tp, params.param_list_);
		ctx->tab->enter(fnc_line, fnc.id_ci_);
	}
	void block_leave(MlaskalCtx *ctx, int line)
	{	// do not leave at the end of a program
		if (ctx->tab->nested())
			ctx->tab->leave(line);
	}
	void var_declare(MlaskalCtx *ctx, MlaskalLval &ids, int type_line, MlaskalLval &type)
	{
		for (auto &id : ids.identifiers_)
			ctx->tab->add_var(type_line, id, type.type_);
	}

	void type_assign(MlaskalCtx *ctx, int name_line, MlaskalLval &name, MlaskalLval &type)
	{
		ctx->tab->add_type(name_line, name.id_ci_, type.type_);
	}
	void type_declare(MlaskalCtx *ctx, MlaskalLval &out, int type_line, MlaskalLval &type)
	{
		type_pointer tp = get_type_pointer(ctx, type_line, type.id_ci_);
		out.type_ = tp;
	}
	void type_declare_ordinal(MlaskalCtx *ctx, MlaskalLval &out, int type_line, MlaskalLval &type)
	{
		type_declare(ctx, out, type_line, type);
		if (out.type_->cat() != type_category::TCAT_RANGE)
		{
			error(DUERR_NOTORDINAL, type_line);
		}
	}
	void range_declare(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &low, int high_line, MlaskalLval &high)
	{
		if (*low.int_ci_ > *high.int_ci_)
		{	// bad range
			error(DUERR_BADRANGE, high_line);
		}

		auto l = ctx->tab->ls_int().add(*low.int_ci_);
		auto h = ctx->tab->ls_int().add(*high.int_ci_);
		out.type_ = ctx->tab->create_range_type(l, h);
	}
	void range_add(MlaskalLval &out, MlaskalLval &in)
	{
		out.ranges_.push_back(in.type_);
	}
	void array_declare(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &range, MlaskalLval &type)
	{
		// (TODO: type can be either identifier or anonym structural type)
		type_pointer tp = type.type_;
		for (auto rt = range.ranges_.rbegin(); rt != range.ranges_.rend(); ++rt)
			tp = ctx->tab->create_array_type(*rt, tp);

		out.type_ = tp;
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

	void identifier_add(MlaskalLval &out, MlaskalLval &in)
	{
		out.identifiers_.push_back(in.id_ci_);
	}
	/*void identifier_copy(MlaskalLval &out_lval, MlaskalLval &in_lval)
	{
		out_lval.identifiers_.insert(out_lval.identifiers_.end(), in_lval.identifiers_.begin(), in_lval.identifiers_.end());
	}*/

	void const_declare(MlaskalCtx *ctx, int line, MlaskalLval &name, MlaskalLval &value)
	{
		switch (value.const_type_)
		{
		case mlc::boolean:
			ctx->tab->add_const_bool(line, name.id_ci_, value.bool_val_);
			break;
		case mlc::integer:
			ctx->tab->add_const_int(line, name.id_ci_, value.int_ci_);
			break;
		case mlc::real:
			ctx->tab->add_const_real(line, name.id_ci_, value.real_ci_);
			break;
		case mlc::string:
			ctx->tab->add_const_str(line, name.id_ci_, value.str_ci_);
			break;
		default:
			break;
		}
	}
	void const_calculate(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &sig, MlaskalLval &value, const_type type)
	{
		symbol_pointer sp;

		switch (type)
		{
		case mlc::integer:
			if (sig.dtge_ == DUTOKGE_MINUS)
				out.int_ci_ = ctx->tab->ls_int().add(-(*value.int_ci_));
			else
				out.int_ci_ = value.int_ci_;
			break;

		case mlc::real:
			if (sig.dtge_ == DUTOKGE_MINUS)
				out.real_ci_ = ctx->tab->ls_real().add(-(*value.real_ci_));
			else
				out.real_ci_ = value.real_ci_;
			break;

		default:
			break;
		}

		out.const_type_ = type;
	}
	void const_load(MlaskalCtx *ctx, MlaskalLval &out, int line, MlaskalLval &id)
	{
		auto sp = ctx->tab->find_symbol(id.id_ci_);
		if (sp->kind() != SKIND_CONST)
		{
			error(DUERR_NOTCONST, line, *id.id_ci_);
		}

		switch (sp->access_typed()->type()->cat())
		{
		case mlc::TCAT_BOOL:
			out.bool_val_ = sp->access_const()->access_bool_const()->bool_value();
			out.const_type_ = const_type::boolean;
			break;
		case mlc::TCAT_INT:
			out.int_ci_ = sp->access_const()->access_int_const()->int_value();
			out.const_type_ = const_type::integer;
			break;
		case mlc::TCAT_REAL:
			out.real_ci_ = sp->access_const()->access_real_const()->real_value();
			out.const_type_ = const_type::real;
			break;
		case mlc::TCAT_STR:
			out.str_ci_ = sp->access_const()->access_str_const()->str_value();
			out.const_type_ = const_type::string;
			break;
		default:
			out.const_type_ = const_type::none;
			break;
		}
	}
	void const_load_and_calculate(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &sig, int id_line, MlaskalLval &id)
	{
		const_load(ctx, id, id_line, id);
		const_calculate(ctx, out, sig, id, id.const_type_);
	}


	/*void test(MlaskalCtx *ctx, MlaskalLval &lval)
	{

	}*/
}
