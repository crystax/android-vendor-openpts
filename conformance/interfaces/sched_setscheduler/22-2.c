/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 * Test that the underlying kernel-scheduled entities for the process
 * contention scope threads that are created after this call completes inherit
 * their scheduling parameters from the process.
 */

#if __APPLE__
int main() { return 0; }
#elif __ANDROID__
/* Temporarily disable it until https://tracker.crystax.net/issues/1114 is fixed */
int main() { return 0; }
#else /* !__ANDROID__ */

#include <sched.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include "posixtest.h"


void * runner(void * arg) {

	while(1) sleep(1);
	return NULL;
}

int main() {
	int new_priority, max_priority, policy, result;
	struct sched_param param;
	pthread_t tid;
	pthread_attr_t attr;

	if(sched_getparam(getpid(), &param) != 0){
		perror("An error occurs when calling sched_getparam()");
		pthread_exit((void*)-1);
	}
	/* Make sure new_priority != old_priority */
	max_priority = sched_get_priority_max(SCHED_FIFO);
	new_priority = (param.sched_priority == max_priority) ? 
		sched_get_priority_min(SCHED_FIFO) :
		max_priority;
	param.sched_priority = new_priority;
	if(sched_setscheduler(getpid(), SCHED_FIFO, &param) != 0){
		if(errno == EPERM) {
			printf("This process does not have the permission to set its own scheduling policy.\nTry to launch this test as root.\n");
			return geteuid() == 0 ? PTS_UNRESOLVED : PTS_PASS;
		}
		perror("An error occurs when calling sched_setscheduler()");
		return PTS_UNRESOLVED;
	}


	if(pthread_attr_init(&attr) != 0) {
		printf("An error occurs when calling pthread_attr_init()");
		return PTS_UNRESOLVED;
	}
	result = pthread_attr_setscope(&attr, PTHREAD_SCOPE_PROCESS);
	if(result == ENOTSUP) {
		printf("Process contention scope threads are not supported.\n");
		return PTS_UNSUPPORTED;
	} else if(result != 0) {
		printf("An error occurs when calling pthread_attr_setscope()");
		return PTS_UNRESOLVED;
	}
	if(pthread_create(&tid, &attr, runner, NULL) != 0) {
		printf("An error occurs when calling pthread_create()");
		return PTS_UNRESOLVED;
	}


	if(pthread_getschedparam(tid , &policy, &param) != 0) {
		printf("An error occurs when calling pthread_getschedparam()");
		return PTS_UNRESOLVED;
	}

	pthread_cancel(tid);

	if(param.sched_priority == new_priority){
		printf("Test PASSED\n");
		return PTS_PASS;
	}
	printf("The thread does not inherit the right param.\n");
	return PTS_FAIL;
}

#endif /* !__ANDROID__ */
