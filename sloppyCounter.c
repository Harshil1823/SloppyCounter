// Harshil Shah
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

// Global variables
int N_Threads = 2;
int Sloppiness = 10;
int WorkTime = 10; // This parameter allows you to control the pace of the work performed by each thread.
int WorkIterations = 100; // thread will repeatedly perform work for the specified number of iterations
// CPU-bound threads, you should aim to keep the CPU cores 
// busy and actively executing code for as much of the work_time as possible. 
// This may involve using busy waiting loops or other CPU-intensive operations.
int CPU_Bound = 0;  // Default to false (I/O bound)
// if DoLogging set to true 
//should print out information about its progress, 
//including the settings and the global counter value at specified intervals
int DoLogging = 0;  // Default to false
// Global counter and its mutex
int global_counter = 0;

pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
// Array of local buckets
int* local_buckets;  // Dynamically allocated based on the number of threads
// Function to simulate work
void simulate_work(int id, int work_time) {
    // if cpu bound is false then do usleep
    // if it's true then do cpu bound work
    if (CPU_Bound) {
        long increments = work_time * 1000;  // Adjust as needed
        for (long j = 0; j < increments; ++j) {
            // This loop will keep the CPU busy for work_time milliseconds
        }
    } else {
        // Sleep-based work
        int sleep_time = (rand() % (work_time / 2)) + work_time / 2;
        usleep(sleep_time * 1000);  // Sleep for sleep_time milliseconds
    }
}

// Function for each thread
void* thread_function(void* arg) {
    int thread_id = *(int*)arg;

    for (int i = 0; i < WorkIterations; i++) {
        simulate_work(thread_id, WorkTime);

        // Check sloppiness and update the global counter
        if (i % Sloppiness == 0) {
            // Acquire the mutex before updating the counter
            pthread_mutex_lock(&counter_mutex);
            global_counter += Sloppiness;
            local_buckets[thread_id] += Sloppiness;  // Update local bucket
            pthread_mutex_unlock(&counter_mutex);
            
            // Check if the global counter has reached the target value
            pthread_mutex_lock(&counter_mutex);
            if (global_counter >= N_Threads * WorkIterations) {
                pthread_mutex_unlock(&counter_mutex);
                return NULL; // Terminate the thread
            }
            pthread_mutex_unlock(&counter_mutex);

        }
    }

    return NULL;
}

// Function for logging
void* log_function(void* arg) {
    int log_interval = *(int*)arg;
    while (1) {
        // Sleep for the specified interval
        usleep(log_interval * 1000);

        // Read and print the global counter
        pthread_mutex_lock(&counter_mutex);
        int counter_value = global_counter;

        printf("Global Ct = %dLocals [", counter_value);
        for (int i = 0; i < N_Threads; i++) {
            printf("%d", local_buckets[i]);
            if (i < N_Threads - 1) {
                printf(",");
            }
        }
        printf("]\n");

        pthread_mutex_unlock(&counter_mutex);
        if (global_counter >= N_Threads * WorkIterations) {
            break;
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    // Set default values for parameters
    int default_N_Threads = 2;
    int default_Sloppiness = 10;
    int default_WorkTime = 10;
    int default_WorkIterations = 100;
    int default_CPU_Bound = 0;  // Default to false (I/O bound)
    int default_DoLogging = 0;  // Default to false

    // Parse command line arguments and set default values where necessary
    if (argc > 1) {
        N_Threads = atoi(argv[1]);
    } else {
        N_Threads = default_N_Threads;
    }
    if (argc > 2) {
        Sloppiness = atoi(argv[2]);
    } else {
        Sloppiness = default_Sloppiness;
    }
    if (argc > 3) {
        WorkTime = atoi(argv[3]);
    } else {
        WorkTime = default_WorkTime;
    }
    if (argc > 4) {
        WorkIterations = atoi(argv[4]);
    } else {
        WorkIterations = default_WorkIterations;
    }
    if (argc > 5) {
        CPU_Bound = (strcmp(argv[5], "true") == 0);
    } else {
        CPU_Bound = default_CPU_Bound;
    }
    if (argc > 6) {
        DoLogging = (strcmp(argv[6], "true") == 0);
    } else {
        DoLogging = default_DoLogging;
    }

    printf("Simulation Parameters:\n");
    printf("#threads = %d\n", N_Threads);
    printf("sloppiness = %d\n", Sloppiness);
    printf("work time = %d\n", WorkTime);
    printf("work iterations = %d\n", WorkIterations);
    if (CPU_Bound) {
        printf("CPU bound = (true)\n");
    } else {
        printf("CPU bound = (false)\n");
    }

    if (DoLogging) {
        printf("Do logging = (true)\n");
    } else {
        printf("Do logging = (false)\n");
    }

    // dynamically allocating based on the number of threads
    // since number of threads are likely to change
    local_buckets = (int*)malloc(N_Threads * sizeof(int));
    if (local_buckets == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }
    // initalize the local bucket to zero
    for (int i = 0; i < N_Threads; i++) {
        local_buckets[i] = 0;
    }

    // Initialize random number generator
    srand((unsigned int)time(NULL));

    // Create and manage threads
    pthread_t threads[N_Threads];
    int thread_ids[N_Threads];

    for (int i = 0; i < N_Threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]);
    }

    // Create a logging thread if logging is enabled
    int logging_frequency = (WorkTime * WorkIterations) / 10;
    pthread_t logging_thread;

    if (DoLogging) {
        pthread_create(&logging_thread, NULL, log_function, &logging_frequency);
    }

    // Wait for all threads to finish
    for (int i = 0; i < N_Threads; i++) {
        pthread_join(threads[i], NULL);
    }

    //  ensures that the main thread 
    // waits for both worker threads and the logging thread to finish
    if (DoLogging) {
        pthread_join(logging_thread, NULL);
    }

    // final local buckets 
    printf("Final Local Buckets:");
    for (int i = 0; i < N_Threads; i++) {
        printf(" %d", local_buckets[i]);
    }
    printf("\n");

    if (DoLogging) {
        pthread_mutex_lock(&counter_mutex);
        printf("Final Global Counter: %d\n", global_counter);
        pthread_mutex_unlock(&counter_mutex);
    } else {
        printf("DoLogging is still false this is to check your answer\n");
        printf("Final Global Counter: %d\n", global_counter);
    }

    // free up the space
    free(local_buckets);
    return 0;
}
