/*	NAME:
		NLinuxSystem.cpp

	DESCRIPTION:
		Linux system support.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include <sys/utsname.h>

#include "NTargetSystem.h"





//============================================================================
//      NTargetSystem::DebugLog : Emit a debug message.
//----------------------------------------------------------------------------
#pragma mark -
void NTargetSystem::DebugLog(const char *theMsg)
{


	// Log the message
	fprintf(stderr, "%s", theMsg);
}





//============================================================================
//      NTargetSystem::FindBundle : Find a bundle.
//----------------------------------------------------------------------------
NFile NTargetSystem::FindBundle(const NString &bundleID)
{


	// dair, to do
	NN_LOG("NTargetSystem::FindBundle not implemented!");
	return(NFile());
}





//============================================================================
//      NTargetSystem::GetOSVersion : Get the OS version.
//----------------------------------------------------------------------------
OSVersion NTargetSystem::GetOSVersion(void)
{	int					sysErr, versMajor, versMinor;
	struct utsname		theInfo;
	OSVersion			theVers;



	// Get the state we need
	sysErr = uname(&theInfo);
	NN_ASSERT_NOERR(sysErr);

	if (sysErr == 0)
		{
		if (!sscanf(theInfo.release, "%d.%d", &versMajor, &versMinor))
			sysErr = -1;
		}



	// Get the version
	theVers = kOSLinux;
	
	if (sysErr == 0)
		{
		if (versMajor == 3 && versMinor == 4)
			theVers = kOSLinuxThreeFour;

		else if (versMajor == 3 && versMinor == 2)
			theVers = kOSLinuxThreeTwo;

		else if (versMajor == 3 && versMinor == 0)
			theVers = kOSLinuxThree;

		else if (versMajor == 2 && versMinor == 6)
			theVers = kOSLinuxTwoSix;

		else if (versMajor == 2 && versMinor == 4)
			theVers = kOSLinuxTwoFour;

		else if (versMajor == 2 && versMinor == 2)
			theVers = kOSLinuxTwoTwo;

		else if (versMajor == 2 && versMinor == 0)
			theVers = kOSLinuxTwo;
		}
	
	return(theVers);
}





//============================================================================
//		NTargetSystem::GetOSName : Get the OS name.
//----------------------------------------------------------------------------
NString NTargetSystem::GetOSName(void)
{	NString				theResult;
	struct utsname		theInfo;
	int					sysErr;



	// Get the state we need
	sysErr = uname(&theInfo);
	NN_ASSERT_NOERR(sysErr);



	// Get the OS name
	if (sysErr == 0)
		theResult.Format("%s %s/%s", theInfo.sysname, theInfo.release, theInfo.version);

	return(theResult);
}





//============================================================================
//		NTargetSystem::GetSystemCPU : Get the clock speed.
//----------------------------------------------------------------------------
UInt64 NTargetSystem::GetSystemCPU(void)
{


	// dair, to do
	NN_LOG("NTargetSystem::GetSystemCPU not implemented!");
	return(0);
}





//============================================================================
//		NTargetSystem::GetSystemRAM : Get the physical memory.
//----------------------------------------------------------------------------
UInt64 NTargetSystem::GetSystemRAM(void)
{


	// dair, to do
	NN_LOG("NTargetSystem::GetSystemRAM not implemented!");
	return(0);
}





//============================================================================
//		NTargetSystem::GetSystemArch : Get the system architecture.
//----------------------------------------------------------------------------
NString NTargetSystem::GetSystemArch(void)
{


	// dair, to do
	NN_LOG("NTargetSystem::GetSystemArch not implemented!");
	return("");
}





//============================================================================
//      NTargetSystem::TaskCreate : Create a task.
//----------------------------------------------------------------------------
TaskInfo NTargetSystem::TaskCreate(const NString &theCmd, const NStringList &theArgs)
{


	// dair, to do
	NN_LOG("NTargetSystem::TaskCreate not implemented!");
	return(TaskInfo());
}





//============================================================================
//      NTargetSystem::TaskDestroy : Destroy a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskDestroy(TaskInfo &theTask)
{


	// dair, to do
	NN_LOG("NTargetSystem::TaskDestroy not implemented!");
}





//============================================================================
//      NTargetSystem::TaskUpdate : Update a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskUpdate(TaskInfo &theTask)
{


	// dair, to do
	NN_LOG("NTargetSystem::TaskUpdate not implemented!");
}





//============================================================================
//      NTargetSystem::TaskRead : Read from a task.
//----------------------------------------------------------------------------
NString NTargetSystem::TaskRead(TaskInfo &theTask, bool stdErr)
{


	// dair, to do
	NN_LOG("NTargetSystem::TaskRead not implemented!");
	return("");
}





//============================================================================
//      NTargetSystem::TaskWrite : Write to a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskWrite(const TaskInfo &theTask, const NString &theText)
{


	// dair, to do
	NN_LOG("NTargetSystem::TaskWrite not implemented!");
}





//============================================================================
//      NTargetSystem::TaskWait : Block for a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskWait(const TaskInfo &/*theTask*/, NTime waitFor)
{


	// dair, to do
	NN_LOG("NTargetSystem::TaskWait not implemented!");
}





//============================================================================
//      NTargetSystem::TaskSignal : Signal a task.
//----------------------------------------------------------------------------
void NTargetSystem::TaskSignal(const TaskInfo &theTask, NTaskSignal theSignal)
{


	// dair, to do
	NN_LOG("NTargetSystem::TaskSignal not implemented!");
}





//============================================================================
//      NTargetSystem::GetLocaleValue : Get a locale value.
//----------------------------------------------------------------------------
NVariant NTargetSystem::GetLocaleValue(const NString &theID, const NString &theKey)
{


	// dair, to do
	NN_LOG("NTargetSystem::GetLocaleValue not implemented!");
	return(0);
}





//============================================================================
//      NTargetSystem::GetLocaleBroadcaster : Get the locale broadcaster.
//----------------------------------------------------------------------------
NBroadcaster *NTargetSystem::GetLocaleBroadcaster(const NString &/*theID*/)
{


	// dair, to do
	NN_LOG("NTargetSystem::GetLocaleBroadcaster not implemented!");
	return(NULL);
}





//============================================================================
//      NTargetSystem::TransformString : Transform a string.
//----------------------------------------------------------------------------
NString NTargetSystem::TransformString(const NString &theString, NStringTransform theTransform)
{


	// dair, to do
	NN_LOG("NTargetSystem::TransformString not implemented!");
	return(theString);
}



