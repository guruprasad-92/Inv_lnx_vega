#ifndef BASIC_H
#define BASIC_H
#include <stdint.h>

#include "color.h"
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <semaphore.h>

#define DBG_PRINT
// #define APP_VER "1.0.0.0"
#define KLG_NM  "RTR-BRZ"
#define DBG_VAR_SZ  2048

typedef struct sDBG_LVL__
{
    uint8_t iDbg_lvl;
    char caDbg_lvl[DBG_VAR_SZ];
    sem_t sm_lock;
}sDBG_LVL_;

extern sDBG_LVL_ sDBG_LVL;

typedef struct MEM_INFO__ 

{
    uint32_t f_memtotal;
    uint32_t f_memfree;
    uint32_t f_memavailable;
    uint32_t f_buffers;
    uint32_t f_cached;

    float r_memused;
    float r_cached;
}MEM_INFO_;


typedef struct UPTM_INFO__
{
    char tm_up[25];
    char tm_dif[20];    
    uint32_t day;
    uint32_t idiff_hr;
    uint32_t idiff_mn;
}UPTM_INFO_;


typedef struct ETH_INFO__
{
    char IP[25];
    char NM[25];
    char ecm[25];
}ETH_INFO_;

/***************************
 * verify wheather the
 *  string is numeric or not
 *  RETURN : 
 *      +(size(str)) : numeric (+ve)
 *      -(size(str)) : numeric (-ve)
 *      0 : Not numeric
 * ************************/
int is_numeric(char *str);

/*********************
 * Print formated string[]
 * with color
 * ******************/
void dbg_print(const char *clr, \
                const char *format, ...);

void msleep(const unsigned int mili);
void cln_NL_CR(char *str);
void rmv_nlcr(char *str);
// int cnt_chr_instr(char *str, const char tok);
int str2numstr(char *str, char *numstr[], int len, uint32_t flt);
void get_time(char *str);
void sys_reboot(void);

int split_line(char *src, char *dst[], int ln_max);
int get_eth_info(ETH_INFO_ *ETH_INFO);
int get_sys_uptime(char *up_tm);
int read_meminfo(MEM_INFO_ *memdata);
int get_sys_uptm(UPTM_INFO_ *UPTM);
void get_formated_time(char *str, uint8_t frmt);
int get_ping_sts(void);

void wait_upto_uptm(uint32_t tm_max);
int wr_klog(char *str);


#endif