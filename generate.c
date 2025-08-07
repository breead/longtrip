#include <stdio.h>
#include "generate.h"

void generate_hotels(int *hotels, int n)
{
    int temp[] = {0, 130, 211, 472, 863, 900};
    for (int i = 0; i < n; i++)
        hotels[i] = temp[i];
}
