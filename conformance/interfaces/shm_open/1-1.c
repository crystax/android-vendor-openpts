/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 * Test that the shm_open() function establish a connection between a shared
 * memory object and a file descriptor.
 *
 * Step:
 *  1. Create a file a shared memory object using shm_open().
 *  2. Write in the file referenced by the file descriptor return by
 *     shm_open().
 *  3. Reopen the file with shm_open().
 *  4. Read in the file.
 * The test pass if it read what it was previously written.
 */

#if 1
/* Temporarily disable it until https://tracker.crystax.net/issues/1132 is fixed */
int main() { return 0; }
#else

/* ftruncate was formerly an XOPEN extension. We define _XOPEN_SOURCE here to
   avoid warning if the implementation does not program ftruncate as a base 
   interface */
#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "posixtest.h"

#define BUF_SIZE 8
#define SHM_NAME "posixtest_1-1"

int main() {
	int fd;
	char str[BUF_SIZE] = "qwerty";
	char *buf;
	
	fd = shm_open(SHM_NAME, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR);
	if(fd == -1) {
		perror("An error occurs when calling shm_open()");
		return PTS_UNRESOLVED;
	}
	
	if(ftruncate(fd, BUF_SIZE) != 0) {
		perror("An error occurs when calling ftruncate()");
		shm_unlink(SHM_NAME);
		return PTS_UNRESOLVED;	
	}

	buf = mmap(NULL, BUF_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
	if( buf == MAP_FAILED) {
		perror("An error occurs when calling mmap()");
		shm_unlink(SHM_NAME);
		return PTS_UNRESOLVED;	
	}	

	strcpy(buf, str);

	fd = shm_open(SHM_NAME, O_RDONLY, S_IRUSR|S_IWUSR);
	if(fd == -1) {
		perror("An error occurs when calling shm_open()");
		shm_unlink(SHM_NAME);
		return PTS_UNRESOLVED;
	}

	buf = mmap(NULL, BUF_SIZE, PROT_READ, MAP_SHARED, fd, 0);
	if( buf == MAP_FAILED) {
		perror("An error occurs when calling mmap()");
		shm_unlink(SHM_NAME);
		return PTS_UNRESOLVED;	
	}	

	shm_unlink(SHM_NAME);
	if(strcmp(buf, str) == 0) {
		printf("Test PASSED\n");
		return PTS_PASS;
	}
	printf("Test FAILED\n");
	return PTS_FAIL;
}

#endif
