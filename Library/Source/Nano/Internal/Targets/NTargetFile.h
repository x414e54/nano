/*	NAME:
		NTargetFile.h

	DESCRIPTION:
		File support.
	
	COPYRIGHT:
		Copyright (c) 2006-2010, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NTARGET_FILE_HDR
#define NTARGET_FILE_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NFileUtilities.h"
#include "NFileMap.h"
#include "NFile.h"





//============================================================================
//		Class declaration
//----------------------------------------------------------------------------
class NTargetFile {
public:
	// Is this a file?
	static bool							IsFile(const NString &thePath);


	// Is this a directory?
	static bool							IsDirectory(const NString &thePath);


	// Is this a link?
	static bool							IsLink(const NString &thePath);


	// Is a file writeable?
	static bool							IsWriteable(const NString &thePath);


	// Does a file exist?
	static bool							Exists(const NString &thePath);


	// Get/set a file's name
	static NString						GetName(const NString &thePath, bool displayName);
	static NString						SetName(const NString &thePath, const NString &fileName, bool renameFile, bool isPath);


	// Get/set a file's size
	static UInt64						GetSize(const NString &thePath);
	static NStatus						SetSize(const NString &thePath, NFileRef theFile, UInt64 theSize);


	// Get the child/parent of a path
	static NString						GetChild( const NString &thePath, const NString &fileName);
	static NString						GetParent(const NString &thePath);


	// Get the target of a path
	static NString						GetTarget(const NString &thePath);


	// Get the children of a path
	static NFileList					GetChildren(const NString &thePath);
	

	// Delete the file
	static NStatus						Delete(const NString &thePath, bool moveToTrash);


	// Get a directory
	static NFile						GetDirectory(NDirectoryDomain theDomain, NDirectoryLocation theLocation);


	// Create a directory
	static NStatus						CreateDirectory(const NString &thePath);


	// Exchange two files
	static NStatus						ExchangeWith(const NString &srcPath, const NString &dstPath);


	// Unmount a volume
	static NStatus						UnmountVolume(const NString &thePath);


	// Open/close a file
	static NFileRef						Open(const NString &thePath, NFilePermission thePermission);
	static void							Close(NFileRef theFile);


	// Get/set the read/write position
	static UInt64						GetPosition(NFileRef theFile);
	static NStatus						SetPosition(NFileRef theFile, SInt64 theOffset, NFilePosition thePosition);


	// Read/write from a file
	static NStatus						Read( NFileRef theFile, UInt64 theSize,       void *thePtr, UInt64 &numRead,    SInt64 theOffset, NFilePosition thePosition, NFileFlags theFlags);
	static NStatus						Write(NFileRef theFile, UInt64 theSize, const void *thePtr, UInt64 &numWritten, SInt64 theOffset, NFilePosition thePosition, NFileFlags theFlags);


	// Map/unmap a file
	static NFileRef						MapOpen(const NFile &theFile, NMapAccess theAccess);
	static void							MapClose(  NFileRef theFile);
	static void						   *MapFetch(  NFileRef theFile,  NMapAccess theAccess, UInt64 theOffset,   UInt32 theSize, bool noCache);
	static void							MapDiscard(NFileRef theFile,  NMapAccess theAccess, const void *thePtr, UInt32 theSize);
};





#endif // NTARGET_FILE_HDR


