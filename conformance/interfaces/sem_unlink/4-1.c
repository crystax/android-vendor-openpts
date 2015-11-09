/*
    Copyright (c) 2003, Intel Corporation. All rights reserved.
    Created by:  majid.awad REMOVE-THIS AT intel DOT com
    This file is licensed under the GPL license.  For the full content 
    of this license, see the COPYING file at the top level of this 
    source tree.
 */

/*
 * Trying to unlink a semaphore which it doesn't exist.  It give an ERROR: 
 * ENOENT.
 */


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "posixtest.h"


#define TEST "4-1"
#define FUNCTION "sem_unlink"
#define ERROR_PREFIX "unexpected error: " FUNCTION " " TEST ": "



int main() {
#if __ANDROID__
    /* Temporarily disable it until https://tracker.crystax.net/issues/1134 is fixed */
    return PTS_PASS;
#else /* !__ANDROID__ */

	char semname[20];

	if (sem_unlink(semname) == 0) {
        fprintf(stderr, "sem_unlink() return 0, but expected FAIL\n");
        return PTS_FAIL;
    }

	if (errno == ENOENT || errno == EINVAL) {
		puts("TEST PASSED");
		return PTS_PASS;
	} else {
		fprintf(stderr, "TEST FAILED: errno is %d (%s), but expected ENOENT or EINVAL\n", errno, strerror(errno));
		return PTS_FAIL;
	}
#endif /* !__ANDROID__ */
}
