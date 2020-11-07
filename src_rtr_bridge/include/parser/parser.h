#ifndef PRSR_H
#define PRSR_H

#include <stdint.h>
#include <cmd/cmd.h>
#include "cmd_rst/cmd_rst.h"
#include "cmd_sim/cmd_sim.h"
#include "cmd_sms/cmd_sms.h"
#include "cmd_sts/cmd_sts.h"



typedef struct CMD_ALL__
{
    CMD_     *CMD;
    CMD_SMS_ *CMD_SMS;
    CMD_RST_ *CMD_RST;
    CMD_SIM_ *CMD_SIM;
    CMD_STS_ *CMD_STS;
}CMD_ALL_;


void print(char *str);
//uint8_t str2cmd(char *str,CMD_ *CMD);
uint8_t cmd2str(CMD_ *CMD, char *str);

uint8_t cmd_verify_tok(const char *str);
uint8_t cmd_split(char *str, CMD_ *CMd);

void cmd_print(CMD_ *CMd);
void cmd_clean(CMD_ *CMD);
uint8_t cmd_parse(char *str, CMD_ALL_ *CMD_ALL);

void cmd_clean_all(CMD_ALL_ *CMD_ALL);


#endif