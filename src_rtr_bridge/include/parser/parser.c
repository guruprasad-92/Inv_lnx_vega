
#include <parser/parser.h>
#include <cmd/cmd.h>
#include <stdio.h>
#include <string.h>

#include "basic/basic.h"

void print(char *str)
{
    printf("print(%s)\n",str);
}

/*
uint8_t str2cmd(char *str,CMD_ *CMD)
{
    int len = strlen(str);
    uint8_t ret=0;
    if (len>CMD_SZ_MAX)
    {
        ret = 0;
    }
    else
    {
        ret = cmd_verify_tok(str);
        //Success case
        if(ret == 1)
        {
            ret = cmd_split(str,CMD);
            if(ret == 1)
            {
                ret = cmd_parse(CMD);
                if (ret == 0) // Error case
                {
                    cmd_clean(CMD);
                }
            }
        }
        else //Error
        {
            ret = 0;
        }
    }
    return ret;
}
*/

/************************
 * Success : 1
 * Error : 0
 * Verify the index of '$','+','='
 * *********************/
uint8_t cmd_verify_tok(const char *str)
{
    uint8_t ret=0;
    char tok[3] = {"$+="};
    char *tmp1, *tmp2 = NULL;
    long int p1=0,p2=0;
    long int idx[3] = {0,4,8};
    p1 = (long int)str;
    int sz = 0;
    uint8_t cnt = 0;

    if (strlen(str) < CMD_STR_MN)
    {
        printf("ERR : size(CMD) = %d. minimum size = %d\n",\
                strlen(str),CMD_STR_MN);
        ret = 0;
    }
    else
    {
        for(int i=0;i<3;i++)
        {
            tmp1 = strchr(str,tok[i]);
            tmp2 = strrchr(str,tok[i]);
            
            if(tmp1!=NULL)
            {
                if(tmp1 == tmp2) 
                {
                    p2 = (long int)tmp1;
                    //printf("p1:0x%lx\np2:0x%lx\n",p1,p2);
                    // printf("p2-p1 = %ld\n",p2-p1);
                    // printf("idx[i] = %ld\n",idx[i]);
                    if( (p2-p1) == idx[i])
                    {
                        cnt += 1;
                    }
                    else
                    {
                        printf("ERR : idx error for token(%c)\n",tok[i]);
                    }
                }
                else
                {
                    printf("ERR : string has multiple token - %c\n",tok[i]);
                }
                
            }
            else
            {
                printf("ERR : string does not have the token token - %c\n",tok[i]);
            }            
        }
    }    
    if(cnt == 3)
    {
        tmp1 = strchr(str,'=');
        if(strlen(tmp1))
        {
            sz = strlen(tmp1);
            printf("Arguments = %s\n",tmp1);
            //printf("Argument size = %d\n",sz);
            if( (sz > 1 ) && (sz < CMD_SZ_MAX) )
            {
                ret = 1;
            }
            else 
            {
                printf("ERR-SZ : argument size mismatch.\n");
                ret = 0;                
            }
        }
    }
    else 
        ret = 0;
    return ret;
}


uint8_t cmd_split(char *str, CMD_ *CMD)
{
    char *p1=NULL, *p2=NULL, *tmp=NULL;
    uint8_t i = 0;
    p1 = strtok(str,"=");
    p2 = strtok(NULL,"=");
    uint8_t brk=0, ef_brk=0;
    uint8_t ret = 0;

    i=0;
    CMD->arg_1[0] = strtok(p1,"+");    
    do
    {
        tmp = strtok(NULL,"+");
        if(tmp)
        {
            CMD->arg_1[++i] = tmp;
        }
    } while (tmp);
    CMD->cnt[0] = i+1;

    if(p2 == NULL)
    {
        printf("ERR : no arguments.\n");
        return 0;
    }

    if(strchr(p2,',')==NULL)
    {
        CMD->arg_2[0] = p2;
        CMD->cnt[1] = 1;
    }
    // else if(strchr(p2,',') == strrchr(p2,','))
    // {
    //     CMD->arg_2[0] = p2;
    //     char *p = strchr(p2,',');
    //     *p = 0;
    //     CMD->cnt[1] = 1;
    // }
    else
    {
        printf("CMD->cnt[0]=%d\n",CMD->cnt[0]);
        printf("CMD->cnt[1]=%d\n",CMD->cnt[1]);
        
        CMD->arg_2[0] = strtok(p2,",");    
        i=1;    
        do
        {
            tmp = strtok(NULL,",");
            if(tmp)
            {
                printf("i=%d\n",i);
                if(i < ARG2_MAX )
                {
                    printf("ii=%d\n",i);
                    CMD->arg_2[i++] = tmp;

                    if( (i==2) && (!strcmp(CMD->arg_1[1],"SMS")) \
                        && (CMD->arg_2[0][0]=='W') )
                    {
                        while(*tmp!=0)
                        {
                            tmp+=1;
                            printf("tmp[i]=%c\n",*tmp);
                        }
                        if(*(++tmp)!=0)
                        {
                            // Messsage exists
                            ef_brk = 1;
                            printf("ii2=%d\n",i);
                            CMD->arg_2[i] = tmp;
                            i += 1;                            
                        }
                        else
                        {
                            //Message does not exist.
                            brk=1;
                        }
                    }                   
                }
                else
                {
                    printf("WARNING : Excluding extra arguments\n");
                    printf("tmp[%d]=%s\n",i,tmp);
                    // tmp[i] = 0;
                    printf("Breaking ... .. .\n");
                    brk = 1;
                }
            }
        } while ( (tmp != NULL) && (brk==0) && (ef_brk==0) );
        CMD->cnt[1] = i;
        dbg_print(Cyan,"\nDBG-SMS : CMD-cnt[1] = %d\n",CMD->cnt[1]);
        
    }  
    if(brk && (CMD->cnt[2] < ARG2_MIN) )
    {
        ret = 0;
    }
    else
    {
        ret = 1;
    }    
    return ret;
}


