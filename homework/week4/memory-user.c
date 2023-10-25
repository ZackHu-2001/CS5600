#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Incorrect number of argument.");
        exit(1);
    }

    // size_t
    int byteToAlloc = atoi(argv[1]);

    int* memory = (int*) malloc(byteToAlloc * 1024 * 1024);

    if (memory == NULL) {
        printf("Failed to alloc memory.");
        exit(1);
    }

    int index = 0;
    int maxindex=  byteToAlloc * 1024 * 1024 / 4;

    while (1)
    {
        // printf("%d\n", index);
        memory[index] = 0;
        index += 1;
        if (index == maxindex) {
            index = 0;
        }
    }
    

}