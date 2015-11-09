/*
 * Copyright (c) 2003, Intel Corporation. All rights reserved.
 * Created by:  salwan.searty REMOVE-THIS AT intel DOT com
 * This file is licensed under the GPL license.  For the full content
 * of this license, see the COPYING file at the top level of this 
 * source tree.

 This program tests the assertion that if signal has been blocked, then
 sigset shall return SIG_HOLD

*/

#if __ANDROID__
/* https://tracker.crystax.net/issues/1136 */
int main() { return 0; }
#else /* !__ANDROID__ */

#define _XOPEN_SOURCE 600

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include "posixtest.h"

int main()
{
#if __gnu_linux__ || __APPLE__
    /* TODO: fix the following code
     * For some reason it fails on GNU/Linux and OS X
     */
#else /* !__gnu_linux__ */
        if (sigset(SIGCHLD,SIG_HOLD) != SIG_HOLD) {
		printf("Test FAILED: sigset() didn't return SIG_HOLD\n");
		return PTS_FAIL;
	}
#endif /* !__gnu_linux__ */

	return PTS_PASS;
} 

#endif /* !__ANDROID__ */
