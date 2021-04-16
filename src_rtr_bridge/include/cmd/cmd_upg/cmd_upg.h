#ifndef CMD_UPG__H
#define CMD_UPG__H

#include <stddef.h>
#include "cmd.h"
#include "mqtt.h"



#define UPG_ARG_MX  12
#define UPG_ARG_MN  1
#define UPG_ARG_CNFG 6
#define UPG_ARG_PRD 10

#define UPG_TO_MX  120*60    // 15min max
#define UPG_TO_MN  3*60     // 3min 

typedef struct CMD_UPG
{
    char op;
    int idx;
    uint8_t iSts;//enable(1)/disable(0)
    uint8_t iCred; //credentials required(1/0)
    char cUsrnm[30];
    char cPswrd[30];
    char cSrvrAddr[300];
    // char cCnctTO[6];//connection time out
    // char cOprtnTO[6];//operation time out
    // char iCnTo[6]; // Connection time out
    uint32_t iCnTo;
    // char iOpTo[6]; // Operation time out
    uint32_t iOpTo;
    char cMin[2];
    char cHr[2];
    char cDom[2]; //day of month
    char cDow[2]; //day of week
    uint8_t cln_sts;
}CMD_UPG_;


void cmd_upg_print(CMD_UPG_ *CMD_UPG);
uint8_t cmd_verify_upg(CMD_ *CMD, CMD_UPG_ *CMD_UPG);
int cmd_upg_periodic(CMD_ *CMD, CMD_UPG_ *CMD_UPG);
int cmd_upg_confg(CMD_ *CMD, CMD_UPG_ *CMD_UPG);
void cmd_upg_clean(CMD_UPG_ *CMD_UPG);
int cmd_op_UPG(CMD_UPG_ *CMD_UPG, stMSQ_DS_ *stMSQ_DS, char *rsp, size_t sz_rsp);

#endif