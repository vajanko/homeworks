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
	/*type_pointer get_symbol_type(MlaskalCtx *ctx, mlc::ls_id_type::const_pointer id)
	{
		auto sp = ctx->tab->find_symbol(id);
		return sp->access_typed()->type();
	}*/

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
		}
	}
	/**
	* Append GLD instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
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
		}
	}
	/**
	* Append LLD instruction for particular type at the end of given block. This function is valid
	* only for BOOLEAN, INTEGER, REAL and STRING.
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
		case symbol_kind::SKIND_FUNCTION:
			// function without parameters
			subprogram_call(ctx, out, val_line, val, val);
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
		auto l_type = left.type_;
		auto r_type = right.type_;

		// TODO: boolean x boolean -> boolean

		if (identical_type(l_type, str_type) && identical_type(r_type, str_type))
		{	// string x string -> string
			append_code_block(left, right);
			out.code_ = left.code_;

			switch (op.dtge_)
			{
			case DUTOKGE_PLUS:
				out.code_->append_instruction(new ai::ADDS());
				break;
			default:
				error(DUERR_SYNTAX, op_line, "unknown binary operator for 'string' type");
				break;
			}

			out.type_ = str_type;
		}
		else if (identical_type(l_type, int_type) && identical_type(r_type, int_type) &&
			op.dtge_ != DUTOKGE_SOLIDUS)
		{	// integer x integer -> integer
			append_code_block(left, right);
			out.code_ = left.code_;

			switch (op.dtge_)
			{
			case DUTOKGE_ASTERISK:
				out.code_->append_instruction(new ai::MULI());
				break;
			case DUTOKGE_DIV:
				out.code_->append_instruction(new ai::DIVI());
				break;
			case DUTOKGE_MOD:
				out.code_->append_instruction(new ai::MODI());
				break;
			case DUTOKGE_PLUS:
				out.code_->append_instruction(new ai::ADDI());
				break;
			case DUTOKGE_MINUS:
				out.code_->append_instruction(new ai::SUBI());
				break;
			default:
				// operator is undefined for given operands - SOLIDUS
				error(DUERR_SYNTAX, op_line, "unknown binary operator for 'integer' type");
				break;
			}

			out.type_ = int_type;
		}
		else if ((identical_type(l_type, real_type) || identical_type(l_type, int_type)) &&
			(identical_type(r_type, real_type) || identical_type(r_type, int_type)))
		{
			// real/integer x real/integer -> real

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
				break;
			case DUTOKGE_SOLIDUS:
				out.code_->append_instruction(new ai::DIVR());
				break;
			case DUTOKGE_PLUS:
				out.code_->append_instruction(new ai::ADDR());
				break;
			case DUTOKGE_MINUS:
				out.code_->append_instruction(new ai::SUBR());
				break;
			default:
				// operator is undefined for given operands - DIV, MOD
				error(DUERR_CANNOTCONVERT, op_line);
				break;
			}

			out.type_ = real_type;
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
		//icblock_pointer block = expr.code_;
		out.code_ = expr.code_;

		/* --> variable, function identifier */
		// find assigned symbol
		auto sp = ctx->tab->find_symbol(id.id_ci_);
		// find out type of the assigned symbol
		id.type_ = sp->access_typed()->type();
			
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
	void store_element(MlaskalCtx *ctx, MlaskalLval &out, int arr_line, MlaskalLval &arr, MlaskalLval &expr)
	{
		out.code_ = icblock_create();
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
	/**
	 * Create code for accessing particualr array element such as arr[1,2][one]
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

		// get all ranges types
		while (tp->cat() == type_category::TCAT_ARRAY)
		{
			out.ranges_.push_back(tp);
			tp = tp->access_array()->element_type();
		}
		out.ranges_.push_back(tp);	// array single element type

		for (auto expr : *idxs.exprs_)
		{
			if (out.code_->empty())
			{
				// access array beginning address
				switch (sp->kind())
				{
				case symbol_kind::SKIND_GLOBAL_VARIABLE:
					out.code_->append_instruction(new ai::GREF(sp->access_typed()->access_global_variable()->address()));
					break;
				case symbol_kind::SKIND_LOCAL_VARIABLE:
					out.code_->append_instruction(new ai::LREF(sp->access_typed()->access_local_variable()->address()));
					break;
				default:
					error(DUERR_NOTVAR, id_line, *id.id_ci_);
					return;
				}

				// calculate index expression
				icblock_append_delete(out.code_, expr);
				
				// substract lower bound
				tp = out.ranges_.front();
				auto rt = tp->access_array()->index_type();
				auto lb = rt->access_range()->lowerBound();
				out.code_->append_instruction(new ai::LDLITI(lb));
				out.code_->append_instruction(new ai::SUBI());
			}
			else
			{
				tp = out.ranges_.front();
				auto rt = tp->access_array()->index_type();

				// multiply previous value by range size
				auto lb = rt->access_range()->lowerBound();
				auto ub = rt->access_range()->upperBound();
				// (ub - lb + 1)
				out.code_->append_instruction(new ai::LDLITI(ub));
				out.code_->append_instruction(new ai::LDLITI(lb));
				out.code_->append_instruction(new ai::SUBI());
				out.code_->append_instruction(new ai::LDLITI(ctx->tab->ls_int().add(1)));
				out.code_->append_instruction(new ai::ADDI());

				out.code_->append_instruction(new ai::MULI());

				// calculate next index expression
				icblock_append_delete(out.code_, expr);
				out.code_->append_instruction(new ai::LDLITI(lb));
				out.code_->append_instruction(new ai::SUBI());

				// and add to the previous result
				out.code_->append_instruction(new ai::ADDI());

				// remove type at the beginning
				//out.ranges_.erase(out.ranges_.begin());
			}

			// remove type at the beginning
			out.ranges_.erase(out.ranges_.begin());
		}
		
		out.type_ = out.ranges_.front();

		delete idxs.exprs_;
		idxs.exprs_ = NULL;

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
			if (out.exprs_ == NULL)
				out.exprs_ = new icblock_list();
			out.exprs_->push_back(expr.code_);
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
				out.type_ = sp->access_typed()->type();
				// allocate space for return type
				append_init_ins(out.code_, out.type_);
			}

			// evaluate subprogram arguments if any
			if (real_params.code_ != NULL)
				append_code_block(out, real_params);

			// call subprogram
			out.code_->append_instruction(new ai::CALL(sp->access_subprogram()->code()));
			
			// dispose variables used by the subprogram in reverse order
			auto params = sp->access_subprogram()->parameters();
			std::vector<mlc::parameter_entry> params_wrap(params->begin(), params->end());
			for (auto param = params_wrap.rbegin(); param != params_wrap.rend(); ++param)
			{
				if (param->partype == parameter_mode::PMODE_BY_VALUE)
				{
					append_dtor_ins(out.code_, param->ltype);
				}
				else if (param->partype == parameter_mode::PMODE_BY_REFERENCE)
				{
					// TODO: du6
				}
			}
		}
		else
		{
			// trying to call variable, constant or other identifier
			error(DUERR_NOTPROC, id_line, *id.id_ci_);
		}
	}
};

/*****************************************/