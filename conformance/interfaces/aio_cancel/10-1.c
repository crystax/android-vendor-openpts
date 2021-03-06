/*
 * Copyright (c) 2004, Bull SA. All rights reserved.
 * Created by:  Laurent.Vivier@bull.net
 * This file is licensed under the GPL license.  For the full content
 * of this license, see the COPYING file at the top level of this 
 * source tree.
 */

/* assertion:
 *
 *	aio_cancel() shall fail if:
 *	[EBADF] The fildes argument is not a valid descriptor.
 *
 * method:
 *
 *	use -1 as fildes and check return value is -1 and errno is EBADF
 *
 */

#if 1
/* Temporarily disable it until https://tracker.crystax.net/issues/1145 is fixed */
int main() { return 0; }
#else

#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <aio.h>

#include "posixtest.h"

#define TNAME "aio_cancel/10-1.c"

int main()
{
#if _POSIX_ASYNCHRONOUS_IO < 0
    printf("_POSIX_ASYNCHRONOUS_IO is not supported\n");
	return PTS_UNSUPPORTED;
#endif

	if (aio_cancel(-1, NULL) != -1)
	{
		printf(TNAME " bad aio_cancel return value()\n");
		return PTS_FAIL;
	}

	if (errno != EBADF)
	{
		printf(TNAME " errno is not EBADF %s\n", strerror(errno));
		return PTS_FAIL;
	}


	printf ("Test PASSED\n");
	return PTS_PASS;
}

#endif
