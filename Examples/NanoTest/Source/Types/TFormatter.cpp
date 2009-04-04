/*	NAME:
		TFormatter.cpp

	DESCRIPTION:
		NFormatter tests.

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
#include "NFormatter.h"

#include "TFormatter.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt8  kValueUInt8										= 8;
static const UInt16	kValueUInt16									= 1616;
static const UInt32	kValueUInt32									= 32323232;
static const UInt64	kValueUInt64									= 6464646464646464ULL;

static const SInt8	kValueSInt8										= -8;
static const SInt16	kValueSInt16									= -1616;
static const SInt32	kValueSInt32									= -32323232;
static const SInt64	kValueSInt64									= -6464646464646464LL;

static const Float32 kValueFloat32									= (Float32) kPi;
static const Float64 kValueFloat64									= (Float64) kPi;

static const char *kValuePtrChar									= "text";
static const void *kValuePtrVoid									= (void *) 0xDEADBEEF;





//============================================================================
//		TFormatter::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TFormatter::Execute(void)
{	NFormatter		theFormatter;
	NString			theResult;


	// Validate types
	theResult = theFormatter.Format("ArgChar [%c]", 'z');
	NN_ASSERT(theResult == "ArgChar [z]");

	theResult = theFormatter.Format("ArgUInt8 [%d]", kValueUInt8);
	NN_ASSERT(theResult == "ArgUInt8 [8]");

	theResult = theFormatter.Format("ArgUInt16 [%d]", kValueUInt16);
	NN_ASSERT(theResult == "ArgUInt16 [1616]");

	theResult = theFormatter.Format("ArgUInt32 [%d]", kValueUInt32);
	NN_ASSERT(theResult == "ArgUInt32 [32323232]");

	theResult = theFormatter.Format("ArgUInt64 [%lld]", kValueUInt64);
	NN_ASSERT(theResult == "ArgUInt64 [6464646464646464]");


	theResult = theFormatter.Format("ArgSInt8 [%d]", kValueSInt8);
	NN_ASSERT(theResult == "ArgSInt8 [-8]");

	theResult = theFormatter.Format("ArgSInt16 [%d]", kValueSInt16);
	NN_ASSERT(theResult == "ArgSInt16 [-1616]");

	theResult = theFormatter.Format("ArgSInt32 [%d]", kValueSInt32);
	NN_ASSERT(theResult == "ArgSInt32 [-32323232]");

	theResult = theFormatter.Format("ArgSInt64 [%lld]", kValueSInt64);
	NN_ASSERT(theResult == "ArgSInt64 [-6464646464646464]");


	theResult = theFormatter.Format("ArgFloat32 [%.6f]", kValueFloat32);
	NN_ASSERT(theResult == "ArgFloat32 [3.141593]");

	theResult = theFormatter.Format("ArgFloat64 [%.15f]", kValueFloat64);
	NN_ASSERT(theResult == "ArgFloat64 [3.141592653589793]");


	theResult = theFormatter.Format("ArgPtrChar [%s]", kValuePtrChar);
	NN_ASSERT(theResult == "ArgPtrChar [text]");

	theResult = theFormatter.Format("ArgPtrVoid [%p]", kValuePtrVoid);
	NN_ASSERT(theResult == "ArgPtrVoid [0xdeadbeef]");



	// Validate counts
	theResult = theFormatter.Format("OneArg [%d]", 42);
	NN_ASSERT(theResult == "OneArg [42]");

	theResult = theFormatter.Format("TwoArgs [%d] [%d]", 42, 23);
	NN_ASSERT(theResult == "TwoArgs [42] [23]");

	theResult = theFormatter.Format("ThreeArgs [%d] [%d] [%d]", 42, 23, 101);
	NN_ASSERT(theResult == "ThreeArgs [42] [23] [101]");



	// Validate special cases
	theResult = theFormatter.Format("PercentArg [%%]");
	NN_ASSERT(theResult == "PercentArg [%]");

	theResult = theFormatter.Format("NoSpecifier [% 12345]");
	NN_ASSERT(theResult == "NoSpecifier [% 12345]");

	theResult = theFormatter.Format("TooFewArgs [%d] [%d]", 42);
	NN_ASSERT(theResult == "TooFewArgs [42] []");

	theResult = theFormatter.Format("TooManyArgs [%d] [%d]", 42, 23, 101);
	NN_ASSERT(theResult == "TooManyArgs [42] [23]");
}
