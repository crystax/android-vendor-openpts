/* 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *
 * Test that sched_setparam() sets errno == EPERM when the requesting process
 * does not have permission to set the scheduling parameters for the specified
 * process, or does not have the appropriate privilege to invoke schedparam().
 */

#if __APPLE__
int main() { return 0; }
#else /* !__APPLE__ */

#include <sched.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include "posixtest.h"



int main(){
	int result;
    struct sched_param param;

	if(sched_getparam(0, &param) == -1) {
		perror("An error occurs when calling sched_getparam()");
		return PTS_UNRESOLVED;
	}

	result = sched_setparam(1, &param);
    if (result == 0) {
        if (geteuid() == 0)
            return PTS_PASS;
        printf("The returned code is not -1\n");
        return PTS_FAIL;
    }

	if(errno == EPERM || errno == EINVAL) {
		printf("Test PASSED\n");
		return PTS_PASS;
	} else {
	        perror("errno is not EPERM");
		return PTS_FAIL;
	}
}

#endif /* !__APPLE__ */
