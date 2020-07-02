/************************************************************************
Name: Hai Nguyen Van
Cellphone: (84) 97-8779-222
Mail:thienhaiblue@ampm.vn
----------------------------------
AMPM JSC
Add: 634/6 Phan Van Suu street , Ward 13, Tan Binh District, HCM City, VN
**************************************************************************/
#ifndef __AMPM_SPRINTF___
#define __AMPM_SPRINTF___
#include "stdint.h"
#include "stdarg.h"

int ampm_vsprintf (char *fp,
	const char* format,	/* Pointer to the format string */
	va_list arg					/* Optional arguments... */
);

int  ampm_sprintf(char *fp,const char *format, ...);

#endif
