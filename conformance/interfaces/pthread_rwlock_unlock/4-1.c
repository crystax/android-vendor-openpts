/*   
 * Copyright (c) 2002, Intel Corporation. All rights reserved.
 * This file is licensed under the GPL license.  For the full content
 * of this license, see the COPYING file at the top level of this 
 * source tree.

 * Test that pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
 *
 *	It 'may' fail if:
 *	[EINVAL]  rwlock doesn't refer to an intialized read-write lock
 *	[EPERM]  the current thread doesn't hold the lock on the rwlock
 *
 *	Testing EINVAL in this test.
 *
 * Steps:
 *  1. Call pthread_rwlock_unlock with an uninitialized rwlock
 *  2. The test will pass even if it returns 0, but with a note stating that the standard
 *     states it 'may' fail.
 * 
 */

#define _XOPEN_SOURCE 600
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "posixtest.h"

int main()
{
	static pthread_rwlock_t rwlock;
	int rc;
	
	rc = pthread_rwlock_unlock(&rwlock);
	if(rc != 0)
	{
		if(rc == EINVAL || rc == EPERM)
		{
			printf("Test PASSED\n");
			return PTS_PASS;
		}

		printf("Test FAILED: Incorrect error code, expected 0 or EINVAL or EPERM, got %d (%s)\n", rc, strerror(rc));
		return PTS_FAIL;
	}
	
	printf("Test PASSED: Note*: Returned 0 instead of EINVAL or EPERM, but standard specified _may_ fail.\n");
	return PTS_PASS;
}
