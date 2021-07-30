#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#ifndef SYS_getcpu
# ifdef __X86_64__
#  define SYS_getcpu 309
# else
#  define SYS_getcpu 318
# endif
#endif

int main(int argc, char* argv[]) {

	int threshold = 5;
	int loops = 100000000;

	switch (argc) {
		case 2:
			loops = atoi(argv[1]);
			break;
		case 3:
			loops = atoi(argv[1]);
			threshold = atoi(argv[2]);
			break;
	}

	size_t i=1, overCnt=0, overMax=0;
	struct timeval  tv[2], tvStart, tvStop;

	gettimeofday(&tvStart, NULL);

	unsigned int cpuNum, nodeNum;
	syscall(SYS_getcpu,&cpuNum, &nodeNum);

	printf("Start: %ld.%ld\n", tvStart.tv_sec, tvStart.tv_usec);
	printf("CPU: on cpu %u node %u\n", cpuNum, nodeNum);
	printf("Samples: Taking %d samples\n", loops);

	// First iteration to avoid useless tests
	gettimeofday(&tv[0], NULL);

	while (i < loops) {
		gettimeofday(&tv[i%2], NULL);
		int diff  =
			( tv[i%2].tv_sec  - tv[(i-1)%2].tv_sec ) * 1000000 +
			( tv[i%2].tv_usec - tv[(i-1)%2].tv_usec);

		if (diff >= threshold) {
			printf("%9.9ld time: %ld.%ld  Diff: %d\n", i, tv[i%2].tv_sec, tv[i%2].tv_usec, diff );
			overCnt++;
			if (diff > overMax)
				overMax = diff;
		}

		i++;
	}

	gettimeofday(&tvStop, NULL);
	int diff =
		( tvStop.tv_sec  - tvStart.tv_sec ) * 1000000 +
		( tvStop.tv_usec - tvStart.tv_usec);
	syscall(SYS_getcpu, &cpuNum, &nodeNum);

	printf("End: %ld.%ld\n", tvStop.tv_sec, tvStop.tv_usec);
	printf("CPU: on %u node %u\n", cpuNum, nodeNum);
	printf("Samples: in %f sec (%ld sample/sec)  got %ld over-threshold (max: %ld)\n",
		 (float)diff/1000000, i*1000000/diff, overCnt, overMax);

	return 0;
}
