/*

DU6SEM.CPP

*/

#include "du6sem.h"
#include "du6tok.h"
#include "duerr.h"

namespace mlc {

#define LOOP_VAR "loop12345678"

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

	void procedure_declare(MlaskalCtx *ctx, MlaskalLval &out, int proc_line, MlaskalLval &proc, MlaskalLval &params)
	{
		ctx->tab->add_proc(proc_line, proc.id_ci_, params.param_list_);
		ctx->tab->enter(proc_line, proc.id_ci_);
		out.id_ci_ = proc.id_ci_;
	}
	void function_declare(MlaskalCtx *ctx, MlaskalLval &out, int fnc_line, MlaskalLval &fnc, MlaskalLval &params, int type_line, MlaskalLval &type)
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
		out.id_ci_ = fnc.id_ci_;
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
			error(DUERR_BADRANGE, high_line);

		// even if the range is bad go on as nothing has happend
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


	void test(MlaskalCtx *ctx, MlaskalLval &lval)
	{

	}

	/**
	* Append INIT instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
	*/
	void append_init_ins(mlc::icblock_pointer block, mlc::type_pointer tp)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::INITB());
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::INITI());
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::INITR());
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::INITS());
			break;
		case type_category::TCAT_ARRAY:
			break;
		}
	}
	/**
	* Append DTOR instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
	*/
	void append_dtor_ins(mlc::icblock_pointer block, mlc::type_pointer tp)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::DTORB());
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::DTORI());
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::DTORR());
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::DTORS());
			break;
		case type_category::TCAT_ARRAY:
			break;
		}
	}
	/**
	* Append GLD instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL, STRING and ARRAY
	*/
	void append_gld_ins(mlc::icblock_pointer block, mlc::type_pointer tp, mlc::stack_address adr)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::GLDB(adr));
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::GLDI(adr));
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::GLDR(adr));
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::GLDS(adr));
			break;
		case type_category::TCAT_ARRAY:
			block->append_instruction(new ai::GREF(adr));
			break;
		}
	}
	/**
	* Append LLD instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL, STRING and ARRAY.
	*/
	void append_lld_ins(mlc::icblock_pointer block, mlc::type_pointer tp, mlc::stack_address adr)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::LLDB(adr));
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::LLDI(adr));
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::LLDR(adr));
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::LLDS(adr));
			break;
		case type_category::TCAT_ARRAY:
			block->append_instruction(new ai::LREF(adr));
			break;
		}
	}
	/**
	* Append LDLIT instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
	*/
	void append_ldlit_ins(mlc::icblock_pointer block, mlc::type_pointer tp, mlc::const_symbol_reference con)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::LDLITB(con->access_bool_const()->bool_value()));
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::LDLITI(con->access_int_const()->int_value()));
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::LDLITR(con->access_real_const()->real_value()));
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::LDLITS(con->access_str_const()->str_value()));
			break;
		}
	}
	/**
	* Append GST instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
	*/
	void append_gst_ins(mlc::icblock_pointer block, mlc::type_pointer tp, mlc::stack_address adr)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::GSTB(adr));
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::GSTI(adr));
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::GSTR(adr));
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::GSTS(adr));
			break;
		}
	}
	/**
	* Append LST instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
	*/
	void append_lst_ins(mlc::icblock_pointer block, mlc::type_pointer tp, mlc::stack_address adr)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::LSTB(adr));
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::LSTI(adr));
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::LSTR(adr));
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::LSTS(adr));
			break;
		}
	}
	/**
	* Append XLD instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
	*/
	void append_xld_ins(mlc::icblock_pointer block, mlc::type_pointer tp)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::XLDB());
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::XLDI());
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::XLDR());
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::XLDS());
			break;
		}
	}
	/**
	* Append XST instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
	*/
	void append_xst_ins(mlc::icblock_pointer block, mlc::type_pointer tp)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::XSTB());
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::XSTI());
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::XSTR());
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::XSTS());
			break;
		}
	}
	/**
	* Append EQ instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
	*/
	void append_eq_ins(mlc::icblock_pointer block, mlc::type_pointer tp)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::EQB());
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::EQI());
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::EQR());
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::EQS());
			break;
		}
	}
	/**
	* Append GE instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
	*/
	void append_ge_ins(mlc::icblock_pointer block, mlc::type_pointer tp)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::GEB());
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::GEI());
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::GER());
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::GES());
			break;
		}
	}
	/**
	* Append GT instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
	*/
	void append_gt_ins(mlc::icblock_pointer block, mlc::type_pointer tp)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::GTB());
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::GTI());
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::GTR());
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::GTS());
			break;
		}
	}
	/**
	* Append LE instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
	*/
	void append_le_ins(mlc::icblock_pointer block, mlc::type_pointer tp)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::LEB());
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::LEI());
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::LER());
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::LES());
			break;
		}
	}
	/**
	* Append LT instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
	*/
	void append_lt_ins(mlc::icblock_pointer block, mlc::type_pointer tp)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::LTB());
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::LTI());
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::LTR());
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::LTS());
			break;
		}
	}
	/**
	* Append NE instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
	*/
	void append_ne_ins(mlc::icblock_pointer block, mlc::type_pointer tp)
	{
		switch (tp->cat())
		{
		case type_category::TCAT_BOOL:
			block->append_instruction(new ai::NEB());
			break;
		case type_category::TCAT_INT:
			block->append_instruction(new ai::NEI());
			break;
		case type_category::TCAT_REAL:
			block->append_instruction(new ai::NER());
			break;
		case type_category::TCAT_STR:
			block->append_instruction(new ai::NES());
			break;
		}
	}
	/**
	* Append LST or GST instruction for particular variable scope. This function is valid for
	* global, local variable and variable passed by reference. Variable type must be
	* BOOLEAN, INTEGER, REAL or STRING.
	*/
	void append_st_var(mlc::icblock_pointer block, mlc::typed_symbol_pointer tsp)
	{
		switch (tsp->kind())
		{
		case symbol_kind::SKIND_LOCAL_VARIABLE:
			append_lst_ins(block, tsp->type(), tsp->access_local_variable()->address());
			break;
		case symbol_kind::SKIND_PARAMETER_BY_REFERENCE:
			append_lst_ins(block, tsp->type(), tsp->access_parameter_by_reference()->address());
			break;
		case symbol_kind::SKIND_GLOBAL_VARIABLE:
			append_gst_ins(block, tsp->type(), tsp->access_global_variable()->address());
			break;
		default:
			break;
		}
	}
	/**
	* Append LLD or GLD instruction for particular variable scope. This function is valid for
	* global and local variable. Variable type must be BOOLEAN, INTEGER, REAL or STRING.
	*/
	void append_ld_var(mlc::icblock_pointer block, mlc::typed_symbol_pointer tsp)
	{
		switch (tsp->kind())
		{
		case symbol_kind::SKIND_LOCAL_VARIABLE:
			append_lld_ins(block, tsp->type(), tsp->access_local_variable()->address());
			break;
		case symbol_kind::SKIND_GLOBAL_VARIABLE:
			append_gld_ins(block, tsp->type(), tsp->access_global_variable()->address());
			break;
		default:
			break;
		}
	}
	/**
	* Append LREF or GREF instruction for particular variable scope. This function is valid for
	* global and local variable or variable passed by reference. Notice that when referencing
	* variable passed by reference we are actually loading the value of the variable which is
	* the reference (pointer)
	*/
	void append_ref_var(mlc::icblock_pointer block, mlc::typed_symbol_pointer tsp)
	{
		switch (tsp->kind())
		{
		case symbol_kind::SKIND_LOCAL_VARIABLE:
			block->append_instruction(new ai::LREF(tsp->access_local_variable()->address()));
			break;
		case symbol_kind::SKIND_GLOBAL_VARIABLE:
			block->append_instruction(new ai::GREF(tsp->access_global_variable()->address()));
			break;
		case symbol_kind::SKIND_PARAMETER_BY_REFERENCE:
			block->append_instruction(new ai::LLDP(tsp->access_parameter_by_reference()->address()));
			break;
		}
	}

	mlc::symbol_pointer create_var(MlaskalCtx *ctx, const std::string &name, mlc::type_pointer tp)
	{
		auto id = ctx->tab->ls_id().add(name);
		auto sp = ctx->tab->find_symbol(id);
		if (sp->kind() == symbol_kind::SKIND_UNDEF)
			sp = ctx->tab->add_var(0, id, tp);

		return sp;
	}
	/**
	* Append a for loop with fixed number of iterations of given code block. "var" is a pointer to the
	* loop variable which will be used to count the number of iterations.
	*/
	void append_for(MlaskalCtx *ctx, mlc::icblock_pointer out, mlc::icblock_pointer body, mlc::symbol_pointer loop_var, int count)
	{
		auto var = loop_var->access_typed();

		auto l1 = mlc::new_label(ctx);
		auto l2 = mlc::new_label(ctx);

		// 1) INIT: initialize the loop variable with zero
		out->append_instruction(new ai::LDLITI(ctx->tab->ls_int().add(0)));
		append_st_var(out, var);
		// jump at the end of loop
		out->append_instruction_with_target(new ai::JMP(out->end()), l2);

		// 2) BODY:
		out->add_label(l1);
		icblock_append_delete(out, body);
		// end of loop body

		// add 1 to loop variable
		append_ld_var(out, var);
		out->append_instruction(new ai::LDLITI(ctx->tab->one()));
		out->append_instruction(new ai::ADDI());
		append_st_var(out, var);

		out->add_label(l2);

		// 3) CONDITION: for loop condition
		append_ld_var(out, var);
		out->append_instruction(new ai::LDLITI(ctx->tab->ls_int().add(count)));
		out->append_instruction(new ai::EQI());
		out->append_instruction_with_target(new ai::JF(out->end()), l1);
	}
	void append_for(MlaskalCtx *ctx, mlc::icblock_pointer out, mlc::icblock_pointer body, int count)
	{
		// create a variable for the loop
		auto var = create_var(ctx, LOOP_VAR, ctx->tab->logical_integer());
		append_for(ctx, out, body, var, count);
	}

	/**
	* Get type of array element even if the array is multidimensional
	*/
	mlc::type_pointer get_element_type(mlc::type_pointer arr_type)
	{
		while (arr_type->cat() == type_category::TCAT_ARRAY)
		{
			arr_type = arr_type->access_array()->element_type();
		}
		return arr_type;
	}
	/**
	* Get total dimension of an array even if the array is multidimensional. This is a total
	* number of elements that can be stored in the array..
	*/
	int get_array_dimension(mlc::type_pointer arr_type)
	{
		int dim = 1;
		while (arr_type->cat() == type_category::TCAT_ARRAY)
		{
			auto rt = arr_type->access_array()->index_type()->access_range();
			int lb = *rt->lowerBound();
			int ub = *rt->upperBound();

			dim *= (ub - lb + 1);

			arr_type = arr_type->access_array()->element_type();
		}

		return dim;
	}

	void append_ld_array(MlaskalCtx *ctx, mlc::icblock_pointer out, mlc::icblock_pointer src, mlc::type_pointer arr_tp)
	{
		auto loop = icblock_create();
		auto elem_tp = get_element_type(arr_tp);

		// create loop variable
		auto var = create_var(ctx, LOOP_VAR, ctx->tab->logical_integer());

		icblock_append_delete(loop, src);
		append_ld_var(loop, var->access_typed());
		loop->append_instruction(new ai::ADDP());
		append_xld_ins(loop, elem_tp);

		int elem_count = get_array_dimension(arr_tp);
		append_for(ctx, out, loop, var, elem_count);
	}
	void append_st_array(MlaskalCtx *ctx, mlc::icblock_pointer out, mlc::icblock_pointer trg, mlc::icblock_pointer src, mlc::type_pointer arr_tp)
	{
		auto loop = icblock_create();
		auto elem_tp = get_element_type(arr_tp);

		// create loop variable
		auto var = create_var(ctx, LOOP_VAR, ctx->tab->logical_integer());

		// calculate source array address and load its value
		icblock_append_delete(loop, src);
		append_ld_var(loop, var->access_typed());
		loop->append_instruction(new ai::ADDP());
		append_xld_ins(loop, elem_tp);

		// calculate target address and store value
		icblock_append_delete(loop, trg);
		append_ld_var(loop, var->access_typed());
		loop->append_instruction(new ai::ADDP());
		append_xst_ins(loop, elem_tp);

		int elem_count = get_array_dimension(arr_tp);
		append_for(ctx, out, loop, var, elem_count);
	}
	void append_dtor_array(MlaskalCtx *ctx, mlc::icblock_pointer block, mlc::type_pointer arr_type)
	{
		auto body = icblock_create();
		append_dtor_ins(body, get_element_type(arr_type));

		auto var = create_var(ctx, LOOP_VAR, ctx->tab->logical_integer());
		int dim = get_array_dimension(arr_type);
		append_for(ctx, block, body, dim);
	}
	
	/**
	* Create new icblock in the provided MlaskalLval instance of does not exit yet.
	*/
	void create_block_if_empty(MlaskalLval &lval)
	{
		if (lval.code_ == NULL)
			lval.code_ = icblock_create();
	}
	/**
	* Associate program or subprogram (function or procedure) with its code. The association is made
	* with the identifier - program, funtion or procedure name. The code_ field of MlaskalLval instance
	* is cleared.
	*/
	void set_block_code(MlaskalCtx *ctx, MlaskalLval &id, MlaskalLval &code, mlc::block_type type)
	{
		switch (type)
		{
		case mlc::program:
			ctx->tab->set_main_code(id.id_ci_, code.code_);
			break;
		case mlc::subprogram:
			ctx->tab->set_subprogram_code(id.id_ci_, code.code_);
			break;
		}

		// variable is reused and otherwise this is uninitialized
		code.code_ = NULL;
	}
	void append_code_block(MlaskalLval &out, MlaskalLval &in)
	{
		if (out.code_ == NULL)
		{
			create_block_if_empty(in);

			out.code_ = in.code_;
		}
		else if (in.code_ != NULL)
		{
			if (out.code_ != in.code_ && !in.code_->empty())
				icblock_append_delete(out.code_, in.code_);
			in.code_ = NULL;
		}
	}
	/**
	* Append instruction(s) which load value of an identifier. This can be global/local variable identifier,
	* constant identifier or function without parameters identifier. type_ field is assigned with the type
	* value represented by the identifier.
	*/
	void load_identifier(MlaskalCtx *ctx, MlaskalLval &out, int val_line, MlaskalLval &val)
	{
		create_block_if_empty(out);

		auto sp = ctx->tab->find_symbol(val.id_ci_);
		out.type_ = sp->access_typed()->type();

		switch (sp->kind())
		{
		case symbol_kind::SKIND_GLOBAL_VARIABLE:
			append_gld_ins(out.code_, out.type_, sp->access_typed()->access_global_variable()->address());
			break;
		case symbol_kind::SKIND_LOCAL_VARIABLE:
			append_lld_ins(out.code_, out.type_, sp->access_typed()->access_local_variable()->address());
			break;
		case symbol_kind::SKIND_CONST:
			append_ldlit_ins(out.code_, out.type_, sp->access_typed()->access_const());
			break;
		case symbol_kind::SKIND_FUNCTION:	// function without parameters
			subprogram_call(ctx, out, val_line, val, val);
			break;
		case symbol_kind::SKIND_PARAMETER_BY_REFERENCE:
			out.code_->append_instruction(new ai::LLDP(sp->access_typed()->access_parameter_by_reference()->address()));
			append_xld_ins(out.code_, out.type_);
			break;
		default:
			error(DUERR_NOTVAR, val_line, *val.id_ci_);
			break;
		}
	}
	/**
	* Append LDLIT instruction which loads value of a constant used in the program. !!! This does not include
	* the case of constant identifer or function !!!. type_ field is assigned with the type of constant.
	*/
	void load_const_value(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &val, const_type type)
	{
		create_block_if_empty(out);

		switch (type)
		{
		case mlc::boolean:
			// this is probably unnecessary because boolean value can be only specified by an identifier
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
	void load_element(MlaskalCtx *ctx, MlaskalLval &out, int val_line, MlaskalLval &val)
	{
		// caluclate address of the element
		append_code_block(out, val);

		// and load the value at this address
		append_xld_ins(out.code_, val.type_);
	}

	void unary_op(MlaskalCtx *ctx, MlaskalLval &out, int op_line, MlaskalLval &op, MlaskalLval &val)
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
				error(DUERR_SYNTAX, op_line, "'+' operator before non-integral expression");
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
				error(DUERR_SYNTAX, op_line, "'-' operator before non-integral expression");
			}
			break;
		default:
			error(DUERR_SYNTAX, op_line, "unknown unary operator");
			break;
		}
	}
	void unary_not(MlaskalCtx *ctx, MlaskalLval &out, int op_line, MlaskalLval &op, MlaskalLval &val)
	{
		out.code_ = val.code_;
		out.type_ = val.type_;

		if (identical_type(val.type_, ctx->tab->logical_bool()))
		{
			out.code_->append_instruction(new ai::NOT());
		}
		else
		{
			error(DUERR_SYNTAX, op_line, "'not' operator before non-boolean expression");
		}
	}
	void binary_op(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &left, int op_line, MlaskalLval &op, MlaskalLval &right)
	{
		auto real_type = ctx->tab->logical_real();
		auto int_type = ctx->tab->logical_integer();
		auto str_type = ctx->tab->logical_string();
		auto bool_type = ctx->tab->logical_bool();
		auto l_type = left.type_;
		auto r_type = right.type_;

		if (identical_type(l_type, bool_type) && identical_type(r_type, bool_type))
		{	// boolean x boolean -> boolean
			append_code_block(left, right);
			out.code_ = left.code_;

			switch (op.dtge_)
			{
			case DUTOKGE_AND:
				out.code_->append_instruction(new ai::AND());
				break;
			case DUTOKGE_OR:
				out.code_->append_instruction(new ai::OR());
				break;
			case DUTOKGE_LT:
				out.code_->append_instruction(new ai::LTB());
				break;
			case DUTOKGE_LE:
				out.code_->append_instruction(new ai::LEB());
				break;
			case DUTOKGE_NE:
				out.code_->append_instruction(new ai::NEB());
				break;
			case DUTOKGE_GE:
				out.code_->append_instruction(new ai::GEB());
				break;
			case DUTOKGE_GT:
				out.code_->append_instruction(new ai::GTB());
				break;
			case DUTOKGE_EQ:
				out.code_->append_instruction(new ai::EQB());
				break;
			default:
				error(DUERR_SYNTAX, op_line, "unknown binary operator for 'boolean' type");
				break;
			}
			
			out.type_ = bool_type;
		}
		else if (identical_type(l_type, str_type) && identical_type(r_type, str_type))
		{	// string x string -> string/boolean
			append_code_block(left, right);
			out.code_ = left.code_;

			switch (op.dtge_)
			{
			case DUTOKGE_PLUS:
				out.code_->append_instruction(new ai::ADDS());
				out.type_ = str_type;
				break;
			case DUTOKGE_LT:
				out.code_->append_instruction(new ai::LTS());
				out.type_ = bool_type;
				break;
			case DUTOKGE_LE:
				out.code_->append_instruction(new ai::LES());
				out.type_ = bool_type;
				break;
			case DUTOKGE_NE:
				out.code_->append_instruction(new ai::NES());
				out.type_ = bool_type;
				break;
			case DUTOKGE_GE:
				out.code_->append_instruction(new ai::GES());
				out.type_ = bool_type;
				break;
			case DUTOKGE_GT:
				out.code_->append_instruction(new ai::GTS());
				out.type_ = bool_type;
				break;
			case DUTOKGE_EQ:
				out.code_->append_instruction(new ai::EQS());
				out.type_ = bool_type;
				break;
			default:
				error(DUERR_SYNTAX, op_line, "unknown binary operator for 'string' type");
				break;
			}
		}
		else if (identical_type(l_type, int_type) && identical_type(r_type, int_type) &&
			op.dtge_ != DUTOKGE_SOLIDUS)
		{	// integer x integer -> integer/boolean
			append_code_block(left, right);
			out.code_ = left.code_;

			switch (op.dtge_)
			{
			case DUTOKGE_ASTERISK:
				out.code_->append_instruction(new ai::MULI());
				out.type_ = int_type;
				break;
			case DUTOKGE_DIV:
				out.code_->append_instruction(new ai::DIVI());
				out.type_ = int_type;
				break;
			case DUTOKGE_MOD:
				out.code_->append_instruction(new ai::MODI());
				out.type_ = int_type;
				break;
			case DUTOKGE_PLUS:
				out.code_->append_instruction(new ai::ADDI());
				out.type_ = int_type;
				break;
			case DUTOKGE_MINUS:
				out.code_->append_instruction(new ai::SUBI());
				out.type_ = int_type;
				break;
			case DUTOKGE_LT:
				out.code_->append_instruction(new ai::LTI());
				out.type_ = bool_type;
				break;
			case DUTOKGE_LE:
				out.code_->append_instruction(new ai::LEI());
				out.type_ = bool_type;
				break;
			case DUTOKGE_NE:
				out.code_->append_instruction(new ai::NEI());
				out.type_ = bool_type;
				break;
			case DUTOKGE_GE:
				out.code_->append_instruction(new ai::GEI());
				out.type_ = bool_type;
				break;
			case DUTOKGE_GT:
				out.code_->append_instruction(new ai::GTI());
				out.type_ = bool_type;
				break;
			case DUTOKGE_EQ:
				out.code_->append_instruction(new ai::EQI());
				out.type_ = bool_type;
				break;
			default:
				// operator is undefined for given operands - SOLIDUS
				error(DUERR_SYNTAX, op_line, "unknown binary operator for 'integer' type");
				break;
			}
		}
		else if ((identical_type(l_type, real_type) || identical_type(l_type, int_type)) &&
			(identical_type(r_type, real_type) || identical_type(r_type, int_type)))
		{
			// real/integer x real/integer -> real/boolean

			// convert all integers to reals
			if (identical_type(r_type, int_type))
			{	// real x integer
				right.code_->append_instruction(new ai::CVRTIR());
			}
			if (identical_type(l_type, int_type))
			{	// integer x real
				left.code_->append_instruction(new ai::CVRTIR());
			}

			append_code_block(left, right);
			out.code_ = left.code_;

			switch (op.dtge_)
			{
			case DUTOKGE_ASTERISK:
				out.code_->append_instruction(new ai::MULR());
				out.type_ = real_type;
				break;
			case DUTOKGE_SOLIDUS:
				out.code_->append_instruction(new ai::DIVR());
				out.type_ = real_type;
				break;
			case DUTOKGE_PLUS:
				out.code_->append_instruction(new ai::ADDR());
				out.type_ = real_type;
				break;
			case DUTOKGE_MINUS:
				out.code_->append_instruction(new ai::SUBR());
				out.type_ = real_type;
				break;
			case DUTOKGE_LT:
				out.code_->append_instruction(new ai::LTR());
				out.type_ = bool_type;
				break;
			case DUTOKGE_LE:
				out.code_->append_instruction(new ai::LER());
				out.type_ = bool_type;
				break;
			case DUTOKGE_NE:
				out.code_->append_instruction(new ai::NER());
				out.type_ = bool_type;
				break;
			case DUTOKGE_GE:
				out.code_->append_instruction(new ai::GER());
				out.type_ = bool_type;
				break;
			case DUTOKGE_GT:
				out.code_->append_instruction(new ai::GTR());
				out.type_ = bool_type;
				break;
			case DUTOKGE_EQ:
				out.code_->append_instruction(new ai::EQR());
				out.type_ = bool_type;
				break;
			default:
				// operator is undefined for given operands - DIV, MOD
				error(DUERR_CANNOTCONVERT, op_line);
				break;
			}
		}
		else
		{
			error(DUERR_SYNTAX, op_line, "unknown binary operator");
		}
	}

	void store_conversion(mlc::icblock_pointer code, int line, mlc::type_pointer lt, mlc::type_pointer rt)
	{
		if (lt->cat() == type_category::TCAT_INT && rt->cat() == type_category::TCAT_REAL)
		{	// conversion from REAL to INTEGER
			code->append_instruction(new ai::CVRTRI());
			error(DUERR_CONVERSION, line);
		}
		else if (lt->cat() == type_category::TCAT_REAL && rt->cat() == type_category::TCAT_INT)
		{	// conversion from INTEGER to REAL - no warnning !!!
			code->append_instruction(new ai::CVRTIR());
			// error(DUERR_CONVERSION, line);
		}
		else if (!identical_type(lt, rt))
		{	// conversion is not allowed
			error(DUERR_CANNOTCONVERT, line);
		}
	}
	void store_identifier(MlaskalCtx *ctx, MlaskalLval &out, int id_line, MlaskalLval &id, MlaskalLval &expr)
	{
		/* --> variable, function identifier */
		// find assigned symbol
		auto sp = ctx->tab->find_symbol(id.id_ci_);
		// find out type of the assigned symbol
		id.type_ = sp->access_typed()->type();

		if (id.type_->cat() == type_category::TCAT_ARRAY)
		{
			create_block_if_empty(id);
			append_ref_var(id.code_, sp->access_typed());
			store_array(ctx, out, id_line, id, expr);
			return;
		}

		out.code_ = expr.code_;

		// possible conversion in the assignment
		// it is also checked whether assignment is possible
		store_conversion(out.code_, id_line, id.type_, expr.type_);

		switch (sp->kind())
		{
		case symbol_kind::SKIND_GLOBAL_VARIABLE:
			append_gst_ins(out.code_, id.type_, sp->access_typed()->access_global_variable()->address());
			break;
		case symbol_kind::SKIND_LOCAL_VARIABLE:
			append_lst_ins(out.code_, id.type_, sp->access_typed()->access_local_variable()->address());
			break;
		case symbol_kind::SKIND_PARAMETER_BY_REFERENCE:
			// load the address ...
			out.code_->append_instruction(new ai::LLDP(sp->access_typed()->access_parameter_by_reference()->address()));
			// ... and then store to that address
			append_xst_ins(out.code_, id.type_);
			break;
		case symbol_kind::SKIND_FUNCTION:
			if (ctx->tab->nested() && ctx->tab->my_function_name() == id.id_ci_)
			{
				append_lst_ins(out.code_, id.type_, ctx->tab->my_return_address());
				break;
			}
			// else go to default branch
		default:
			error(DUERR_NOTVAR, id_line, *id.id_ci_);
			break;
		}
	}
	
	void store_array(MlaskalCtx *ctx, MlaskalLval &out, int arr_line, MlaskalLval &arr, MlaskalLval &expr)
	{
		out.code_ = icblock_create();

		if (expr.type_->cat() == type_category::TCAT_ARRAY)
		{	// store complex array element - whole array

			// just check whether both array are the same
			store_conversion(out.code_, arr_line, arr.type_, expr.type_);

			append_st_array(ctx, out.code_, arr.code_, expr.code_, expr.type_);
		}
		else
		{	// store single array element

			// first of all we calculate the value to be stored in the array
			append_code_block(out, expr);

			// then we convert calculate value to the array element type
			// it is also checked whether assignment is possible
			store_conversion(out.code_, arr_line, arr.type_, expr.type_);

			// then calculate the address of the array element
			append_code_block(out, arr);

			// and store the value at this address
			append_xst_ins(out.code_, arr.type_);
		}
	}
	/**
	* Create code for accessing particular array element such as arr[1,2][one]
	*/
	void array_element(MlaskalCtx *ctx, MlaskalLval &out, int id_line, MlaskalLval &id, MlaskalLval &idxs)
	{
		create_block_if_empty(out);

		auto sp = ctx->tab->find_symbol(id.id_ci_);
		auto tp = sp->access_typed()->type();
		if (tp->cat() != type_category::TCAT_ARRAY)
		{	// indexing a non-array variable
			error(DUERR_NOTARRAY, id_line, *id.id_ci_);
			return;
		}

		for (auto expr : idxs.exprs_)
		{
			if (out.code_->empty())
			{	// access array beginning address

				if (sp->kind() != symbol_kind::SKIND_GLOBAL_VARIABLE && sp->kind() != symbol_kind::SKIND_LOCAL_VARIABLE
					&& sp->kind() != symbol_kind::SKIND_PARAMETER_BY_REFERENCE)
				{
					error(DUERR_NOTVAR, id_line, *id.id_ci_);
					return;
				}
				append_ref_var(out.code_, sp->access_typed());

				// calculate index expression
				icblock_append_delete(out.code_, expr);

				// substract lower bound
				auto rt = tp->access_array()->index_type();
				auto lb = rt->access_range()->lowerBound();
				out.code_->append_instruction(new ai::LDLITI(lb));
				out.code_->append_instruction(new ai::SUBI());
			}
			else
			{
				// check whether current tp is array, otherwise we are indexing a non-array variable
				if (tp->cat() != type_category::TCAT_ARRAY)
				{
					error(DUERR_NOTARRAY, id_line, *id.id_ci_);
					return;
				}

				tp = tp->access_array()->element_type();
				auto rt = tp->access_array()->index_type();

				// multiply previous value by range size
				auto lb = rt->access_range()->lowerBound();
				auto ub = rt->access_range()->upperBound();
				// (ub - lb + 1)
				auto dim = ctx->tab->ls_int().add(*ub - *lb + 1);
				out.code_->append_instruction(new ai::LDLITI(dim));

				out.code_->append_instruction(new ai::MULI());

				// calculate next index expression
				icblock_append_delete(out.code_, expr);
				out.code_->append_instruction(new ai::LDLITI(lb));
				out.code_->append_instruction(new ai::SUBI());

				// and add to the previous result
				out.code_->append_instruction(new ai::ADDI());
			}
		}

		tp = tp->access_array()->element_type();
		out.type_ = tp;
		idxs.exprs_.clear();

		if (tp->cat() == type_category::TCAT_ARRAY)
		{
			int dim = get_array_dimension(tp);
			out.code_->append_instruction(new ai::LDLITI(ctx->tab->ls_int().add(dim)));
			out.code_->append_instruction(new ai::MULI());
		}

		// add index to the array begin pointer
		out.code_->append_instruction(new ai::ADDP());
	}
	/**
	* Store code for array indexer which will be then used to access particular array element.
	*/
	void array_index(MlaskalCtx *ctx, MlaskalLval &out, int expr_line, MlaskalLval &expr)
	{
		if (identical_type(expr.type_, ctx->tab->logical_integer()))
		{
			out.exprs_.push_back(expr.code_);
		}
		else
		{	// indexing with non-integral type
			error(DUERR_CANNOTCONVERT, expr_line);
		}
	}

	void subprogram_call(MlaskalCtx *ctx, MlaskalLval &out, int id_line, MlaskalLval &id, MlaskalLval &real_params)
	{
		create_block_if_empty(out);

		auto sp = ctx->tab->find_symbol(id.id_ci_);

		if (sp->kind() == symbol_kind::SKIND_PROCEDURE || sp->kind() == symbol_kind::SKIND_FUNCTION)
		{
			if (sp->kind() == symbol_kind::SKIND_FUNCTION)
			{
				// prepare space for return type
				out.type_ = sp->access_typed()->type();
				append_init_ins(out.code_, out.type_);
			}

			// this is just a fix - code for real params must not be appended at
			// any other place
			real_params.code_ = NULL;

			auto params = sp->access_subprogram()->parameters();
			if (real_params.exprs_.size() != params->size())
			{
				error(DUERR_PARNUM, id_line, *id.id_ci_);
			}
			else
			{
				auto ex = real_params.exprs_.begin();
				auto id = real_params.identifiers_.begin();
				for (std::size_t i = 0; i < params->size(); ++i)
				{
					auto param = params->begin() + i;
					if (param->partype == parameter_mode::PMODE_BY_VALUE)
					{	// evaluate subprogram argument passed by value
						if (param->ltype->cat() == type_category::TCAT_ARRAY)
						{	// load array by value
							append_ld_array(ctx, out.code_, ex[i], param->ltype);
						}
						else
						{	// evaluate (or load) expression value
							icblock_append_delete(out.code_, ex[i]);
						}
					}
					else if (param->partype == parameter_mode::PMODE_BY_REFERENCE)
					{	// get variable address passed by reference

						// TODO: rafactoring is needed

						if (param->ltype->cat() == type_category::TCAT_ARRAY)
						{	// evaluate array element address
							icblock_append_delete(out.code_, ex[i]);
						}
						else
						{
							if (id[i] != param->idx) // TODO: how to solve these || ex[i]->size() != 1
							{
								error(DUERR_NOTPARAMVAR, id_line);

								// evaluate subprogram argument passed by value
								icblock_append_delete(out.code_, ex[i]);
							}
							else
							{
								auto sp = ctx->tab->find_symbol(id[i]);
								if (sp->kind() == symbol_kind::SKIND_PARAMETER_BY_REFERENCE)
								{
									if (ex[i]->size() != 2)
									{
										error(DUERR_NOTPARAMVAR, id_line);

										icblock_append_delete(out.code_, ex[i]);
									}
									else
									{
										out.code_->append_instruction(new ai::LLDP(sp->access_typed()->access_parameter_by_reference()->address()));
									}
								}
								else
								{
									if (ex[i]->size() != 1)
									{
										error(DUERR_NOTPARAMVAR, id_line);

										icblock_append_delete(out.code_, ex[i]);
									}
									else
									{
										append_ref_var(out.code_, sp->access_typed());
									}
								}
							}
						}
					}
				}

				// call subprogram
				out.code_->append_instruction(new ai::CALL(sp->access_subprogram()->code()));

				// dispose variables used by the subprogram in reverse order
				std::vector<mlc::parameter_entry> params_wrap(params->begin(), params->end());
				auto it = real_params.identifiers_.rbegin();
				for (auto param = params_wrap.rbegin(); param != params_wrap.rend(); ++param, ++it)
				{
					if (param->partype == parameter_mode::PMODE_BY_VALUE)
					{
						if (param->ltype->cat() == type_category::TCAT_ARRAY)
						{	// destroy array passed by value
							append_dtor_array(ctx, out.code_, param->ltype);
						}
						else
						{	// destroy single expression value
							append_dtor_ins(out.code_, param->ltype);
						}
					}
					else if (param->partype == parameter_mode::PMODE_BY_REFERENCE)
					{
						out.code_->append_instruction(new ai::DTORP());
					}
				}
			}
		}
		else
		{
			// trying to call variable, constant or other identifier
			error(DUERR_NOTPROC, id_line, *id.id_ci_);
		}

		//delete real_params.exprs_;
		real_params.exprs_.clear();
	}

	void label_target(MlaskalCtx *ctx, MlaskalLval &out, int label_line, MlaskalLval &label)
	{
		create_block_if_empty(out);

		auto lb = ctx->tab->find_label(label.int_ci_);
		if (!lb == false)
		{
			out.code_->add_label(lb->label());
			lb->label_found(label_line);
		}
		else
		{	// label number that is not a label
			error(DUERR_NOTLABEL, label_line, *label.int_ci_);
		}
	}
	void label_goto(MlaskalCtx *ctx, MlaskalLval &out, int label_line, MlaskalLval &label)
	{
		create_block_if_empty(out);

		auto lb = ctx->tab->find_label(label.int_ci_);
		if (!lb == false)
		{
			out.code_->append_instruction_with_target(new ai::JMP(out.code_->end()), lb->label());
			lb->goto_found(label_line);
		}
		else
		{	// goto number that is not a label
			error(DUERR_NOTLABEL, label_line, *label.int_ci_);
		}
	}

	bool check_boolean_expr(MlaskalCtx *ctx, int expr_line, MlaskalLval &expr)
	{
		if (!identical_type(expr.type_, ctx->tab->logical_bool()))
		{	// non-boolean expression
			error(DUERR_CANNOTCONVERT, expr_line);
			return false;
		}

		return true;
	}
	void if_else_stmt(MlaskalCtx *ctx, MlaskalLval &out, int expr_line, MlaskalLval &expr, MlaskalLval &stmt1, MlaskalLval &stmt2)
	{
		if (check_boolean_expr(ctx, expr_line, expr))
		{
			// evaluate the condition
			append_code_block(out, expr);
			auto l1 = mlc::new_label(ctx);
			// conditionaly jump to the else branch
			out.code_->append_instruction_with_target(new ai::JF(out.code_->end()), l1);

			// then branch with jump behind the else branch
			append_code_block(out, stmt1);
			auto l2 = mlc::new_label(ctx);
			out.code_->append_instruction_with_target(new ai::JMP(out.code_->end()), l2);

			// jump here when if expression is false
			out.code_->add_label(l1);

			append_code_block(out, stmt2);

			// jump here when if expression is false and after the then statement is executed
			out.code_->add_label(l2);
		}
	}
	void if_stmt(MlaskalCtx *ctx, MlaskalLval &out, int expr_line, MlaskalLval &expr, MlaskalLval &stmt)
	{
		if (check_boolean_expr(ctx, expr_line, expr))
		{
			// evaluate the condition
			append_code_block(out, expr);
			auto l1 = mlc::new_label(ctx);
			// conditionaly jump behind the than branch
			out.code_->append_instruction_with_target(new ai::JF(out.code_->end()), l1);

			// then branch statement
			append_code_block(out, stmt);

			// jump here when if expression is false
			out.code_->add_label(l1);
		}
	}
	void while_stmt(MlaskalCtx *ctx, MlaskalLval &out, int expr_line, MlaskalLval &expr, MlaskalLval &stmt)
	{
		if (check_boolean_expr(ctx, expr_line, expr))
		{
			create_block_if_empty(out);

			auto l1 = mlc::new_label(ctx);	// before loop body - repeat loop
			auto l2 = mlc::new_label(ctx);	// behind loop body - condition evaluation

			// jump behind the loop body
			out.code_->append_instruction_with_target(new ai::JMP(out.code_->end()), l2);
			
			// target for the looping
			out.code_->add_label(l1);

			// append loop body statement
			append_code_block(out, stmt);

			out.code_->add_label(l2);

			// evaluate the condition
			append_code_block(out, expr);
			// and jump back before the loop body
			out.code_->append_instruction_with_target(new ai::JT(out.code_->end()), l1);
		}
	}
	void repeat_stmt(MlaskalCtx *ctx, MlaskalLval &out, int expr_line, MlaskalLval &expr, MlaskalLval &stmt)
	{
		if (check_boolean_expr(ctx, expr_line, expr))
		{
			create_block_if_empty(out);

			auto l1 = mlc::new_label(ctx);
			// target for the looping
			out.code_->add_label(l1);

			// append loop body statement - execute directly
			append_code_block(out, stmt);

			// evaluate the condition
			append_code_block(out, expr);

			// jump back at the loop beginning if condition is false
			out.code_->append_instruction_with_target(new ai::JF(out.code_->end()), l1);
		}
	}
	void for_stmt(MlaskalCtx *ctx, MlaskalLval &out, int id_line, MlaskalLval &id, MlaskalLval &init, MlaskalLval &dir, MlaskalLval &end, MlaskalLval &stmt)
	{
		auto sp = ctx->tab->find_symbol(id.id_ci_);
		auto tsp = sp->access_typed();

		if (sp->kind() != symbol_kind::SKIND_GLOBAL_VARIABLE && sp->kind() != symbol_kind::SKIND_LOCAL_VARIABLE
			&& sp->kind() != symbol_kind::SKIND_PARAMETER_BY_REFERENCE)
		{	// identifier is not a variable, it is either undefined or a function, constant or procedure identifier
			error(DUERR_FORNOTLOCAL, id_line, *id.id_ci_);
		}
		else if (tsp->access_typed()->type()->cat() != type_category::TCAT_INT)
		{
			error(DUERR_FORNOTINTEGER, id_line, *id.id_ci_);
		}
		else
		{
			create_block_if_empty(out);

			auto l1 = mlc::new_label(ctx);		// loop repeat - before loop body
			auto l2 = mlc::new_label(ctx);		// loop break - after loop body

			// evaluate the initial expression
			append_code_block(out, init);	// execute initialization of loop variable
			// TODO: add possible conversion from real
			append_st_var(out.code_, tsp);	// store expression value to the loop variable

			// jump at the loop end before the break condition - this jump results in possible skipping of loop body
			out.code_->append_instruction_with_target(new ai::JMP(out.code_->end()), l2);

			// next execution jumps here to run loop body again
			out.code_->add_label(l1);

			// append loop body
			append_code_block(out, stmt);

			// increment or decrement loop variable by one
			append_ld_var(out.code_, tsp);
			out.code_->append_instruction(new ai::LDLITI(ctx->tab->one()));
			if (dir.dtge_ == DUTOKGE_TO)
				out.code_->append_instruction(new ai::ADDI());
			else if (dir.dtge_ == DUTOKGE_DOWNTO)
				out.code_->append_instruction(new ai::SUBI());
			append_st_var(out.code_, tsp);

			// first execution jumps here to evaluate the break condition only, but not
			// the increment or decrement
			out.code_->add_label(l2);

			// evaluate the break conditions
			append_code_block(out, end);		// evaluate end expression
			append_ld_var(out.code_, tsp);		// load loop variable
			if (dir.dtge_ == DUTOKGE_TO)
				out.code_->append_instruction(new ai::GEI());	// compare loop variable and the end expression result
			else if (dir.dtge_ == DUTOKGE_DOWNTO)
				out.code_->append_instruction(new ai::LEI());
			out.code_->append_instruction_with_target(new ai::JT(out.code_->end()), l1);
		}
	}
	/*
	DUERR_SEMERROR = 6000,
	DUERR_DUPSYM,
	DUERR_DUPLABEL,
	DUERR_NOTTYPE,
	DUERR_NOTORDINAL,
	DUERR_NOTLABEL,
	DUERR_NOTMINE,
	DUERR_NOTLVALUE,
	DUERR_NOTPARAMVAR,
	DUERR_TYPEMISMATCH,
	DUERR_NOTPROC,
	.DUERR_NOTARRAY,
	.DUERR_NOTRECORD,
	DUERR_NOTSCALAR,
	DUERR_PARNUM,
	DUERR_NOTVAR,
	DUERR_NOTFNC,
	DUERR_NOTCONST,
	DUERR_NOTINTEGRAL,
	.DUERR_CANNOTCONVERT,
	.DUERR_FORNOTLOCAL,
	.DUERR_FORNOTINTEGER,
	.DUERR_BADRANGE,
	.DUERR_DUPLABELDEF,
	.DUERR_MISLABELDEF,
	DUERR_NOTFIELD,
	*/
};

/*****************************************/