/*	NAME:
		NFileIterator.cpp

	DESCRIPTION:
		Filesystem iterator.

	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NSTLUtilities.h"
#include "NFileIterator.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UInt32 kMaxFileIteratorCount								= 50;





//============================================================================
//		NFileIterator::NFileIterator : Constructor.
//----------------------------------------------------------------------------
NFileIterator::NFileIterator(void)
{


	// Initialize ourselves
	mRecurseLimit = 0;
	mRecurseFilterProc = NULL;
	mRecurseFilterData = NULL;
	
	mItemFilterProc = NULL;
	mItemFilterData = NULL;

	mCatalogInfo  = kFSCatInfoNone;

	mDepth    = 0;
	mStopNow  = false;
	mIterator = NULL;
}





//============================================================================
//		NFileIterator::~NFileIterator : Destructor.
//----------------------------------------------------------------------------
NFileIterator::~NFileIterator(void)
{


	// Clean up
	Close();
}





//============================================================================
//		NFileIterator::GetRecurseLimit : Get the recurse limit.
//----------------------------------------------------------------------------
UInt32 NFileIterator::GetRecurseLimit(void) const
{


	// Get the limit
	return(mRecurseLimit);
}





//============================================================================
//		NFileIterator::SetRecurseLimit : Set the recurse limit.
//----------------------------------------------------------------------------
void NFileIterator::SetRecurseLimit(UInt32 theLimit)
{


	// Set the limit
	mRecurseLimit = theLimit;
}





//============================================================================
//		NFileIterator::GetRecurseFilter : Get the recurse filter.
//----------------------------------------------------------------------------
IteratorFilterProc NFileIterator::GetRecurseFilter(void **userData) const
{


	// Get the filter
	if (userData != NULL)
		*userData = mRecurseFilterData;
		
	return(mRecurseFilterProc);
}





//============================================================================
//		NFileIterator::SetRecurseFilter : Set the recurse filter.
//----------------------------------------------------------------------------
void NFileIterator::SetRecurseFilter(IteratorFilterProc theFilter, void *userData)
{


	// Set the filter
	mRecurseFilterProc = theFilter;
	mRecurseFilterData = userData;
}





//============================================================================
//		NFileIterator::GetItemFilter : Get the item filter.
//----------------------------------------------------------------------------
IteratorFilterProc NFileIterator::GetItemFilter(void **userData) const
{


	// Get the filter
	if (userData != NULL)
		*userData = mItemFilterData;
		
	return(mItemFilterProc);
}





//============================================================================
//		NFileIterator::SetItemFilter : Set the item filter.
//----------------------------------------------------------------------------
void NFileIterator::SetItemFilter(IteratorFilterProc theFilter, void *userData)
{


	// Set the filter
	mItemFilterProc = theFilter;
	mItemFilterData = userData;
}





//============================================================================
//		NFileIterator::GetCatalogInfo : Get the catalog info.
//----------------------------------------------------------------------------
FSCatalogInfoBitmap NFileIterator::GetCatalogInfo(void) const
{


	// Get the limit
	return(mCatalogInfo);
}





//============================================================================
//		NFileIterator::SetCatalogInfo : Set the catalog info.
//----------------------------------------------------------------------------
void NFileIterator::SetCatalogInfo(FSCatalogInfoBitmap theInfo)
{


	// Set the limit
	mCatalogInfo = theInfo;
}





//============================================================================
//		NFileIterator::GetPatternName : Get the file name pattern.
//----------------------------------------------------------------------------
NString NFileIterator::GetPatternName(void) const
{


	// Get the pattern
	return(mPatternName);
}





//============================================================================
//		NFileIterator::SetPatternName : Set the file name pattern.
//----------------------------------------------------------------------------
void NFileIterator::SetPatternName(const NString &thePattern)
{


	// Set the pattern
	mPatternName = thePattern;
}





//============================================================================
//		NFileIterator::GetPatternPath : Get the file path pattern.
//----------------------------------------------------------------------------
NString NFileIterator::GetPatternPath(void) const
{


	// Get the pattern
	return(mPatternPath);
}





//============================================================================
//		NFileIterator::SetPatternPath : Set the file path pattern.
//----------------------------------------------------------------------------
void NFileIterator::SetPatternPath(const NString &thePattern)
{


	// Set the pattern
	mPatternPath = thePattern;
}





//============================================================================
//		NFileIterator::Open : Open the iterator.
//----------------------------------------------------------------------------
OSStatus NFileIterator::Open(const NFile &startDir)
{	IteratorStart	theStart;



	// Validate our state
	NN_ASSERT(mIterator == NULL);
	NN_ASSERT(mRemaining.empty());



	// Update our state
	theStart.theFile  = startDir;
	theStart.theDepth = 0;

	mRemaining.push_back(theStart);



	// Begin the search
	return(CreateIterator());
}





//============================================================================
//		NFileIterator::Close : Close the iterator.
//----------------------------------------------------------------------------
void NFileIterator::Close(void)
{


	// Close the iterator
	DestroyIterator();

	mDepth   = 0;
	mStopNow = false;
	
	mRemaining.clear();
	mResultFiles.clear();
	mResultInfo.clear();
}





//============================================================================
//		NFileIterator::GetNext: Get the next file.
//----------------------------------------------------------------------------
OSStatus NFileIterator::GetNext(NFile &theFile, FSCatalogInfo *theInfo)
{	FSCatalogInfo	tmpInfo;
	OSStatus		theErr;



	// Get the state we need
	theErr = noErr;

	if (theInfo == NULL)
		theInfo = &tmpInfo;



	// Collect some more results
	while (theErr == noErr && mResultFiles.empty())
		theErr = CollectResults();



	// Get the next file
	if (!mResultFiles.empty())
		{
		theFile  = extract_back(mResultFiles);
		*theInfo = extract_back(mResultInfo);
		theErr   = noErr;
		}

	return(theErr);
}





//============================================================================
//		NFileIterator::GetFiles : Get the files.
//----------------------------------------------------------------------------
NFileList NFileIterator::GetFiles(const NFile &startDir)
{	NFileList		theFiles;
	NFile			theFile;
	OSStatus		theErr;



	// Get the files
	theErr = Open(startDir);
	if (theErr == noErr)
		{
		while (GetNext(theFile) == noErr)
			{
			theFiles.push_back(theFile);
			if (mStopNow)
				break;
			}
		
		Close();
		}
	
	return(theFiles);
}





//============================================================================
//		NFileIterator::CollectResults : Collect some more results.
//----------------------------------------------------------------------------
#pragma mark -
OSStatus NFileIterator::CollectResults(void)
{	FSRefList				resultFSRef;
	FSCatalogInfoList		resultInfo;
	HFSUniStr255List		resultName;
	FSCatalogInfoBitmap		whichInfo;
	Boolean					didChange;
	HFSUniStr255			*namePtr;
	UInt32					numItems;
	OSStatus				theErr;



	// Validate our state
	NN_ASSERT(mResultFiles.empty());
	NN_ASSERT(mResultInfo.empty());



	// Get the state we need
	resultFSRef.resize(kMaxFileIteratorCount);
	resultInfo.resize( kMaxFileIteratorCount);
	resultName.resize( kMaxFileIteratorCount);

	numItems  = 0;
	whichInfo = mCatalogInfo | kFSCatInfoNodeFlags;
	namePtr   = mPatternName.IsNotEmpty() ? &resultName[0] : NULL;



	// Create the iterator
	theErr = CreateIterator();
	if (theErr != noErr)
		return(theErr);



	// Collect the results
	if (theErr == noErr)
		theErr = FSGetCatalogInfoBulk(mIterator, kMaxFileIteratorCount, &numItems, &didChange, whichInfo, &resultInfo[0], &resultFSRef[0], NULL, namePtr);

	ProcessResults(numItems, &resultFSRef[0], &resultInfo[0], namePtr);



	// Reset when the iterator is exhausted
	//
	// When the iterate is done, we destroy the iterator and consume the error;
	// the next iterator will be created when the current results are consumed.
	if (theErr == errFSNoMoreItems)
		{
		DestroyIterator();
		theErr = noErr;
		}
	
	return(noErr);
}





//============================================================================
//		NFileIterator::CreateIterator : Create the iterator.
//----------------------------------------------------------------------------
OSStatus NFileIterator::CreateIterator(void)
{	FSRef			theFSRef;
	IteratorStart	theStart;
	OSStatus		theErr;



	// Check our state
	if (mIterator != NULL)
		return(noErr);
	
	if (mRemaining.empty())
		return(errFSNoMoreItems);



	// Create the next iterator
	theStart = extract_back(mRemaining);
	theFSRef = theStart.theFile.GetFSRef();
	
	theErr = FSOpenIterator(&theFSRef, kFSIterateFlat, &mIterator);
	NN_ASSERT_NOERR(theErr);



	// Update our state
	mDepth = theStart.theDepth;
	
	return(theErr);
}





//============================================================================
//		NFileIterator::DestroyIterator : Destroy the iterator.
//----------------------------------------------------------------------------
void NFileIterator::DestroyIterator(void)
{	OSStatus	theErr;



	// Destroy the iterator
	if (mIterator != NULL)
		{
		theErr    = FSCloseIterator(mIterator);
		mIterator = NULL;
		
		NN_ASSERT_NOERR(theErr);
		}
}





//============================================================================
//		NFileIterator::ProcessResults : Process the results.
//----------------------------------------------------------------------------
void NFileIterator::ProcessResults(UInt32					numItems,
									const FSRef				*theFSRefs,
									const FSCatalogInfo		*theInfo,
									const HFSUniStr255		*theNames)
{	bool					isMatch, canRecurse, matchFile, matchPath;
	IteratorFilterInfo		filterInfo;
	IteratorStart			theStart;
	NFile					theFile;
	UInt32					n;



	// Validate our parameters
	if (mPatternName.IsNotEmpty())
		NN_ASSERT(theNames != NULL);



	// Get the state we need
	matchFile = mPatternName.IsNotEmpty();
	matchPath = mPatternPath.IsNotEmpty();



	// Process the results
	for (n = 0; n < numItems; n++)
		{
		// Get the state we need
		theFile.SetFSRef(theFSRefs[n]);

		filterInfo.theFile = theFile;
		filterInfo.theInfo = &theInfo[n];

		isMatch    = true;
		canRecurse = true;



		// Collect the matches
		if (isMatch && matchFile)
			isMatch = NString(theNames[n]).Contains(mPatternName, kCFComparePatternNoCase);

		if (isMatch && matchPath)
			isMatch = theFile.GetPath().Contains(mPatternName, kCFComparePatternNoCase);
		
		if (isMatch && mItemFilterProc != NULL)
			{
			filterInfo.userData = mItemFilterData;
			isMatch             = mItemFilterProc(mStopNow, filterInfo);
			}
			
		if (isMatch)
			{
			mResultFiles.push_back(theFile);
			mResultInfo.push_back(theInfo[n]);
			}
		
		
		
		// Collect directories for later
		if (theInfo[n].nodeFlags & kFSNodeIsDirectoryMask)
			{
			theStart.theFile  = theFile;
			theStart.theDepth = mDepth + 1;
			
			if (canRecurse && mRecurseLimit != 0)
				canRecurse = (theStart.theDepth >= mRecurseLimit);
			
			if (canRecurse && mRecurseFilterProc != NULL)
				{
				filterInfo.userData = mRecurseFilterData;
				canRecurse          = mRecurseFilterProc(mStopNow, filterInfo);
				}

			if (canRecurse)
				mRemaining.push_back(theStart);
			}
		
		
		
		// Stop the search
		if (mStopNow)
			break;
		}
}


