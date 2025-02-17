/*
*  The contents of this file are subject to the Initial
*  Developer's Public License Version 1.0 (the "License");
*  you may not use this file except in compliance with the
*  License. You may obtain a copy of the License at
*  http://www.ibphoenix.com/idpl.html.
*
*  Software distributed under the License is distributed on
*  an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either
*  express or implied.  See the License for the specific
*  language governing rights and limitations under the License.
*
*  The Original Code was created by Simon Carter (s1cart3r@gmail.com).
*
*  Copyright (c) 2017 Simon Carter.  All Rights Reserved.
*/
#include <Windows.h>
#include <winnt.h>
#include <string.h>
#include "fbLockList.h"


#ifndef FB_LOCK
#define FB_LOCK

typedef  unsigned int uInt;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#ifdef FB_LOCK_FUNCTIONS
#define FBUDF_API __declspec(dllexport)
#else
#define FBUDF_API __declspec(dllimport)
#endif
#endif

extern "C" 
{
	FBUDF_API int fbServerLockGet(const char *lockName, int *maxAge);
	FBUDF_API int fbServerLockRel(const char *lockName);
	FBUDF_API int fbServerLockClr();
	FBUDF_API uInt fbGetLockCount();
}

#endif