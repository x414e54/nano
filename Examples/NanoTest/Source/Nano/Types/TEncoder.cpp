/*	NAME:
		TEncoder.cpp

	DESCRIPTION:
		NEncoder tests.

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
#include "NEncodable.h"
#include "NEncoder.h"

#include "TEncoder.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
// Keys
static const NString kKeyBoolean1							= "Test Boolean 1";
static const NString kKeyBoolean2							= "Test Boolean 2";
static const NString kKeyNumber1							= "Test Number 1";
static const NString kKeyNumber2							= "Test Number 2";
static const NString kKeyNumber3							= "Test Number 3";
static const NString kKeyNumber4							= "Test Number 4";
static const NString kKeyData								= "Test Data";
static const NString kKeyString								= "Test String";
static const NString kKeyObject								= "Test Object";



// Values
static const bool    kValueBoolean1						= true;
static const bool    kValueBoolean2						= false;
static const NNumber kValueNumber1						= NVariant( 1234);
static const NNumber kValueNumber2						= NVariant(-5678);
static const NNumber kValueNumber3						= NVariant( 1234.123);
static const NNumber kValueNumber4						= NVariant(-5678.567);
static const UInt8   kValueData[]						= { 0xAA, 0xBB, 0xCC, 0xDD };
static const NString kValueString						= "This is a string";
static const NString kValueObject						= "This is an object";





//============================================================================
//		Internal class definition
//----------------------------------------------------------------------------
class TEncodable :	public NEncodable,
					public NComparable<TEncodable> {
public:
	DECLARE_NENCODABLE(TEncodable);


	// Encode the object
	void EncodeSelf(NEncoder &theEncoder) const
	{	
		theEncoder.EncodeBoolean(kKeyBoolean1, kValueBoolean1);
		theEncoder.EncodeBoolean(kKeyBoolean2, kValueBoolean2);
		theEncoder.EncodeNumber( kKeyNumber1,  kValueNumber1);
		theEncoder.EncodeNumber( kKeyNumber2,  kValueNumber2);
		theEncoder.EncodeNumber( kKeyNumber3,  kValueNumber3);
		theEncoder.EncodeNumber( kKeyNumber4,  kValueNumber4);
		theEncoder.EncodeData(   kKeyData,     NData(GET_ARRAY_SIZE(kValueData), kValueData));
		theEncoder.EncodeString( kKeyString,   kValueString);
		theEncoder.EncodeObject( kKeyObject,   kValueObject);
	}


	// Decode the object
	void DecodeSelf(const NEncoder &theEncoder)
	{
	}
	
	
	// Compare the value
	NComparison Compare(const TEncodable &theValue) const
	{
		return(GetComparison(this, &theValue));
	}

};

DEFINE_NENCODABLE(TEncodable);





//============================================================================
//		TEncoder::Execute : Execute the tests.
//----------------------------------------------------------------------------
void TEncoder::Execute(void)
{	NEncoder		theEncoder;
	TEncodable		theObject;
	NString			theText;
	NData			theData;



	// Encoding
	theData = theEncoder.Encode(theObject);
	theText.SetData(theData);
	
	NN_LOG("[%@]", theText);



	// Decoding



}


