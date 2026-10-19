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
    uint64_t start;
} Timer;

static inline Timer *NewTimer(char *name) {
#ifdef USE_PERF
    Timer *t = malloc(sizeof(Timer));

    strncpy(t->name, name, sizeof(t->name));
    if (t->name[TIMER_NAME_SIZE - 1] != '\0') {
        t->name[TIMER_NAME_SIZE - 1] = '\0';
    }
    printf("Timer started with name: %s.\n", name);
    t->start = clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
    // clock_gettime(CPU_CLOCK_CFG, &(t->start));
    return t;
#else
    return NULL;

#endif
}

static inline void DeleteTimer(Timer *t) {
#ifdef USE_PERF
    free(t);
#endif
}

static inline void Lap(Timer *t) {
#ifdef USE_PERF
    uint64_t now = clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
    uint64_t elapsed = now - t->start;

    printf("%s: %lld ns\n", t->name, elapsed);
    t->start = clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
#endif
}



#endif  // PERF_H