#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define STACK_SIZE (1024 * 1024)
#define CLONES_COUNT 1000
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int c[1000];
char child_stack[STACK_SIZE * CLONES_COUNT];

int child_proc(void* param) {
    int i = *(int*) param;
    while (c[i] < 20) {
        usleep(50000);
        pthread_mutex_lock(&mutex);
        printf("child number [%d] : %d\n",i, c[i]++);
        pthread_mutex_unlock(&mutex);
    }
    printf("finish child pid: %d\n", getpid());
    _exit(0);
}

int main() {
    int result[1000];
    for (int i = 1; i <= CLONES_COUNT; ++i) {
        result[i] = clone(child_proc, child_stack + i * STACK_SIZE,CLONE_VM, &i);
        if (-1 == result[i]) {
            perror("clone error");
        }    
        printf("clone number [%d] result = %d\n", i , result[i]);
    }
    while(1);
    return 0;
}
