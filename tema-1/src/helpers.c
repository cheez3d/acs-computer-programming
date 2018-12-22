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

    const unsigned long long left = *(const unsigned long long *const)a;
    const unsigned long long right = *(const unsigned long long *const)b;

    if (left > right) {
        res = -1;
    } else if (left < right) {
        res = 1;
    }

    return res;
}
