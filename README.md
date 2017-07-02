# fbLockUDF
fbLockUDF is a Windows 32 bit and 64 bit Firebird Server Wide locking mechanism

Usage, there are 4 functions:

fbServerLockGet - Obtain a lock
Parameters:
Name - String, name of lock.
Time - Int, number of seconds to lock (any value between 1 and 1440)


fbServerLockRel - Releases a lock
Parameters:
Name - String, name of lock


fbGetLockCount - Returns the number of locks
Parameters:
None


fbServerLockClr - Clears all locks
Parameters:
None


Return Values:
0 - Success.
-1 - Too many locks exist.
-2 - Lock already exists.
-8 - Invalid lock name.
-9 - Invalid parameter.
-10 Invalid Lock age.
-11 - Lock not found.
-999 - Failed.

DECLARE EXTERNAL FUNCTION FB_SERVERLOCKREL
CSTRING(100)
RETURNS Integer BY VALUE 
ENTRY_POINT 'fbServerLockRel'
MODULE_NAME 'fbLockUdf';

DECLARE EXTERNAL FUNCTION FB_SERVERLOCKGET
CSTRING(100), Integer
RETURNS Integer BY VALUE 
ENTRY_POINT 'fbServerLockGet'
MODULE_NAME 'fbLockUdf';

DECLARE EXTERNAL FUNCTION FB_SERVERLOCKCOUNT
RETURNS Integer BY VALUE 
ENTRY_POINT 'fbGetLockCount'
MODULE_NAME 'fbLockUdf';

DECLARE EXTERNAL FUNCTION FB_SERVERLOCKCLEAR
RETURNS Integer BY VALUE 
ENTRY_POINT 'fbServerLockClr'
MODULE_NAME 'fbLockUdf';

-- test procedure



SET TERM ^ ;

CREATE OR ALTER PROCEDURE TestLocking
RETURNS 
( 
    lockName varchar(100), 
    result integer,
    seconds integer,
    pass VARCHAR(3)
)
AS 
  DECLARE VARIABLE vSeconds INTEGER = 30;
BEGIN
  -- if run again within 30 seconds this result will be -2 (lock exists)
  lockName = 'Custom Lock 1';
  seconds = vSeconds;
  result = FB_SERVERLOCKGET(lockName, seconds);
  IF (result = 0) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;
  
  result = FB_SERVERLOCKGET(lockName, seconds);
  IF (result = -2) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;

  lockName = 'Custom Lock 2';
  seconds = vSeconds;
  result = FB_SERVERLOCKGET(lockName, seconds);
  IF (result = 0) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;

  lockName = 'Custom Lock 3';
  seconds = vSeconds;
  result = FB_SERVERLOCKGET(lockName, seconds);
  IF (result = 0) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;

  lockName = 'Custom Lock 4';
  seconds = vSeconds;
  result = FB_SERVERLOCKGET(lockName, seconds);
  IF (result = 0) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;

  lockName = 'Count';
  result = FB_SERVERLOCKCOUNT();
  IF (result = 4) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;
  
  lockName = 'Custom Lock 5';
  seconds = vSeconds;
  result = FB_SERVERLOCKGET(lockName, seconds);
  IF (result = 0) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;

  lockName = 'Custom Lock 6';
  seconds = vSeconds;
  result = FB_SERVERLOCKGET(lockName, seconds);
  IF (result = 0) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;

  lockName = 'Custom Lock 7';
  seconds = vSeconds;
  result = FB_SERVERLOCKGET(lockName, seconds);
  IF (result = 0) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;

  lockName = 'Custom Lock 8';
  seconds = vSeconds;
  result = FB_SERVERLOCKGET(lockName, seconds);
  IF (result = 0) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;

  lockName = 'Custom Lock 9';
  seconds = vSeconds;
  result = FB_SERVERLOCKGET(lockName, seconds);
  IF (result = 0) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;

  lockName = 'Custom Lock 10';
  seconds = vSeconds;
  result = FB_SERVERLOCKGET(lockName, seconds);
  IF (result = 0) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;

  lockName = 'Custom Lock 11';
  seconds = vSeconds;
  result = FB_SERVERLOCKGET(lockName, seconds);
  IF (result = -1) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;

  lockName = 'Custom Lock 12';
  seconds = vSeconds;
  result = FB_SERVERLOCKGET(lockName, seconds);
  IF (result = -1) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;

  lockName = 'Count';
  result = FB_SERVERLOCKCOUNT();
  IF (result = 10) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;
  
  lockName = 'Clear All';
  result = FB_SERVERLOCKCLEAR();
  IF (result = 0) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;
 
  lockName = 'Custom Lock 1';
  seconds = vSeconds;
  result = FB_SERVERLOCKGET(lockName, seconds);
  IF (result = 0) THEN pass = 'Yes'; ELSE pass = 'No';
  SUSPEND;
END^

SET TERM ; ^

SELECT lockName, Result, seconds, pass
FROM TestLocking;
