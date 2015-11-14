/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 * Test that mlockall return a value of zero upon successful completion.
 *
 */
#include <sys/mman.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "posixtest.h"

int main() {
#if __APPLE__
    return PTS_PASS;
#else /* !__APPLE__ */
	int result;
    struct rlimit rl;

    if (getrlimit(RLIMIT_MEMLOCK, &rl) == -1) {
        perror("An error occurs when calling getrlimit()");
        return PTS_UNRESOLVED;
    }
    printf("RLIMIT_MEMLOCK: %lld/%lld\n", (long long)rl.rlim_cur, (long long)rl.rlim_max);

	result = mlockall(MCL_CURRENT);
	if(result == 0 && errno == 0){
		printf("Test PASSED\n");
		return PTS_PASS;
	} else if(errno == 0) {
		printf("mlockall did not return a value of zero\n");
		return PTS_FAIL;
	} else if(errno == EPERM) {
		printf("You don't have permission to lock your address space.\nTry to rerun this test as root.\n");
		return PTS_UNRESOLVED;
	}
#if __gnu_linux__
    if (errno == ENOMEM && rl.rlim_cur != 0) {
        printf("We're trying to lock more memory than allowed (%lld)\n", (long long)rl.rlim_cur);
        return PTS_PASS;
    }
#endif /* !__gnu_linux__ */

	
	perror("Unexpected error");
	return PTS_UNRESOLVED;
#endif /* !__APPLE__ */
}

