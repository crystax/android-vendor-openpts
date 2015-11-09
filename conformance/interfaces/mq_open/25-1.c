/*
 * Copyright (c) 2003, Intel Corporation. All rights reserved.
 * Created by:  julie.n.fleischer REMOVE-THIS AT intel DOT com
 * This file is licensed under the GPL license.  For the full content
 * of this license, see the COPYING file at the top level of this
 * source tree.
 */

/*
 * Will not test mq_open() failing with EINVAL if mq_open() is not
 * supported for the name parameter as unsupported names are implementation
 * defined.
 */

#if 1
/* Temporarily disable it until https://tracker.crystax.net/issues/1133 is fixed */
int main() { return 0; }
#else

#include <stdio.h>
#include "posixtest.h"

int main()
{
	printf("Will not test mq_open() failing with EINVAL if mq_open()\n");
	printf("is not supported for the name parameter as\n");
	printf("unsupported names are implementation defined.\n");
	return PTS_UNTESTED;
}

#endif
