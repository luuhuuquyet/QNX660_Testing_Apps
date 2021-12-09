#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <process.h>
#include <inttypes.h>
#include <sys/neutrino.h>
#include <sys/syspage.h>
#include <sys/mman.h>

#define	ITER_CPY		16000000
#define	MAX_MSGSIZE		8*1024*1024

void print_times(uint64_t time, int iter, int size) {
	uint64_t	cps = (SYSPAGE_ENTRY(qtime)->cycles_per_sec >> CLOCKCYCLES_INCR_BIT);
	double		tot;

	tot = (double)time * 1000000.0 / (double)cps / (double)iter;

	printf(" %8.2f", (double)size * 1e6 / tot / 1024.0 / 1024.0);
	fflush(stdout);
}

void readword(void *p, int size){
	volatile uint32_t	*u = p;
	uint32_t	dummy;
	int			i;

	for (i = 0; i < size / sizeof(dummy); i++)
		dummy = *u++;
}


void do_copy(void * dst,  void *src, int size){
	uint64_t	start, stop, diff;
	int			i;
	start = ClockCycles() >> CLOCKCYCLES_INCR_BIT;
	for (i = 0; i < ITER_CPY/size; i++){
		memcpy(dst, src, size);
	}
	stop = ClockCycles() >> CLOCKCYCLES_INCR_BIT; 
	diff = stop - start;
	print_times(diff, i, size);
}

void do_write(void * dst, int size){
	uint64_t	start, stop, diff;
	int			i;
	start = ClockCycles() >> CLOCKCYCLES_INCR_BIT;
	for (i = 0; i < ITER_CPY/size; i++){
		memset(dst, 0, size);
	}
	stop = ClockCycles() >> CLOCKCYCLES_INCR_BIT; 
	diff = stop - start;
	print_times(diff, i, size);
}
void do_read(void *src, int size){
	uint64_t	start, stop, diff;
	int			i;
	start = ClockCycles() >> CLOCKCYCLES_INCR_BIT;
	for (i = 0; i < ITER_CPY/size; i++){
		readword(src, size);
	}
	stop = ClockCycles() >> CLOCKCYCLES_INCR_BIT; 
	diff = stop - start;
	print_times(diff, i, size);
}
/*
 * -------------------------------------------------------------------------
 * -------------------------------------------------------------------------
 */

int
main(int argc, char *argv[])
{
	int		size;
	void	*csrc;
	void	*cdst;
	void	*usrc;
	void	*udst;

	csrc = mmap(0, MAX_MSGSIZE, PROT_READ, MAP_ANON | MAP_PRIVATE, NOFD, 0);
	if ( (void *)MAP_FAILED == csrc) {
		perror("Failed to map source area with cache ");
		exit(1);
	}

	cdst = mmap(0, MAX_MSGSIZE, PROT_READ|PROT_WRITE, MAP_ANON | MAP_PRIVATE, NOFD, 0);
	if ((void *)MAP_FAILED == cdst) {
		perror("Failed to map destination area with cache ");
		exit(1);
	}

	usrc = mmap(0, MAX_MSGSIZE, PROT_READ|PROT_NOCACHE, MAP_ANON  | MAP_PRIVATE, NOFD, 0);
	if ((void *)MAP_FAILED == usrc) {
		perror("Failed to map source area without cache ");
		exit(1);
	}
	udst = mmap(0, MAX_MSGSIZE, PROT_WRITE|PROT_NOCACHE, MAP_ANON  | MAP_PRIVATE, NOFD, 0);
	if ((void *)MAP_FAILED == udst) {
		perror("Failed to map destination area without cache ");
		exit(1);
	}
	memcpy(cdst, csrc, MAX_MSGSIZE);
	memcpy(udst, usrc, MAX_MSGSIZE);

	printf("csrc=%p cdst=%p usrc=%p udst=%p\n", csrc, cdst, usrc, udst);
	printf("Speed is MB/s\n");
	printf("   size cpy C->C cpy U->C cpy C->U cpy U->U    set C    set U   read C   read U\n");
	printf("   ---- -------- -------- -------- --------    -----    -----   ------   ------\n");
	fflush(stdout);
	for (size = 16; size <= MAX_MSGSIZE; size <<= 1) {
		printf("%7d", size);	
		fflush(stdout);

		// cached -> cached
		do_copy(cdst, csrc, size);

		// uncached -> cached
		do_copy(cdst, usrc, size);

		// cached -> uncached
		do_copy(udst, csrc, size);

		// uncached -> uncached
		do_copy(udst, usrc, size);

		// cached
		do_write(cdst, size);

		// uncached
		do_write(udst, size);

		// cached
		do_read(csrc, size);

		// uncached
		do_read(usrc, size);

		printf("\n");
	}

	fflush(stdout);

	return 0;
}
