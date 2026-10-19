#ifndef PERF_H
#define PERF_H
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define USE_PERF

#define TIMER_NAME_SIZE 64

typedef struct Timer {
    char name[TIMER_NAME_SIZE];
    struct timespec start;
    struct timespec lap;
} Timer;

Timer *NewTimer(char *name) {
#ifdef USE_PERF
    Timer *t = malloc(sizeof(Timer));

    strncpy(t->name, name, sizeof(t->name));
    if (t->name[TIMER_NAME_SIZE - 1] != '\0') {
        t->name[TIMER_NAME_SIZE - 1] = '\0';
    }
    printf("Timer started with name: %s.\n", name);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &(t->start));
    return t;
#else
    return NULL;

#endif
}

void DeleteTimer(Timer *t) {
#ifdef USE_PERF
    free(t);
#endif
}

void Lap(Timer *t) {
#ifdef USE_PERF
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &(t->lap));

    long long diff_sec = t->lap.tv_sec - t->start.tv_sec;
    long long diff_nsec = t->lap.tv_nsec - t->start.tv_nsec;

    // 2. Combine them into a single total nanosecond count
    long long total_ns = (diff_sec * 1000000000LL) + diff_nsec;

    printf("%s: %05.05lld ns\n", t->name, total_ns);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &(t->start));
#endif
}



#endif  // PERF_H