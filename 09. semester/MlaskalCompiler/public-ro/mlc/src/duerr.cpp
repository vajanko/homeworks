/*

	DUERR.CPP

	JY

	Error facility

*/

#include <cstdio>
#include <cstdlib>
#include <utility>
#include <map>
#include <cassert>
#include <algorithm>

#include "duerr.h"

using namespace std;

namespace {

	typedef pair<int, const mlc::errs *> ERRMP;
	typedef map<int, const mlc::errs *> ERRMM;
	typedef ERRMM::const_iterator ERRMCI;

	ERRMM err_map;

	class ftor_err_map_insert {
	public:
		ftor_err_map_insert(ERRMM &em) : em_(&em) { }
		void operator()(const mlc::errs &e) const {
			bool rv = em_->insert(ERRMP(e.errc, &e)).second;
			assert(rv);
		}
	private:
		ERRMM		*em_;
	};


}

namespace mlc {

	int n_warnings, n_errors;

	void error_common(int err, unsigned line)
	{
		if(err_map.empty())		// the map is empty
		{
			for_each(base_errors.begin(), base_errors.end(), ftor_err_map_insert(err_map));
			for_each(sem_errors.begin(), sem_errors.end(), ftor_err_map_insert(err_map));
		}

		if(err>=DUERR_WARNING && err<DUERR_ERROR)
			++n_warnings;
		else if(err>=DUERR_ERROR && err<DUERR_FATAL)
			++n_errors;

		fputs(err>=DUERR_FATAL?"Fatal error":err>=DUERR_ERROR?"Error":err>=DUERR_WARNING?"Warning":"", stdout);
		if(line)
			printf(" (line %u)", line);
		if(err>=DUERR_WARNING)
			fputs(": ", stdout);
	}

	void error(int err, unsigned line)
	{
		error_common( err, line);

		ERRMCI mci = err_map.find(err);

		if(mci!=err_map.end())
		{
			const errs *e = mci->second;
			if ( e->errt == DUERRT_VOID )
				fputs(e->msg, stdout);
			else
				printf("Missing error parameter %d: %s", err, e->msg);
		}
		else
			printf("Unknown error %d", err);

		puts("");

		if(err>=DUERR_FATAL)
			exit(16);
	}

	void error(int err, unsigned line, const char *s)
	{
		error_common( err, line);

		ERRMCI mci = err_map.find(err);

		if(mci!=err_map.end())
		{
			const errs * e = mci->second;
			if ( e->errt == DUERRT_S )
				printf(e->msg, s);
			else
				printf("Missing error parameter %d: %s", err, e->msg);
		}
		else
			printf("Unknown error %d", err);

		puts("");

		if(err>=DUERR_FATAL)
			exit(16);
	}

	void error(int err, unsigned line, char c, int i)
	{
		error_common( err, line);

		ERRMCI mci = err_map.find(err);

		if(mci!=err_map.end())
		{
			const errs *e = mci->second;
			if ( e->errt == DUERRT_CI )
				printf(e->msg, c, i);
			else
				printf("Missing error parameter %d: %s", err, e->msg);
		}
		else
			printf("Unknown error %d", err);

		puts("");

		if(err>=DUERR_FATAL)
			exit(16);
	}

	void error(int err, unsigned line, int i)
	{
		error_common( err, line);

		ERRMCI mci = err_map.find(err);

		if(mci!=err_map.end())
		{
			const errs *e = mci->second;
			if ( e->errt == DUERRT_I )
				printf(e->msg, i);
			else
				printf("Missing error parameter %d: %s", err, e->msg);
		}
		else
			printf("Unknown error %d", err);

		puts("");

		if(err>=DUERR_FATAL)
			exit(16);
	}


	void error(int err, unsigned line, const char *s, int i)
	{
		error_common( err, line);

		ERRMCI mci = err_map.find(err);

		if(mci!=err_map.end())
		{
			const errs *e = mci->second;
			if ( e->errt == DUERRT_SI )
				printf(e->msg, s, i);
			else
				printf("Missing error parameter %d: %s", err, e->msg);
		}
		else
			printf("Unknown error %d", err);

		puts("");

		if(err>=DUERR_FATAL)
			exit(16);
	}

	void error(int err, unsigned line, int i, int j)
	{
		error_common( err, line);

		ERRMCI mci = err_map.find(err);

		if(mci!=err_map.end())
		{
			const errs *e = mci->second;
			if ( e->errt == DUERRT_II )
				printf(e->msg, i, j);
			else
				printf("Missing error parameter %d: %s", err, e->msg);
		}
		else
			printf("Unknown error %d", err);

		puts("");

		if(err>=DUERR_FATAL)
			exit(16);
	}

	void error(int err, unsigned line, int i, const char *s)
	{
		error_common( err, line);

		ERRMCI mci = err_map.find(err);

		if(mci!=err_map.end())
		{
			const errs *e = mci->second;
			if ( e->errt == DUERRT_IS )
				printf(e->msg, i, s);
			else
				printf("Missing error parameter %d: %s", err, e->msg);
		}
		else
			printf("Unknown error %d", err);

		puts("");

		if(err>=DUERR_FATAL)
			exit(16);
	}

}
