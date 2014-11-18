/*

	DU3.CPP

	JY

	Main for DU3

*/

#include <cstdio>

#include "du3.h"
#include "du3tok.h"
#include "duerr.h"
//#include "du3g.hpp"

using namespace std;
using namespace mlc;
using namespace mlaskal;

extern FILE *yyin;

int main(int argc, char **argv)
{
	if(argc<2)
	{
		puts("du3 <input_file>");
		return 16;
	}

	if((yyin=fopen(argv[1], "r"))==0)
	{
		error(DUERR_NOINFILE, 0, argv[1]);
		return 16;
	}

	MlaskalCtx ctx;
	fake_symbol_tables fst;
	ctx.tab = &fst;

	yyparse(&ctx);

	fclose(yyin);

	if(n_errors)
		error(DUERR_ERRS, 0, n_errors, n_warnings);
	else if(n_warnings)
		error(DUERR_NOK, 0, n_warnings);
	else
		error(DUERR_OK, 0);

	return 0;
}
