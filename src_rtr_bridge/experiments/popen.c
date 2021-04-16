#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
    mosquito related commands for sim switching and
    active sim slot tracking
    {
        mosquitto_pub -d -t "Isensev2/SimSwitch/CommandRequest" -m "<,1,SELSIM2,\"2020-09-21 11:00:00\",>"
        mosquitto_sub -d -t "Isensev2/SimSwitch/CommandResponse"
    }
    {
        Tracking the data usage
        AT+CUSD=1,"<USSD>",15  ---    USSD is Number provided by cellular operator
    }
*/

#define SZ_RD   1024

int str2numstr(char *str, char *numstr[], int len)
{
    int sz = strlen(str);
    int c = 0, j =0;
    char *st, *end;
    for(int i=0;i<sz;i++)
    {
        if( (str[i] >= '0') && (str[i] <= '9') )
        {
            if( j==0 )
            {
                if( (c+1) < len )
                {
                    numstr[c++] = &str[i];
                    printf("start num = %c idx = %d\n",str[i],i);
                }
            }
            if(i==0)
            {
                j = -1;
            }
            else
            {
                j = i;
            }
        }
        else if( (i-j) == 1)
        {
            j = 0;
            str[i] = 0;
        }
    }
    return c;
}

int str2int(char *str)
{
    size_t sz = strlen(str);
    char num[10] = {0};
    int cnt = 0;
    int j = 0;
    for(int i=0;i<sz;i++)
    {
        if( (str[i] >= '0') && (str[i] <= '9') )
        {
            if(j==0)
            {
                j = i;
                num[cnt++] = str[i];
            }
            else if( (i-j) == 1)
            {

            }
            
        }
    }
}

int split_line(char *src, char *dst[], int ln_max)
{
    int sz = strlen(src);
    char *tmp=NULL;
    int cnt = 0;
    tmp = strtok(src,"\n");
    for(int i=0;i<sz;i++)
    {        
        if(tmp != NULL)
        {
            if(cnt++ < ln_max)
            {
                dst[cnt-1] = tmp;
            }
            else
            {
                break;
            }
            tmp = strtok(NULL,"\n");
        }
        else
        {
            printf("\n\rNo more <NL>\n\r");
            break;
        }        
    }
    return cnt;
}

/**************************
 * read the string
 * Arguments : 
 *      1. src_atr : "\n\rOK\n\r"
 * Replace \n with "<NL>" and "\r" with "<CR>"
 * ***********************/
void print_mdm(char *src_str)
{
    int len = strlen(src_str);
    for(int i=0;i<len;i++)
    {
        if(src_str[i] == '\n')
        {
            printf("<NL>");
        }
        else if(src_str[i] == '\r')
        {
            printf("<CR>");
        }
        else
        {
            printf("%c",src_str[i]);
        }        
    }
    printf("\n\r");
}


unsigned int cm_rsp(const char *cmd, char *rsp, char *rsp_ln[], int ln_max)
{
    FILE *fp = NULL;
    fp = popen(cmd,"r");
    size_t ret;
    int ln_cnt = 0;
    if(fp == NULL)
    {
        perror("\n\rpopen() failed due to :");
        printf("Exiting ... .. .\n\r");
        exit(0);
    }
    else
    {
        ret = fread_unlocked((void*)rsp,1000,1000,fp);
        printf("fread_unlocked() = %d\n",ret);
        ln_cnt = split_line(rsp,rsp_ln,ln_max);
        printf("split_line() = %d\n",ln_cnt);
        //printf("--------\n%s\n%s\n------\n",rsp_ln[0],rsp_ln[1]);
    }
    pclose(fp);
    return ln_cnt;    
}





// rd_flg : {1 - read using AT; 0 - read using cm; 2 - read using legato}
void read_core_tmp(unsigned int rd_flg, int *tmp)
{
    switch(rd_flg)
    {
        case 0:
        {        
            char rsp[100];
            char *rsp_ln[4];
            int ln_cnt = cm_rsp("cm temp",rsp,rsp_ln,4);
            char *numstr[5] = {0};
            int ret = 0;
            printf("cm_rsp() = %d\n",ln_cnt);
            if(ln_cnt > 0)
            {
                for(int i=0;i<2;i++)
                {
                    if(rsp_ln[i] != NULL)
                    {
                        ret = str2numstr(rsp_ln[i],numstr,5);
                        printf("str2numstr() = %d\n",ret);
                        if(ret > 0)
                        {
                            tmp[i] = atoi(numstr[ret-1]);
                        }
                    }
                }
            }        
        }
    }
}


int mdm_selSim(int sim)
{
    FILE *fp_rsp = NULL;
    FILE *fp_snd = NULL;
    int ret = 0;
    char mos_cmd[200]={0};
    char buf[1024] = {0};
    int Dtm=15; // Delay time for 13Sec.
    int sts = 0;
    if( (sim < 1) && (sim > 2) )
    {
        printf("\nInvalid sim-%d\n",sim);
        ret = -1;
    }
    else
    {
        sprintf(mos_cmd,"mosquitto_pub -d -t \"Isensev2/SimSwitch/CommandRequest\" -m \"<,1,SELSIM%d,\\\"2020-09-21 11:00:00\\\",>\"",sim);
        fp_snd = popen(mos_cmd,"r");
        if(fp_snd == NULL)
        {
            perror("\nmosquitto_pub() failed due to : ");
            ret = -1;
        }
        else
        {
            ret = fread_unlocked((void*)buf,1000,1000,fp_snd);
            printf("mosquito_pub(%d) : \n%s\n",sim,buf);
            printf("\nSelecting SIM%d. Waiting for %dSec.\n",sim,Dtm);
            sleep(Dtm);
            memset(buf,0,1000);
            pclose(fp_snd);
            ret = sim;
        }
    }
}

int main(void)
{
    // char CMD[100]={0};
    // char rd_cmd[SZ_RD]={0};
    // char *lns[30] = {0};
    // size_t ret = 0;
    // int tmp[2]={0};
    // //read_core_tmp(0,tmp);
    // // printf("power amplifier temp : %d\n",tmp[0]);
    // // printf("power core temp : %d\n",tmp[1]);
    // char rsp[500]={0};
    // char *ln_rsp[50]={0};
    // unsigned int ln = cm_rsp("cm info",rsp,ln_rsp,48);
    printf("enter sim : ");
    int a;
    scanf("%d",&a);
    //printf("\\--\n");
    //printf("mosquitto_pub -d -t \"Isensev2/SimSwitch/CommandRequest\" -m \"<,1,SELSIM2,\\\"2020-09-21 11:00:00\\\",>\"");
    printf("sim_switch() = %d\n",mdm_selSim(a));
}