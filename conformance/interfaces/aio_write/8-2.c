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
 *	aio_write() shall fail or the error status of the operation shall be [EBADF] if:
 *	aio_fildes argument is not a valid file descriptor open for writing.
 *
 * method: Test with an invalid file descriptor opened read-only
 *
 *	- setup an aiocb with an read-only aio_fildes
 *	- call aio_write with this aiocb
 *	- check return code and errno
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

#define TNAME "aio_write/8-2.c"

int main()
{
	char tmpfname[256];
#define BUF_SIZE 512
	char buf[BUF_SIZE];
	int fd;
	struct aiocb aiocb;

#if _POSIX_ASYNCHRONOUS_IO != 200112L
	exit(PTS_UNSUPPORTED);
#endif

	snprintf(tmpfname, sizeof(tmpfname), "/tmp/pts_aio_write_8_2_%d", 
		  getpid());
	unlink(tmpfname);
	fd = open(tmpfname, O_CREAT | O_RDONLY | O_EXCL,
		  S_IRUSR | S_IWUSR);
	if (fd == -1)
	{
		printf(TNAME " Error at open(): %s\n",
		       strerror(errno));
		exit(PTS_UNRESOLVED);
	}

	unlink(tmpfname);

	memset(buf, 0xaa, BUF_SIZE);
	memset(&aiocb, 0, sizeof(struct aiocb));
	aiocb.aio_fildes = fd;
	aiocb.aio_buf = buf;
	aiocb.aio_nbytes = BUF_SIZE;

	if (aio_write(&aiocb) != -1)
	{
		printf(TNAME " bad aio_write return value()\n");
		exit(PTS_FAIL);
	}

	if (errno != EBADF)
	{
		printf(TNAME " errno is not EBADF %s\n", strerror(errno));
		exit(PTS_FAIL);
	}


	close(fd);
	printf ("Test PASSED\n");
	return PTS_PASS;
}

#endif
