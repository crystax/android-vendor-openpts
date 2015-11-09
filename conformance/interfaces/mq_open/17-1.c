/*
 * Copyright (c) 2003, Intel Corporation. All rights reserved.
 * Created by:  julie.n.fleischer REMOVE-THIS AT intel DOT com
 * This file is licensed under the GPL license.  For the full content
 * of this license, see the COPYING file at the top level of this
 * source tree.
 */

/*
 * Will not test setting O_EXCL without O_CREAT because results are
 * undefined.
 */

#if 1
/* Temporarily disable it until https://tracker.crystax.net/issues/1133 is fixed */
int main() { return 0; }
#else

#include <stdio.h>
#include "posixtest.h"

int main()
{
	printf("Will not test setting O_EXCL without O_CREAT because\n");
	printf("results are undefined.\n");
	return PTS_UNTESTED;
}

#endif
