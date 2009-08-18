/*	NAME:
		TDataCompressor.cpp

	DESCRIPTION:
		NDataCompressor tests.

	COPYRIGHT:
		Copyright (c) 2006-2009, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSystemUtilities.h"
#include "NDataCompressor.h"
#include "NDataDigest.h"

#include "TDataCompressor.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const NIndex kNullSize							= 524;
static const UInt32 kNullAdler							= 0x7CECBDEB;

static const NIndex kZLibSize							= 312;
static const UInt32 kZLibAdler							= 0xB82392EC;

static const UInt32 kTestAdler							= 0xEC6CBC2C;
static const UInt8  kTestData[]							= { 0x4C, 0x6F, 0x72, 0x65, 0x6D, 0x20, 0x69, 0x70, 0x73, 0x75, 0x6D, 0x20, 0x64, 0x6F, 0x6C, 0x6F,
															0x72, 0x20, 0x73, 0x69, 0x74, 0x20, 0x61, 0x6D, 0x65, 0x74, 0x2C, 0x20, 0x63, 0x6F, 0x6E, 0x73,
															0x65, 0x63, 0x74, 0x65, 0x74, 0x75, 0x72, 0x20, 0x61, 0x64, 0x69, 0x70, 0x69, 0x73, 0x63, 0x69,
															0x6E, 0x67, 0x20, 0x65, 0x6C, 0x69, 0x74, 0x2E, 0x20, 0x4D, 0x6F, 0x72, 0x62, 0x69, 0x20, 0x73,
															0x65, 0x64, 0x20, 0x65, 0x6E, 0x69, 0x6D, 0x20, 0x75, 0x72, 0x6E, 0x61, 0x2C, 0x20, 0x6E, 0x6F,
															0x6E, 0x20, 0x69, 0x6D, 0x70, 0x65, 0x72, 0x64, 0x69, 0x65, 0x74, 0x20, 0x6E, 0x75, 0x6C, 0x6C,
															0x61, 0x2E, 0x20, 0x43, 0x72, 0x61, 0x73, 0x20, 0x65, 0x67, 0x65, 0x74, 0x20, 0x68, 0x65, 0x6E,
															0x64, 0x72, 0x65, 0x72, 0x69, 0x74, 0x20, 0x65, 0x73, 0x74, 0x2E, 0x20, 0x49, 0x6E, 0x74, 0x65,
															0x67, 0x65, 0x72, 0x20, 0x6A, 0x75, 0x73, 0x74, 0x6F, 0x20, 0x6C, 0x69, 0x62, 0x65, 0x72, 0x6F,
															0x2C, 0x20, 0x62, 0x69, 0x62, 0x65, 0x6E, 0x64, 0x75, 0x6D, 0x20, 0x65, 0x74, 0x20, 0x66, 0x72,
															0x69, 0x6E, 0x67, 0x69, 0x6C, 0x6C, 0x61, 0x20, 0x6E, 0x65, 0x63, 0x2C, 0x20, 0x65, 0x67, 0x65,
															0x73, 0x74, 0x61, 0x73, 0x20, 0x71, 0x75, 0x69, 0x73, 0x20, 0x70, 0x75, 0x72, 0x75, 0x73, 0x2E,
															0x20, 0x50, 0x72, 0x6F, 0x69, 0x6E, 0x20, 0x6D, 0x61, 0x6C, 0x65, 0x73, 0x75, 0x61, 0x64, 0x61,
															0x20, 0x64, 0x69, 0x67, 0x6E, 0x69, 0x73, 0x73, 0x69, 0x6D, 0x20, 0x6C, 0x69, 0x62, 0x65, 0x72,
															0x6F, 0x2C, 0x20, 0x61, 0x20, 0x63, 0x6F, 0x6E, 0x67, 0x75, 0x65, 0x20, 0x6E, 0x69, 0x73, 0x69,
															0x20, 0x74, 0x69, 0x6E, 0x63, 0x69, 0x64, 0x75, 0x6E, 0x74, 0x20, 0x69, 0x64, 0x2E, 0x20, 0x43,
															0x75, 0x72, 0x61, 0x62, 0x69, 0x74, 0x75, 0x72, 0x20, 0x70, 0x6C, 0x61, 0x63, 0x65, 0x72, 0x61,
															0x74, 0x20, 0x68, 0x65, 0x6E, 0x64, 0x72, 0x65, 0x72, 0x69, 0x74, 0x20, 0x74, 0x72, 0x69, 0x73,
															0x74, 0x69, 0x71, 0x75, 0x65, 0x2E, 0x20, 0x41, 0x65, 0x6E, 0x65, 0x61, 0x6E, 0x20, 0x71, 0x75,
															0x61, 0x6D, 0x20, 0x6C, 0x61, 0x63, 0x75, 0x73, 0x2C, 0x20, 0x70, 0x6F, 0x73, 0x75, 0x65, 0x72,
															0x65, 0x20, 0x69, 0x64, 0x20, 0x6F, 0x72, 0x6E, 0x61, 0x72, 0x65, 0x20, 0x61, 0x63, 0x2C, 0x20,
															0x76, 0x75, 0x6C, 0x70, 0x75, 0x74, 0x61, 0x74, 0x65, 0x20, 0x73, 0x69, 0x74, 0x20, 0x61, 0x6D,
															0x65, 0x74, 0x20, 0x70, 0x75, 0x72, 0x75, 0x73, 0x2E, 0x20, 0x41, 0x6C, 0x69, 0x71, 0x75, 0x61,
															0x6D, 0x20, 0x65, 0x75, 0x20, 0x64, 0x6F, 0x6C, 0x6F, 0x72, 0x20, 0x74, 0x6F, 0x72, 0x74, 0x6F,
															0x72, 0x2E, 0x20, 0x41, 0x6C, 0x69, 0x71, 0x75, 0x61, 0x6D, 0x20, 0x64, 0x75, 0x69, 0x20, 0x6A,
															0x75, 0x73, 0x74, 0x6F, 0x2C, 0x20, 0x63, 0x6F, 0x6E, 0x76, 0x61, 0x6C, 0x6C, 0x69, 0x73, 0x20,
															0x6E, 0x6F, 0x6E, 0x20, 0x6C, 0x6F, 0x62, 0x6F, 0x72, 0x74, 0x69, 0x73, 0x20, 0x65, 0x75, 0x2C,
															0x20, 0x66, 0x65, 0x75, 0x67, 0x69, 0x61, 0x74, 0x20, 0x73, 0x63, 0x65, 0x6C, 0x65, 0x72, 0x69,
															0x73, 0x71, 0x75, 0x65, 0x20, 0x6E, 0x69, 0x73, 0x69, 0x2E, 0x20, 0x43, 0x6C, 0x61, 0x73, 0x73,
															0x20, 0x61, 0x70, 0x74, 0x65, 0x6E, 0x74, 0x20, 0x74, 0x61, 0x63, 0x69, 0x74, 0x69, 0x20, 0x73,
															0x6F, 0x63, 0x69, 0x6F, 0x73, 0x71, 0x75, 0x20, 0x61, 0x64, 0x20, 0x6C, 0x69, 0x74, 0x6F, 0x72,
															0x61, 0x20, 0x74, 0x6F, 0x72, 0x71, 0x75, 0x65, 0x6E, 0x74, 0x20, 0x61, 0x6D, 0x65, 0x74, 0x2E };





//============================================================================
//		TDataCompressor::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TDataCompressor::Execute(void)
{	NData				dataSrc, dataDst, dataOut;
	NDataCompressor		theCompressor;
	UInt32				adlerValue;
	NDataDigest			theDigest;



	// Get the state we need
	dataSrc = NData(GET_ARRAY_SIZE(kTestData), kTestData);



	// Null
	dataDst = theCompressor.Compress(dataSrc, kNCompressionNull);
	NN_ASSERT(dataDst.GetSize() == kNullSize);

	adlerValue = theDigest.GetAdler32(dataDst);
	NN_ASSERT(adlerValue == kNullAdler);

	dataOut = theCompressor.Decompress(dataDst);
	NN_ASSERT(dataOut.GetSize() == GET_ARRAY_SIZE(kTestData));

	adlerValue = theDigest.GetAdler32(dataOut);
	NN_ASSERT(adlerValue == kTestAdler);




	// ZLib
	dataDst = theCompressor.Compress(dataSrc, kNCompressionZLib);
	NN_ASSERT(dataDst.GetSize() == kZLibSize);

	adlerValue = theDigest.GetAdler32(dataDst);
	NN_ASSERT(adlerValue == kZLibAdler);

	dataOut = theCompressor.Decompress(dataDst);
	NN_ASSERT(dataOut.GetSize() == GET_ARRAY_SIZE(kTestData));

	adlerValue = theDigest.GetAdler32(dataOut);
	NN_ASSERT(adlerValue == kTestAdler);
}







