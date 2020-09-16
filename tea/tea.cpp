#include "tea.h"

const unsigned int c_delta = 0x9E3779B9;
const unsigned int c_sum = 0xC6EF3720;

void encode(unsigned int* v, const unsigned int* k) {
    unsigned int v0 = v[0],
                 v1 = v[1],
                 i,
                 sum = 0;
    for(i = 0; i < 32; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
        sum += c_delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
    }
    v[0] = v0;
    v[1] = v1;
}

void decode(unsigned int* v, const unsigned int* k) {
    unsigned int v0 = v[0],
                 v1 = v[1],
                 i,
                 sum = c_sum;
    for(i = 0; i < 32; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum >> 11) & 3]);
        sum -= c_delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
    }
    v[0] = v0;
    v[1] = v1;
}
