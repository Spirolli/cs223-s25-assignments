#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// Helpful macros for working with colors
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

pthread_mutex_t mutex;

struct t_arg {
    char **file_array; // pointer to shared file array
    long length; // number of elements in array
    char *keyword; // the keyword to search for in the files in file array
    long num_threads; // total number of threads
    long *successful_matches; // shared memory of successful matches
    long id; // logical thread id
};

int searchFile(char *filename, char *keyword) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        perror("Error opening file\n");
        return 0;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int total_matches = 0;

    while ((read = getline(&line, &len, fp)) != -1) {
        if (strstr(line, keyword) != NULL) {
            printf("%ld) "ANSI_COLOR_MAGENTA"[%s]"ANSI_COLOR_RESET" %s",
                    pthread_self(), filename, line);

            total_matches++;
        }
    }

    free(line);
    fclose(fp);
    return total_matches;
}

void *searchFilesByThreads(void* args) {
    // cast to a struct t_arg from void
    struct t_arg * myargs = (struct t_arg *) args;

    long myid = myargs->id;
    char **file_array = myargs->file_array;
    long length = myargs->length;
    char *keyword = myargs->keyword;
    long num_threads = myargs->num_threads;

    long chunk = length / num_threads;
    long start = myid * chunk;
    long end = (myid + 1) * chunk;
    if (myid == num_threads - 1) {
        end = length;
    }
    
    long i;

    long matches;
    for (i = start; i < end; i++) {
        matches += searchFile(file_array[i], keyword);
    }

    pthread_mutex_lock(&mutex);
    *(myargs->successful_matches) += matches;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

void printElapsedTime(struct timeval start, struct timeval end) {
    long seconds = end.tv_sec - start.tv_sec;
    long microseconds = end.tv_usec - start.tv_usec;
    if (microseconds < 0) {
        seconds -= 1;
        microseconds += 1000000;
    }
    printf("Elapsed time is %ld.%06ld\n", seconds, microseconds);
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("usage: ./grep < NumThreads > < Keyword > < Files >\n");
        return 1;
    }

    int i;
    pthread_t *thread_array; // pointer to future thread array
    struct t_arg *thread_args;

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        fprintf(stderr, "Number of threads must be greater than 0.\n");
        return 1;
    }

    char *keyword = argv[2];

    // parse the files
    int num_files = argc - 3;

    if (num_threads > num_files)
        num_threads = num_files;

    thread_array = malloc(num_threads * sizeof(pthread_t));
    thread_args = malloc(num_threads * sizeof(struct t_arg));

    long successful_matches = 0;

    // Populate thread arguments for all the threads.
    for (i = 0; i < num_threads; i++) {
        thread_args[i].file_array = &argv[3];
        thread_args[i].length = (long)num_files;
        thread_args[i].keyword = keyword;
        thread_args[i].successful_matches = &successful_matches;
        thread_args[i].num_threads = num_threads;
        thread_args[i].id = i;
    }

    printf("Searching %d files for keyword: %s\n", num_files, keyword);

    struct timeval start, end;
    gettimeofday(&start, NULL);
    printf(ANSI_COLOR_YELLOW"Thread_ID) "ANSI_COLOR_MAGENTA"File:"ANSI_COLOR_RESET " search found\n");
    
    pthread_mutex_init(&mutex, NULL); // initialize the mutex
    // Do searching...
    for (i = 0; i < num_threads; i++) {
        pthread_create(&thread_array[i], NULL, searchFilesByThreads, &thread_args[i]);
    }

    for (i = 0; i < num_threads; i++) {
        pthread_join(thread_array[i], NULL);
    }
    pthread_mutex_destroy(&mutex); // destroy (free) the mutex

    gettimeofday(&end, NULL);

    printf("Total matches: %ld\n", successful_matches);

    printElapsedTime(start, end);

    free(thread_array);
    free(thread_args);

    return 0;
}
