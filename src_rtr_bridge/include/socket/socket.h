#ifndef SOCKET__H
#define SOCKET__H

#include <parser/parser.h>
#include "mqtt.h"

#define P1_40   40
#define P2_41   41

#define TM_OUT

#define SZ_TBF  1024*2
#define SZ_BF   400

typedef struct TH_ARG__
{
    int port;
    char th_info[5];
    char *th_buf;
    int th_buf_sz;
    sem_t *th_sem;
    sem_t *th_semMqtt;
    CMD_ALL_ *CMD_ALL;  
    stMSQ_DS_ *stMSQ_DS;
}TH_ARG_;

typedef struct TH_ARG_P2__
{
    int port;
    char th_info[5];
    char *th_buf;
    char *pr_sts;
    sem_t *th_sem;
    sem_t *th_semMqtt;
    stMSQ_DS_ *stMSQ_DS;
}TH_ARG_P2_;



void sk_flush(int fd_sk);

int set_sock_opt(int fd_srvr);
int start_srvr( int *fd_srvr, int *fd_clnt, \
                struct sockaddr_in *addr, \
                unsigned long len_addr, int port);
void *th_port1(void *arg);
void *th_port2(void *arg);

#endif