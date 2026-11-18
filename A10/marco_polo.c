#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

volatile sig_atomic_t child_exited = 0;

void sigalarm_handler(int sig) {
    (void)sig;
    printf("Polo [%d]\n", getpid());
    fflush(stdout);
}

void sigchld_handler(int sig) {
    (void)sig;
    child_exited = 1;
}

int main()
{
    printf("Parent is %d\n", getpid());

    pid_t pid1;
    char char_input;

    if (signal(SIGALRM, sigalarm_handler) == SIG_ERR) {
        printf("Error call to signal, SIGALRM\n");
        exit(1);
    }

    if (signal(SIGCHLD, sigchld_handler) == SIG_ERR) {
        perror("Error call to signal, SIGCHLD");
        exit(1);
    }
    
    pid1 = fork();
    if (pid1 < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid1 == 0) {
        // child process
        signal(SIGALRM, SIG_DFL); // Child should not run parent's Polo
        
        while (1) {
            if (scanf(" %c", &char_input) != 1) {
                break;
            }
            if (char_input == 'm') {
                fflush(stdout);
                kill(getppid(), SIGALRM);
                printf("Marco [%d]\n", getpid());
            } else if (char_input == 'q') {
                printf("Killing child process\n");
                fflush(stdout);
                exit(0);
            } else {
                printf("Command '%c' not recognized\n", char_input);
                fflush(stdout);
            }
        }
    } else {
        // parent process
        while (!child_exited) {
            pause();
            if (!child_exited) {
                fflush(stdout);
            }
        }
        waitpid(pid1, NULL, 0);
    }

    return 0;
}
