#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "generate.h"

void longtrip(uint64_t *hotels, int nhotels, uint64_t *stops);
void longtrip2(uint64_t *hotels, int nhotels, uint64_t *stops);

int penalty(int dist) {
    int diff = 200 - dist;
    return diff * diff;
}

void print_stops(uint64_t *hotels, uint64_t *stops, int nstops)
{
    int printed = 0;
    int i;
    for (i = nstops>20 ? nstops-20 : 0; i < nstops; i++) {
        if (stops[i]) {
            printf("stop at %d: %5llu ", i, hotels[i]);
            printed++;
        if (printed != 0 && printed % 4 == 0)
            printf("\n");
        }
    }
    printf("\n");
}

int main()
{
    // int hotels_medium[] = {
    //     0,286,454,718,1015,1192,1272,1408,1608,1802,
    //     1975,2245,2527,2674,2866,3027,3246,3544,3669,3868,
    //     3973,4115,4220,4483,4577,4805,5079,5213,5419    };
    // int hotels_small[] = {0, 130, 211, 472, 863, 900};

    int nhotels = 10;
    uint64_t *hotels = malloc(sizeof(uint64_t) * nhotels);
    generate_hotels(hotels, nhotels);

    uint64_t *stops = malloc(sizeof(uint64_t) * nhotels);
    for (int i = 0; i < nhotels; i++)
        stops[i] = 0;

    clock_t t0, t1;
    double elapsed_ms;

    t0 = clock();
    longtrip(hotels, nhotels, stops);
    t1 = clock();
    elapsed_ms = (double)(t1-t0) * 1000.0 / CLOCKS_PER_SEC;
    printf("longtrip() took %.1f ms\n", elapsed_ms);

    // t0 = clock();
    // longtrip2(hotels, nhotels, stops);
    // t1 = clock();
    // elapsed_ms = (double)(t1-t0) * 1000.0 / CLOCKS_PER_SEC;
    // printf("longtrip2() took %.1f ms\n", elapsed_ms);

    // TODO: do I need to actually verify my solution somehow?
    uint64_t current = 0;
    uint64_t total = 0;
    for (int i = 0; i < nhotels; i++) {
        if (stops[i]) {
            int dist = hotels[i] - current;
            total += penalty(dist);
            current = hotels[i];
        }
    }
    print_stops(hotels, stops, nhotels);
    printf("optimal penalty: %llu\n", total);
    free(hotels);
}

/* longtrip: dumbly solve assignment 6.2
* array hotels containing distances to hotels a_1 to a_{nhotels} 
* int nhotels
* array stops, each value is 1 if you stop at hotel stops[i], 0 otherwise*/
void longtrip(uint64_t *hotels, int nhotels, uint64_t *stops)
{
    uint64_t beststops[nhotels];
    uint64_t lowest_penalty = UINT64_MAX;

    typedef unsigned long long U64;
    U64 total_combinations = 1ULL << nhotels;
    for (U64 mask = 1; mask < total_combinations; ++mask) {
        if (!(mask & (1ULL << (nhotels -1))))
            continue;
        uint64_t current = 0;
        uint64_t total = 0;

        for (int i = 0; i < nhotels; i++) {
            if (mask & (1ULL << i)) {
                int dist = hotels[i] - current;

                total += penalty(dist);
                current = hotels[i];
            }
        }
        if (total < lowest_penalty) {
            lowest_penalty = total;
            for (int i = 0; i < nhotels; i++)
                beststops[i] = (mask & (1 << i)) ? 1 : 0;
        }
    }
    for (int i = 0; i < nhotels; i++)
        stops[i] = beststops[i];
}

void longtrip2(uint64_t hotels[], int nhotels, uint64_t stops[])
{
    uint64_t d[nhotels];
    uint64_t prev[nhotels]; // previous stop in optimal path
    d[0] = hotels[0]; // hotels[0] should be 0
    prev[0] = -1;

    for (int j = 1; j < nhotels; j++) {
        uint64_t min_penalty = UINT64_MAX;
        uint64_t current_penalty;
        uint64_t dist;
        for (int i = 0; i < j; i++) {
            dist = hotels[j] - hotels[i];
            current_penalty = d[i] + penalty(dist);
            if (current_penalty < min_penalty) {
                min_penalty = current_penalty;
                prev[j] = i;
            }
        }
        d[j] = min_penalty;
    }

    // TODO: disable/change for large tests?
    // for (int i = 0; i < nhotels; i++)
    //     printf("d[%d] = %d, prev[%d] = %d\n", i, d[i], i, prev[i]);
    for (int i = 0; i < nhotels; i++)
        stops[i] = 0;
    for (int j = nhotels - 1; j > 0; j = prev[j])
        stops[j] = 1;
}
