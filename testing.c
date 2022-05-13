#include "bub_semaph.h"
#include <stdio.h>
#include <stdlib.h>

/*
 This testing code will create 15 threads, which will wait 1 second each.
 They are constrained by a semaphore that only allows at most 5 threads to wait at once.

 The expected runtime is around three (15/5) seconds.
*/

bub_semaphore_t* semaph;

void* thread_code(void* arg) {
    int idx = *((int*)arg);
    bub_semaphore_acquire(semaph);
    printf("T%i: After acquisition: %i\n", idx, semaph->count);
    printf("Thread %i started sleeping\n", idx);
    sleep(1);
    printf("Thread %i stopped sleeping\n", idx);
    bub_semaphore_release(semaph);
    printf("T%i: After release: %i\n", idx, semaph->count);
    return NULL;
}

int main() {
    semaph = malloc(sizeof(bub_semaphore_t));
    bub_semaphore_init(semaph, 5);
    
    const int N = 15;
    pthread_t T[N];
    int args[N]; // this will exist over the whole course of the threads' lifetimes
    for (int i = 0; i < N; i++) {
        args[i] = i;
        pthread_create(&T[i], NULL, &thread_code, &args[i]);
    }

    for (int i = 0; i < N; i++)
        pthread_join(T[i], NULL);

    bub_semaphore_destroy(semaph);
    free(semaph);
}
