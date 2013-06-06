/*	NAME:
		TUndoManager.cpp

	DESCRIPTION:
		NUndoManager tests.

	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
//============================================================================
//		Include files
//----------------------------------------------------------------------------
#include "NUndoManager.h"
#include "NTestFixture.h"





//============================================================================
//		Test fixture
//----------------------------------------------------------------------------
#define TEST_NUNDOMANAGER(_name, _desc)								NANO_TEST(TUndoManager, _name, _desc)

NANO_FIXTURE(TUndoManager)
{
	NUndoManager	undoManager;
};





//============================================================================
//		Test case
//----------------------------------------------------------------------------
TEST_NUNDOMANAGER("Empty", "Empty undo manager")
{


	// Perform the test
	REQUIRE(!undoManager.CanUndo());
	REQUIRE(!undoManager.CanRedo());
}





