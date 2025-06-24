#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <curl/curl.h>
#include <time.h>
#include "utils.h"
#include "downloader.h"
#include "processor.h"
#include "stats.h"

char urls[MAX_URLS][MAX_URL_LEN];
int url_count = 0;
int url_index = 0;
int char_freq[26] = {0};
int *sentence_counts;
int *word_counts;
long long download_time_ns   = 0;
long long processing_time_ns = 0;
long long stats_time_ns      = 0;

pthread_mutex_t url_mutex    = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t time_mutex   = PTHREAD_MUTEX_INITIALIZER;

void *worker(void *arg) {
    (void)arg;
    while (1) {
        pthread_mutex_lock(&url_mutex);
        int idx = url_index++;
        pthread_mutex_unlock(&url_mutex);
        if (idx >= url_count) break;

        // --- download timing ---
        long long dt_ns;
        char *content = download_url(urls[idx], &dt_ns);
        pthread_mutex_lock(&time_mutex);
        download_time_ns += dt_ns;
        pthread_mutex_unlock(&time_mutex);

        if (!content) continue;

        // --- processing timing ---
        struct timespec p_start, p_end;
        clock_gettime(CLOCK_MONOTONIC, &p_start);

        int sc = count_sentences(content);
        int wc = count_words(content);
        update_char_freq(content, char_freq);

        clock_gettime(CLOCK_MONOTONIC, &p_end);
        long long pt_ns = (p_end.tv_sec  - p_start.tv_sec) * 1000000000LL
                        + (p_end.tv_nsec - p_start.tv_nsec);
        pthread_mutex_lock(&time_mutex);
        processing_time_ns += pt_ns;
        pthread_mutex_unlock(&time_mutex);

        // --- stats timing (unchanged) ---
        record_stats(idx, sc, wc, sentence_counts, word_counts, &stats_time_ns);

        free(content);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <url_file> <num_threads> <chunk_size>\n", argv[0]);
        return 1;
    }
    int num_threads = atoi(argv[2]);
    curl_global_init(CURL_GLOBAL_ALL);

    read_urls(argv[1], urls, &url_count);
    sentence_counts = calloc(url_count, sizeof(int));
    word_counts     = calloc(url_count, sizeof(int));

    // start total timer
    struct timespec total_start, total_end;
    clock_gettime(CLOCK_MONOTONIC, &total_start);

    pthread_t *threads = malloc(num_threads * sizeof(*threads));
    for (int i = 0; i < num_threads; i++)
        pthread_create(&threads[i], NULL, worker, NULL);
    for (int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    // end total timer
    clock_gettime(CLOCK_MONOTONIC, &total_end);
    double total_time_s = (total_end.tv_sec  - total_start.tv_sec)
                        + (total_end.tv_nsec - total_start.tv_nsec) / 1e9;

    printf("\nResults:\n");
    for (int i = 0; i < url_count; i++) {
        printf("URL No%d: %s\n  Sentences: %d\n  Words: %d\n",
               i+1, urls[i], sentence_counts[i], word_counts[i]);
    }
    printf("\nCharacter Frequency (a-z):\n");
    for (int i = 0; i < 26; i++) if (char_freq[i] > 0)
        printf("%c: %d\n", 'a'+i, char_freq[i]);

    printf("\nTiming Summary (s):\n");
    printf("  Total    : %.3f\n", total_time_s);
    printf("  Download : %.3f\n", download_time_ns   / 1e9);
    printf("  Process  : %.3f\n", processing_time_ns / 1e9);

    curl_global_cleanup();
    free(threads);
    free(sentence_counts);
    free(word_counts);
    return 0;
}
