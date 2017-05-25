// headers.hpp
#pragma once

//==============================================================================
// warning

#pragma warning( disable : 4100 ) // warning C4100: 'argument' : �����͊֐��̖{�̕��� 1 �x���Q�Ƃ���܂���B
#pragma warning( disable : 4127 ) // warning C4127: ���������萔�ł��B
#pragma warning( disable : 4201 ) // warning C4201: ��W���̊g���@�\���g�p����Ă��܂� : �����̍\���̂܂��͋��p�̂ł��B
#pragma warning( disable : 4290 ) // warning C4290: C++ �̗�O�̎w��͖�������܂��B�֐��� __declspec(nothrow) �łȂ����Ƃ̂ݕ\������܂��B

//==============================================================================
// system headers

#ifndef _WINDOWS_

/*
#define WINVER			0x0501

#if WINVER > 0x0500
#	define _WIN32_WINDOWS	0x0501
#	define _WIN32_WINNT		0x0501
#	define _WIN32_IE		0x0600
#else
#	define _WIN32_WINDOWS	0x0500
#	define _WIN32_WINNT		0x0500
#	define _WIN32_IE		0x0501
#endif
*/

#define _ATL_ALL_WARNINGS
#define _ATL_NO_OPENGL
#define _ATL_NO_OLD_NAMES
#define _WTL_NO_CSTRING			// CString �ł͂Ȃ��AATL::CStringT ���g�p���邽�߁B
#define _WTL_NO_WTYPES			// CSize, CPoint, CRect �ł͂Ȃ��AATL�̃N���X���g�p���邽�߁B
#define _WTL_NO_UNION_CLASSES	// ATL �ɂ��铯���̃N���X���g�p����B

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN	
#define _CRTDBG_MAP_ALLOC

#endif

//==============================================================================
// check compile options

/*
#if _WIN32_WINDOWS < 0x0500
#	error _WIN32_WINDOWS must be 0x0500 or lator.
#endif
*/
#ifndef _MT
#	error �}���`�X���b�h�ݒ�Ńr���h����K�v������܂��B
#endif

#ifndef _CPPUNWIND
#	error C++��O������L���ɂ���K�v������܂��B
#endif

#ifndef _NATIVE_WCHAR_T_DEFINED
#	error wchar_t ��g�ݍ��݌^�Ƃ��Ĉ����K�v������܂��B
#endif

//==============================================================================
// intrinsic

#include <windows.h>
/*
extern "C"
{
   long  __cdecl _InterlockedIncrement(long volatile *Addend);
   long  __cdecl _InterlockedDecrement(long volatile *Addend);
   long  __cdecl _InterlockedCompareExchange(long* volatile Dest, long Exchange, long Comp);
   long  __cdecl _InterlockedExchange(long* volatile Target, long Value);
   long  __cdecl _InterlockedExchangeAdd(long* volatile Addend, long Value);
}
*/

#pragma intrinsic (_InterlockedCompareExchange)
#define InterlockedCompareExchange _InterlockedCompareExchange

#pragma intrinsic (_InterlockedExchange)
#define InterlockedExchange _InterlockedExchange 

#pragma intrinsic (_InterlockedExchangeAdd)
#define InterlockedExchangeAdd _InterlockedExchangeAdd

#pragma intrinsic (_InterlockedIncrement)
#define InterlockedIncrement _InterlockedIncrement

#pragma intrinsic (_InterlockedDecrement)
#define InterlockedDecrement _InterlockedDecrement

//==============================================================================
// standard headers

#include <cstdio>
#include <cstdlib>
#include <malloc.h>
#include <crtdbg.h>
#include <atlbase.h>
#include <commctrl.h>
#include <commoncontrols.h>

//==============================================================================
// macro

#ifndef MAKEDWORD
#	define MAKEDWORD(ch0, ch1, ch2, ch3) ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) | ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24 ))
#endif

#ifndef GET_X_LPARAM
#	define GET_X_LPARAM(lp)	((int)(short)LOWORD(lp))
#endif
#ifndef GET_Y_LPARAM
#	define GET_Y_LPARAM(lp)	((int)(short)HIWORD(lp))
#endif
#define GET_XY_LPARAM(lp)	GET_X_LPARAM(lp), GET_Y_LPARAM(lp)

#ifdef UNICODE
const bool IS_UNICODE_CHARSET	= true;
#else
const bool IS_UNICODE_CHARSET	= false;
#endif

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __FILE_W__     WIDEN(__FILE__)
#define __FUNCTION_W__ WIDEN(__FUNCTION__)

using REAL32 = float;	///< 32bit��������.
using REAL64 = double;	///< 64bit��������.
