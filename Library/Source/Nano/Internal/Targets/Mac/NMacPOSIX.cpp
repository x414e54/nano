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
#include "NMathUtilities.h"
#include "NTargetPOSIX.h"





//============================================================================
//		NTargetPOSIX::is_nan : Mac isnan.
//----------------------------------------------------------------------------
bool NTargetPOSIX::is_nan(Float64 x)
{


	// Get the value
	return(isnan(x));
}





//============================================================================
//		NTargetPOSIX::is_inf : Mac isinf.
//----------------------------------------------------------------------------
bool NTargetPOSIX::is_inf(Float64 x)
{


	// Get the value
	return(isinf(x));
}





//============================================================================
//      NTargetPOSIX::asin : Calculate the inverse sine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::asin(Float64 x)
{


    // Get the value
	return(::asin(x));
}





//============================================================================
//      NTargetPOSIX::acos : Calculate the inverse cosine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::acos(Float64 x)
{


    // Get the value
	return(::acos(x));
}





//============================================================================
//      NTargetPOSIX::sinh : Calculate the hyperbolic sine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::sinh(Float64 x)
{


    // Get the value
	return(::sinh(x));
}





//============================================================================
//      NTargetPOSIX::cosh : Calculate the hyperbolic cosine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::cosh(Float64 x)
{


    // Get the value
	return(::cosh(x));
}





//============================================================================
//      NTargetPOSIX::tanh : Calculate the hyperbolic tangent.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::tanh(Float64 x)
{


    // Get the value
	return(::tanh(x));
}





//============================================================================
//      NTargetPOSIX::asinh : Calculate the inverse hyperbolic sine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::asinh(Float64 x)
{


    // Get the value
	return(::asinh(x));
}





//============================================================================
//      NTargetPOSIX::acosh : Calculate the inverse hyperbolic cosine.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::acosh(Float64 x)
{


    // Get the value
	return(::acosh(x));
}





//============================================================================
//      NTargetPOSIX::atanh : Calculate the inverse hyperbolic tangent.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::atanh(Float64 x)
{


    // Get the value
	return(::atanh(x));
}





//============================================================================
//      NTargetPOSIX::rint : Round to the nearest integer.
//----------------------------------------------------------------------------
Float64 NTargetPOSIX::rint(Float64 x)
{


    // Get the value
	return(::rint(x));
}





//============================================================================
//		NTargetPOSIX::snprintf : Mac snprintf.
//----------------------------------------------------------------------------
NIndex NTargetPOSIX::snprintf(char *s, size_t n, const char *format, ...)
{	NIndex		theResult;
	va_list		argList;



	// Print the value
	va_start(argList, format);
	theResult = ::vsnprintf(s, n, format, argList);
	va_end(argList);
	
	return(theResult);
}





//============================================================================
//		NTargetPOSIX::vsnprintf : Mac vsnprintf.
//----------------------------------------------------------------------------
NIndex NTargetPOSIX::vsnprintf(char *s, size_t n, const char *format, va_list argList)
{	NIndex		theResult;



	// Print the value
	theResult = ::vsnprintf(s, n, format, argList);
	
	return(theResult);
}





//============================================================================
//		NTargetPOSIX::gmtime : Mac gmtime.
//----------------------------------------------------------------------------
struct tm NTargetPOSIX::gmtime(time_t theTime)
{	struct tm		theResult;



	// Get the value
	::gmtime_r(&theTime, &theResult);
	
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


