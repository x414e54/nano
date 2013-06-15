/*	NAME:
		NData.h

	DESCRIPTION:
		Data object.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NDATA_HDR
#define NDATA_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NStringFormatter.h"
#include "NSharedValue.h"
#include "NEncodable.h"
#include "NContainer.h"
#include "NHashable.h"
#include "NRange.h"





//============================================================================
//      Types
//----------------------------------------------------------------------------
class NData;

typedef std::vector<UInt8>											NDataValue;
typedef NDataValue::iterator										NDataValueIterator;
typedef NDataValue::const_iterator									NDataValueConstIterator;

typedef std::vector<NData>											NDataList;
typedef NDataList::iterator											NDataListIterator;
typedef NDataList::const_iterator									NDataListConstIterator;

typedef NSharedValue<NDataValue>									NSharedValueData;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NData :	public NContainer,
				public NHashable,
				public NEncodable,
				public NComparable<NData>,
				public NSharedValueData {
public:
										NENCODABLE_DECLARE(NData);

										NData(NIndex theSize, const void *thePtr=NULL, bool makeCopy=true);
										NData(const NData &theValue);

										NData(void);
	virtual							   ~NData(void);


	// Clear the value
	void								Clear(void);


	// Get/set the size
	//
	// Increasing the size will zero-fill the new content.
	NIndex								GetSize(void) const;
	bool								SetSize(NIndex theSize);


	// Reserve additional space
	//
	// Reserving space pre-allocates internal storage to amortize future resizes.
	void								Reserve(NIndex theSize);


	// Get/set the data
	//
	// GetData will return a NULL pointer if the data object is empty.
	//
	// SetData(xxx, NULL) is equivalent to Clear+AppendData(xxx);
	const UInt8						   *GetData(NIndex theOffset=0) const;
	UInt8							   *GetData(NIndex theOffset=0);
	
	NData								GetData(const NRange &theRange) const;

	void								SetData(NIndex theSize, const void *thePtr, bool makeCopy=true);


	// Append data
	//
	// Appending a NULL pointer will zero-fill the new content.
	//
	// Returns a pointer to the newly-appended data, or NULL if no data was appended.
	UInt8							   *AppendData(NIndex theSize, const void *thePtr=NULL);
	UInt8							   *AppendData(const NData &theData);


	// Remove data
	void								RemoveData(const NRange &theRange);


	// Replace data
	//
	// Returns a pointer to the newly-modified data.
	UInt8							   *ReplaceData(const NRange &theRange, NIndex theSize, const void *thePtr);
	UInt8							   *ReplaceData(const NRange &theRange, const NData &theData);


	// Compare the value
	NComparison							Compare(const NData &theValue) const;


	// Operators
	const NData&						operator =  (const NData &theValue);
	const NData&						operator += (const NData &theValue);
	const NData							operator +	(const NData &theValue) const;


	// Operators
										operator NFormatArgument(void) const;


protected:
	// Get the value
	NDataValue						   *GetMutable(void);


	// Get the null value
	const NDataValue				   *GetNullValue(void) const;


	// Calculate the hash
	NHashCode							CalculateHash(void) const;


	// Encode/decode the object
	void								EncodeSelf(      NEncoder &theEncoder) const;
	void								DecodeSelf(const NEncoder &theEncoder);


private:
	void								Resize(NDataValue *theValue, NIndex theSize);
	

private:
	NIndex								mExternalSize;
	const void						   *mExternalPtr;
};





#endif // NDATA_HDR


