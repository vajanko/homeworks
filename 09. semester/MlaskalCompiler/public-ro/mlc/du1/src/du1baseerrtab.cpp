/*

	DU1BASEERRTAB.CPP

	JY

	Base errors for DU1

*/

#include "duerr.h"

using namespace mlaskal;
using namespace mlc;

namespace {

	errs base_errors_int_[] = {
		{ DUERR_OK,								DUERRT_VOID,			"Everything is OK" },
		{ DUERR_NOK,								DUERRT_I,			"Everything is nearly OK, although there were %d warnings" },
		{ DUERR_ERRS,								DUERRT_II,			"There were some errors %d and warnings %d" },
		{ DUERR_UNKCHAR,						DUERRT_CI,			"Unknown character '%c'(%#02x) on the input" },
		{ DUERR_NOINFILE,						DUERRT_S,			"Unable to open input file '%s'" },
	};

}

namespace mlc {

	carray<errs> base_errors(base_errors_int_);

}
