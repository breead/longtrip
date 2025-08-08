#include <stdio.h>
#include <stdlib.h>
#include "generate.h"

// TODO: change to 64 bit ints?
void generate_hotels(uint64_t *hotels, int n)
{
    uint64_t current = 0;
    for (int i = 0; i < n; i++) {
         hotels[i] = current;
         current += 1 + rand() % 400 + 1;
    }
}
