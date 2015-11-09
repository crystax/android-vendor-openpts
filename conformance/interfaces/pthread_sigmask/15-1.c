/*   
 * Copyright (c) 2003, Intel Corporation. All rights reserved.
 * Created by:  salwan.searty REMOVE-THIS AT intel DOT com
 * This file is licensed under the GPL license.  For the full content
 * of this license, see the COPYING file at the top level of this 
 * source tree.

 Simply, if pthread_sigmask returns a 0 here, test passes.
 
*/

#if __ANDROID__
/* Temporarily disable it until https://tracker.crystax.net/issues/1128 is fixed */
int main() { return 0; }
#else /* !__ANDROID__ */

#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <signal.h>
#include "posixtest.h"

int main()
{

	sigset_t set;
	sigaddset (&set, SIGABRT);

	if (pthread_sigmask(SIG_SETMASK, &set, NULL) != 0) {
		perror("pthread_sigmask failed -- returned -- test aborted");
		return PTS_FAIL;
	} 
	printf("pthread_sigmask passed\n");
	return PTS_PASS;
}

#endif /* !__ANDROID__ */
