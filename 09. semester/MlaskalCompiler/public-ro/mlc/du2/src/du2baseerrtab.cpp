/*

	DU2BASEERRTAB.CPP

	JY

	Base errors for DU2

*/

#include "duerr.h"

using namespace mlaskal;
using namespace mlc;

namespace {

	struct errs base_errors_int_[] = {
		{ DUERR_OK,								DUERRT_VOID,			"Everything is OK" },
		{ DUERR_NOK,								DUERRT_I,			"Everything is nearly OK, although there were %d warnings" },
		{ DUERR_ERRS,								DUERRT_II,			"There were some errors %d and warnings %d" },
		{ DUERR_INTOUTRANGE,						DUERRT_S,			"Integer number '%s' is out of range, will be truncated" },
		{ DUERR_REALOUTRANGE,						DUERRT_S,			"Real number '%s' is out of range, will be truncated" },
		{ DUERR_UNKCHAR,						DUERRT_CI,			"Unknown character '%c'(%#02x) on the input" },
		{ DUERR_EOFINCMT,						DUERRT_VOID,			"EOF in comment" },
		{ DUERR_EOLINSTRCHR,						DUERRT_VOID,			"EOL in string" },
		{ DUERR_EOFINSTRCHR,						DUERRT_VOID,			"EOF in string" },
		{ DUERR_UNEXPENDCMT,						DUERRT_VOID,			"Unexpected end of comment" },
		{ DUERR_BADINT,								DUERRT_S,			"Integer number '%s' is malformed" },
		{ DUERR_BADREAL,						DUERRT_S,			"Real number '%s' is malformed" },
		{ DUERR_NOINFILE,						DUERRT_S,			"Unable to open input file '%s'" },
	};

}

namespace mlc {

	carray<errs> base_errors(base_errors_int_);

}
