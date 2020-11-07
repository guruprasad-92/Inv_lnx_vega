#ifndef CMD_RST__H
#define CMD_RST__H

#include <cmd.h>

#define RST_ARG_MX  1
#define RST_MX  2
#define RST_MN  1


typedef struct CMD_RST
{
    uint8_t cln_sts;
    uint8_t rst;
}CMD_RST_;

uint8_t cmd_verify_rst(CMD_ *CMD, CMD_RST_ *CMD_RST);
void cmd_rst_print(const CMD_RST_ *CMD_RST);
void cmd_rst_clean(CMD_RST_ *CMD_RST);
int cmd_op_rst(CMD_RST_ *CMD_RST);


#endif