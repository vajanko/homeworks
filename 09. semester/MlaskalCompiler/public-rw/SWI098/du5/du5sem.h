/*

	DU5SEM.H

*/

#ifndef DU5SEM_H_
#define DU5SEM_H_

#include "du5.h"
#include "du5lval.hpp"

namespace mlc {

	/* type of procedure or function parameter */
	enum param_type
	{
		/* parameter is passed by value */
		value,
		/* parameter is passed by reference */
		reference
	};

	/**
	* Assignment of a type to an identifier (name). This method  expects type MlaskalLval to have
	* property set type_ field. This type can be already known or an anonymous type (i.e. array ...)
	*/
	void type_assign(MlaskalCtx *ctx, int name_line, MlaskalLval &name, MlaskalLval &type);
	/**
	* Lookup already existing type in the semantic tables and assign out.type_ field. type is the
	* identifier.
	*/
	void type_declare(MlaskalCtx *ctx, MlaskalLval &out, int type_line, MlaskalLval &type);
	/**
	* The same as type_declare method but it is checked that the resulted type is ordinal (i.e. range)
	*/
	void type_declare_ordinal(MlaskalCtx *ctx, MlaskalLval &out, int type_line, MlaskalLval &type);
	/**
	* Declare a new range. low and high contain the range values. It is checked whether low is lower than high.
	* out.type_ is assign with created range type.
	*/
	void range_declare(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &low, int high_line, MlaskalLval &high);
	/**
	* Add type assigned in in.type_ to out list of range types.
	*/
	void range_add(MlaskalLval &out, MlaskalLval &in);
	/**
	* Declare an array type. range contains a list of array ranges (i.e. array[1..2,3..4] will result
	* in two ranges 1..2 and 3..4. type.type_ is the type of array element. out.type_ is assigned with
	* the created array type.
	*/
	void array_declare(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &range, MlaskalLval &type);

	/**
	* Add declaration of a procedure to the semantic tables. proc is procedure identifier and
	* it is exptected that params has field param_list_ properly assigned with procedure parameters.
	*/
	void procedure_declare(MlaskalCtx *ctx, MlaskalLval &out, int proc_line, MlaskalLval &proc, MlaskalLval &params);
	/**
	* Add declaration of a function to the semantic tables. fnc is function identifier, it is expected that
	* params has field param_list_ properly assigned with function parameters. type is function return type identifier,
	* it must already exist at this point and it must by a scalar type.
	*/
	void function_declare(MlaskalCtx *ctx, MlaskalLval &out, int fnc_line, MlaskalLval &fnc, MlaskalLval &params, int type_line, MlaskalLval &type);
	/**
	* Add one parameter section to the list of parameters stored in out value. ids contains a list of identifiers of the same type
	* and all are passed either by value or by reference. type if parameter section type identifier ant must already exit.
	* pt indicates wheter parameters in this section are passed by value or by reference.
	*/
	void parameter_add(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &ids, int type_line, MlaskalLval &type, param_type pt);
	/**
	* Indicate that procedure or function block has been left.
	* This method must be called when procedure or function declaration is finised (after the body)
	*/
	void block_leave(MlaskalCtx *ctx, int line);
	/**
	* Declare variables of one type. It is expected that type has field type_ properly set with variables type.
	* ids contains a list of variable identifiers.
	*/
	void var_declare(MlaskalCtx *ctx, MlaskalLval &ids, int type_line, MlaskalLval &type);
	/**
	* Add identifier in the in value to the out list of identifiers. This method is used in procedure or function
	* declaration for parameter section with multiple parameters or for variables declaration.
	*/
	void identifier_add(MlaskalLval &out, MlaskalLval &in);
	/**
	* Add a constant to semantic tables. Based of value.const_type_ particular field of MlaskalLval structure is set.
	* If const_type_ is integer than int_ci_ is set to already added number integer in the tables. Name contains
	* constant name. Only constant of boolean, interger, real and string can be defined. Constants based on other
	* constant will already have its value properly calculated in the value structure.
	*/
	void const_declare(MlaskalCtx *ctx, int line, MlaskalLval &name, MlaskalLval &value);
	/**
	* Calculate value of integer or real constant. value contains number, type indicates whether it should be real
	* or integer constant. sig contains the sign + or -. out is assigned with the retult value.
	*/
	void const_calculate(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &sig, MlaskalLval &value, const_type type);
	/**
	* Load value of already declared constant and store it in the out value. If id is not an identifier of a constant
	* out.const_type_ is assigned to const_type::none. Otherwise the const_type_ is assign according to the declared
	* constant type.
	*/
	void const_load(MlaskalCtx *ctx, MlaskalLval &out, int line, MlaskalLval &id);
	/**
	* Shortcut for constant based on other constant (i.e. identifier) which is prefixed by a sign. This method
	* is just a composition of const_load and const_calculate.
	*/
	void const_load_and_calculate(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &sig, int id_line, MlaskalLval &id);

	/* testing method for debug purpose */
	void test(MlaskalCtx *ctx, MlaskalLval &lval);


	/**/

	void set_block_code(MlaskalCtx *ctx, MlaskalLval &id, MlaskalLval &code, mlc::block_type type);
	void append_code_block(MlaskalLval &out, MlaskalLval &in);

	void load_const_value(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &val, mlc::const_type type);
	void load_identifier(MlaskalCtx *ctx, MlaskalLval &out, int val_line, MlaskalLval &val);
	void load_element(MlaskalCtx *ctx, MlaskalLval &out, int val_line, MlaskalLval &val);
	void store_identifier(MlaskalCtx *ctx, MlaskalLval &out, int id_line, MlaskalLval &id, MlaskalLval &expr);

	void store_element(MlaskalCtx *ctx, MlaskalLval &out, int arr_line, MlaskalLval &arr, MlaskalLval &expr);
	void array_element(MlaskalCtx *ctx, MlaskalLval &out, int id_line, MlaskalLval &id, MlaskalLval &idxs);
	void array_index(MlaskalCtx *ctx, MlaskalLval &out, int expr_line, MlaskalLval &expr);

	void unary_op(MlaskalCtx *ctx, MlaskalLval &out, int op_line, MlaskalLval &op, MlaskalLval &val);
	void unary_not(MlaskalCtx *ctx, MlaskalLval &out, int op_line, MlaskalLval &op, MlaskalLval &val);
	void binary_op(MlaskalCtx *ctx, MlaskalLval &out, MlaskalLval &left, int op_line, MlaskalLval &op, MlaskalLval &right);

	void subprogram_call(MlaskalCtx *ctx, MlaskalLval &out, int id_line, MlaskalLval &id, MlaskalLval &real_params);
}

#endif
