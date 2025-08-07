#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "generate.h"

void longtrip(int *hotels, int nhotels, int *stops);
void longtrip2(int *hotels, int nhotels, int *stops);

int penalty(int dist) {
    int diff = 200 - dist;
    return diff * diff;
}

void print_stops(int *hotels, int *stops, int nstops)
{
    int printed = 0;
    for (int i = 0; i < nstops; i++) {
        if (stops[i]) {
            printf("stop at: %5d ", hotels[i]);
            printed++;
        }
        if (printed != 0 && printed % 4 == 0)
            printf("\n");
    }
    printf("\n");
}

int main()
{
    int hotels_medium[] = {
        0,286,454,718,1015,1192,1272,1408,1608,1802,
        1975,2245,2527,2674,2866,3027,3246,3544,3669,3868,
        3973,4115,4220,4483,4577,4805,5079,5213,5419    };
    int hotels_small[] = {0, 130, 211, 472, 863, 900};

    int nhotels = 6;
    int *hotels = malloc(sizeof(int) * nhotels);
    generate_hotels(hotels, nhotels);

    // TODO: måste nog också använda malloc
    int stops[nhotels];

    for (int i = 0; i < nhotels; i++)
        stops[i] = 0;

    clock_t t0, t1;
    double elapsed_ms;

    // t0 = clock();
    // longtrip(hotels, nhotels, stops);
    // t1 = clock();
    // elapsed_ms = (double)(t1-t0) * 1000.0 / CLOCKS_PER_SEC;
    // printf("longtrip() took %.1f ms\n", elapsed_ms);

    t0 = clock();
    // for (int k = 0; k<10000; k++)
        longtrip2(hotels, nhotels, stops);
    t1 = clock();
    elapsed_ms = (double)(t1-t0) * 1000.0 / CLOCKS_PER_SEC;
    printf("longtrip2() took %.1f ms\n", elapsed_ms);

    int current = 0;
    int total = 0;
    for (int i = 0; i < nhotels; i++) {
        if (stops[i]) {
            int dist = hotels[i] - current;
            total += penalty(dist);
            current = hotels[i];
        }
    }
    print_stops(hotels, stops, nhotels);
    printf("optimal penalty: %d\n", total);
    free(hotels);
}

/* longtrip: dumbly solve assignment 6.2
* array hotels containing distances to hotels a_1 to a_{nhotels} 
* int nhotels
* array stops, each value is 1 if you stop at hotel stops[i], 0 otherwise*/
void longtrip(int *hotels, int nhotels, int *stops)
{
    int beststops[nhotels];
    int lowest_penalty = INT_MAX;

    typedef unsigned long long U64;
    U64 total_combinations = 1ULL << nhotels;
    for (U64 mask = 1; mask < total_combinations; ++mask) {
        if (!(mask & (1ULL << (nhotels -1))))
            continue;
        int current = 0;
        int total = 0;

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

void longtrip2(int hotels[], int nhotels, int stops[])
{
    int d[nhotels];
    int prev[nhotels]; // previous stop in optimal path
    d[0] = hotels[0]; // hotels[0] should be 0
    prev[0] = -1;

    for (int j = 1; j < nhotels; j++) {
        int min_penalty = INT_MAX;
        int current_penalty;
        int dist;
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

    for (int i = 0; i < nhotels; i++)
        printf("d[%d] = %d, prev[%d] = %d\n", i, d[i], i, prev[i]);
    for (int i = 0; i < nhotels; i++)
        stops[i] = 0;
    for (int j = nhotels - 1; j > 0; j = prev[j])
        stops[j] = 1;
}
