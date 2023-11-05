#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Variables
#define BUFFER_SIZE 100
#define PRODUCER_SLEEP_TIME 2
#define CONSUMER_SLEEP_TIME 1
#define CONSUMER_UNITS_TO_BUY 10
#define PRODUCER_UNITS_TO_SUPPLY 18

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t empty_cond = PTHREAD_COND_INITIALIZER;
static pthread_cond_t full_cond = PTHREAD_COND_INITIALIZER;

static int stock = 0;

// Producer thread function
static void* thread_produce(void *args)
{
    int ret;
    for (;;) {  // infinite loop
        ret = pthread_mutex_lock(&mtx);
        if (ret != 0) return NULL;

        // Check if there is enough space in the buffer
        while (stock + PRODUCER_UNITS_TO_SUPPLY > BUFFER_SIZE) {
            ret = pthread_cond_wait(&full_cond, &mtx);
            if (ret != 0) return NULL;
        }

        // Produce units and update stock
        stock += PRODUCER_UNITS_TO_SUPPLY;
        printf("Produced %d units. Stock: %d\n", PRODUCER_UNITS_TO_SUPPLY, stock);

        ret = pthread_mutex_unlock(&mtx);
        if (ret != 0) return NULL;

        // Signal that the buffer is not empty
        pthread_cond_signal(&empty_cond);

        // Sleep for a specified time before producing again
        usleep(PRODUCER_SLEEP_TIME * 1000000);
    }
}

// Consumer thread function
static void* thread_consume(void *args)
{
    int ret;
    for (;;) {
        ret = pthread_mutex_lock(&mtx);
        if (ret != 0) return NULL;

        // Check if there are enough units to buy
        while (stock < CONSUMER_UNITS_TO_BUY) {
            ret = pthread_cond_wait(&empty_cond, &mtx);
            if (ret != 0) return NULL;
        }

        // Consume units and update stock
        stock -= CONSUMER_UNITS_TO_BUY;
        printf("Consumed %d units. Stock: %d\n", CONSUMER_UNITS_TO_BUY, stock);

        ret = pthread_mutex_unlock(&mtx);
        if (ret != 0) return NULL;

        // Signal that the buffer is not full
        pthread_cond_signal(&full_cond);

        // Sleep for a specified time before consuming again
        usleep(CONSUMER_SLEEP_TIME * 1000000);
    }
}

// Main function
int main(int argc, char *argv[])
{
    pthread_t producers, consumers;
    int ret;

    // Create producer and consumer threads
    ret = pthread_create(&producers, NULL, thread_produce, NULL);
    ret = pthread_create(&consumers, NULL, thread_consume, NULL);

    // Wait for consumer thread to finish
    pthread_join(consumers, NULL);

    // Cancel the producer thread (it may still be sleeping)
    pthread_cancel(producers);

    // Wait for the producer thread to finish
    pthread_join(producers, NULL);

    return 0;
}
