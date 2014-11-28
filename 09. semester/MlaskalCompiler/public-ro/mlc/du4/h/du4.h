/*

DU6.H

JY

Main interface for DU4

*/

#ifndef DU4_H_
#define DU4_H_

#include "dutables.h"
#include "abstract_ic.hpp"

namespace mlc {

	/***************** THE BISON CONTEXT ***************************/

	/// structure for DU6 bison context
	struct MlaskalCtx {
		mlaskal::abstract_ic		*aic;
		mlc::symbol_tables		*tab;
	};

	/// creates the symbol tables
	/** will be called before the parser is started
	
		returns 0, if OK; otherwise !=0 
	**/
	int symtab_preprocess(struct MlaskalCtx *ctx, bool debug);

	/// destroys the symbol tables
	/** will be called after the parser has finished

		returns 0, if OK; otherwise !=0
	**/
	int symtab_postprocess(struct MlaskalCtx *ctx, const std::string & dump_fname);

	/// generates unique intermediate code label
	inline ic_label new_label( MlaskalCtx *)
	{
		return mlaskal::uid_all.get_UID();
	}
};

// parser interface
int yyparse(mlc::MlaskalCtx *ctx);

#endif
