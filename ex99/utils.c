#include <math.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

void radixSortObjInt(void *toSort2, size_t type_size, size_t size, size_t offset_in_struct, void *buffer2) {
    uint8_t *a = toSort2;
    uint8_t *b = buffer2;
    size_t const c = 8;
    size_t const d = pow(2, c); 
    for (size_t x = 0; x < size; x++) *(a+x*type_size+offset_in_struct+sizeof(int)-1) ^= 0b10000000;
    int counts[d+1];
    for (size_t i = 0; i < sizeof(int); i++) {
        memset(counts, 0, sizeof(counts));
        for (size_t x = 0; x < size; x++) counts[*(a+x*type_size+offset_in_struct+i)+1]++;
        for (size_t x = 1; x < d; x++) counts[x] += counts[x-1];
        for (size_t x = 0; x < size; x++) memcpy(b + type_size*counts[*(a+x*type_size+offset_in_struct+i)]++, a+x*type_size, type_size);
        uint8_t *e = a;
        a = b;
        b = e;
    }
    if (sizeof(int) % 2) memcpy(b, a, size*type_size);
    for (size_t x = 0; x < size; x++) *(((uint8_t *)(a+(x*type_size)))+offset_in_struct+sizeof(int)-1) ^= 0b10000000;
}