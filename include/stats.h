#ifndef STATS_H
#define STATS_H

void record_stats(int idx, int sentences, int words,
                  int *sentence_counts, int *word_counts,
                  long long *stats_time_ns);

#endif // STATS_H
