#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h> 

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int ready = 0;
static int done = 0;

static void* thread_produce(void *args)
{
    int ret;
    for (;done <100 ;){
        ret = pthread_mutex_lock(&mtx);
        if (ret != 0)
            return NULL;
        ++ready;

        printf("+");
        ret = pthread_mutex_unlock(&mtx);
    }
}

static void* thread_consume(void *args)
{
    int ret;
    for(;done < 10 ;)
    {
        ret = pthread_mutex_lock(&mtx);
        if(ret != 0) return NULL;
        while (ready == 0){
            ret = pthread_cond_wait(&cond, &mtx);
            if(ret != 0) return NULL;
        }
        while (ready > 0){
            --ready;
            ++done;
            printf("-");
        }
        ret = pthread_mutex_unlock(&mtx);
        if(ret != 0) return NULL;
    }
    return NULL;
}