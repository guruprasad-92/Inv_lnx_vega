#ifndef CMD_SMS__H
#define CMD_SMS__H

#include <cmd.h>

#define SMS_ARG_MX  3
#define SMS_ARG_MN  2

#define NUM_12 12
#define NUM_10 10
#define SMS_SZ  512
#define SMS_IDX_MAX 6


uint8_t cmd_verify_sms(CMD_ *CMD, CMD_SMS_ *CMD_SMS);
void cmd_sms_print(CMD_SMS_ *CMD_SMS);
void cmd_sms_clean(CMD_SMS_ *CMD_SMS);
int cmd_op_sms(CMD_SMS_ *CMD_SMS);

#endif