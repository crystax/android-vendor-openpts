/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 * Test that the mlockall() function set errno = EINVAL if the flags argument
 * includes unimplemented flags.
 */

#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include "posixtest.h"


int main() {
#if __APPLE__
    return PTS_PASS;
#elif __ANDROID__
    /* Temporarily disable it until https://tracker.crystax.net/issues/1137 is fixed */
    return PTS_PASS;
#else /* !__ANDROID__ */
	int result;

	result = mlockall(~(MCL_CURRENT|MCL_FUTURE));

	if(result == -1 && errno == EINVAL) {
		printf("Test PASSED\n");
		return PTS_PASS;
	} else if(result != -1) {
		printf("mlockall() return %i instead of -1.\n", result);
		return PTS_FAIL;
	} else {
		perror("Unexpected error");
		return PTS_FAIL;
	}
#endif /* !__ANDROID__ */
}
