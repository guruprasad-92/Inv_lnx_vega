#ifndef CMD_STS__H
#define CMD_STS__H

#include <cmd.h>

typedef struct _CMD_STS_
{
    uint8_t cln_sts;
    char sts;
}CMD_STS_;

// static CMD_STS_ CMD_STS;


uint8_t cmd_verify_sts(CMD_ *CMD, CMD_STS_ *CMD_STS);
void cmd_sts_print(const CMD_STS_ *CMD_STS);
void cmd_sts_clean(CMD_STS_ *CMD_STS);
int cmd_op_sts(CMD_STS_ *CMD_STS);


#endif