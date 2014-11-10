/*

	DU2.CPP

	JY

	Main for DU2

*/

#include <cstdio>
#include <map>
#include <utility>

#include "du2.h"
#include "du2tok.h"
#include "duerr.h"

using namespace std;
using namespace mlc;
using namespace mlaskal;

extern FILE *yyin;

namespace {

	typedef pair<const char *, unsigned> TOKV;
	typedef pair<int, TOKV> TOKP;
	typedef map<int, TOKV> TOKM;
	typedef TOKM::const_iterator TOKCI;

	class ftor_tok_map_insert {
	public:
		ftor_tok_map_insert(TOKM &tm) : tm_(&tm) { }
		void operator()(const DUTOK &dtk) const {
			tm_->insert(TOKP(dtk.tok, TOKV(dtk.text, dtk.flags)));
		}
	private:
		TOKM		*tm_;
	};

	typedef pair<int, const char *> TOKGEP;
	typedef map<int, const char *> TOKGEM;
	typedef TOKGEM::const_iterator TOKGECI;

	class ftor_tokge_map_insert {
	public:
		ftor_tokge_map_insert(TOKGEM &tm) : tm_(&tm) { }
		void operator()(const DUTOKGE &dtk) const {
			tm_->insert(TOKGEP(dtk.tokge, dtk.text));
		}
	private:
		TOKGEM		*tm_;
	};

}

int main(int argc, char **argv)
{
	if(argc<2)
	{
		puts("du2 <input_file>");
		return 16;
	}

	if((yyin=fopen(argv[1], "r"))==0)
	{
		error(DUERR_NOINFILE, 0, argv[1]);
		return 16;
	}

	TOKM tok_map;
	for_each(tokens.begin(), tokens.end(), ftor_tok_map_insert(tok_map));

	TOKGEM tokge_map;
	for_each(group_elements.begin(), group_elements.end(), ftor_tokge_map_insert(tokge_map));

	int tok;
	unsigned line;
	MlaskalLval lv;
	MlaskalCtx ctx;
	fake_symbol_tables fst;
	ctx.tab = &fst;

	while((tok=yylex(&lv, &line, &ctx))!=0)
	{
		TOKCI tci;
		if((tci=tok_map.find(tok))!=tok_map.end())
		{
			printf("%u: %s", line, tci->second.first);
			switch(tci->second.second & DUTOKM_SUBTYPE)
			{
			case DUTOKF_ST_NOTHING:
				break;
			case DUTOKF_ST_HAS_GE:
				{
					TOKGECI tgeci;
					if((tgeci=tokge_map.find(lv.dtge_))!=tokge_map.end())
						printf(" [%s]", tgeci->second);
				}
				break;
			case DUTOKF_ST_LIT_ID:
				printf(" [%s]", lv.id_ci_->c_str());
				break;
			case DUTOKF_ST_LIT_UINT:
				printf(" [%i]", *lv.int_ci_);
				break;
			case DUTOKF_ST_LIT_REAL:
				printf(" [%g]", *lv.real_ci_);
				break;
			case DUTOKF_ST_LIT_STRING:
				printf(" [%s]", lv.str_ci_->c_str());
				break;
			}
			puts("");				// EOL
		}
		else
			printf("%u: Unknown token %d\n", line, tok);
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
