#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "generate.h"

void generate_hotels(uint64_t *hotels, int n)
{
    srand(time(NULL));
    uint64_t current = 0;
    for (int i = 0; i < n; i++) {
        hotels[i] = current;
        // printf("hotels[%d] = %llu\n", i, hotels[i]);
        current += 1 + rand() % 400 + 1;
    }
}
