#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

// Q1

// int main(int argc, char* argv[]) {
//     int x = 100;
//     int rc = fork();

//     if (rc < 0) {
//         fprintf(stderr, "fork failed\n");
//         exit(1);
//     } else if (rc == 0) {
//         printf("X in child process: %d\n", x);
//         x = 50;
//         printf("X in child process after change: %d\n\n", x);
//     } else {
//         printf("X in child process: %d\n", x);
//         x = 200;
//         printf("X in parent process after change: %d\n\n", x);
//     }

//     return 0;
// }

// Q: What value is the variable in the child process?
// A: The value in child process is 100 as well, because the child process
// is an exact copy of parent process.

// Q: What happens to the variable when both the child and parent change x?
// A: The values are changed seperately, cause they are independent value stored
// with in each process.


// Q2

// int main(int argc, char* argv[]) {
//     int fd;
//     const char* fileName = "example.txt";
//     fd = open(fileName, O_WRONLY | O_CREAT, 0644);

//     if (fd == -1) {
//         fprintf(stderr, "Failed to open file.");
//         exit(1);
//     }

//     int rc = fork();
//     if (rc == -1) {
//         fprintf(stderr, "Failed to fork.");
//         exit(1);
//     } else if (rc == 0) {
//         const char* sentence = "Hello world from child!\n";
//         ssize_t byte_written;

//         for (int i=0; i<10000; i++) {
//             byte_written = write(fd, sentence, strlen(sentence));
//         }

//         if (byte_written == -1) {
//             fprintf(stderr, "Child failed to write into the file.");
//             exit(1);
//         } else {
//             printf("Child write to file successfully.\n");
//         }
//     } else {
//         const char* sentence = "Hello world from parent!\n";
//         ssize_t byte_written;
//         for (int i=0; i<10000; i++) {
//             byte_written = write(fd, sentence, strlen(sentence));
//         }

//         if (byte_written == -1) {
//             fprintf(stderr, "Parent failed to write into the file.");
//             exit(1);
//         } else {
//             printf("Parent write to file successfully.\n");
//         }

//         wait(NULL);
//         close(fd);
//         printf("Parent wait for child and close the file descriptor.\n");
//     }

//     return 0;
// }

// Q: Can both the childand parent access the file descriptor returned by open()? 
// A: Yes, because both process is created by the same user that is also the user who creates
// the file, therefore, they share the same privilege to read and write into the file.

// Q: What happens when they are writing to the file concurrently, i.e., at the same time?
// A: It works properly, sometimes parent process write to file and sometimes child write.
// I think GCC might have done some optimization?? and make the write to file an atomic 
// instruction that they do not interleave with each other.


// Q3

// int main(int argc, char* argv[]) {
//     int rc = fork();
//     int status;

//     if (rc == -1) {
//         fprintf(stderr, "Failed to create child process.\n");
//         exit(1);
//     } else if (rc == 0) {
//         printf("Hello!\n");
//     } else {
//         // choice one: use waitPid()
//         waitpid(rc, &status, 0);
//         // choice two: use sleep() to pause the process for 2 seconds
//         sleep(2);

//         printf("GoodBye!\n");
//     }

//     return 1;
// }

// Q: can you do this without calling wait() in the parent?
// A: Yes, we can use waitpid() or sleep() to replace wait().
// However, there is limitation on sleep() as it can not guarentee
// that child process finished with in the second you set.


// Q4

// int main(int argc, char* argv[]) {
//     int rc = fork();

//     if (rc == -1) {
//         fprintf(stderr, "Failed to create child process.\n");
//         exit(1)
//     } else if (rc == 0) {
//         char * myArgs[2];
//         myArgs[0] = "ls";
//         myArgs[1] = NULL;

//         char *const envp[] = {"VAR=value", NULL};
        
//         // basic version
//         execl("/bin/ls", "ls", NULL);
//         // allow define environment variable
//         execle("/bin/ls", "ls", NULL, envp);
//         // search program from PATH rather specify program location
//         execlp("ls", "ls", NULL);
//         // takes an array of command-line arguments instead of a variable-length argument list.
//         execv("/bin/ls", myArgs);
//         // search program from PATH rather specify program location
//         execvp("ls", myArgs);
//     } else {
//         // do nothing in parent process
//     }

//     return 1;
// }

// Q: Why do you think there are so many variants of the same basic call?
// A: Because different variants provides the flexibility to switch to process 
// in our desired manner. 


// Q5

// int main(int argc, char* argv[]) {
//     int rc = fork();
//     int status;

//     if (rc == -1) {
//         fprintf(stderr, "Failed to create child process.\n");
//         exit(1);
//     } else if (rc == 0) {
//         int wait_result =wait(&status);
//         if (wait_result == -1) {
//             fprintf(stderr, "No child process to wait.\n");
//             exit(1);
//         }
//     } else {
//         int wait_result = wait(&status);
//         printf("wait returned value: %d, child PID: %d\n", wait_result, rc);
//     }

//     return 1;
// }

// Q: What does wait() return? 
// A: Returned the child's PID.

// Q: What happens if you use wait() in the child?
// A: Returned -1, which indicates no child process to wait.


// Q6

// int main(int argc, char* argv[]) {
//     int rc = fork();
//     int status;

//     if (rc == -1) {
//         fprintf(stderr, "Failed to create child process.\n");
//         exit(1);
//     } else if (rc == 0) {
//         sleep(1);
//     } else {
//         int wait_result = waitpid(rc, &status, 0);
//         printf("wait returned value: %d, child PID: %d\n", wait_result, rc);
//     }

//     return 1;
// }

// Q: When would waitpid() be useful?
// A: Under the situation that you have several child processes and you
// want specific control. As it offers greater flexibility as it allows 
// us to wait for specific child processes or specify additional options.


// Q7

// int main(int argc, char* argv[]) {
//     int rc = fork();

//     if (rc == -1) {
//         fprintf(stderr, "Failed to create child process.\n");
//         exit(1);
//     } else if (rc == 0) {
//         close(STDOUT_FILENO);
//         printf("Can you see this line?");
//     }

//     return 1;
// }

// Q: What happens if the child calls printf() to print some output after closing the descriptor?
// A: That printf does not work because the STDOUT_FILENO is used to 
// write output to standard output and is turned off in this senario.


// Q8

int main(int argc, char* argv[]) {
    char buf[BUFSIZ];
    int fd[2];

    if (pipe(fd) < 0) {
        fprintf(stderr, "pipe failed");
        exit(1);
    }

    int child1 = fork();
    if (child1 == -1) {
        fprintf(stderr, "Failed to create child process.\n");
        exit(1);
    } else if (child1 == 0) {
        close(fd[0]);
        strcpy(buf, "Hello from child process 1!\n");
        write(fd[1], buf, strlen(buf));
        close(fd[1]);
        exit(EXIT_SUCCESS);
    }

    int child2 = fork();
    if (child2 == -1) {
        fprintf(stderr, "Failed to create child process.\n");
        exit(1);
    } else if (child2 == 0) {
        close(fd[1]);
        while (read(fd[0], buf, BUFSIZ) > 0) {
            printf("Child process 2 received message:\n%s", buf);
        }
        close(fd[0]);
        exit(EXIT_SUCCESS);
    }
    close(fd[0]);
    close(fd[1]);

    wait(NULL);
    wait(NULL);
    return 1;
}
