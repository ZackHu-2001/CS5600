#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

enum { NS_PER_SECOND = 1000000000 };

void sub_timespec(struct timespec t1, struct timespec t2, struct timespec *td)
{
    td->tv_nsec = t2.tv_nsec - t1.tv_nsec;
    td->tv_sec  = t2.tv_sec - t1.tv_sec;
    if (td->tv_sec > 0 && td->tv_nsec < 0)
    {
        td->tv_nsec += NS_PER_SECOND;
        td->tv_sec--;
    }
    else if (td->tv_sec < 0 && td->tv_nsec > 0)
    {
        td->tv_nsec -= NS_PER_SECOND;
        td->tv_sec++;
    }
}

int counter;

void *increaseCounter(pthread_mutex_t* lock)
{
    pthread_mutex_lock(lock);
    for (int i=0; i<100; i++) {
        counter += 1;
    }
    pthread_mutex_unlock(lock);
    pthread_exit(NULL);
}

int main(void)
{
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);

    struct timespec start, finish, delta;
    clock_gettime(CLOCK_REALTIME, &start);

    for (int i=0; i<8; i++) {
        fork(increaseCounter(&lock)));
    }
    clock_gettime(CLOCK_REALTIME, &finish);
    sub_timespec(start, finish, &delta);
    printf("%d.%.9ld\n", (int)delta.tv_sec, delta.tv_nsec);
    
    return 0;
}