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

#include "fbLockList.h"
#include "fbLock.h"
#include <string.h>

#ifndef FB_LOCK_FUNCTIONS
#define FB_LOCK_FUNCTIONS
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)

BOOL APIENTRY DllMain( HANDLE ,	DWORD ul_reason_for_call, LPVOID)
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif

namespace FBServerLock
{
	FBServerLock::fbLockList __activeLockList{};
}

FBUDF_API int fbServerLockRel(const char *lockName)
{
	try
	{
		if (!lockName)
			return (FBServerLock::LR_INVALID_LOCK_NAME);

		if (strlen(lockName) > 100)
			return (FBServerLock::LR_INVALID_PARAMETER);

		return (FBServerLock::__activeLockList.remove(std::string(lockName)));
	}
	catch (...)
	{
		return (FBServerLock::LR_FAILED);
	}

	return (FBServerLock::LR_FAILED);
}

FBUDF_API int fbServerLockGet(const char *lockName, int *maxAge)
{
	try
	{
		if (!lockName)
			return (FBServerLock::LR_INVALID_LOCK_NAME);

		if (*maxAge < 0 || *maxAge > 1440)
			return (FBServerLock::LR_INVALID_AGE);

		if (strlen(lockName) > 100)
			return (FBServerLock::LR_INVALID_PARAMETER);

		return (FBServerLock::__activeLockList.add(std::string(lockName), *maxAge));
	}
	catch (...)
	{
		return (FBServerLock::LR_FAILED);
	}

	return (FBServerLock::LR_FAILED);
}

FBUDF_API int fbServerLockGetT(const char *lockName, int *maxAge, long *transactionID)
{
	try
	{
		if (!lockName)
			return (FBServerLock::LR_INVALID_LOCK_NAME);

		if (*maxAge < 0 || *maxAge > 1440)
			return (FBServerLock::LR_INVALID_AGE);

		if (strlen(lockName) > 100)
			return (FBServerLock::LR_INVALID_PARAMETER);

		return (FBServerLock::__activeLockList.add(std::string(lockName), *maxAge, *transactionID));
	}
	catch (...)
	{
		return (FBServerLock::LR_FAILED);
	}

	return (FBServerLock::LR_FAILED);
}

