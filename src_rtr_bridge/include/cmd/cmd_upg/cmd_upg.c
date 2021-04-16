#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>

#include <cmd_upg/cmd_upg.h>
#include <basic/basic.h>

#include "mqtt.h"

/*************************************************
 * String has to be verified before callinng 
 * this function.
 * **********************************************/
uint8_t cmd_verify_upg(CMD_ *CMD, CMD_UPG_ *CMD_UPG)
{
    uint8_t ret = 0;
    uint8_t cnt = 0;
    char arg[4] = {"IPCS"};
    
    if(CMD->cnt[1] < UPG_ARG_MN)
    {
        printf("ERR-UPG : Minimum argument %d\n",UPG_ARG_MN);
        return 0;
    }
    if(CMD->cnt[1] > UPG_ARG_MX)
    {
        printf("ERR-UPG : Maximum argument %d\n",UPG_ARG_MX);
        return 0;
    }
    if ( strlen(CMD->arg_2[0]) == 1)
    {
        for(uint8_t i=0;i<4;i++)
        {
            if(CMD->arg_2[0][0] == arg[i])
            {
                cnt += 1;
            }
        }
        if(cnt==1)
        {
            CMD_UPG->op = CMD->arg_2[0][0];
            ret += 1;
        }
        else
        {
            ret = 0;
            return ret;
        }

        if(CMD_UPG->op == 'I')
        {
            if(CMD->cnt[1] != UPG_ARG_MN)
            {
                printf("ERR-UPG : For 'I' max arg is 1\n");
                ret = 0;
                return ret;
            }
            ret += 1;
        }
        else if(CMD_UPG->op == 'P')
        {
            ret = cmd_upg_periodic(CMD,CMD_UPG);
        }
        else if(CMD_UPG->op == 'C')
        {
           ret = cmd_upg_confg(CMD,CMD_UPG);
        }
        else if(CMD_UPG->op == 'S')
        {
             if(CMD->cnt[1] != UPG_ARG_MN)
            {
                printf("ERR-UPG : For 'S' max arg is 1\n");
                ret = 0;
                return ret;
            }
            ret += 1;           
        }
    }
    if(ret)
    {
        CMD_UPG->cln_sts = 0;
    }
    return ret;
}

void cmd_upg_print(CMD_UPG_ *CMD_UPG)
{
    if(CMD_UPG->cln_sts == 0)
    {
        if(CMD_UPG->op == 'P')
        {
            printf("UPG_STATUS = %d\n",CMD_UPG->iSts);
            printf("UPG_CREDENTIAL = %d\n",CMD_UPG->iCred);
            printf("UPG_USERNAME = %s\n",CMD_UPG->cUsrnm);
            printf("UPG_PASSWORD = %s\n",CMD_UPG->cPswrd);
            printf("UPG_SRVR_ADDR = %s\n",CMD_UPG->cSrvrAddr);
            printf("UPG_CONNECTION_TIME_OUT = %d\n",CMD_UPG->iCnTo);
            printf("UPG_OPERATION_TIME_OUT = %d\n",CMD_UPG->iOpTo);
            printf("UPG_MINUTE = %s\n",CMD_UPG->cMin);
            printf("UPG_HOUR = %s\n",CMD_UPG->cHr);
            printf("UPG_DAY_OF_MONTH = %s\n",CMD_UPG->cDom);
            printf("UPG_DAY_OF_WEEK = %s\n",CMD_UPG->cDow);
        }
        else if(CMD_UPG->op == 'C')
        {
            printf("UPG_USERNAME = %s\n",CMD_UPG->cUsrnm);
            printf("UPG_PASSWORD = %s\n",CMD_UPG->cPswrd);
            printf("UPG_SRVR_ADDR = %s\n",CMD_UPG->cSrvrAddr);
            printf("UPG_CONNECTION_TIME_OUT = %d\n",CMD_UPG->iCnTo);
            printf("UPG_OPERATION_TIME_OUT = %d\n",CMD_UPG->iOpTo);
        }
    }
}

