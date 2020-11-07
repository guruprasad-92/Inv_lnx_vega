#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <semaphore.h>
#include <parser_mdm.h>

#include "parser_mdm.h"
#include "basic.h"

int main(void)
{
    //char buffer[] = "MDM_SN : 352913090128354\nMDM_MDL : WP7608\nMFN : Sierra Wireless, Incorporated\nFMWV : SWI9X07Y_02.37.03.00 73df45 jenkins 2020/04/08 10:59:14\nCSQ :  28,99\nPower amp tmp : 43.00\nPower core tmp : 43.00";
    char buffer[]= "MDM_MDL : WP7608\nMFN : Sierra_Wireless__Incorporated\nIMSI_1 : 404450959353533\nMDM_SN : 352913090131929\nCCID_1 : 89914509009593535332\nSPN_1 : airtel\nSYS_UPTIME : uptime=2020-10-29 10:00:27;diff_time=9:10;diff_day=0;\nVER_MDM_FMW : SWI9X07Y_02.37.03.00 73df45 jenkins 2020/04/08 10:59:14\nVER_RTR_FMW : SWI9X07Y_02.37.07.00__3.18.140\nVER_APP_RLS : 1.00\nVER_APP_INV : 1.0.0.1\nCSQ : 17,99\nPower amp tmp : 46.00\nPower core tmp : 46.00\nNET_STS(1) : 1\nPNG_STS(1) : 1\nIP_ecm0 : 192.168.2.2\nIP_eth0 : 10.1.1.105\nIP_eth1 : 192.168.3.2\nIP_eth2 : 10.1.1.107\nMEM_INFO : Total=158484; used=54.213673; cached=21.581989\nRTC_TM : 29092020191107\nACTV_SIM_SLT : 1\nSIM_RAT_1 : LTE\nACT_SLT_STS : 1\nRST_INTERVAL : 243\nBTP_SLT_STS : 2";
    //char buffer[]="hgcccccccccccccc";
    int temp1 = 0, ret = 0;
    char delimiter = '\n';
    char *dest[35]={0};
    int i = 0;
    int rt_prs = 0, cnt=0;
    sVEGA_PRD_ obj;
    temp1 = split_string(buffer,dest,&delimiter);
    Parse_func_ parse_func[26];
    ret = prs_mdm(parse_func);
    
    
    for(i=0;i<temp1;i++)//bootup
    {
        rt_prs = parse_func[i].f_prs_mdm(dest,temp1,&obj);
        if(rt_prs == 0)
        {
            cnt++;
        }
    }
    if(cnt == 12)
    {
        //send acknowledgment;
    }

    for(i=12;i<ret;i++)//periodic
    {
        rt_prs = parse_func[i].f_prs_mdm(dest,temp1,&obj);
    }
    return 0;
}


