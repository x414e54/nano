/*	NAME:
		NMacPOSIX.cpp

	DESCRIPTION:
		Mac POSIX support.
	
	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NTargetPOSIX.h"





//============================================================================
//		NTargetPOSIX::is_nan : Mac isnan.
//----------------------------------------------------------------------------
int NTargetPOSIX::is_nan(double r)
{


	// Get the value
	return(isnan(r));
}





//============================================================================
//		NTargetPOSIX::is_inf : Mac isinf.
//----------------------------------------------------------------------------
int NTargetPOSIX::is_inf(double r)
{


	// Get the value
	return(isinf(r));
}





//============================================================================
//		NTargetPOSIX::snprintf : Mac snprintf.
//----------------------------------------------------------------------------
int NTargetPOSIX::snprintf(char *s, size_t n, const char *format, ...)
{	int			theResult;
	va_list		argList;



	// Print the value
	va_start(argList, format);
	theResult = vsnprintf(s, n, format, argList);
	va_end(argList);
	
	return(theResult);
}





//============================================================================
//		NTargetPOSIX::gmtime : Mac gmtime.
//----------------------------------------------------------------------------
struct tm NTargetPOSIX::gmtime(time_t theTime)
{	struct tm		theResult;



	// Get the value
	gmtime_r(&theTime, &theResult);
	
	return(theResult);
}





//============================================================================
//		NTargetPOSIX::timegm : Mac timegm.
//----------------------------------------------------------------------------
time_t NTargetPOSIX::timegm(const struct tm *tm)
{	struct tm localTM = *tm;



	// Get the value
	return(::timegm(&localTM));
}





//============================================================================
//		NTargetPOSIX::getcwd : Mac getcwd.
//----------------------------------------------------------------------------
char *NTargetPOSIX::getcwd(char *buf, size_t size)
{


	// Get the value
	return(::getcwd(buf, size));
}


