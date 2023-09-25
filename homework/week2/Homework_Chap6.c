#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

int main() {
    clock_t start, end;
    double cpu_time_used_in_total, cpu_time_used_per_syscall;

    // read 0 size for a million times
    int iteration = 10000000;

    start = clock();
    char buffer;

    for (int i=0; i<iteration; i++) {
        read(STDIN_FILENO, &buffer, 0);
    }

    end = clock();
    printf("CPU clock tick used: %ld.\n", (end - start));
    cpu_time_used_in_total = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Time used in total: %lfs.\n", cpu_time_used_in_total);
    cpu_time_used_per_syscall = cpu_time_used_in_total / iteration;

    printf("Time used per system call: %lf microseconds\n", cpu_time_used_per_syscall*1000000);

    return 0;
}
