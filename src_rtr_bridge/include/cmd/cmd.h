#ifndef CMD_H
#define CMD_H

#include <stdint.h>

#define RTR "$RTR"

#define CMD_STR_MN  10
#define CMD_STR_MX  100
#define CMD_MX  4

#define C_SMS   "SMS"
#define C_SMS_AMAX  3
#define C_SMS_AMIN  2

#define C_SIM   "SIM"
#define C_SIM_ARG  1 // Fixed argument

#define C_RST   "RST"
#define C_RST_ARG  1 // Fixed argument

#define C_STS   "STS"
#define C_STS_ARG   1 //Fixed Argument

#define CMD_CNT     4
#define CMD_SZ_MAX  200
#define ARG1_MAX    2
#define ARG2_MAX    4
#define ARG2_MIN    1

typedef enum 
{
    EN_SMS = 1,
    EN_SIM,
    EN_STS,
    EN_RST
}EN_CMD_PRS;

typedef struct _CMD_
{
    char *arg_1[ARG1_MAX];
    char *arg_2[ARG2_MAX];
    uint8_t cnt[2];
}CMD_;

typedef struct CMD_SMS
{
    char op;
    int idx;
    char num[12];
    char msg[128];
    uint8_t cln_sts;
}CMD_SMS_;


typedef struct CMD_FRMT__
{
    char type[3];
    uint8_t arg2_max;
    uint8_t arg2_min;
}CMD_FRMT_;

typedef struct _CMD_2_
{
    CMD_FRMT_ CMD_FRMT[CMD_MX];
    uint32_t cmd_cnt;
    
}CMD_2_;

static CMD_2_ GS__CMD_2;

//void cmd_print(char *str);
//void cmd_clean(char *);

void cmd_clean(CMD_ *CMD);
void cmd_print(CMD_ *CMD);

#endif