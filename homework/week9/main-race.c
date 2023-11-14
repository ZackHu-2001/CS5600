#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>

typedef struct {
    atomic_flag flag;
} Spinlock;

void spinlock_init(Spinlock *lock) {
    atomic_flag_clear(&lock->flag);
}

void spinlock_lock(Spinlock *lock) {
    while (atomic_flag_test_and_set(&lock->flag)) {
        // spin until the lock is acquired
    }
}

void spinlock_unlock(Spinlock *lock) {
    atomic_flag_clear(&lock->flag);
}

int balance = 0;
Spinlock lock; // Initialize the lock

void* worker(void* arg) {
    spinlock_lock(&lock);
    balance++; // protected access
    spinlock_unlock(&lock);
    return NULL;
}

int main(int argc, char *argv[]) {
    spinlock_init(&lock); // Initialize the spinlock before using it

    pthread_t p;
    pthread_create(&p, NULL, worker, NULL);

    spinlock_lock(&lock);
    balance++; // protected access
    spinlock_unlock(&lock);

    pthread_join(p, NULL);
    return 0;
}
