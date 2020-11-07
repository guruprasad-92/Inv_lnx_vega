
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmd_sms/cmd_sms.h>
#include <basic/basic.h>

/*************************************************
 * String has to be verified before callinng 
 * this function.
 * **********************************************/
uint8_t cmd_verify_sms(CMD_ *CMD, CMD_SMS_ *CMD_SMS)
{
    uint8_t ret = 0;
    uint8_t cnt = 0;
    char arg[3] = {"RDW"};
    int is_num = 0;
    uint8_t arg_cnt = 0;

    if(CMD->cnt[1] < SMS_ARG_MN )
    {
        printf("ERR-SMS : Minimum argument 2\n");
        return 0;
    }
    if(CMD->cnt[1] > SMS_ARG_MX)
    {
        printf("ERR-SMS : Maximum argument 3\n");
        return 0;
    }

    if ( strlen(CMD->arg_2[0]) == 1)
    {
        for(uint8_t i=0;i<3;i++)
        {
            if(CMD->arg_2[0][0] == arg[i])
            {
                cnt += 1;
            }
        }
        if(cnt==1)
        {
            CMD_SMS->op = CMD->arg_2[0][0];
            ret += 1;
        }
        else
        {
            ret = 0;
            return ret;
        }
        
        is_num = is_numeric(CMD->arg_2[1]);
        printf("DBG-SMS : arg(2) = %d\n\r",is_num);
        if(CMD_SMS->op == 'W')
        {
            if(CMD->cnt[1] != SMS_ARG_MX)
            {
                printf("ERR-SMS : For 'W' max arg is 3\n");
                ret = 0;
                return ret;
            }
            printf("DBG-SMS : size(ph-no) = %d\n",is_num);
            if( (is_num != NUM_10) && is_num != NUM_12 )
            {
                printf("ERR-SMS : size(ph-no) = %d\n",is_num);
                printf("ERR-SMS : arg-2(ph-no) is invalid.\n");
                ret = 0;
                return ret;
            }
            else
            {
                strcpy(CMD_SMS->num,CMD->arg_2[1]);
                ret += 1;
            }
            if(CMD->cnt[1]==3)
            {
                dbg_print(Bold_Green,"\nDBG-SMS-W : err-check\n");
                dbg_print(Green,"MSG : CMD-arg_2[2] = %s\n",CMD->arg_2[2]);
                strcpy(CMD_SMS->msg,CMD->arg_2[2]);
                cnt+=1;
                dbg_print(Bold_Green,"\nDBG-SMS-W : err-check\n");
            }
            else
            {
                printf("ERR-SMS : arg(3) is missing(message)\n");
                ret = 0;
            }
            
        }
        else // for 'R' and 'D'
        {
            if(CMD->cnt[1] != SMS_ARG_MN)
            {
                printf("ERR-SMS : For R/D max arg is 2\n");
                ret = 0;
                return ret;
            }
            if(is_num > SMS_IDX_MAX)
            {
                printf("ERR-SMS : arg-2(idx) is invalid.\n");
                ret = 0;
            }
            else if(is_num < -1 )
            {
                printf("ERR-SMS : arg-2(idx(-1)) is invalid.\n");
                ret = 0;
            }
            else if(is_num == 0)
            {
                printf("ERR-SMS : arg-2 is not numeric\n\r");
                ret = 0;
            }
            else
            {
                int num = atoi(CMD->arg_2[1]);
                if ( (num == -1) || \
                    ( (num >= 0) && (num <= SMS_IDX_MAX)) )
                {
                    ret += 1; 
                    CMD_SMS->idx = num;
                }
                else
                {
                    printf("ERR-SMS : invalid idx.\n");
                    ret = 0;
                }
            }
        }
    }
    if(ret)
    {
        CMD_SMS->cln_sts = 0;
    }
    return ret;
}

void cmd_sms_print(CMD_SMS_ *CMD_SMS)
{
    printf("DBG-SMS : clean status = %d\n",CMD_SMS->cln_sts);
    if(CMD_SMS->cln_sts == 0)
    {
        printf("SMS_op = %c\n",CMD_SMS->op);
        if(CMD_SMS->op == 'W')
        {
            printf("SMS_num = %s\n",CMD_SMS->num);
            printf("SMS-msg = %s\n",CMD_SMS->msg);
        }
        else
        {
            printf("SMS-idx = %d\n",CMD_SMS->idx);
        }
    }
}

void cmd_sms_clean(CMD_SMS_ *CMD_SMS)
{
    CMD_SMS->cln_sts = 1;
    CMD_SMS->idx = -9;
    memset(CMD_SMS->num,0,NUM_12);
    memset(CMD_SMS->msg,0,128);
    CMD_SMS->op = 0;
}

int cmd_op_sms(CMD_SMS_ *CMD_SMS)
{
    int ret = 0;
    printf("DBG-SMS : Performing SMS operation\n");
    if(CMD_SMS->op == 'W')
    {
        printf("DBG-SMS-OP : Performing SMS write.\n");
        printf("DBG-SMS-OP : Not implimented yet\n");
    }
    else if(CMD_SMS->op == 'R')
    {
        printf("DBG-SMS-OP : Performing SMS read.\n");
        printf("DBG-SMS-OP : Not implimented yet\n");
    }
    else if(CMD_SMS->op == 'D')
    {
        printf("DBG-SMS-OP : Performing SMS delete.\n");
        printf("DBG-SMS-OP : Not implimented yet\n");
    }
    return ret;
}

