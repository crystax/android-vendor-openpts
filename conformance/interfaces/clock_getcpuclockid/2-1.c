/*   
 * Copyright (c) 2004, Intel Corporation. All rights reserved.
 * This file is licensed under the GPL license.  For the full content
 * of this license, see the COPYING file at the top level of this 
 * source tree.
 *
 *  If pid=0, then clock_getcpuclockid() will return the CPU-time clock of
 *  the calling process.
 * 
 */

#if __APPLE__
int main() { return 0; }
#elif __ANDROID__
/* Temporarily disabled it until https://tracker.crystax.net/issues/1129 is fixed */
int main() { return 0; }
#else /* !__ANDROID__ */

#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include "posixtest.h"

int main(int argc, char *argv[])
{
#if _POSIX_CPUTIME == -1
        printf("_POSIX_CPUTIME unsupported\n");
        return PTS_UNSUPPORTED;
#else
	unsigned long time_to_set;	
	clockid_t clockid_1, clockid_2;
	struct timespec tp1, tp2;

	if (sysconf(_SC_CPUTIME) == -1) {
		printf("_POSIX_CPUTIME unsupported\n");
		return PTS_UNSUPPORTED;
	}

	if (clock_getcpuclockid(getpid(), &clockid_1) != 0) {
		printf("clock_getcpuclockid(getpid(), ) failed\n");
		return PTS_FAIL;
	}
	
	if (clock_getcpuclockid(0, &clockid_2) != 0) {
		printf("clock_getcpuclockid(0, ) failed\n");
		return PTS_FAIL;
	}

	/* Set clockid_1 as a random value from 1 sec to 10 sec */
	srand((unsigned long)time(NULL));
	time_to_set = rand() * 10.0 / RAND_MAX + 1;
	tp1.tv_sec = time_to_set;
	tp1.tv_nsec = 0;	 
	if (clock_settime(clockid_1, &tp1) != 0) {
        if (geteuid() != 0 && errno == EPERM)
            return PTS_PASS;
        fprintf(stderr, "clock_settime() failed: %s\n", strerror(errno));
		return PTS_FAIL;
	}
	/* Get the time of clockid_2, should almost the same as clockid_1 */
	if (clock_gettime(clockid_2, &tp2) != 0) {
        fprintf(stderr, "clock_gettime() failed: %s\n", strerror(errno));
		return PTS_FAIL;
	}
	if (tp1.tv_sec == tp2.tv_sec)	
	{
		printf("Test PASSED\n");
		return PTS_PASS;
	}
	printf("Test FAILED\n");
	return PTS_FAIL;
	
#endif
}
#endif /* !__ANDROID__ */