void cmd_upg_clean(CMD_UPG_ *CMD_UPG)
{
    CMD_UPG->cln_sts = 1;

    CMD_UPG->iSts = 0;
    CMD_UPG->iCred = 0;
    memset(CMD_UPG->cUsrnm,0,sizeof(CMD_UPG->cUsrnm));
    memset(CMD_UPG->cPswrd,0,sizeof(CMD_UPG->cPswrd));
    memset(CMD_UPG->cSrvrAddr,0,sizeof(CMD_UPG->cSrvrAddr));
    CMD_UPG->iCnTo = 0;
    CMD_UPG->iOpTo = 0;
    memset(CMD_UPG->cMin,0,sizeof(CMD_UPG->cMin));
    memset(CMD_UPG->cHr,0,sizeof(CMD_UPG->cHr));
    memset(CMD_UPG->cDom,0,sizeof(CMD_UPG->cDom));
    memset(CMD_UPG->cDow,0,sizeof(CMD_UPG->cDow));
    CMD_UPG->op = 0;
}

/**********************
 * Success  : 0
 * Error    : -1
 * *******************/
int cmd_op_UPG(CMD_UPG_ *CMD_UPG, stMSQ_DS_ *stMSQ_DS, char *rsp, size_t sz_rsp)
{
    int ret = 0;
    int tm_out = 5;
    char msq_cmd[TPQ_UPGSRV_CFG_MSG_MX] = {0};
    memset(rsp,0,strlen(rsp));
    printf("DBG-UPG-OP : Performing Upgrade operation\n");
    if(CMD_UPG->op == 'I')
    {
        printf("DBG-UPG-OP : Instantaneous upgrade.\n");
        
        stMSQ_DS->cpTpq_sub = TPQ_UPGINST_RESP;
        stMSQ_DS->cpTpq_pub = TPQ_UPGINST_RQST;
        stMSQ_DS->cpMsg = TPQ_UPGINST_MSG;
        stMSQ_DS->iCur_tpqNo = TPQ_UPGINST_NO;
        ret = mqtt_sub_pub(stMSQ_DS,10);
        printf("ret = %d\n",ret);
        dbg_print(Bold_Yellow,"payload : %s\n", stMSQ_DS->spMSQ_Uobj->caMsg);
        if(stMSQ_DS->spMSQ_Uobj->caMsg != NULL)
        {
            if(sz_rsp <= SZ_MQTT_MSG)
            {
                strcat(rsp,stMSQ_DS->spMSQ_Uobj->caMsg);
            }
            ret = 0;
        }
        else
        {
            ret = -1;
        }
        
    }
    else if(CMD_UPG->op == 'P')
    {
        printf("DBG-UPG-OP : Periodic upgrade.\n");
        return -1;
    }
    else if(CMD_UPG->op == 'C')
    {
        printf("DBG-UPG-OP : Configuration upgrade.\n");
        stMSQ_DS->iCur_tpqNo = TPQ_UPGSRV_CFG_NO;
        stMSQ_DS->cpTpq_sub = TPQ_UPGSRV_CFG_RESP;
        stMSQ_DS->cpTpq_pub = TPQ_UPGSRV_CFG_RQST;
        sprintf(msq_cmd,"<,%s,%s,%s,%s,%d,%d,>", \
                TPQ_UPGSRV_CFG_M1,CMD_UPG->cUsrnm,CMD_UPG->cPswrd,\
                CMD_UPG->cSrvrAddr,CMD_UPG->iCnTo,CMD_UPG->iOpTo);
        stMSQ_DS->cpMsg = msq_cmd;
        // dbg_print(Bo)
        ret = mqtt_sub_pub(stMSQ_DS,10);
        printf("ret = %d\n",ret);
        dbg_print(Bold_Yellow,"payload : %s\n", stMSQ_DS->spMSQ_Uobj->caMsg);
        if(stMSQ_DS->spMSQ_Uobj->caMsg != NULL)
        {
            if(sz_rsp <= SZ_MQTT_MSG)
            {
                strcat(rsp,stMSQ_DS->spMSQ_Uobj->caMsg);
            }
            ret = 0;
        }
    }
    else if(CMD_UPG->op == 'S')
    {
        printf("DBG-UPG-OP : Upgrade status.\n");
        ret = -1;
    }
    return ret;
}

