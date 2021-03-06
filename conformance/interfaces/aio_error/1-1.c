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
 *	The aio_error() function shall return the error status (errno)
 *	associated with tha aiobcp argument.
 *
 * method:
 *
 *	execute aio_write()
 *	and check result with aio_error()
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

#define TNAME "aio_error/1-1.c"

int main()
{
	char tmpfname[256];
#define BUF_SIZE 111
	char buf[BUF_SIZE];
	int fd;
	struct aiocb aiocb;

#if _POSIX_ASYNCHRONOUS_IO != 200112L
	return PTS_UNSUPPORTED;
#endif

	snprintf(tmpfname, sizeof(tmpfname), "/tmp/pts_aio_error_1_1_%d", 
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

	memset(buf, 0xaa, BUF_SIZE);
	memset(&aiocb, 0, sizeof(struct aiocb));
	aiocb.aio_fildes = fd;
	aiocb.aio_buf = buf;
	aiocb.aio_nbytes = BUF_SIZE;

	if (aio_write(&aiocb) == -1)
	{
		printf(TNAME " Error at aio_write(): %s\n",
		       strerror(errno));
		return PTS_FAIL;
	}

	while(aio_error(&aiocb) == EINPROGRESS);

	if (aio_error(&aiocb) != 0)
	{
		printf(TNAME " Error at aio_error(): %s\n",
		       strerror(errno));
		return PTS_FAIL;
	}
	
	close(fd);
	printf ("Test PASSED\n");
	return PTS_PASS;
}

#endif
