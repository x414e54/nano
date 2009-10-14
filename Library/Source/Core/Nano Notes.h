/*	NAME:
		Nano Notes.h

	DESCRIPTION:
		Nano release notes.
		
		This file contains information about important changes between
		releases of Nano, particularly for changes that will not produce
		a compiler or linker error.
		
		To turn off these messages, your prefix header should include a
		definition of NN_SEEN_RELEASE_NOTES_FOR that matches the current
		version of Nano.
		
		For example:
		
		    #define NN_SEEN_RELEASE_NOTES_FOR			0x00100300
	
		By defining this prior to including Nano.h, the warnings related to
		this (or older) versions of Nano can be suppressed while warnings
		for future versions of Nano will still be seen.
	
	COPYRIGHT:
		Copyright (c) 2006-2007, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_NOTES_HDR
#define NANO_NOTES_HDR
//============================================================================
//		Build Constants
//----------------------------------------------------------------------------
#define NN_RELEASE_NOTES_1_3									0x00100300
#define NN_RELEASE_NOTES_1_2									0x00100200
#define NN_RELEASE_NOTES_1_1									0x00100100

#define NN_RELEASE_NOTES_SHOW_NONE								0xFFFFFFFF
#define NN_RELEASE_NOTES_SHOW_ALL								0x00000000

#ifndef NN_SEEN_RELEASE_NOTES_FOR
#define NN_SEEN_RELEASE_NOTES_FOR								NN_RELEASE_NOTES_SHOW_ALL
#endif





//============================================================================
//		Release Notes
//----------------------------------------------------------------------------
#if NN_SEEN_RELEASE_NOTES_FOR < NN_RELEASE_NOTES_1_3

	// NString
	//
	// See NString.h
	#warning "1.3: NString::EqualTo is now case-INsensitive by default"
	#warning "1.3: NString::IsMatch        replaced with NString::Contains(kCFComparePattern)"
	#warning "1.3: NString::FindPattern    replaced with NString::FindAll( kCFComparePattern)"
	#warning "1.3: NString::TrimWhitespace replaced with NString::Trim()"


	// NUTI
	//
	// See NUTI.h
	#warning "1.3: NUTI::EqualTo replaced with operator =="


	// NCFObject
	//
	// See NCFObject.h
	#warning "1.3: NCFObject now provides copy-on-write; review any custom sub-classes of NCFObject"


	// NTextUtilities
	//
	// See NTextUtilities.h
	#warning "1.3: NTextUtilities Get/FromUInt32 replaced with SInt64"


	// NSystemUtilities
	//
	// See NSTLUtilities.h
	#warning "1.3: CREATE_VECTOR and STL helpers moved from NSystemUtilities to NSTLUtilities"


	// NTimerTask
	//
	// See NTimer.h
	#warning "1.3: NTimerTask replaced wth functor-based NTimer"


	// NAutoReleasePool
	//
	// See NAppKitUtilities.h
	#warning "1.3: NAutoReleasePool replaced wth StAutoReleasePool"

#endif



#if NN_SEEN_RELEASE_NOTES_FOR < NN_RELEASE_NOTES_1_2

	// PrefPanelInfo
	//
	// See NWindowPanels.h
	#warning "1.2: PrefPanelInfo renamed to PanelInfo to support NWindowPanels; rename usage to match"


	// NCFDate
	//
	// See NDate.h
	#warning "1.2: NCFDate was replaced with NDate"
	
	
	// NEditText
	//
	// See NEditText.h
	#warning "1.2: NEditText::[GS]etSelection now uses CFRange rather than ControlEditTextSelectionRec"


	// NSystemUtilities
	//
	// See NSystemUtilities.h
	#warning "1.2: NSystemUtilities::ShowEffect moved to NThemeUtilities"


	// NString
	//
	// See NString.h
	#warning "1.2: NString::Find/FindAll now treats range length of -1 as end-of-string (was 0)"


	// Projects
	#warning "1.2: Add Security.framework, SystemConfiguration.framework, WebKit.framework, and Nano.nib to your project - required for NSoftwareUpdate"


	// Properties
	//
	// See NApplication::InitializeProperties.
	#warning "1.2: NApplication::InitializeProperties now adds kAppDefaultPrefsKey, update rather than replacing this dictionary in your InitializeProperties"

#endif



#if NN_SEEN_RELEASE_NOTES_FOR < NN_RELEASE_NOTES_1_1

	// kHICommandNToggleToolbar
	//
	// See NCarbonEventHandler.h.
	#warning "1.1: kHICommandNToggleToolbar changed from 'Ntog' to 'tbtg' to match 10.5; update .nib usage"

#endif






#endif // NANO_NOTES_HDR