int cmd_upg_periodic(CMD_ *CMD, CMD_UPG_ *CMD_UPG)
{
    uint8_t ret = 0;
    int tmp = 0, chk = 0;
    if(is_numeric(CMD->arg_2[1]) <= 0)
    {
        printf("ERR-UPG : Status must be numeric\n\r");
        ret = 0;
    }
    else
    {
        tmp = atoi(CMD->arg_2[1]);
        if( (tmp==0) || (tmp==1) )
            CMD_UPG->iSts = (uint8_t)tmp;
        else
        {
            printf("ERR-UPG : Argument status must be 0(disable) / 1(enable)\n\r");
            ret = 0;
            return ret;
        }        
    }
    tmp = 0;
    if(is_numeric(CMD->arg_2[2]) <= 0)
    {
        printf("ERR-UPG : Credential must be numeric\n\r");
        ret = 0;
        return ret;
    }
    else
    {
        tmp = atoi(CMD->arg_2[2]);
        if( (tmp==0) || (tmp==1) )
            CMD_UPG->iCred = (uint8_t)tmp;
        else
        {
            //error
            printf("ERR-UPG : Argument credential must be 0/1\n\r");
            ret = 0;
            return ret;
        }        
    }
    if(CMD_UPG->iCred == 0)
    {
        if(CMD->cnt[1] == UPG_ARG_PRD)
        {
            strcpy(CMD_UPG->cSrvrAddr,CMD->arg_2[3]);
            chk = is_numeric(CMD->arg_2[4]);
            if(chk > 0)
            {
                chk = atoi(CMD->arg_2[4]);
                if( (chk >= UPG_TO_MN) && (chk <= UPG_TO_MX) )
                {
                    CMD_UPG->iCnTo = chk;
                }
                else
                {   
                    dbg_print(Bold_Red,\
                        "ERR-UPG : parameter-5(Cn_TmOut) exceeds boundary<%d-%d> values",\
                            UPG_TO_MN,UPG_TO_MX);
                    return 0;
                }
            }
            else
            {
                printf("ERR-UPG: Connection Time out should be numeric");
                ret = 0;
                return ret;
            }
            chk = is_numeric(CMD->arg_2[5]);
            if(chk > 0)
            {
                chk = atoi(CMD->arg_2[5]);
                if( (chk >= UPG_TO_MN) && (chk <= UPG_TO_MX) )
                {
                    CMD_UPG->iOpTo = chk;
                }
                else
                {   
                    dbg_print(Bold_Red,\
                        "ERR-UPG : parameter-5(Op_TmOut) exceeds boundary<%d-%d> values",\
                            UPG_TO_MN,UPG_TO_MX);
                    return 0;
                }
            }
            else
            {
                printf("ERR-UPG: Operation Time Out should be numeric");
                ret = 0;
                return ret;
            }
            chk = is_numeric(CMD->arg_2[6]);
            if(chk > 0)
            {
                strcpy(CMD_UPG->cMin,CMD->arg_2[6]);
            }
            else
            {
                printf("ERR-UPG: Minute should be numeric");
                ret = 0;
                return ret;
            }
            chk = is_numeric(CMD->arg_2[7]);
            if(chk > 0)
            {
                strcpy(CMD_UPG->cHr,CMD->arg_2[7]);
            }
            else
            {
                printf("ERR-UPG: Hour should be numeric");
                ret = 0;
                return ret;
            }
            chk = is_numeric(CMD->arg_2[8]);
            if(chk > 0)
            {
                strcpy(CMD_UPG->cDom,CMD->arg_2[8]);
            }
            else
            {
                printf("ERR-UPG: Day of Month should be numeric");
                ret = 0;
                return ret;
            }
            chk = is_numeric(CMD->arg_2[9]);
            if(chk > 0)
            {
                strcpy(CMD_UPG->cDow,CMD->arg_2[9]);
            }
            else
            {
                printf("ERR-UPG: Day of Week should be numeric");
                ret = 0;
                return ret;
            }
            ret +=1;
        }
        else
        {
            printf("ERR-UPG:  Minimum argument is 10\n\r");
            ret = 0;
            return ret;
        }
        
    }
    else if(CMD_UPG->iCred == 1)
    {

        if(CMD->cnt[1] == UPG_ARG_MX)
        {
            strcpy(CMD_UPG->cUsrnm,CMD->arg_2[3]);
            strcpy(CMD_UPG->cPswrd,CMD->arg_2[4]);
            strcpy(CMD_UPG->cSrvrAddr,CMD->arg_2[5]);
            chk = is_numeric(CMD->arg_2[6]);
            if(chk > 0)
            {
                chk = atoi(CMD->arg_2[6]);
                if( (chk >= UPG_TO_MN) && (chk <= UPG_TO_MX) )
                {
                    CMD_UPG->iCnTo = chk;
                }
                else
                {   
                    dbg_print(Bold_Red,\
                        "ERR-UPG : parameter-5(Cn_TmOut) exceeds boundary<%d-%d> values",\
                            UPG_TO_MN,UPG_TO_MX);
                    return 0;
                }
            }
            else
            {
                printf("ERR-UPG: Connect Time Out should be numeric");
                ret = 0;
                return ret;
            }
            
            chk = is_numeric(CMD->arg_2[7]);
            if(chk > 0)
            {
                chk = atoi(CMD->arg_2[7]);
                if( (chk >= UPG_TO_MN) && (chk <= UPG_TO_MX) )
                {
                    CMD_UPG->iOpTo = chk;
                }
                else
                {   
                    dbg_print(Bold_Red,\
                        "ERR-UPG : parameter-5(Op_TmOut) exceeds boundary<%d-%d> values",\
                            UPG_TO_MN,UPG_TO_MX);
                    return 0;
                }
            }
            else
            {
                printf("ERR-UPG: Operation Time Out should be numeric");
                ret = 0;
                return ret;
            }
            chk = is_numeric(CMD->arg_2[8]);
            if(chk > 0)
            {
                strcpy(CMD_UPG->cMin,CMD->arg_2[8]);
            }
            else
            {
                printf("ERR-UPG: Minute should be numeric");
                ret = 0;
                return ret;
            }
            chk = is_numeric(CMD->arg_2[9]);
            if(chk > 0)
            {
                strcpy(CMD_UPG->cHr,CMD->arg_2[9]);
            }
            else
            {
                printf("ERR-UPG: Hour should be numeric");
                ret = 0;
                return ret;
            }
            chk = is_numeric(CMD->arg_2[10]);
            if(chk > 0)
            {
                strcpy(CMD_UPG->cDom,CMD->arg_2[10]);
            }
            else
            {
                printf("ERR-UPG: Day of month should be numeric");
                ret = 0;
                return ret;
            }
            chk = is_numeric(CMD->arg_2[11]);
            if(chk > 0)
            {
                strcpy(CMD_UPG->cDow,CMD->arg_2[11]);
            }
            else
            {
                printf("ERR-UPG: Day of week should be numeric");
                ret = 0;
                return ret;
            }

            ret += 1;
        }
        else
        {
            printf("ERR-UPG:  Minimum argument is 12\n\r");
            ret = 0;
            return ret;
        }

    }
    else
    {
        printf("ERR-UPG : arg-2(credential req) is invalid.\n");
        ret = 0;
        return ret;
    }
    return ret;
}

