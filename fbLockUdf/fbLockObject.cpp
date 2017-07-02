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

namespace FBServerLock
{
	fbLockObject::fbLockObject(const int &maxAge)
	{
		double maxLockAge = static_cast<double>(maxAge);

		if (maxLockAge < 1 || maxLockAge > 1440)
			maxLockAge = 10; //default 10 seconds

		time(&startTime);
		maximumAge = maxLockAge;
		lockName = "standard_lock";
	}

	fbLockObject::fbLockObject(const int &maxAge, const std::string &name)
		: fbLockObject(maxAge)
	{
		lockName = std::string(name);
	}

	fbLockObject::~fbLockObject()
	{

	}

	std::string fbLockObject::getLockName()
	{
		return (lockName);
	}

	bool fbLockObject::getIsExpired()
	{
		time_t current;
		time(&current);
		return (difftime(current, startTime) > maximumAge);
	}
}