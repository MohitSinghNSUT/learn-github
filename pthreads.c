#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define NUM_ITERATIONS 100000

int counter = 0;                   // Shared variable
pthread_mutex_t lock;              // Mutex for synchronizing access to 'counter'

// Thread function that increments the shared counter
void *increment_counter(void *arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        // Lock the mutex before accessing the shared resource
        pthread_mutex_lock(&lock);
        counter++;
        // Unlock the mutex after the update is done
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;

    // Initialize the mutex
    if (pthread_mutex_init(&lock, NULL) != 0) {
        fprintf(stderr, "Error initializing mutex\n");
        return 1;
    }
    
    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        rc = pthread_create(&threads[i], NULL, increment_counter, NULL);
        if (rc) {
            fprintf(stderr, "Error creating thread %d, return code: %d\n", i, rc);
            exit(EXIT_FAILURE);
        }
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    // Print the final value of the counter
    printf("Final counter value: %d\n", counter);
    
    // Clean up and destroy the mutex
    pthread_mutex_destroy(&lock);
    
    return 0;
}
