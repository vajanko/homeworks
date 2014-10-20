/*

	DUBASEERR.H

	JY

	Error definition for DU1

*/

#ifndef DUBASEERR_H_
#define DUBASEERR_H_

namespace mlc {

	enum err_code {
		DUERR_OK,									/* everything is OK */
		DUERR_NOK,									/* nearly OK */
		DUERR_ERRS,									/* there were some errors */

		DUERR_WARNING=1000,

		DUERR_SEMWARN=2000,

		DUERR_ERROR=5000,
		DUERR_UNKCHAR,									/* unknown character on the input */

		DUERR_SEMERROR=6000,							/* start of semantic errors */

		DUERR_FATAL=10000,
		DUERR_NOINFILE									/* unable to open input file %s */
	};

}

#endif
