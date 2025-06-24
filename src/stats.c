#include <time.h>
#include "stats.h"

void record_stats(int idx, int sentences, int words,
                  int *sentence_counts, int *word_counts,
                  long long *stats_time_ns) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    sentence_counts[idx] = sentences;
    word_counts[idx] = words;
    clock_gettime(CLOCK_MONOTONIC, &end);
    *stats_time_ns += (end.tv_sec - start.tv_sec) * 1000000000LL +
                      (end.tv_nsec - start.tv_nsec);
}
