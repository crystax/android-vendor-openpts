/*
 * Copyright (c) 2004, Bull SA. All rights reserved.
 * Created by:  Laurent.Vivier@bull.net
 * This file is licensed under the GPL license.  For the full content
 * of this license, see the COPYING file at the top level of this 
 * source tree.
 */

/*
 * assertion:
 *
 * The error status of a succesfully queued operation shall be:
 * [EINVAL] if aio_offset woud be invalid, or aio_reqprio is not a valid
 * value, or aio_nbytes is an invalid value.
 *
 * method:
 *
 *	UNTESTED
 *
 */

#if 1
/* Temporarily disable it until https://tracker.crystax.net/issues/1145 is fixed */
int main() { return 0; }
#else

#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <unistd.h>
#include <aio.h>

#include "posixtest.h"

#define TNAME "aio_read/14-1.c"

int main()
{
#if _POSIX_ASYNCHRONOUS_IO != 200112L
	exit(PTS_UNSUPPORTED);
#endif

	return PTS_UNTESTED;
}

#endif
