#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "ring_buffer.h"

void rng_buf_init(stRNG_BUFR_ *stRNG_BUFR)
{
    stRNG_BUFR->cur = stRNG_BUFR->cbuffer + 0;
    stRNG_BUFR->st = stRNG_BUFR->cbuffer + 0;
    stRNG_BUFR->end = stRNG_BUFR->cbuffer + SZ_BFR;
}

int rng_buf_push(const char *str, stRNG_BUFR_ *stRNG_BUFR)
{
    size_t sz_fre = \
        stRNG_BUFR->end - stRNG_BUFR->cur;
    size_t sz_fil = stRNG_BUFR->cur - stRNG_BUFR->st;
    size_t sz_str = strlen(str);
    char *new_st = NULL;
    size_t sz_new = 0;

    if( sz_str > SZ_BFR)
    {
        return -1;
    }
    new_st = stRNG_BUFR->st + sz_str;
    sz_new = stRNG_BUFR->end - new_st;

    // printf("sz_free = %ld\n",sz_fre);
    // printf("sz_fil = %ld\n",sz_fil);
    // printf("sz_new = %ld\n",sz_new);
    
    if(sz_fre < sz_str )
    {
        memcpy(stRNG_BUFR->st,new_st,sz_new);
        stRNG_BUFR->cur = stRNG_BUFR->st + sz_new - sz_fre;
        // printf("cur = %ld\n",stRNG_BUFR->cur - stRNG_BUFR->st);
        memcpy(stRNG_BUFR->cur,str,sz_str);
        stRNG_BUFR->cur += sz_str;
        // printf("cur = %ld\n",stRNG_BUFR->cur - stRNG_BUFR->st);
    }
    else
    {
        strcat(stRNG_BUFR->cbuffer,str);
        stRNG_BUFR->cur += sz_str;
    }
    return sz_str;
}