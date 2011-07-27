/*	NAME:
		NWinThread.cpp

	DESCRIPTION:
		Windows thread support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <intrin.h>

#include "NWindows.h"
#include "NWinTarget.h"
#include "NTargetThread.h"





//============================================================================
//		Internal constants
//----------------------------------------------------------------------------
static const UINT WM_INVOKEMAINTHREADFUNCTORS						= WM_USER + 1;





//============================================================================
//		Internal types
//----------------------------------------------------------------------------
// R/W lock info
//
// Based on Stone Steps BSD R/W lock:
//
//	http://forums.stonesteps.ca/thread.asp?t=105
typedef struct {
	CRITICAL_SECTION	theLock;
	HANDLE				eventRead;
	HANDLE				eventWrite;

	NIndex				readersActive;
	NIndex				readersWaiting;

	NIndex				writersActive;
	NIndex				writersWaiting;
} RWLockInfo;





//============================================================================
//		Internal class declaration
//----------------------------------------------------------------------------
class NFunctorInvoker {
public:
										NFunctorInvoker(const NFunctor &theFunctor, NSemaphore *theSemaphore=NULL);
	virtual							   ~NFunctorInvoker(void) { }


	// Invoke the functor
	void								Invoke(void);


private:
	NFunctor							mFunctor;
	NSemaphore						   *mSemaphore;
};





//============================================================================
//		Function prototypes
//----------------------------------------------------------------------------
static HWND CreateMainThreadWindow(void);





//============================================================================
//		Internal globals
//----------------------------------------------------------------------------
// Main thread
//
// This assumes that the main thread performs static initialisation.
static DWORD				gMainThreadID     = GetCurrentThreadId();
static HWND					gMainThreadWindow = CreateMainThreadWindow();
static ThreadFunctorList	gMainThreadFunctors;





//============================================================================
//		NFunctorInvoker::NFunctorInvoker : Constructor
//----------------------------------------------------------------------------
NFunctorInvoker::NFunctorInvoker(const NFunctor &theFunctor, NSemaphore *theSemaphore)
{


	// Initialise ourselves
	mFunctor   = theFunctor;
	mSemaphore = theSemaphore;
}





//============================================================================
//		NFunctorInvoker::Invoke : Invoke the functor.
//----------------------------------------------------------------------------
void NFunctorInvoker::Invoke(void)
{


	// Invoke the functor
	mFunctor();
	
	if (mSemaphore != NULL)
		mSemaphore->Signal();

	delete this;
}





//============================================================================
//		Internal functions
//----------------------------------------------------------------------------
//		ThreadEntry : Thread entry point.
//----------------------------------------------------------------------------
static DWORD WINAPI ThreadEntry(void *userData)
{	NFunctorInvoker		*theInvoker;



	// Invoke the thread
	theInvoker = (NFunctorInvoker *) userData;
	theInvoker->Invoke();
	
	return(0);
}





//============================================================================
//		InvokeMainThreadFunctors : Invoke the main-thread functors.
//----------------------------------------------------------------------------
static void InvokeMainThreadFunctors(void)
{	static bool			sIsInvoking = false;

	NFunctor			theFunctor;



	// Validate our state
	NN_ASSERT(NTargetThread::ThreadIsMain());



	// Invoke the functors
	//
	// If a main thread functor invokes NThread::Sleep (perhaps by waiting for
	// a lock) then we can be called recursively.
	//
	// This can lead to deadlocks, as we can try and call a functor which needs
	// to acquire a lock while executing another functor which also needs to
	// acquire the same lock.
	//
	// If the lock is meant to synchronise the main thread with a worker thread,
	// this can mean the main thread deadlocks itself.
	if (!sIsInvoking)
		{
		sIsInvoking = true;
		
		while (gMainThreadFunctors.PopFront(theFunctor))
			theFunctor();
		
		sIsInvoking = false;
		}
}





//============================================================================
//		HelperWindowProc : Helper window proc.
//----------------------------------------------------------------------------
static LRESULT CALLBACK HelperWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{	LRESULT		theResult;



	// Handle the message
	theResult = 0;

	switch (uMsg) {
		case WM_INVOKEMAINTHREADFUNCTORS:
			InvokeMainThreadFunctors();
			break;

		default:
			theResult = DefWindowProc(hWnd, uMsg, wParam, lParam);
			break;
		}

	return(theResult);
}





//============================================================================
//		CreateMainThreadWindow : Create a window in the main thread.
//----------------------------------------------------------------------------
static HWND CreateMainThreadWindow(void)
{	WNDCLASSEX		classInfo;
	HWND			theWindow;
	ATOM			theClass;



	// Register the class
	memset(&classInfo, 0x00, sizeof(classInfo));

	classInfo.cbSize		= sizeof(WNDCLASSEX);
	classInfo.style			= CS_GLOBALCLASS;
	classInfo.lpfnWndProc	= HelperWindowProc;
	classInfo.cbClsExtra	= 0;
	classInfo.cbWndExtra	= 0;
	classInfo.hInstance		= NULL;
	classInfo.hIcon			= NULL;
	classInfo.hCursor		= NULL;
	classInfo.hbrBackground	= NULL;
	classInfo.lpszMenuName	= NULL;
	classInfo.lpszClassName	= L"Nano::NWinThread";
	classInfo.hIconSm		= NULL;

	theClass = RegisterClassEx(&classInfo);
	NN_ASSERT(theClass != NULL);



	// Create the window
	theWindow = CreateWindow((LPCTSTR) theClass, 0, 0, 0, 0, 0, 0, NULL, NULL, NULL, NULL);
	NN_ASSERT(theWindow != NULL);

	return(theWindow);
}





//============================================================================
//		RWReadLock : Acquire a read lock on a RW lock.
//----------------------------------------------------------------------------
static NStatus RWReadLock(RWLockInfo *lockInfo, NTime waitFor)
{	bool		mustWait;
	DWORD		timeMS;



	// Get the state we need
	timeMS   = NWinTarget::ConvertTimeMS(waitFor);
	mustWait = false;



	// Acquire the lock
	do
		{
		// Try and acquire the lock
		EnterCriticalSection(&lockInfo->theLock);

		if (lockInfo->writersActive == 0 && lockInfo->writersWaiting == 0)
			{
			if (mustWait)
				{
				lockInfo->readersWaiting--;
				mustWait = false;
				}

			lockInfo->readersActive++;
			}

		else
			{
			if (!mustWait)
				{
				lockInfo->readersWaiting++;
				mustWait = true;
				}
			
			ResetEvent(lockInfo->eventRead);
			}

		LeaveCriticalSection(&lockInfo->theLock);



		// Wait for the lock
		if (mustWait)
			{
			if (WaitForSingleObject(lockInfo->eventRead, timeMS) != WAIT_OBJECT_0)
				{
				EnterCriticalSection(&lockInfo->theLock);

				lockInfo->readersWaiting--;
				SetEvent(lockInfo->eventRead);
				SetEvent(lockInfo->eventWrite);

				LeaveCriticalSection(&lockInfo->theLock);
				return(kNErrTimeout);
				}
			}
		}
	while (mustWait);
	
	return(kNoErr);
}





//============================================================================
//		RWReadUnlock : Release a read lock on a RW lock.
//----------------------------------------------------------------------------
static void RWReadUnlock(RWLockInfo *lockInfo)
{


	// Release the lock
	EnterCriticalSection(&lockInfo->theLock);

	lockInfo->readersActive--;

	if (lockInfo->writersWaiting != 0)
		SetEvent(lockInfo->eventWrite);

	else if (lockInfo->readersWaiting != 0)
		SetEvent(lockInfo->eventRead);

	LeaveCriticalSection(&lockInfo->theLock);
}





//============================================================================
//		RWWriteLock : Acquire a write lock on a RW lock.
//----------------------------------------------------------------------------
static NStatus RWWriteLock(RWLockInfo *lockInfo, NTime waitFor)
{	bool		mustWait;
	DWORD		timeMS;



	// Get the state we need
	timeMS   = NWinTarget::ConvertTimeMS(waitFor);
	mustWait = false;



	// Acquire the lock
	do
		{
		// Try and acquire the lock
		EnterCriticalSection(&lockInfo->theLock);

		if (lockInfo->readersActive == 0 && lockInfo->writersActive == 0)
			{
			if (mustWait)
				{
				lockInfo->writersWaiting--;
				mustWait = false;
				}

			lockInfo->writersActive++;
			}
		else
			{
			if (!mustWait)
				{
				lockInfo->writersWaiting++;
				mustWait = true;
				}

			ResetEvent(lockInfo->eventWrite);
			}

		LeaveCriticalSection(&lockInfo->theLock);



		// Wait for the lock
		if (mustWait)
			{
			if (WaitForSingleObject(lockInfo->eventWrite, timeMS) != WAIT_OBJECT_0)
				{
				EnterCriticalSection(&lockInfo->theLock);
				
				lockInfo->writersWaiting--;
				SetEvent(lockInfo->eventRead);
				SetEvent(lockInfo->eventWrite);
				
				LeaveCriticalSection(&lockInfo->theLock);
				return(kNErrTimeout);
				}
			}
		}
	while (mustWait);

	return(kNoErr);
}





//============================================================================
//		RWWriteUnlock : Release a write lock on a RW lock.
//----------------------------------------------------------------------------
static void RWWriteUnlock(RWLockInfo *lockInfo)
{


	// Release the lock
	EnterCriticalSection(&lockInfo->theLock);

	lockInfo->writersActive--;

	if (lockInfo->writersWaiting != 0)
		SetEvent(lockInfo->eventWrite);

	else if (lockInfo->readersWaiting != 0)
		SetEvent(lockInfo->eventRead);

	LeaveCriticalSection(&lockInfo->theLock);
}





//============================================================================
//		NTargetThread::GetCPUCount : Get the number of CPUs.
//----------------------------------------------------------------------------
#pragma mark -
UInt32 NTargetThread::GetCPUCount(void)
{	SYSTEM_INFO		sysInfo;



	// Get the CPU count
	GetSystemInfo(&sysInfo);
	
	return(sysInfo.dwNumberOfProcessors);
}





//============================================================================
//		NTargetThread::AtomicCompareAndSwap32 : Atomic 32-bit compare-and-swap.
//----------------------------------------------------------------------------
bool NTargetThread::AtomicCompareAndSwap32(UInt32 &theValue, UInt32 oldValue, UInt32 newValue)
{


	// Validate our parameters and state
	NN_ASSERT_ALIGNED_4(&theValue);
	NN_ASSERT(sizeof(UInt32) == sizeof(LONG));
	


	// Compare and swap
	return(InterlockedCompareExchange((LONG *) &theValue, newValue, oldValue) == (LONG) oldValue);
}





//============================================================================
//		NTargetThread::AtomicAdd32 : Atomic 32-bit add.
//----------------------------------------------------------------------------
void NTargetThread::AtomicAdd32(SInt32 &theValue, SInt32 theDelta)
{


	// Validate our parameters and state
	NN_ASSERT_ALIGNED_4(&theValue);
	NN_ASSERT(sizeof(UInt32) == sizeof(LONG));
	


	// Add the value
	InterlockedExchangeAdd((LONG *) &theValue, theDelta);
}





//============================================================================
//		NTargetThread::AtomicAnd32 : Atomic 32-bit and.
//----------------------------------------------------------------------------
void NTargetThread::AtomicAnd32(UInt32 &theValue, UInt32 theMask)
{


	// Validate our parameters and state
	NN_ASSERT_ALIGNED_4(&theValue);
	NN_ASSERT(sizeof(UInt32) == sizeof(LONG));
	


	// AND the value
	_InterlockedAnd((LONG *) &theValue, theMask);
}





//============================================================================
//		NTargetThread::AtomicXor32 : Atomic 32-bit xor.
//----------------------------------------------------------------------------
void NTargetThread::AtomicXor32(UInt32 &theValue, UInt32 theMask)
{


	// Validate our parameters and state
	NN_ASSERT_ALIGNED_4(&theValue);
	NN_ASSERT(sizeof(UInt32) == sizeof(LONG));
	


	// XOR the value
	_InterlockedXor((LONG *) &theValue, theMask);
}





//============================================================================
//		NTargetThread::AtomicOr32 : Atomic 32-bit or.
//----------------------------------------------------------------------------
void NTargetThread::AtomicOr32(UInt32 &theValue, UInt32 theMask)
{


	// Validate our parameters and state
	NN_ASSERT(NN_ALIGNED_TO(theValue, sizeof(UInt32)));
	NN_ASSERT(sizeof(UInt32) == sizeof(LONG));
	


	// OR the value
	_InterlockedOr((LONG *) &theValue, theMask);
}





//============================================================================
//		NTargetThread::ThreadIsMain : Is this the main thread?
//----------------------------------------------------------------------------
bool NTargetThread::ThreadIsMain(void)
{


	// Check our state
	return(GetCurrentThreadId() == gMainThreadID);
}





//============================================================================
//		NTargetThread::ThreadSleep : Sleep the current thread.
//----------------------------------------------------------------------------
void NTargetThread::ThreadSleep(NTime theTime)
{


	// Sleep the thread
	Sleep(NWinTarget::ConvertTimeMS(theTime));



	// Invoke the functors
	//
	// Sleeping the main thread will prevent functors due to be executed on the
	// main thread from firing.
	//
	// To avoid deadlocks where the main thread is waiting for a thread to exit
	// and that thread is waiting inside InvokeMain for a functor to complete,
	// sleeping the main thread will also invoke any queued functors.
	if (ThreadIsMain())
		InvokeMainThreadFunctors();
}





//============================================================================
//		NTargetThread::ThreadCreate : Create a thread.
//----------------------------------------------------------------------------
NStatus NTargetThread::ThreadCreate(const NFunctor &theFunctor)
{	NFunctorInvoker		*theInvoker;
	NStatus				theErr;



	// Get the state we need
	theInvoker = new NFunctorInvoker(theFunctor);
	theErr     = kNoErr;



	// Create the thread
	if (CreateThread(NULL, 0, ThreadEntry, theInvoker, 0, NULL) == NULL)
		theErr = NWinTarget::GetLastError();
	
	return(theErr);
}





//============================================================================
//		NTargetThread::ThreadInvokeMain : Invoke the main thread.
//----------------------------------------------------------------------------
void NTargetThread::ThreadInvokeMain(const NFunctor &theFunctor)
{	BOOL				wasOK, wasDone;
	NSemaphore			theSemaphore;
	NFunctorInvoker		*theInvoker;



	// Invoke the functor
	if (ThreadIsMain())
		theFunctor();
	
	
	// Pass it to the main thread
	//
	// An invoker is used to invoke the functor then set our semaphore.
	//
	// The invoker is executed by the main thread, via gMainThreadFunctors,
	// either due to the main thread being blocked in ThreadSleep or due
	// to the event loop running as normal and processing the message.
	else
		{
		theInvoker = new NFunctorInvoker(theFunctor, &theSemaphore);
		gMainThreadFunctors.PushBack(BindMethod(theInvoker, NFunctorInvoker::Invoke));
		
		wasOK = PostMessage(gMainThreadWindow, WM_INVOKEMAINTHREADFUNCTORS, 0, 0);
		NN_ASSERT(wasOK);

		wasDone = theSemaphore.Wait();
		NN_ASSERT(wasDone);
		}
}





//============================================================================
//		NTargetThread::LocalCreate : Create a thread-local value.
//----------------------------------------------------------------------------
NThreadLocalRef NTargetThread::LocalCreate(void)
{	DWORD	keyRef;



	// Validate our state
	NN_ASSERT(sizeof(DWORD) <= sizeof(NThreadLocalRef));



	// Create the key
	keyRef = TlsAlloc();
	
	return((NThreadLocalRef) keyRef);
}





//============================================================================
//		NTargetThread::LocalDestroy : Destroy a thread-local value.
//----------------------------------------------------------------------------
void NTargetThread::LocalDestroy(NThreadLocalRef theKey)
{	DWORD		keyRef = (DWORD) theKey;
	BOOL		wasOK;



	// Destroy the key
	wasOK = TlsFree(keyRef);
	NN_ASSERT(wasOK);
}





//============================================================================
//		NTargetThread::LocalGetValue : Get a thread-local value.
//----------------------------------------------------------------------------
void *NTargetThread::LocalGetValue(NThreadLocalRef theKey)
{	DWORD		keyRef = (DWORD) theKey;
	void		*theValue;



	// Get the value
	theValue = TlsGetValue(keyRef);

	return(theValue);
}





//============================================================================
//		NTargetThread::LocalSetValue : Set a thread-local value.
//----------------------------------------------------------------------------
void NTargetThread::LocalSetValue(NThreadLocalRef theKey, void *theValue)
{	DWORD		keyRef = (DWORD) theKey;
	BOOL		wasOK;



	// Set the value
	wasOK = TlsSetValue(keyRef, theValue);
	NN_ASSERT(wasOK);
}





//============================================================================
//		NTargetThread::SemaphoreCreate : Create a semaphore.
//----------------------------------------------------------------------------
NSemaphoreRef NTargetThread::SemaphoreCreate(NIndex theValue)
{	HANDLE		semHnd;



	// Create the semaphore
	semHnd = CreateSemaphore(NULL, theValue, kNIndexMax, NULL);
	NN_ASSERT(semHnd != NULL);
	
	return((NSemaphoreRef) semHnd);
}





//============================================================================
//		NTargetThread::SemaphoreDestroy : Destroy a semaphore.
//----------------------------------------------------------------------------
void NTargetThread::SemaphoreDestroy(NSemaphoreRef theSemaphore)
{	HANDLE		semHnd = (HANDLE) theSemaphore;



	// Destroy the semaphore
	CloseHandle(semHnd);
}





//============================================================================
//		NTargetThread::SemaphoreSignal : Signal a semaphore.
//----------------------------------------------------------------------------
void NTargetThread::SemaphoreSignal(NSemaphoreRef theSemaphore)
{	HANDLE		semHnd = (HANDLE) theSemaphore;
	BOOL		wasOK;



	// Signal the semaphore
	wasOK = ReleaseSemaphore(semHnd, 1, NULL);
	NN_ASSERT(wasOK);
}





//============================================================================
//		NTargetThread::SemaphoreWait : Wait for a semaphore.
//----------------------------------------------------------------------------
NStatus NTargetThread::SemaphoreWait(NSemaphoreRef theSemaphore, NTime waitFor)
{	HANDLE		semHnd = (HANDLE) theSemaphore;
	DWORD		timeMS, theResult;



	// Get the state we need
	timeMS = NWinTarget::ConvertTimeMS(waitFor);



	// Wait for the semaphore
	theResult = WaitForSingleObject(semHnd, timeMS);
	NN_ASSERT(theResult == WAIT_OBJECT_0 || theResult == WAIT_TIMEOUT);

	return(theResult == WAIT_OBJECT_0 ? kNoErr : kNErrTimeout);
}





//============================================================================
//      NTargetThread::MutexCreate : Create a mutex lock.
//----------------------------------------------------------------------------
NLockRef NTargetThread::MutexCreate(void)
{	HANDLE		lockHnd;



	// Create the lock
	lockHnd = CreateMutex(NULL, false, NULL);
	
	return((NLockRef) lockHnd);
}





//============================================================================
//      NTargetThread::MutexDestroy : Destroy a mutex lock.
//----------------------------------------------------------------------------
void NTargetThread::MutexDestroy(NLockRef theLock)
{	HANDLE		lockHnd = (HANDLE) theLock;



	// Destroy the lock
	CloseHandle(lockHnd);
}





//============================================================================
//      NTargetThread::MutexLock : Lock a mutex lock.
//----------------------------------------------------------------------------
NStatus NTargetThread::MutexLock(NLockRef theLock, NTime waitFor)
{	HANDLE		lockHnd = (HANDLE) theLock;
	DWORD		timeMS, theResult;



	// Get the state we need
	timeMS = NWinTarget::ConvertTimeMS(waitFor);



	// Wait for the semaphore
	theResult = WaitForSingleObject(lockHnd, timeMS);
	NN_ASSERT(theResult == WAIT_OBJECT_0 || theResult == WAIT_TIMEOUT);

	return(theResult == WAIT_OBJECT_0 ? kNoErr : kNErrTimeout);
}





//============================================================================
//      NTargetThread::MutexUnlock : Unlock a mutex lock.
//----------------------------------------------------------------------------
void NTargetThread::MutexUnlock(NLockRef theLock)
{	HANDLE		lockHnd = (HANDLE) theLock;
	BOOL		wasOK;



	// Release the lock
	wasOK = ReleaseMutex(lockHnd);
	NN_ASSERT(wasOK);
}





//============================================================================
//      NTargetThread::ReadWriteCreate : Create a read-write lock.
//----------------------------------------------------------------------------
NLockRef NTargetThread::ReadWriteCreate(void)
{	RWLockInfo		*lockInfo;



	// Create the lock
	lockInfo = new RWLockInfo;
	if (lockInfo == NULL)
		return(kNLockRefNone);



	// Initialize the lock
	InitializeCriticalSection(&lockInfo->theLock);
	
	lockInfo->eventRead  = CreateEvent(NULL, false, true, NULL);
	lockInfo->eventWrite = CreateEvent(NULL, false, true, NULL);
	
	lockInfo->readersActive  = 0;
	lockInfo->readersWaiting = 0;

	lockInfo->writersActive  = 0;
	lockInfo->writersWaiting = 0;

	return((NLockRef) lockInfo);
}





//============================================================================
//      NTargetThread::ReadWriteDestroy : Destroy a read-write lock.
//----------------------------------------------------------------------------
void NTargetThread::ReadWriteDestroy(NLockRef theLock)
{	RWLockInfo		*lockInfo = (RWLockInfo *) theLock;



	// Validate our parameters
	NN_ASSERT(lockInfo->readersActive  == 0);
	NN_ASSERT(lockInfo->readersWaiting == 0);

	NN_ASSERT(lockInfo->writersActive  == 0);
	NN_ASSERT(lockInfo->writersWaiting == 0);



	// Destroy the lock
	WNSafeCloseHandle(lockInfo->eventRead);
	WNSafeCloseHandle(lockInfo->eventWrite);
	DeleteCriticalSection(&lockInfo->theLock);
	
	delete lockInfo;
}





//============================================================================
//      NTargetThread::ReadWriteLock : Lock a read-write lock.
//----------------------------------------------------------------------------
NStatus NTargetThread::ReadWriteLock(NLockRef theLock, bool forRead, NTime waitFor)
{	RWLockInfo		*lockInfo = (RWLockInfo *) theLock;
	NStatus			theErr;



	// Acquire the lock
	if (forRead)
		theErr = RWReadLock( lockInfo, waitFor);
	else
		theErr = RWWriteLock(lockInfo, waitFor);
	
	return(theErr);
}





//============================================================================
//      NTargetThread::ReadWriteUnlock : Unlock a read-write lock.
//----------------------------------------------------------------------------
void NTargetThread::ReadWriteUnlock(NLockRef theLock, bool forRead)
{	RWLockInfo		*lockInfo = (RWLockInfo *) theLock;



	// Release the lock
	if (forRead)
		RWReadUnlock( lockInfo);
	else
		RWWriteUnlock(lockInfo);
}




 