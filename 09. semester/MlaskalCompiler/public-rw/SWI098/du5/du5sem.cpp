/*

DU5SEM.CPP

*/

#include "du5sem.h"
#include "dubaseerr.h"
#include "duerr.h"
#include "du5tok.h"

namespace mlc {

	/**
	 * Gets type of type-identifier and prints an error when there is no such type
	 */
	type_pointer get_type_pointer(MlaskalCtx *ctx, int line, mlc::ls_id_type::const_pointer id)
	{
		auto sp = ctx->tab->find_symbol(id);
		type_pointer tp = sp->access_typed()->type();
		if (sp->kind() != SKIND_TYPE)
		{	// type does not exit
			error(DUERR_NOTTYPE, line, *id);
		}

		return tp;
	}
	/**
	 *  Gets type of an identifier (variable, function, constant)
	 */
	type_pointer get_symbol_type(MlaskalCtx *ctx, mlc::ls_id_type::const_pointer id)
	{
		auto sp = ctx->tab->find_symbol(id);
		return sp->access_typed()->type();
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


	/*  */
	void set_block_code(MlaskalCtx *ctx, MlaskalLval &id, MlaskalLval &code, block_type type)
	{
		switch (type)
		{
		case mlc::program:
			ctx->tab->set_main_code(id.id_ci_, code.code_);
			break;
		case mlc::subprogram:
			//ctx->tab->set_subprogram_code(id.id_ci_, code.code_);
			break;
		}
	}
	void append_code_block(MlaskalLval &out, MlaskalLval &in)
	{
		if (out.code_ == NULL)
		{
			out.code_ = in.code_;
		}
		else if (in.code_ != NULL)
		{
			icblock_append_delete(out.code_, in.code_);
			in.code_ = NULL;
		}
	}

	/**
	 * type_ is assigned with the type of constant
	 */
	void load_value(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &val, const_type type)
	{
		out.code_ = icblock_create();

		switch (type)
		{
		case mlc::identifier:
			out.type_ = get_symbol_type(ctx, val.id_ci_);
			break;
		case mlc::boolean:
			out.code_->append_instruction(new ai::LDLITB(val.bool_val_));
			out.type_ = ctx->tab->logical_bool();
			break;
		case mlc::integer:
			out.code_->append_instruction(new ai::LDLITI(val.int_ci_));
			out.type_ = ctx->tab->logical_integer();
			break;
		case mlc::real:
			out.code_->append_instruction(new ai::LDLITR(val.real_ci_));
			out.type_ = ctx->tab->logical_real();
			break;
		case mlc::string:
			out.code_->append_instruction(new ai::LDLITS(val.str_ci_));
			out.type_ = ctx->tab->logical_string();
			break;
		}
	}
	void mul_factor(MlaskalLval &out, MlaskalLval &left, MlaskalLval &op, MlaskalLval &right)
	{
		icblock_append_delete(left.code_, right.code_);
		out.code_ = left.code_;
		// TODO: conversion, real/integer multiplication/division, bool and

		switch (op.dtge_)
		{
		case DUTOKGE_ASTERISK:
			out.code_->append_instruction(new ai::MULI());
			break;
		case DUTOKGE_SOLIDUS:
			break;
		case DUTOKGE_DIV:
			break;
		case DUTOKGE_MOD:
			break;
		case DUTOKGE_AND:
			break;
		}
	}
	void unary_op(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &op, MlaskalLval &val)
	{
		out.code_ = val.code_;
		out.type_ = val.type_;

		switch (op.dtge_)
		{
		case DUTOKGE_PLUS:
			// just ignore it, but check whether operand is either real or integer
			if (!identical_type(val.type_, ctx->tab->logical_integer()) &&
				!identical_type(val.type_, ctx->tab->logical_real()))
			{
				error(DUERR_SYNTAX, 0);// TODO: operator line
			}
			break;
		case DUTOKGE_MINUS:
			if (identical_type(val.type_, ctx->tab->logical_integer()))
			{
				out.code_->append_instruction(new ai::MINUSI());
			}
			else if (identical_type(val.type_, ctx->tab->logical_real()))
			{
				out.code_->append_instruction(new ai::MINUSR());
			}
			else
			{
				error(DUERR_SYNTAX, 0);// TODO: operator line
			}
			break;
			// todo: NOT
		}
	}
	void binary_op(MlaskalLval &out, MlaskalLval &left, MlaskalLval &op, MlaskalLval &right)
	{
		icblock_append_delete(left.code_, right.code_);
		out.code_ = left.code_;

		switch (op.dtge_)
		{
		case DUTOKGE_ASTERISK:
			out.code_->append_instruction(new ai::MULI());
			break;
		case DUTOKGE_SOLIDUS:
			break;
		case DUTOKGE_DIV:
			break;
		case DUTOKGE_MOD:
			break;
		case DUTOKGE_PLUS:
			break;
		case DUTOKGE_MINUS:
			break;
		}
	}
	
	
	void add_conversion(mlc::icblock_pointer code, int line, mlc::type_pointer lt, mlc::type_pointer rt)
	{
		if (lt->cat() == type_category::TCAT_INT && rt->cat() == type_category::TCAT_REAL)
		{	// conversion from REAL to INTEGER
			code->append_instruction(new ai::CVRTRI());
			error(DUERR_CONVERSION, line);
		}
		else if (lt->cat() == type_category::TCAT_REAL && rt->cat() == type_category::TCAT_INT)
		{	// conversion from INTEGER to REAL
			code->append_instruction(new ai::CVRTIR());
			error(DUERR_CONVERSION, line);
		}
		else if (!identical_type(lt, rt))
		{	// conversion is not allowed
			error(DUERR_CANNOTCONVERT, line);
		}
	}
	void assign(MlaskalCtx *ctx, MlaskalLval &out, int id_line, MlaskalLval &id, MlaskalLval &expr)
	{
		icblock_pointer block = expr.code_;
		out.code_ = block;

		/* --> variable, function identifier */
		// find assigned symbol
		auto sp = ctx->tab->find_symbol(id.id_ci_);
		// find out type of the assigned symbol
		id.type_ = get_symbol_type(ctx, id.id_ci_);
			
		// possible conversion in the assignment
		// it is also checked whether assignment is possible
		add_conversion(block, id_line, id.type_, expr.type_);

		type_category tcat = id.type_->cat();
		if (sp->kind() == symbol_kind::SKIND_GLOBAL_VARIABLE)
		{	// global variable assignment - for each type different kind of instruction
			stack_address adr = sp->access_typed()->access_global_variable()->address();
			if (tcat == type_category::TCAT_REAL)
				block->append_instruction(new ai::GSTR(adr));
			else if (tcat == type_category::TCAT_INT)
				block->append_instruction(new ai::GSTI(adr));
			else if (tcat == type_category::TCAT_STR)
				block->append_instruction(new ai::GSTS(adr));
		}
		else if (sp->kind() == symbol_kind::SKIND_LOCAL_VARIABLE)
		{	// local variable assignment - for each type different kind of instruction
			stack_address adr = sp->access_typed()->access_local_variable()->address();
			if (tcat == type_category::TCAT_REAL)
				block->append_instruction(new ai::LSTR(adr));
			else if (tcat == type_category::TCAT_INT)
				block->append_instruction(new ai::LSTI(adr));
			else if (tcat == type_category::TCAT_STR)
				block->append_instruction(new ai::LSTS(adr));
		}
		else if (sp->kind() == symbol_kind::SKIND_FUNCTION)
		{
			
		}
		else
		{
			error(DUERR_NOTVAR, id_line, *id.id_ci_);
			return;
		}
	}
	
};

/*****************************************/