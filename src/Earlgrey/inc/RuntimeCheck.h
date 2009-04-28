#pragma once
//! \ref http://msdn.microsoft.com/ko-kr/library/s9chhb36.aspx

/* 
* Copyright (c) Microsoft Corporation.  All rights reserved.
*
* Demonstration of the use of the Run-Time Error Checking (RTC) API without 
* using the C Run-Time Library (CRT).
*
*/


#ifndef _rtcsample_h
#define _rtcsample_h

#include <rtcapi.h>
#include <windows.h>

#ifdef NOCRT
#include <strsafe.h>
#endif

namespace Earlgrey
{
	// 예제용으로 있떤 함수는 제거한다.
	/*
	extern int GetVal(void);
	extern void doSomething(short val);
	extern void LoseInfoInIntToShortCast(void);
	extern void thrashVar(int *i);
	extern void TrashStackVariable(void);
	extern void UseUninitializedVariable(void);
	*/
	extern void DoRtcInitialization(void);
	extern void DoRtcTermination(void);

	#ifdef __MSVC_RUNTIME_CHECKS
	#ifdef NOCRT
	extern wchar_t *IntToString(int i);
	#endif
	extern int Catch_RTC_Failure(int errType, const wchar_t *file, int line, const wchar_t *module, const wchar_t *format, ...);
	#endif
}

#endif // _rtcsample_h
