/*
 * Copyright (c) 2002-2003, Intel Corporation. All rights reserved.
 * Created by:  rusty.lynch REMOVE-THIS AT intel DOT com
 * This file is licensed under the GPL license.  For the full content
 * of this license, see the COPYING file at the top level of this 
 * source tree.

  Test case for assertion #8 of the sigaction system call that verifies 
  that if signals in the sa_mask (passed in the sigaction struct of the 
  sigaction function call) are added to the process signal mask during
  execution of the signal-catching function.
*/

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include "posixtest.h"

int SIGXCPU_count = 0;

void SIGXCPU_handler(int signo)
{
	SIGXCPU_count++;
	printf("Caught SIGXCPU\n");
}

void SIGXFSZ_handler(int signo)
{
	printf("Caught SIGXFSZ\n");
	raise(SIGXCPU);
	if (SIGXCPU_count) {
		printf("Test FAILED\n");
		exit(-1);
	}
}

int main()
{
	struct sigaction act;
	
	act.sa_handler = SIGXFSZ_handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGXCPU);
	if (sigaction(SIGXFSZ,  &act, 0) == -1) {
		perror("Unexpected error while attempting to "
		       "setup test pre-conditions");
		return PTS_UNRESOLVED;
	}

	act.sa_handler = SIGXCPU_handler;
	act.sa_flags = 0;
	sigemptyset(&act.sa_mask);
	if (sigaction(SIGXCPU,  &act, 0) == -1) {
		perror("Unexpected error while attempting to "
		       "setup test pre-conditions");
		return PTS_UNRESOLVED;
	}
	

	if (raise(SIGXFSZ) == -1) {
		perror("Unexpected error while attempting to "
		       "setup test pre-conditions");
		return PTS_UNRESOLVED;
	}


	printf("Test PASSED\n");
	return PTS_PASS;
}

