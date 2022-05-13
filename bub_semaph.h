#include <pthread.h>
#include <unistd.h>

typedef struct {
    pthread_cond_t cond;
    pthread_mutex_t mutex;
    int count;
} bub_semaphore_t;

int bub_semaphore_init(
    bub_semaphore_t*,
    int
);

int bub_semaphore_acquire(
    bub_semaphore_t*
);

int bub_semaphore_release(
    bub_semaphore_t*
);

void bub_semaphore_destroy(
    bub_semaphore_t*
);