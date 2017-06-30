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

#ifndef FBLOCKOBJECT
#define FBLOCKOBJECT

#include <Windows.h>
#include <winnt.h>
#include <string>
#include <time.h>


namespace FBServerLock
{
	class fbLockObject
	{
	private:
		double maximumAge;
		time_t startTime;
	public:
		std::string lockName;

		bool getIsExpired();

		fbLockObject(ULONG32 maxAge, std::string name);

		~fbLockObject();

		bool operator==(const fbLockObject& other) const  {
			return (true);
		}
	};
}


#endif