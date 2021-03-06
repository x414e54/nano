/*	NAME:
		NThreadUtilities.h

	DESCRIPTION:
		Thread utilities.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTHREADUTILITIES_HDR
#define NTHREADUTILITIES_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFunctor.h"
#include "NTimer.h"





//============================================================================
//		Constants
//----------------------------------------------------------------------------
static const NTime kNFunctorDelayTime								= 250 * kNTimeMillisecond;





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NThreadUtilities {
public:
	// Get the number of CPUs
	static UInt32						GetCPUCount(void);


	// Detach a functor
	//
	// Invokes the functor on a new thread.
	static void							DetachFunctor(const NFunctor &theFunctor);


	// Delay a functor
	//
	// The functor can be invoked on the main thread, or on a new one-shot thread.
	static void							DelayFunctor(const NFunctor &theFunctor, NTime theDelay=kNFunctorDelayTime, bool onMainThread=true);


	// Atomic compare and swap
	//
	// Inserts a read/write memory barrier.
	//
	// Returns as theValue was updated from oldValue to newValue.
	static bool							AtomicCompareAndSwap32 (SInt32 &theValue, SInt32 oldValue, SInt32 newValue);
	static bool							AtomicCompareAndSwapPtr(void  *&theValue, void  *oldValue, void  *newValue);


	// Atomic addition
	//
	// Inserts a read/write memory barrier.
	//
	// Returns the new value stored in theValue after the addition.
	static SInt32						AtomicAdd32(SInt32 &theValue, SInt32 theDelta);


	// Atomic bit operations
	//
	// Inserts a read/write memory barrier.
	static void							AtomicAnd32(UInt32 &theValue, UInt32 theMask);
	static void							AtomicOr32( UInt32 &theValue, UInt32 theMask);
	static void							AtomicXor32(UInt32 &theValue, UInt32 theMask);


	// Insert a memory barrier
	//
	// Inserts a read/write memory barrier.
	static void							MemoryBarrier(void);


private:
	static void							DelayedFunctor(NTimer *theTimer, const NFunctor &theFunctor, bool onMainThread);
};





#endif // NTHREADUTILITIES_HDR


