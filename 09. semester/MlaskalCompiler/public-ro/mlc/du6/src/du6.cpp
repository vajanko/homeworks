/*

	DU6.CPP

	DB

	Main for DU6

*/

#include <cstdio>
#include <fstream>

#include "du6.h"
#include "du6tok.h"
#include "duerr.h"
#include "dutables.h"
#include "flat_icblock.hpp"
#include "labeled_icblock.hpp"
#include "ai_factory.hpp"

#if defined( DEBUG_MEMORY_LEAKS) && defined( _MSC_VER) && defined( _DEBUG)
#define _AFXDLL
#include "afx.h"
#endif

using namespace std;
using namespace mlc;
using namespace mlaskal;

extern FILE *yyin;

namespace mlc {

	int symtab_preprocess(struct MlaskalCtx *ctx, bool debug)
	{
		ctx->aic = new abstract_ic();
		ctx->tab = new symbol_tables( debug, ctx->aic);

		ctx->tab->init_builtins();

		return 0;
	}

	int symtab_postprocess(struct MlaskalCtx *ctx, const std::string & dump_fname)
	{
		if ( ! dump_fname.empty() )
		{
			std::ofstream out( dump_fname.c_str());
			if ( out.fail() )
			{
				error( DUERR_NOOUTFILE, 0, dump_fname);
			}
			else
			{
				ctx->tab->dump( out);
			}
		}

		delete ctx->tab;
		ctx->tab = 0;
		delete ctx->aic;
		ctx->aic = 0;

		return 0;
	}
}

#if defined( _MSC_VER)
void myInvalidParameterHandler(const wchar_t* expression,
   const wchar_t* function, 
   const wchar_t* file, 
   unsigned int line, 
   uintptr_t pReserved)
{
   wprintf(L"Invalid parameter detected in function %s."
			L" File: %s Line: %d\n", function, file, line);
   wprintf(L"Expression: %s\n", expression);
   assert( 0);
}
#endif

int main(int argc, char **argv)
{
string asm_file;
bool debug_symtab=false;
string dump_fname;

	if(argc<3)
	{
		cout << "Usage: du6 <input_file> <output_file> [options]\n";
		cout << "Options:\n";
		cout << "\t-S<asm_file>\tassembler listing\n";
		cout << "\t-B\t\tdebug symbol tables\n";
		cout << "\t-D<xml_file>\t\tdump symbol tables canonically\n";
		return 16;
	}

	for(int i=3;i<argc;++i)
		if(argv[i][0]=='/' || argv[i][0]=='-')
			switch(argv[i][1])
			{
			case 'D':
				dump_fname = argv[i]+2;
				break;
			case 'S':
				asm_file = argv[i]+2;
				break;
			case 'B':
				debug_symtab = true;
				break;
			}
		else
			cout << "Unknown parameter \"" << argv[i] << "\"\n";

	if((yyin=fopen(argv[1], "r"))==0)
	{
		error(DUERR_NOINFILE, 0, argv[1]);
		return 16;
	}

#if defined( _MSC_VER)
		{
				int rv1 = _set_error_mode(_REPORT_ERRMODE);
				int rv2 = _set_error_mode(_OUT_TO_STDERR);
				int rv3 = _set_error_mode(_REPORT_ERRMODE);
				//std::cerr << "Error mode switched: " << rv1 << "," << rv2 << "," << rv3 << std::endl;
		}
		{
				int rv1 = _CrtSetReportMode( _CRT_WARN, _CRTDBG_REPORT_MODE);
				int rv2 = _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE);
				int rv3 = _CrtSetReportMode( _CRT_WARN, _CRTDBG_REPORT_MODE);
				//std::cerr << "_CRT_WARN mode switched: " << rv1 << "," << rv2 << "," << rv3 << std::endl;
		}
		{
				_HFILE rv1 = _CrtSetReportFile( _CRT_WARN, _CRTDBG_REPORT_FILE );
				_HFILE rv2 = _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
				_HFILE rv3 = _CrtSetReportFile( _CRT_WARN, _CRTDBG_REPORT_FILE );
				//std::cerr << "_CRT_WARN file switched: " << rv1 << "," << rv2 << "," << rv3 << std::endl;
		}
		{
				int rv1 = _CrtSetReportMode( _CRT_ERROR, _CRTDBG_REPORT_MODE);
				int rv2 = _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE);
				int rv3 = _CrtSetReportMode( _CRT_ERROR, _CRTDBG_REPORT_MODE);
				//std::cerr << "_CRT_ERROR mode switched: " << rv1 << "," << rv2 << "," << rv3 << std::endl;
		}
		{
				_HFILE rv1 = _CrtSetReportFile( _CRT_ERROR, _CRTDBG_REPORT_FILE );
				_HFILE rv2 = _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
				_HFILE rv3 = _CrtSetReportFile( _CRT_ERROR, _CRTDBG_REPORT_FILE );
				//std::cerr << "_CRT_ERROR file switched: " << rv1 << "," << rv2 << "," << rv3 << std::endl;
		}
		{
				int rv1 = _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_REPORT_MODE);
				int rv2 = _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE);
				int rv3 = _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_REPORT_MODE);
				//std::cerr << "_CRT_ASSERT mode switched: " << rv1 << "," << rv2 << "," << rv3 << std::endl;
		}
		{
				_HFILE rv1 = _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_REPORT_FILE );
				_HFILE rv2 = _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );
				_HFILE rv3 = _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_REPORT_FILE );
				//std::cerr << "_CRT_ASSERT file switched: " << rv1 << "," << rv2 << "," << rv3 << std::endl;
		}
		{
		   _invalid_parameter_handler oldHandler, newHandler;
		   newHandler = myInvalidParameterHandler;
		   oldHandler = _set_invalid_parameter_handler(newHandler);
		   //std::cerr << "invalid_parameter_handler switched: " << oldHandler << "," << newHandler << std::endl;
		}
#endif

#if defined( DEBUG_MEMORY_LEAKS) && defined( _MSC_VER) && defined( _DEBUG)
	CMemoryState msOld;
	msOld.Checkpoint();
#endif

	{
		MlaskalCtx ctx;

		symtab_preprocess( & ctx, debug_symtab);

		yyparse(&ctx);

	//	append_library(*ctx.aic);

		flat_icblock ficb(*ctx.aic);

		ficb.save(argv[2]);
		if(!asm_file.empty())
			ficb.save_asm(asm_file);

		symtab_postprocess( & ctx, dump_fname);
	}

#if defined( DEBUG_MEMORY_LEAKS) && defined( _MSC_VER) && defined( _DEBUG)
	msOld.DumpAllObjectsSince();
#endif

	fclose(yyin);

	if(n_errors)
		error(DUERR_ERRS, 0, n_errors, n_warnings);
	else if(n_warnings)
		error(DUERR_NOK, 0, n_warnings);
	else
		error(DUERR_OK, 0);

	return 0;
}
