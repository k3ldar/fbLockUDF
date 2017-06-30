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

namespace FBServerLock
{
	std::mutex globalListLock;

	fbLockList::fbLockList()
	{

	}

	fbLockList::fbLockList(DWORD maxItems)
		:fbLockList()
	{
		maxItems = maxItems;
	}

	fbLockList::fbLockList(DWORD maxItems, DWORD timeout)
		: fbLockList(maxItems)
	{
		maxWait = timeout;
	}

	fbLockList::~fbLockList()
	{
		std::lock_guard<std::mutex> guard(globalListLock);

		_lockObjects.clear();
	}

	void fbLockList::setMaxItems(unsigned int total)
	{
		if (total < 1 || total > 100)
			return;

		maxItems = total;
	}

	void fbLockList::cleanItems()
	{
		std::lock_guard<std::mutex> guard(globalListLock);

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

		std::lock_guard<std::mutex> guard(globalListLock);

		if (_lockObjects.size() > maxItems)
			return (LR_MAX_ITEMS_EXCEEDED);

		for (std::list<fbLockObject>::iterator i = _lockObjects.begin(); i != _lockObjects.end(); i++)
		{
			if (i->lockName.compare(name) == 0)
			{
				return (LR_NAME_EXISTS);
			}
		}

		_lockObjects.push_front(fbLockObject(maxAge, name));

		return (LR_SUCCESS);
	}

	int fbLockList::remove(const std::string &name)
	{
		cleanItems();
		std::lock_guard<std::mutex> guard(globalListLock);

		for (std::list<fbLockObject>::iterator i = _lockObjects.begin(); i != _lockObjects.end(); i++)
		{
			if (i->lockName.compare(name) == 0)
			{
				_lockObjects.erase(i);
				return (LR_SUCCESS);
			}
		}

		return (LR_INVALID_NAME);
	}

	int fbLockList::clearAll()
	{
		std::lock_guard<std::mutex> guard(globalListLock);
		_lockObjects.clear();

		return (LR_SUCCESS);
	}

	int fbLockList::getLockCount()
	{
		cleanItems();

		std::lock_guard<std::mutex> guard(globalListLock);

		return (_lockObjects.size());
	}
}