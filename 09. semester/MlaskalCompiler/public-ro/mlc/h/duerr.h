/*

	DUERR.H

	JY

	Error facility

*/

#ifndef DUERR_H_
#define DUERR_H_

#include "carray.hpp"

namespace mlc {

	enum err_type { DUERRT_VOID, DUERRT_S, DUERRT_CI, DUERRT_I, DUERRT_SI, DUERRT_II, DUERRT_IS };

	struct errs {
		int						errc;
		enum err_type		errt;
		const char				*msg;
	};

	void error(int err, unsigned line);
	void error(int err, unsigned line, const char *s);
	void error(int err, unsigned line, char c, int i);
	void error(int err, unsigned line, int i);
	void error(int err, unsigned line, const char *s, int i);
	void error(int err, unsigned line, int i, int j);
	void error(int err, unsigned line, int i, const char *s);

	inline void error(int err, unsigned line, const std::string & s)
	{
		error( err, line, s.c_str());
	}

	extern int n_warnings, n_errors;

	extern mlaskal::carray<errs> base_errors;
	extern mlaskal::carray<errs> sem_errors;

}

#include "dubaseerr.h"
#include "dusemerr.h"

#endif
