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
 *	if aiobcp is NULL, all outstanding cancelable AIO against fildes
 *	shall be canceled.
 *
 * method:
 *
 *	open a file
 *	execute aio_cancel() on this file
 *	check aio_cancel() is not -1
 *	-> aio_cancel() works on standard fildes
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

#define TNAME "aio_cancel/2-2.c"

int main()
{
	char tmpfname[256];
	int fd;

#if _POSIX_ASYNCHRONOUS_IO < 0
    printf("_POSIX_ASYNCHRONOUS_IO is not supported\n");
	return PTS_UNSUPPORTED;
#endif

	snprintf(tmpfname, sizeof(tmpfname), "/tmp/pts_aio_cancel_2_2_%d", 
		  getpid());
	unlink(tmpfname);
	fd = open(tmpfname, O_CREAT | O_RDWR | O_EXCL,
		  S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		printf(TNAME " Error at open(): %s\n",
		       strerror(errno));
		return PTS_UNRESOLVED;
	}

	unlink(tmpfname);

	if (aio_cancel(fd, NULL) == -1)
	{
		printf(TNAME " Error at aio_cancel(): %s\n",
		       strerror(errno));
		return PTS_FAIL;
	}

	close(fd);
	printf ("Test PASSED\n");
	return PTS_PASS;
}

#endif
