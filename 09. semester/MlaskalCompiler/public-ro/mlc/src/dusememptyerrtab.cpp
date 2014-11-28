/*

	DUSEMEMPTYERRTAB.CPP

	JY

	Empty semantic error table

*/

#include "duerr.h"

using namespace mlaskal;
using namespace mlc;

namespace {

	errs sem_errors_int_[] = {
		{ DUSEMERR_NOERROR, DUERRT_VOID, 0 },
	};

}

namespace mlc {

	carray<errs> sem_errors(sem_errors_int_);

}