/**************************
 * Expects a string.
 * processed by cmd_split()
 * ***********************/
uint8_t cmd_parse(char *str, CMD_ALL_ *CMD_ALL)
{
    uint8_t ret = 0;

    ret = cmd_verify_tok(str);
    if(ret == 1)
    {
        cmd_clean(CMD_ALL->CMD);
        ret = cmd_split(str,CMD_ALL->CMD);
        dbg_print(NULL,"cmd_split()=%d\n",ret);
        if(ret ==1)
        {
            if(strcmp(CMD_ALL->CMD->arg_1[1],C_SMS)==0)
            {
                if(cmd_verify_sms(CMD_ALL->CMD,\
                    CMD_ALL->CMD_SMS))
                {
                    // printf("\n------------- chk2 -------------\n");
                    cmd_print(CMD_ALL->CMD);
                    printf("\n----------------- xxxxxxxxxx ------------\n");
                    dbg_print(Bold_Green,"DBG-SMS : VAlid SMS.\n");
                    printf("Printing the cmd sms : \n");
                    cmd_sms_print(CMD_ALL->CMD_SMS);
                    cmd_sms_clean(CMD_ALL->CMD_SMS);
                    // dbg_print(Bold_Yellow,"addr_cmd : %p\n",CMD_ALL->CMD);
                    // dbg_print(Bold_Yellow,"addr_cmd_arg_1 : %p\n",CMD_ALL->CMD->arg_1);
                    // dbg_print(Bold_Yellow,"addr_cmd_arg_2 : %p\n",CMD_ALL->CMD->arg_2);
                    // dbg_print(Bold_Yellow,"addr_cmd_SMS : %p\n",CMD_ALL->CMD_SMS);
                    ret = EN_SMS;
                    // printf("\n------------- chk2.2 -------------\n");
                    cmd_print(CMD_ALL->CMD);
                    printf("\n----------------- xxxxxxxxxx ------------\n");
                    
                }
                else
                {
                    dbg_print(Bold_Red,\
                    "ERR-CMD : Invalid CMD format(SMS).\n\r");
                    ret = 0;
                }
            }
            else if(strcmp(CMD_ALL->CMD->arg_1[1],C_SIM)==0)
            {
                if(cmd_verify_sim(CMD_ALL->CMD,CMD_ALL->CMD_SIM))
                {
                    printf("DBG-SIM : VAlid SIM.\n");
                    printf("Printing the cmd sim : \n");
                    cmd_sim_print(CMD_ALL->CMD_SIM);
                    //cmd_sim_clean(CMD_ALL->CMD_SIM);
                    ret = EN_SIM;
                }
                else
                {
                    dbg_print(Bold_Red,\
                    "ERR-CMD : Invalid CMD format(SIM).\n\r");
                    ret = 0;
                }
            }
            else if(strcmp(CMD_ALL->CMD->arg_1[1],C_RST) == 0)
            {
                if(cmd_verify_rst(CMD_ALL->CMD,CMD_ALL->CMD_RST))
                {
                    printf("DBG-RST : VAlid RST.\n");
                    printf("Printing the cmd RST : \n");
                    cmd_rst_print(CMD_ALL->CMD_RST);
                    ret = EN_RST;
                }
                else
                {
                    dbg_print(Bold_Red,\
                    "ERR-CMD : Invalid CMD format(RST).\n\r");
                    ret = 0;
                }
            }
            else if(strcmp(CMD_ALL->CMD->arg_1[1],C_STS) == 0)
            {   
                if(cmd_verify_sts(CMD_ALL->CMD,CMD_ALL->CMD_STS))
                {
                    printf("DBG-RST : Command STS has validated.\n");
                    printf("Printing the cmd STS : \n");
                    cmd_sts_print(CMD_ALL->CMD_STS);
                    cmd_sts_clean(CMD_ALL->CMD_STS);
                    ret = EN_STS;
                }
                else
                {
                    dbg_print(Bold_Red,\
                    "ERR-CMD : Invalid CMD format(STS).\n\r");
                    ret = 0;
                }
            }
            else
            {
                dbg_print(Bold_Red,
                    "\n\rERR-CMD : Invalid command(%s)\n\r",
                        CMD_ALL->CMD->arg_1[1]);
                ret = 0;
            }
        } // cmd_split()
        else
        {
            dbg_print(Bold_Red,"\n\rERR-CMD : cmd_split() failed.\n\r");
            ret = 0;
        }
    }//cmd_verify_tok()
    else
    {
        dbg_print(Bold_Red,\
        "\n\rERR-CMD : cmd_ferify_tok() failed.\n\r");
        ret = 0;
    }  
    
    return ret;
}

void cmd_clean_all(CMD_ALL_ *CMD_ALL)
{
    memset(CMD_ALL->CMD,0,sizeof(*CMD_ALL->CMD));
    memset(CMD_ALL->CMD_RST,0,sizeof(*CMD_ALL->CMD_RST));
    memset(CMD_ALL->CMD_STS,0,sizeof(*CMD_ALL->CMD_STS));
    memset(CMD_ALL->CMD_SIM,0,sizeof(*CMD_ALL->CMD_SIM));
    memset(CMD_ALL->CMD_SMS,0,sizeof(*CMD_ALL->CMD_SMS));
}
