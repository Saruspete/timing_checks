#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {

	int threshold = 3;
	int loops = 10000000;

	printf("Called: %s with %d args\n", argv[0], argc);

	switch (argc) {
		case 2:
			loops = atoi(argv[1]);
			break;
		case 3:
			loops = atoi(argv[1]);
			threshold = atoi(argv[2]);
			break;
	}

	size_t i=1;
	struct timeval  tv[2];


	while (i < loops) {
		gettimeofday(&tv[i%2], NULL);
		int diff  =
			( tv[i%2].tv_sec  - tv[(i-1)%2].tv_sec ) * 1000000 +
			( tv[i%2].tv_usec - tv[(i-1)%2].tv_usec);

		if (diff >= threshold) {
			printf("%9.9ld time: %ld.%ld  Diff: %d\n", i, tv[i%2].tv_sec, tv[i%2].tv_usec, diff );
		}

		i++;
	}

	return 0;
}
