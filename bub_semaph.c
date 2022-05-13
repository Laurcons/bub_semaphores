#include "bub_semaph.h"

int bub_semaphore_init(bub_semaphore_t* sem, int startCount) {
    int err;
    err = pthread_mutex_init(&sem->mutex, NULL);
    if (err != 0)
        return err;
    err = pthread_cond_init(&sem->cond, NULL);
    if (err != 0) {
        pthread_mutex_destroy(&sem->mutex); // what if this fails too?
        return err;
    }
    sem->count = startCount; // no way this can fail
    return 0;
}

int bub_semaphore_acquire(bub_semaphore_t* sem) {
    int err;
    err = pthread_mutex_lock(&sem->mutex);
    if (err != 0)
        return err;
    while (sem->count == 0) {
        err = pthread_cond_wait(&sem->cond, &sem->mutex);
        if (err != 0)
            // per the manual, if it errors out, it means nothing has been modified
            return err;
    }
    // semaphore is acquired
    sem->count--;
    err = pthread_cond_signal(&sem->cond);
    if (err != 0)
        return err; // would some other cleanup/state management be needed here?
    err = pthread_mutex_unlock(&sem->mutex);
    if (err != 0)
        return err;
    return 0;
}

int bub_semaphore_release(bub_semaphore_t* sem) {
    int err;
    err = pthread_mutex_lock(&sem->mutex);
    if (err != 0)
        return err;
    sem->count++;
    err = pthread_cond_signal(&sem->cond);
    if (err != 0)
        return err;
    err = pthread_mutex_unlock(&sem->mutex);
    if (err != 0)
        return err;
    return 0;
}

void bub_semaphore_destroy(bub_semaphore_t* sem) {
    // if any of these fail it's kinda tough luck, i guess...
    pthread_cond_destroy(&sem->cond);
    pthread_mutex_destroy(&sem->mutex);
}
