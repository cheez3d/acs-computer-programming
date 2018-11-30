#include "helpers.h"

unsigned long long gcd(unsigned long long a, unsigned long long b) {
    while (b != 0) {
        a %= b;

        // xor swap
        a ^= b;
        b ^= a;
        a ^= b;
    }

    return a;
}

int compareDescending(const void *const a, const void *const b) {
    int res = 0;

    if (*(unsigned long long*)a > *(unsigned long long*)b) {
        res = -1;
    } else if (*(unsigned long long*)a < *(unsigned long long*)b) {
        res = 1;
    }

    return res;
}
