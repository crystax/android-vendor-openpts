/*   
 * Copyright (c) 2003, Intel Corporation. All rights reserved.
 * Created by:  salwan.searty REMOVE-THIS AT intel DOT com
 * This file is licensed under the GPL license.  For the full content
 * of this license, see the COPYING file at the top level of this 
 * source tree.

 Simply, if sigrelse returns a 0 here, test passes.
 
*/

#if __ANDROID__
/* https://tracker.crystax.net/issues/1136 */
int main() { return 0; }
#else /* !__ANDROID__ */

#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <signal.h>
#include "posixtest.h"

int main()
{

	if ((int)sigrelse(SIGABRT) != 0) {
		perror("sigrelse failed -- returned -- test aborted");
		return PTS_UNRESOLVED;
	} 
	printf("sigrelse passed\n");
	return PTS_PASS;
}

#endif /* !__ANDROID__ */
