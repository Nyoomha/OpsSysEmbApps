#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static int glob = 0;

static void* thread_function(void *args)
{
    int loops = *((int *) args);
    int ix, ret;
    for (ix=0; ix<loops; ++ix)
    {
        ++glob;
    }

    return NULL;
}

int main(int argc, char*argv[])
{
    pthread_t t1, t2;
    int loops, ret;
    loops = (argc > 1) ? atoi(argv[1]) : 1000000;

    ret = pthread_create(&t1, NULL, thread_function, &loops);
    ret = pthread_create(&t2, NULL, thread_function, &loops);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Total = %d\n", glob);

    return 0;

}