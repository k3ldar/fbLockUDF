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

#include "fbLockObject.h"
#include "fbLockList.h"

namespace FBServerLock
{
	fbLockList::fbLockList()
	{
		_mutexLockHandle = CreateMutexA(NULL, true, "fb_lock_list_access_mutex");
	}

	fbLockList::fbLockList(DWORD maxItems)
		:fbLockList()
	{
		_maxItems = maxItems;
	}

	fbLockList::fbLockList(DWORD maxItems, DWORD timeout)
		: fbLockList(maxItems)
	{
		_maxWait = timeout;
	}

	fbLockList::~fbLockList()
	{
		for (std::list<fbLockObject>::iterator i = _lockObjects.begin(); i != _lockObjects.end(); i++)
			i->setIsExpired(true);

		_lockObjects.clear();
	}

	void fbLockList::setMaxItems(unsigned int total)
	{
		if (total < 1 || total > 100)
			return;

		_maxItems = total;
	}

	int fbLockList::getItemCount()
	{
		cleanItems();

		int Result = 0;

		for (std::list<fbLockObject>::iterator i = _lockObjects.begin(); i != _lockObjects.end(); i++)
		{
			if (!i->getIsExpired())
				Result++;
		}

		return (Result);
	}

	void fbLockList::cleanItems()
	{
		for (std::list<fbLockObject>::iterator i = _lockObjects.begin(); i != _lockObjects.end(); i++)
		{
			if (i->getIsExpired())
			{
				_lockObjects.erase(i);
				cleanItems();
				return;
			}
		}
	}

	int fbLockList::add(const std::string &name, ULONG32 maxAge)
	{
		cleanItems();

		HANDLE mutexHwnd = CreateMutex(NULL, false, name.c_str());

		switch (GetLastError())
		{
		case ERROR_ALREADY_EXISTS:
			ReleaseMutex(mutexHwnd);
			CloseHandle(mutexHwnd);
			mutexHwnd = INVALID_HANDLE_VALUE;
			return (LR_NAME_EXISTS);
		case ERROR_INVALID_HANDLE:
			return (LR_INVALID_HANDLE);
		case ERROR_ACCESS_DENIED:
			return (LR_ACCESS_DENIED);
		}

		fbLockObject* lockObj = new fbLockObject(mutexHwnd, maxAge, name);
		return (add(*lockObj));
	}

	int fbLockList::add(const fbLockObject &lObj)
	{
		switch (WaitForSingleObject(_mutexLockHandle, _maxWait))
		{
			case WAIT_TIMEOUT:
				return (LR_TIME_OUT);
			case WAIT_FAILED:
				return (LR_WAIT_FAILED);
			case WAIT_ABANDONED:
				return (LR_WAIT_ABANDONED);
		}

		if (_lockObjects.size() > _maxItems)
			return (LR_MAX_ITEMS_EXCEEDED);

		_lockObjects.push_front(lObj);

		ReleaseMutex(_mutexLockHandle);

		return (LR_LOCK_OBTAINED);
	}

	int fbLockList::remove(const std::string &name)
	{
		cleanItems();

		switch (WaitForSingleObject(_mutexLockHandle, _maxWait))
		{
		case WAIT_TIMEOUT:
			return (LR_TIME_OUT);
		case WAIT_FAILED:
			return (LR_WAIT_FAILED);
		case WAIT_ABANDONED:
			return (LR_WAIT_ABANDONED);
		}

		int Result = LR_INVALID_HANDLE;

		for (std::list<fbLockObject>::iterator i = _lockObjects.begin(); i != _lockObjects.end(); i++)
		{
			if (i->mutexName.compare(name) == 0)
			{
				i->setIsExpired(true);
				_lockObjects.erase(i);
				Result = LR_LOCK_OBTAINED;
				break;
			}
		}

		ReleaseMutex(_mutexLockHandle);

		return (Result);
	}

	int fbLockList::clearAll()
	{
		switch (WaitForSingleObject(_mutexLockHandle, _maxWait))
		{
		case WAIT_TIMEOUT:
			return (LR_TIME_OUT);
		case WAIT_FAILED:
			return (LR_WAIT_FAILED);
		case WAIT_ABANDONED:
			return (LR_WAIT_ABANDONED);
		}

		for (std::list<fbLockObject>::iterator i = _lockObjects.begin(); i != _lockObjects.end(); i++)
		{
			i->setIsExpired(true);
		}

		cleanItems();

		ReleaseMutex(_mutexLockHandle);

		return (LR_LOCK_OBTAINED);
	}

	int fbLockList::getLockCount()
	{
		switch (WaitForSingleObject(_mutexLockHandle, _maxWait))
		{
		case WAIT_TIMEOUT:
			return (LR_TIME_OUT);
		case WAIT_FAILED:
			return (LR_WAIT_FAILED);
		case WAIT_ABANDONED:
			return (LR_WAIT_ABANDONED);
		}

		cleanItems();

		int Result = 0;

		for (std::list<fbLockObject>::iterator i = _lockObjects.begin(); i != _lockObjects.end(); i++)
		{
			Result++;
		}

		ReleaseMutex(_mutexLockHandle);

		return (Result);
	}
}