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

#ifndef FBLOCKLIST
#define FBLOCKLIST

#include <list>
#include <mutex>

#include "fbLockObject.h"
#include "fbLockList.h"

namespace FBServerLock
{
	typedef enum LockResult
	{
		LR_SUCCESS = 0,
		LR_MAX_ITEMS_EXCEEDED = -1,
		LR_NAME_EXISTS = -2,
		LR_INVALID_NAME = -3,
		LR_INVALID_LOCK_NAME = -8,
		LR_INVALID_PARAMETER = -9,
		LR_INVALID_AGE = -10,
		LR_LOCK_NOT_FOUND =- 11,
		LR_FAILED = -999
	};

	class fbLockList
	{
	private:
		unsigned int maxItems = 10;
		DWORD maxWait = 2000;
		std::list<FBServerLock::fbLockObject> _lockObjects;

		void cleanItems();
	public:
		fbLockList();
		fbLockList(DWORD maxItems);
		fbLockList(DWORD maxItems, DWORD timeout);
		~fbLockList();

		void setMaxItems(unsigned int total);
		
		int add(const std::string &name, ULONG32 maxAge);
		
		int remove(const std::string &name);
		int clearAll();
		int getLockCount();
	};
}

#endif