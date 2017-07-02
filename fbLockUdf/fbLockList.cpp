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


#if _WIN64
// remove warning when building in 64 bit for size_t uInt comparison
#pragma warning( disable : 4267)
#endif

namespace FBServerLock
{
	std::mutex globalListLock;

	fbLockList::fbLockList()
	{

	}

	fbLockList::fbLockList(DWORD maxItems)
		:fbLockList()
	{
		maximumLocks = maxItems;
	}

	fbLockList::~fbLockList()
	{
		lockObjects.clear();
	}

	void fbLockList::setMaxItems(unsigned int total)
	{
		if (total < 1 || total > 100)
			return;

		maximumLocks = total;
	}

	void fbLockList::cleanItems()
	{
		std::lock_guard<std::mutex> guard(globalListLock);

		if (lockObjects.size() == 0)
			return;

		for (int i = lockObjects.size() - 1; i >= 0; i--)
		{
			if (lockObjects.at(i).getIsExpired())
			{
				lockObjects.erase(lockObjects.begin() + i);
			}
		}
	}

	int fbLockList::add(const std::string &name, ULONG32 maxAge)
	{
		cleanItems();

		std::lock_guard<std::mutex> guard(globalListLock);

		if (lockObjects.size() > maximumLocks -1)
			return (LR_MAX_ITEMS_EXCEEDED);

		for (std::vector<fbLockObject>::iterator iter = lockObjects.begin(); iter != lockObjects.end(); iter++)
		{
			if (iter->getLockName().compare(name) == 0)
			{
				return (LR_NAME_EXISTS);
			}
		}

		lockObjects.push_back(fbLockObject(maxAge, name));
		return (LR_SUCCESS);
	}

	int fbLockList::remove(const std::string &name)
	{
		cleanItems();

		std::lock_guard<std::mutex> guard(globalListLock);

		for (std::vector<fbLockObject>::iterator iter = lockObjects.begin(); iter != lockObjects.end(); iter++)
		{
			if (iter->getLockName().compare(name) == 0)
			{
				lockObjects.erase(iter);
				return (LR_SUCCESS);
			}
		}

		return (LR_LOCK_NOT_FOUND);
	}

	int fbLockList::clearAll()
	{
		std::lock_guard<std::mutex> guard(globalListLock);

		lockObjects.clear();

		return (LR_SUCCESS);
	}

	uInt fbLockList::getLockCount()
	{
		cleanItems();

		std::lock_guard<std::mutex> guard(globalListLock);

		return (lockObjects.size() + 1);
	}
}