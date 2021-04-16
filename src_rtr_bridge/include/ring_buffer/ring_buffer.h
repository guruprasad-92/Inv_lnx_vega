#ifndef RING_BUFFER__H
#define RING_BUFFER__H
// 18112020_074300;
#define SZ_BFR  16*4

typedef struct stRNG_BUFR__
{
    char cbuffer[SZ_BFR];
    char *st;
    char *end;
    char *cur;
}stRNG_BUFR_;

int rng_buf_push(const char *str, stRNG_BUFR_ *stRNG_BUFR);
void rng_buf_init(stRNG_BUFR_ *stRNG_BUFR);
#endif