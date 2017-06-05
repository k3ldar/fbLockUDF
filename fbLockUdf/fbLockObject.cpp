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

#include <time.h>

#include "fbLockObject.h"

namespace FBServerLock
{
	bool fbLockObject::getIsExpired()
	{
		if (_canDispose)
			return (true);

		time_t current;
		time(&current);
		double seconds = difftime(current, _startTime);
		_canDispose = seconds > _maximumAge;
		return (_canDispose);
	}

	void fbLockObject::setIsExpired(bool isExpired)
	{
		_canDispose = isExpired;
	}

	bool fbLockObject::canDelete()
	{
		return (_isManaged);
	}

	long fbLockObject::getTransactionID()
	{
		return (_transactionID);
	}

	fbLockObject::fbLockObject(HANDLE mutexHandle, ULONG32 maxAge)
	{
		time(&_startTime);
		_mutexHandle = mutexHandle;
		_maximumAge = static_cast<double>(maxAge);
		_canDispose = false;
	}

	fbLockObject::fbLockObject(HANDLE mutexHandle, ULONG32 maxAge, std::string name)
		: fbLockObject(mutexHandle, maxAge)
	{
		mutexName = std::string(name);
		_isManaged = true;
	}

	fbLockObject::~fbLockObject()
	{
		if (_canDispose && _mutexHandle != INVALID_HANDLE_VALUE)
		{
			ReleaseMutex(_mutexHandle);
			CloseHandle(_mutexHandle);
			_mutexHandle = INVALID_HANDLE_VALUE;
		}
	}

	
}