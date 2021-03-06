/*	NAME:
		NanoTarget.h

	DESCRIPTION:
		Nano build targets.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_TARGET_HDR
#define NANO_TARGET_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Target
//----------------------------------------------------------------------------
// Baseline
#define NN_TARGET_MAC												0
#define NN_TARGET_WINDOWS											0
#define NN_TARGET_IOS												0
#define NN_TARGET_LINUX												0


// Mac
#if (defined(__APPLE_CPP__) || defined(__APPLE_CC__)) && !defined(__arm__) && !defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
	#undef  NN_TARGET_MAC
	#define NN_TARGET_MAC											1
#endif


// Windows
#if defined(_MSC_FULL_VER)
	#undef  NN_TARGET_WINDOWS
	#define NN_TARGET_WINDOWS										1
#endif


// iOS
#if (defined(__APPLE_CPP__) || defined(__APPLE_CC__)) && (defined(__arm__) || defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__))
	#undef  NN_TARGET_IOS
	#define NN_TARGET_IOS											1
#endif


// Linux
#if defined(__linux__)
	#undef  NN_TARGET_LINUX
	#define NN_TARGET_LINUX											1
#endif


// Validate
#if !NN_TARGET_MAC && !NN_TARGET_WINDOWS && !NN_TARGET_IOS && !NN_TARGET_LINUX
	ERROR - Unable to identify target platform
#endif





//============================================================================
//		Endian-ness
//----------------------------------------------------------------------------
// Baseline
#define NN_TARGET_ENDIAN_BIG										0
#define NN_TARGET_ENDIAN_LITTLE										0


// Mac
#if NN_TARGET_MAC
	#if defined(__ppc__) || defined(__ppc64__)
		#undef  NN_TARGET_ENDIAN_BIG
		#define NN_TARGET_ENDIAN_BIG								1
	#else
		#undef  NN_TARGET_ENDIAN_LITTLE
		#define NN_TARGET_ENDIAN_LITTLE								1
	#endif
#endif


// Windows
#if NN_TARGET_WINDOWS
	#undef  NN_TARGET_ENDIAN_LITTLE
	#define NN_TARGET_ENDIAN_LITTLE									1
#endif


// iOS
#if NN_TARGET_IOS
	#undef  NN_TARGET_ENDIAN_LITTLE
	#define NN_TARGET_ENDIAN_LITTLE									1
#endif


// Linux
#if NN_TARGET_LINUX
	#if defined(__ppc__) || defined(__ppc64__)
		#undef  NN_TARGET_ENDIAN_BIG
		#define NN_TARGET_ENDIAN_BIG								1
	#else
		#undef  NN_TARGET_ENDIAN_LITTLE
		#define NN_TARGET_ENDIAN_LITTLE								1
	#endif
#endif


// Validate
#if NN_TARGET_ENDIAN_BIG == NN_TARGET_ENDIAN_LITTLE
	ERROR - Unable to identify target endian-ness
#endif





//============================================================================
//		Architecture size
//----------------------------------------------------------------------------
// Baseline
#define NN_TARGET_ARCH_64											0
#define NN_TARGET_ARCH_32											0


// Mac
#if NN_TARGET_MAC
	#if defined(__x86_64__) || defined(__ppc64__)
		#undef  NN_TARGET_ARCH_64
		#define NN_TARGET_ARCH_64									1
	#else
		#undef  NN_TARGET_ARCH_32
		#define NN_TARGET_ARCH_32									1
	#endif
#endif


// Windows
#if NN_TARGET_WINDOWS
	#if defined(_WIN64)
		#undef  NN_TARGET_ARCH_64
		#define NN_TARGET_ARCH_64									1
	#else
		#undef  NN_TARGET_ARCH_32
		#define NN_TARGET_ARCH_32									1
	#endif
#endif


// iOS
#if NN_TARGET_IOS
	#undef  NN_TARGET_ARCH_32
	#define NN_TARGET_ARCH_32										1
#endif


// Linux
#if NN_TARGET_LINUX
	#if defined(__x86_64__) || defined(__amd64__)
		#undef  NN_TARGET_ARCH_64
		#define NN_TARGET_ARCH_64									1
	#else
		#undef  NN_TARGET_ARCH_32
		#define NN_TARGET_ARCH_32									1
	#endif
#endif


// Validate
#if NN_TARGET_ARCH_64 == NN_TARGET_ARCH_32
	ERROR - Unable to identify target architecture size
#endif





//============================================================================
//		Compiler
//----------------------------------------------------------------------------
// Baseline
#define NN_TARGET_COMPILER_CLANG									0
#define NN_TARGET_COMPILER_GCC										0
#define NN_TARGET_COMPILER_MSC										0


// Clang
#if defined(__clang__)
	#undef  NN_TARGET_COMPILER_CLANG
	#define NN_TARGET_COMPILER_CLANG								((__clang_major__      * 10000 ) + \
																	 (__clang_minor__      * 100   ) + \
																	 (__clang_patchlevel__ * 1))
#endif


// GCC
//
// Clang also defines __GNUC__
#if defined(__GNUC__) && !defined(__clang__)
	#undef  NN_TARGET_COMPILER_GCC
	#define NN_TARGET_COMPILER_GCC									((__GNUC__            * 10000) + \
																	 (__GNUC_MINOR__      * 100)   + \
																	 (__GNUC_PATCHLEVEL__ * 1))
#endif


// Visual Studio
#if defined(_MSC_FULL_VER)
	#undef  NN_TARGET_COMPILER_MSC
	#define NN_TARGET_COMPILER_MSC									_MSC_FULL_VER
#endif


// Validate
#if (( (NN_TARGET_COMPILER_CLANG!=0) + (NN_TARGET_COMPILER_GCC!=0) + (NN_TARGET_COMPILER_MSC!=0)) == 0)
	ERROR - Unable to identify target compiler
#endif

#if (( (NN_TARGET_COMPILER_CLANG!=0) + (NN_TARGET_COMPILER_GCC!=0) + (NN_TARGET_COMPILER_MSC!=0))  > 1)
	ERROR - Unable to uniquely identify target compiler
#endif





#endif // NANO_TARGET_HDR


