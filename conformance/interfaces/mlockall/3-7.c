/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 * Test that mlockall lock the mapped files pages currently mapped into the
 * address space of the process when MCL_CURRENT is set.
 *
 * This test use msync to check that the page is locked.
 */

#if __APPLE__
int main() { return 0; }
#else /* !__APPLE__ */

#define _POSIX_C_SOURCE 200112L

 /* We need the XSI extention for the mkstemp() routine */
#ifndef WITHOUT_XOPEN
#define _XOPEN_SOURCE	600
#endif

#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "posixtest.h"

char tmpfilename[4096];

void cleanup()
{
    unlink(tmpfilename);
}

const char *mktmpfile()
{
    int fd;
    char buf[4096];

    const char *tmpdir = getenv("TMPDIR");
    if (!tmpdir) tmpdir = "/tmp";

    snprintf(tmpfilename, sizeof(tmpfilename), "%s/pts_mlockall_3-7-XXXXXX", tmpdir);
    printf("tmp file template: %s\n", tmpfilename);

    fd = mkstemp(tmpfilename);
    if (fd == -1) {
        perror("Can't open tmp file");
        exit(PTS_UNRESOLVED);
    }

    atexit(cleanup);

    if (write(fd, buf, sizeof(buf)) != sizeof(buf)) {
        perror("Can't write to tmp file");
        exit(PTS_UNRESOLVED);
    }

    if (close(fd) == -1) {
        perror("Can't close tmp file");
        exit(PTS_UNRESOLVED);
    }

    printf("tmp file name: %s\n", tmpfilename);
    return tmpfilename;
}

int main() {
	void *page_ptr;
	size_t page_size;
	int result, fd;
	void *foo;
    struct rlimit rl;

    if (getrlimit(RLIMIT_MEMLOCK, &rl) == -1) {
        perror("An error occurs when calling getrlimit()");
        return PTS_UNRESOLVED;
    }
    printf("RLIMIT_MEMLOCK: %lld/%lld\n", (long long)rl.rlim_cur, (long long)rl.rlim_max);

	page_size = sysconf(_SC_PAGESIZE);
	if(errno) {
		perror("An error occurs when calling sysconf()");
		return PTS_UNRESOLVED;
	}

	fd = open(mktmpfile(), O_RDONLY);
	if(fd == -1) {
		perror("An error occurs when calling open()");
		return PTS_UNRESOLVED;	
	}

	foo = mmap(NULL, page_size, PROT_READ, MAP_SHARED, fd, 0);
		if(foo == MAP_FAILED) {
		perror("An error occurs when calling mmap()");
		return PTS_UNRESOLVED;
	}

	if(mlockall(MCL_CURRENT) == -1) {
#if __gnu_linux__
        if (errno == ENOMEM && rl.rlim_cur != 0) {
            printf("We're trying to lock more memory than allowed (%lld)\n", (long long)rl.rlim_cur);
            return PTS_PASS;
        }
#endif /* !__gnu_linux__ */

		if(errno == EPERM){
			printf("You don't have permission to lock your address space.\nTry to rerun this test as root.\n");
		} else {
			perror("An error occurs when calling mlockall()");
		}
		return PTS_UNRESOLVED;
	}

	page_ptr = (void*) ( (long)foo - ((long)foo % page_size) );

	result = msync(page_ptr, page_size, MS_SYNC|MS_INVALIDATE);
	if(result == -1 && errno == EBUSY) {
		printf("Test PASSED\n");
		return PTS_PASS;
	} else if(result == 0) {
		printf("The mapped files pages of the process are not locked.\n");
		return PTS_FAIL;
	}
	perror("Unexpected error");
	return PTS_UNRESOLVED;
}

#endif /* !__APPLE__ */
