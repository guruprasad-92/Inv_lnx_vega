#include <stdio.h>
#include <termios.h>
#include "basic/basic.h"
#include "operation/mdm_operation.h"
#include "mqtt.h"

static stMSQ_calbak_ stMSQ_calbak = {0};
static stMSQ_Uobj_ stMSQ_Uobj = {0};
static stMSQ_DS_ stMSQ_DS = {0};
msq_ins_ *spMsq_instans;

int main(void)
{
    int ret,sim = 0;

    //------- msq init ------
    struct termios Tos;
    int fd = mdm_init(&Tos);
    if(fd < 0)
    {
        dbg_print(Bold_Red,"ERR : mdm_init() failed()\n");
        dbg_print(Bold_Red,"Returning... .. .\n");
        return -1;
    }
    stMSQ_calbak.mqtt_cbk_pub = mqtt_calBak_pub;
    stMSQ_calbak.mqtt_cbk_sub = mqtt_calBak_sub;
    stMSQ_calbak.mqtt_cbk_msg = mqtt_calBak_msg;

    stMSQ_DS.spMSQ_Uobj = &stMSQ_Uobj;
    ret = mqtt_init(&spMsq_instans,&stMSQ_Uobj,&stMSQ_calbak);
    if(ret < 0)
    {
        printf("mqtt_init() failed\n");
        printf("Returning... .. .\n");
        return -1;
    }
    stMSQ_DS.stpMsq_instns = spMsq_instans;

    int type = 0, tm_tkn=0;
    dbg_print(Bold_Cyan,"Choose the type of sim selection.\n1. With data\n2. Without data\n");
    scanf("%d",&type);
    dbg_print(Bold_Green,"Enter the sim-slot to select : ");
    scanf("%d",&sim);

    switch(type)
    {
        case 1 :
        {
            dbg_print(Bold_Cyan,"Sim selection type : With data (Max time = 4min)\n");
            dbg_print(Bold_Cyan,"Selecting sim : %d\n",sim);
            ret = mdm_selSim(sim,&tm_tkn);
            dbg_print(Bold_Yellow,"Time taken to switch = %d\n",tm_tkn);
        }break;
        case 2 :
        {
            dbg_print(Bold_Cyan,"Sim selection type : With data (Max time = 30Sec)\n");
            dbg_print(Bold_Cyan,"Selecting sim : %d\n",sim);
            ret = mdm_selSim_ndt(&stMSQ_DS,sim);
        }break;
        default :
        {
            dbg_print(Bold_Red,"Invalid sim selection type. %d\nExiting... .. .\n",type);
            return -1;
        }
    }
    
    do
    {
        ret = mdm_get_Slt_Sts(fd);
    } while ( ret );    
    
    printf("sim = %d\n",sim);
    printf("Slot status : %d\n",ret);

}