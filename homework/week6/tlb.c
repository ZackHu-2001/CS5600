#include<stdio.h>
#include<time.h>
#include<stdlib.h>

#define PAGESIZE 4096

int main(int argc, char *argv[])
{
	if(argc != 3) {
		fprintf(stderr, "error parameters!");
		exit(0);
	}
	int pageNum = atoi(argv[1]);
    int num = atoi(argv[2]);

    struct timespec start, end;

	int i, j;
	char arr[PAGESIZE * pageNum];
	for(i=0; i<PAGESIZE*pageNum; ++i) {
		arr[i] = 0;
	}

    // Use clock_gettime to obtain the current time
    clock_gettime(CLOCK_MONOTONIC, &start);

	for(j=0; j<num; ++j) {
		for(i=0; i<pageNum; ++i) {
			arr[i*4096]=10;
		}
	}

    clock_gettime(CLOCK_MONOTONIC, &end);	
    double elapsed_time = (end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec);
	printf("time per fetch: %lf ns\n", (elapsed_time / num) / pageNum);
	return 0;
}