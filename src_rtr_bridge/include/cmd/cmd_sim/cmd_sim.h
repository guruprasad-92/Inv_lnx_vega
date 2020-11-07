/****************************
 * 
 *  Author  : Guruprasad
 *  Date    :
 * 
 * *************************/

#ifndef CMD_SIM__H
#define CMD_SIM__H

#include <cmd.h>

#define SIM_ARG_MX  1
#define SIM_SLT_MX  2


typedef struct CMD_SIM
{
    uint8_t cln_sts;
    uint8_t sim;
}CMD_SIM_;

uint8_t cmd_verify_sim(CMD_ *CMD, CMD_SIM_ *CMD_SIM);
void cmd_sim_print(const CMD_SIM_ *CMD_SIM);
void cmd_sim_clean(CMD_SIM_ *CMD_SIM);
int cmd_op_sim(CMD_SIM_ *CMD_SIM);

#endif