int cmd_upg_confg(CMD_ *CMD, CMD_UPG_ *CMD_UPG)
{
    uint8_t ret = 0;
    int chk =0;
    if(CMD->cnt[1] != UPG_ARG_CNFG)
    {
        printf("ERR-UPG : For 'C' max arg is 6\n");
        ret = 0;
        return ret;
    }
    strcpy(CMD_UPG->cUsrnm,CMD->arg_2[1]);
    strcpy(CMD_UPG->cPswrd,CMD->arg_2[2]);
    strcpy(CMD_UPG->cSrvrAddr,CMD->arg_2[3]);
    chk = is_numeric(CMD->arg_2[4]);
    if(chk > 0)
    {
        // strcpy(CMD_UPG->iCnTo,CMD->arg_2[4]);
        chk = atoi(CMD->arg_2[4]);
        if( (chk >= UPG_TO_MN) && (chk <= UPG_TO_MX) )
        {
            CMD_UPG->iCnTo = chk;
        }
        else
        {
            dbg_print(Bold_Red,\
                "ERR-UPG : parameter-5(Con_TmOut) exceeds boundary<%d-%d> values",\
                    UPG_TO_MN,UPG_TO_MX);
            return 0;
        }
    }
    else
    {
        dbg_print(Bold_Red,"ERR-UPG: Connection Time out should be numeric");
        ret = 0;
        return ret;
    }
    chk = is_numeric(CMD->arg_2[5]);
    if(chk > 0)
    {
        // strcpy(CMD_UPG->iCnTo,CMD->arg_2[5]);
        chk = atoi(CMD->arg_2[5]);
        if( (chk >= UPG_TO_MN) && (chk <= UPG_TO_MX) )
        {
            CMD_UPG->iOpTo = chk;
        }
        else
        {
            dbg_print(Bold_Red,\
                "ERR-UPG : parameter-5(Op_TmOut) exceeds boundary<%d-%d> values",\
                    UPG_TO_MN,UPG_TO_MX);
            return 0;
        }
    }
    else
    {
        printf("ERR-UPG: Operation Time Out should be numeric");
        ret = 0;
        return ret;
    }
    ret +=1;
    return ret;
}
        
