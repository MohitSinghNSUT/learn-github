#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 5
#define NUM_ITERATIONS 10

int counter = 0;
pthread_mutex_t counter_mutex;

void *increment_counter(void *arg) {
    int idx = *(int*)arg;
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        pthread_mutex_lock(&counter_mutex);
        counter++;
        printf("Thread %d : counter : %d\n",idx,counter);
        pthread_mutex_unlock(&counter_mutex);
    }
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    
    pthread_mutex_init(&counter_mutex, NULL);
    
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, increment_counter, &i) != 0) {
            perror("Failed to create thread");
            exit(1);
        }
    }
    
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Final counter value: %d\n", counter);
    
    pthread_mutex_destroy(&counter_mutex);
    return 0;
}