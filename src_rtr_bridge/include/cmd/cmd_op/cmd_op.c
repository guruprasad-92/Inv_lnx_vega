#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#include "cmd/cmd.h"
#include "basic/basic.h"
#include "cmd_op/cmd_op.h"
#include "parser/parser.h" // for CMD_ALL_

uint8_t cmd_operation(CMD_ALL_ *CMD_ALL, uint8_t op)
{
    uint8_t ret = 0;
    switch(op)
    {
        case EN_SMS :
        {
            ret = 0;
        }break;
        case EN_SIM :
        {
            ret = 0;
        }break;
        case EN_STS :
        {
            ret = 0;
        }break;
        case EN_RST :
        {
            
        }break;
        default :
        {

        }
    }
}