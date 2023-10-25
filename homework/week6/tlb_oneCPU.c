#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sched.h>  // Include the sched.h header for CPU affinity macros

#define PAGESIZE 4096

struct threadData
{
    int core_id;
    int pageNum;
    int num;
};


void* tlb(void* arg) {
    struct threadData* data = (struct threadData*) arg;
    int core_id = data->core_id;

    // Set thread affinity to a specific CPU core
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
        perror("pthread_setaffinity_np");
    }

    // Your thread's code here
    int pageNum = data->pageNum;
    int num = data->num;

    struct timespec start, end;

	int i, j;
	char arr[PAGESIZE * pageNum];

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

    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t thread;
    int core_id = 0; // Replace with the desired CPU core number

    struct threadData data;
    data.core_id = 0;
    data.pageNum = atoi(argv[1]);
    data.num = atoi(argv[2]);
    
    // int core_ids[] = {core_id};

    // Create the thread
    if (pthread_create(&thread, NULL, tlb, &data) != 0) {
        perror("pthread_create");
        return 1;
    }

    // Wait for the thread to finish (optional)
    pthread_join(thread, NULL);

    return 0;
}
