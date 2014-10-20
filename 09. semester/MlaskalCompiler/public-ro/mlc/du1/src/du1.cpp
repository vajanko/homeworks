/*

	DU1.CPP

	JY

	Main for DU1

*/

#include <cstdio>
#include <map>
#include <utility>
#include <algorithm>

#include "du1.h"
#include "du1tok.h"
#include "duerr.h"

using namespace std;
using namespace mlc;
using namespace mlaskal;

extern FILE *yyin;

namespace {

	typedef pair<int, const char *> TOKP;
	typedef map<int, const char *> TOKM;
	typedef TOKM::const_iterator TOKCI;

	class ftor_tok_map_insert {
	public:
		ftor_tok_map_insert(TOKM &tm) : tm_(&tm) { }
		void operator()(const DUTOK &dtk) const {
			tm_->insert(TOKP(dtk.tok, dtk.text));
		}
	private:
		TOKM		*tm_;
	};

}

int main(int argc, char **argv)
{
	if(argc<2)
	{
		puts("du1 <input_file>");
		return 16;
	}

	if((yyin=fopen(argv[1], "r"))==0)
	{
		error(DUERR_NOINFILE, 0, argv[1]);
		return 16;
	}

	TOKM tok_map;

	for_each(tokens.begin(), tokens.end(), ftor_tok_map_insert(tok_map));

	int tok;

	while((tok=yylex())!=0)
	{
		TOKCI tci;
		if((tci=tok_map.find(tok))!=tok_map.end())
			printf("%s\n", tci->second);
		else
			printf("Unknown token %d\n", tok);
	}

	fclose(yyin);

	if(n_errors)
		error(DUERR_ERRS, 0, n_errors, n_warnings);
	else if(n_warnings)
		error(DUERR_NOK, 0, n_warnings);
	else
		error(DUERR_OK, 0);

	return 0;
}
