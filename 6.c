#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_FIB 10

void *compute_fib(void *arg) {
    int n = *(int *)arg;
    long long a = 0, b = 1, c;
    if (n == 0) {
        printf("Fib(%d) = %lld\n", n, a);
        return NULL;
    }
    if (n == 1) {
        printf("Fib(%d) = %lld\n", n, b);
        return NULL;
    }
    for (int i = 2; i <= n; i++) {
        c = a + b;
        a = b;
        b = c;
    }
    printf("Fib(%d) = %lld\n", n, b);
    return NULL;
}

int main() {
    pthread_t threads[NUM_FIB+1];
    int indices[NUM_FIB];
    
    for (int i = 0; i < NUM_FIB; i++) {
        indices[i] = i;
        if (pthread_create(&threads[i], NULL, compute_fib, &indices[i]) != 0) {
            perror("Failed to create thread");
            exit(1);
        }
    }
    
    for (int i = 0; i < NUM_FIB; i++) {
        pthread_join(threads[i], NULL);
    }
    
    return 0;
}