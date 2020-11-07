#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <semaphore.h>

#include "basic.h"
#include "parser_mdm.h"




/**/

int prs_mdm_sn_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0, i = 0;
    //memset(obj->cMdm_sn,0,sizeof(obj->cMdm_sn));
    if((typ == 0) || (typ == PRS_TYP_BTP))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"MDM_SN");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                strcpy(obj->cMdm_sn,p2);
                //dbg_print(Bold_Cyan,"MDM_SN = %s\n",obj->cMdm_sn);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }  
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }    
    return ret;
}

int prs_mdm_model_data(char *dest[], int sz, sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0;
    int i=0;
    if((typ == 0) || (typ == PRS_TYP_BTP))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"MDM_MDL");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                //memset(obj->cMdm_mdl,0,sizeof(obj->cMdm_mdl));
                strcpy(obj->cMdm_mdl,p2);
                //printf("MDM_MDL = %s\n",obj->cMdm_mdl);
                //dbg_print(Bold_Cyan,"MDM_MDL = %s\n",obj->cMdm_mdl);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    return ret;
}

int prs_mdm_mfn_data(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0;
    int i=0;
    if(typ == 0 || typ == PRS_TYP_BTP)
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"MFN");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                //memset(obj->cMdm_mfn,0,sizeof(obj->cMdm_mfn));
                strcpy(obj->cMdm_mfn,p2);
                //printf("MFN = %s\n",obj->cMdm_mfn);
                //dbg_print(Bold_Cyan,"MFN = %s\n",obj->cMdm_mfn);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }       
    }
    else
    {
        ret = 1;
    }
     
    return ret;
}

int prs_sim_rat(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char rat_data[20]={0};
    int ret = 0, r=0;
    if((typ==0)||(typ==PRS_TYP_PRD))
    {
        r = read_SIM_RAT2(dest,rat_data,sz,1);
        if(r<0)
        {
            ret = -1;
            memset(rat_data,0,sizeof(rat_data));
        }
        memset(rat_data,0,sizeof(rat_data));
        strcpy(obj->sSim_info[0].cRAT,rat_data);
        //dbg_print(Bold_Cyan,"RAT[0] = %s\n",obj->sSim_info[0].cRAT);
        memset(rat_data,0,sizeof(rat_data));
        ret = read_SIM_RAT2(dest,rat_data,sz,2);
        if(r<0)
        {
            ret = -1;
            memset(rat_data,0,sizeof(rat_data));
        }
        strcpy(obj->sSim_info[1].cRAT,rat_data);
        //dbg_print(Bold_Cyan,"RAT[1] = %s\n",obj->sSim_info[1].cRAT);
    }
    else
    {
        ret = 1;
    }
    
    return ret;
}

int prs_mdmFMW_ver(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0;
    int i=0;
    if((typ == 0) || (typ == PRS_TYP_BTP))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"VER_MDM_FMW");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                //memset(obj->sVersion.cVER_MDM_FMW,0,sizeof(obj->sVersion.cVER_MDM_FMW));
                strcpy(obj->sVersion.cVER_MDM_FMW,p2);
                //printf("VER_MDM_FMW = %s\n",obj->sVersion.cVER_MDM_FMW);
                //dbg_print(Bold_Cyan,"VER_MDM_FMW = %s\n",obj->sVersion.cVER_MDM_FMW);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    return ret;
}

int prs_rtrFMW_ver(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0, i = 0;
    if((typ == 0) || (typ == PRS_TYP_BTP))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"VER_RTR_FMW");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                //memset(obj->sVersion.cVER_RTR_FMW,0,sizeof(obj->sVersion.cVER_RTR_FMW));
                strcpy(obj->sVersion.cVER_RTR_FMW,p2);
                //printf("VER_RTR_FMW = %s\n",obj->sVersion.cVER_RTR_FMW);
                //dbg_print(Bold_Cyan,"VER_RTR_FMW = %s\n",obj->sVersion.cVER_RTR_FMW);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    
    return ret;
}

int prs_appRLS_ver(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0, i = 0;
    if((typ == 0) || (typ == PRS_TYP_BTP))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"VER_APP_RLS");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                //memset(obj->sVersion.cVER_APP_RLS,0,sizeof(obj->sVersion.cVER_APP_RLS));
                strcpy(obj->sVersion.cVER_APP_RLS,p2);
                //printf("VER_APP_RLS = %s\n",obj->sVersion.cVER_APP_RLS);
                //dbg_print(Bold_Cyan,"VER_APP_RLS = %s\n",obj->sVersion.cVER_APP_RLS);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
        return ret;
    }
}

int prs_appINV_ver(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0, i = 0;
    if((typ == 0) || (typ == PRS_TYP_BTP))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"VER_APP_INV");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
               // memset(obj->sVersion.cVER_APP_INV,0,sizeof(obj->sVersion.cVER_APP_INV));
                strcpy(obj->sVersion.cVER_APP_INV,p2);
                //printf("VER_APP_INV = %s\n",obj->sVersion.cVER_APP_INV);
                //dbg_print(Bold_Cyan,"VER_APP_INV = %s\n",obj->sVersion.cVER_APP_INV);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    return ret;
}

int prs_mdm_IP_ecm0_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0 , i = 0;

    if( (typ==0) || (typ==PRS_TYP_PRD))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"IP_ecm0");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 == NULL)
        {
            memset(obj->ip.cIP_ecm0,0,sizeof(obj->ip.cIP_ecm0));
            ret = -1;
            //printf("IP_eth0 = %s\n",obj->ip.cIP_ecm0);
            //dbg_print(Bold_Cyan,"IP_ecm0 = %s\n",obj->ip.cIP_ecm0);
        }
        if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                memset(obj->ip.cIP_ecm0,0,sizeof(obj->ip.cIP_ecm0));
                strcpy(obj->ip.cIP_ecm0,p2);
                //printf("IP_ecm0 = %s\n",obj->ip.cIP_ecm0);
                //dbg_print(Bold_Cyan,"IP_ecm0 = %s\n",obj->ip.cIP_ecm0);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    
    return ret;
}

int prs_mdm_IP_eth0_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0, i =0;
    if((typ == 0) || (typ == PRS_TYP_PRD))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"IP_eth0");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 == NULL)
        {
            memset(obj->ip.cIP_eth0,0,sizeof(obj->ip.cIP_eth0));
            ret = -1;
        }
        else if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                memset(obj->ip.cIP_eth0,0,sizeof(obj->ip.cIP_eth0));
                strcpy(obj->ip.cIP_eth0,p2);
                //printf("IP_eth0 = %s\n",obj->ip.cIP_eth0);
                //dbg_print(Bold_Cyan,"IP_eth0 = %s\n",obj->ip.cIP_eth0);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    
    return ret;
}

int prs_mdm_IP_eth1_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0, i=0;
    if((typ == 0) || (typ == PRS_TYP_PRD))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"IP_eth1");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 == NULL)
        {
            memset(obj->ip.cIP_eth1,0,sizeof(obj->ip.cIP_eth1));
            ret = -1;
        }
        else if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                memset(obj->ip.cIP_eth1,0,sizeof(obj->ip.cIP_eth1));
                strcpy(obj->ip.cIP_eth1,p2);
                //printf("IP_eth1 = %s\n",obj->ip.cIP_eth1);
                //dbg_print(Bold_Cyan,"IP_eth1 = %s\n",obj->ip.cIP_eth1);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    
    return ret;
}

int prs_mdm_IP_eth2_data(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0, i=0;
    if((typ == 0) || (typ == PRS_TYP_PRD))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"IP_eth2");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 == NULL)
        {
            memset(obj->ip.cIP_eth2,0,sizeof(obj->ip.cIP_eth2));
            ret = -1;
        }
        else if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                memset(obj->ip.cIP_eth2,0,sizeof(obj->ip.cIP_eth2));
                strcpy(obj->ip.cIP_eth2,p2);
                //printf("IP_eth2 = %s\n",obj->ip.cIP_eth2);
                //dbg_print(Bold_Cyan,"IP_eth2 = %s\n",obj->ip.cIP_eth2);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    
    return ret;
}

int prs_mdm_sys_uptime(char *dest[],int sz, sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    char *buff[20]={0};
    char *buff1[20]={0};
    int ret = 0, len =0, temp = 0,temp1 = 0, i=0;
    if( (typ==0) || (typ==PRS_TYP_PRD) )
    {

        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"SYS_UPTIME");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 == NULL)
        {
            memset(obj->sSys_uptime.cUpdate,0,sizeof(obj->sSys_uptime.cUpdate));
            //printf("update = %s\n",obj->sSys_uptime.cUpdate);
            //dbg_print(Bold_Cyan,"update = %s\n",obj->sSys_uptime.cUpdate);
            memset(obj->sSys_uptime.cUptime,0,sizeof(obj->sSys_uptime.cUptime));
            //printf("uptime = %s\n",obj->sSys_uptime.cUptime);
            //dbg_print(Bold_Cyan,"uptime = %s\n",obj->sSys_uptime.cUptime);
            memset(obj->sSys_uptime.cDiff_time,0,sizeof(obj->sSys_uptime.cDiff_time));
            //printf("diff_time = %s\n",obj->sSys_uptime.cDiff_time);
            //dbg_print(Bold_Cyan,"diff_time = %s\n",obj->sSys_uptime.cDiff_time);
            obj->sSys_uptime.iDiff_day = -1;
            //printf("diff_day = %d\n",obj->sSys_uptime.iDiff_day);
            //dbg_print(Bold_Cyan,"diff_day = %d\n",obj->sSys_uptime.iDiff_day);
            ret = -1;
        }
        else if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                temp = split_string(p2,buff,";");
                p1 = strchr(buff[1],'=');
                p1+=1;
                p2 = strchr(buff[2],'=');
                temp1 = split_string(buff[0],buff1," ");
                buff1[0]= strchr(buff1[0],'=');
                buff1[0]+=1;
                memset(obj->sSys_uptime.cUpdate,0,sizeof(obj->sSys_uptime.cUpdate));
                strcpy(obj->sSys_uptime.cUpdate,buff1[0]);
                //printf("update = %s\n",obj->sSys_uptime.cUpdate);
                //dbg_print(Bold_Cyan,"update = %s\n",obj->sSys_uptime.cUpdate);
                memset(obj->sSys_uptime.cUptime,0,sizeof(obj->sSys_uptime.cUptime));
                strcpy(obj->sSys_uptime.cUptime,buff1[1]);
                //printf("uptime = %s\n",obj->sSys_uptime.cUptime);
                //dbg_print(Bold_Cyan,"uptime = %s\n",obj->sSys_uptime.cUptime);
                memset(obj->sSys_uptime.cDiff_time,0,sizeof(obj->sSys_uptime.cDiff_time));
                strcpy(obj->sSys_uptime.cDiff_time,p1);
                //printf("diff_time = %s\n",obj->sSys_uptime.cDiff_time);
                //dbg_print(Bold_Cyan,"diff_time = %s\n",obj->sSys_uptime.cDiff_time);
                obj->sSys_uptime.iDiff_day = atoi(p2);
                //printf("diff_day = %d\n",obj->sSys_uptime.iDiff_day);
                //dbg_print(Bold_Cyan,"diff_day = %d\n",obj->sSys_uptime.iDiff_day);
                ret = 0; 
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        } 
    }
    else
    {
        ret = 1;
    }
    
    return ret;
}

/*int prs_mdm_memInfo_data(char *dest[], int sz, sVEGA_PRD_ *obj)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0, len = 0, temp = 0;
    char *buff[20]={0};
    for(int i=0;i<sz;i++)
    {
        p1 = strstr(dest[i],"MEM_INFO");
        if(p1 != NULL)
        {
            break;
        }
    }
    if(p1 == NULL)
    {
        memset(obj->sMem_info.cTotal,0,sizeof(obj->sMem_info.cTotal));
        printf("Total = %s\n",obj->sMem_info.cTotal);
        memset(obj->sMem_info.cUsed,0,sizeof(obj->sMem_info.cUsed));
        printf("Used = %s\n",obj->sMem_info.cUsed);
        memset(obj->sMem_info.cCached,0,sizeof(obj->sMem_info.cCached));
        printf("Cached = %s\n",obj->sMem_info.cCached);
    }
    else if(p1 != NULL)
    {
        p2 = strchr(p1,':');
        if(p2 != NULL)
        {
            temp = split_string(p2,buff,";");
            buff[0]= strchr(buff[0],'=');
            buff[0]+=1;
            strcpy(obj->sMem_info.cTotal,buff[0]);
            printf("Total = %s\n",obj->sMem_info.cTotal);
            buff[1]= strchr(buff[1],'=');
            buff[1]+=1;
            strcpy(obj->sMem_info.cUsed,buff[1]);
            printf("Used = %s\n",obj->sMem_info.cUsed);
            buff[2]= strchr(buff[2],'=');
            buff[2]+=1;
            strcpy(obj->sMem_info.cCached,buff[2]);
            printf("Cached = %s\n",obj->sMem_info.cCached);
            ret = 0;
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = -1;
    }
    return ret;
} */

int prs_mdm_csq_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    char *numstr[10]={0};
    int ret = 0, len =0, i=0, j=0;
    if((typ==0) || (typ==PRS_TYP_PRD))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"CSQ");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 == NULL)
        {
            obj->cCsq[0] = -1;
            obj->cCsq[1] = -1;
            //dbg_print(Bold_Cyan,"csq[0]= %d\n",obj->cCsq[0]);
            //dbg_print(Bold_Cyan,"csq[1]= %d\n",obj->cCsq[1]);
            ret = -1;
        }
        else if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                len = str2numstr(p2,numstr,10,1);
                for(j=0;j<len;j++)
                {
                    obj->cCsq[j]=(atoi(numstr[j]));
                    //printf("csq[%d]= %d\n",j,obj->cCsq[j]);
                //dbg_print(Bold_Cyan,"csq[%d]= %d\n",j,obj->cCsq[j]);
                    ret = 0;
                }
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    
    return ret;
}

int prs_mdm_memInfo_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    char *numstr[10]={0};
    int ret = 0, len =0,i=0;
    if((typ == 0) || (typ == PRS_TYP_PRD))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"MEM_INFO");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 == NULL)
        {
            obj->sMem_info.iTotal = -1;
            obj->sMem_info.fUsed = -1;
            obj->sMem_info.fCached = -1;
            ret = -1;
        }
        else if(p1 != NULL)
        {
                len = str2numstr(p1,numstr,10,1);
                //printf("len = %d\n",len);
                obj->sMem_info.iTotal=(atoi(numstr[0]));
                //printf("Total= %d\n",obj->sMem_info.iTotal);
                //dbg_print(Bold_Cyan,"Total= %d\n",obj->sMem_info.iTotal);
                obj->sMem_info.fUsed=(atof(numstr[1]));
                //printf("Used= %f\n",obj->sMem_info.fUsed);
                //dbg_print(Bold_Cyan,"Used= %f\n",obj->sMem_info.fUsed);
                obj->sMem_info.fCached=(atof(numstr[2]));
                //printf("Cached= %f\n",obj->sMem_info.fCached);
                //dbg_print(Bold_Cyan,"Cached= %f\n",obj->sMem_info.fCached);
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    
    return ret;
}


int prs_mdm_temp_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0, len =0, i=0;
    char *numstr[10]={0};
    if((typ==0) || (typ==PRS_TYP_PRD))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"amp tmp");
            if(p1 != NULL)
            {
                break;
            }
        }
        for(i=0;i<sz;i++)
        {
            p2 = strstr(dest[i],"core tmp");
            if(p2 != NULL)
            {
                break;
            }
        }
        if(p1 == NULL)
        {
            obj->fTemp[0] = -1;
            //printf("temp[0]= %f\n",obj->fTemp[0]);
            //dbg_print(Bold_Cyan,"temp[0]= %.2f\n",obj->fTemp[0]);
            ret = 1;
        }
        else if(p1 != NULL)
        {
            len = str2numstr(p1,numstr,10,1);
            if(len > 0)
            {
                obj->fTemp[0] = atof(numstr[0]);
                //printf("temp[0]= %.2f\n",obj->fTemp[0]);
                //dbg_print(Bold_Cyan,"temp[0]= %.2f\n",obj->fTemp[0]);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
        if(p2 == NULL)
        {
            obj->fTemp[1] = -1;
            //printf("temp[1]= %f\n",obj->fTemp[1]);
            //dbg_print(Bold_Cyan,"temp[1]= %.2f\n",obj->fTemp[1]);
        }
        else if(p2 != NULL)
        {
            // printf("p2 = %s\n",p2);
            len = str2numstr(p2,numstr,10,1);
            if(len > 0)
            {
                obj->fTemp[1] = atof(numstr[0]);
                //printf("temp[1]= %.2f\n",obj->fTemp[1]);
                //dbg_print(Bold_Cyan,"temp[1]= %.2f\n",obj->fTemp[1]);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        } 
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }    
    return ret;
}

int read_SIM_IMSI2(char *dest[], char *imsi_data,int sz,int sim)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0, i=0;
    char  imsi[7] = {0};
    if(sim == 1  || sim == 2)
        sprintf(imsi,"IMSI_%d",sim);
    else
    {
        return -1;
    }
    for(i=0;i<sz;i++)
    {
        p1 = strstr(dest[i],imsi);
        if(p1 != NULL)
        {
            break;
        }
    }
    if(p1 == NULL)
    {
        memset(imsi_data,0,sizeof(imsi_data));
        ret = -1;
    }
    else if(p1 != NULL)
    {
        p2 = strchr(p1,':');
        p2+=2;
        if(p2 != NULL)
        {
            memset(imsi_data,0,sizeof(imsi_data));
            strcpy(imsi_data,p2);
            ret = 0;
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = -1;
    }
    return ret;
}

int read_SIM_CCID2(char *dest[], char *ccid_data,int sz,int sim)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0, i=0;
    char  ccid[7];
    if(sim == 1  || sim == PRS_TYP_PRD)
        sprintf(ccid,"CCID_%d",sim);
    else
    {
        return -1;
    }
    for(i=0;i<sz;i++)
    {
        p1 = strstr(dest[i],ccid);
        if(p1 != NULL)
        {
            break;
        }
    }
    if(p1 == NULL)
    {
        memset(ccid_data,0,sizeof(ccid_data));
        ret = -1;
    }
    else if(p1 != NULL)
    {
        p2 = strchr(p1,':');
        p2+=2;
        if(p2 != NULL)
        {
            memset(ccid_data,0,sizeof(ccid_data));
            strcpy(ccid_data,p2);
            ret = 0;
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = -1;
    }
    return ret;
}

int read_SIM_SPN2(char *dest[], char *spn_data,int sz,int sim)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0;
    int i=0;
    char  spn[7];
    if(sim == 1  || sim == PRS_TYP_PRD)
        sprintf(spn,"SPN_%d",sim);
    else
    {
        return -1;
    }
    for(i=0;i<sz;i++)
    {
        p1 = strstr(dest[i],spn);
        if(p1 != NULL)
        {
            break;
        }
    }
    if(p1 == NULL)
    {
        memset(spn_data,0,sizeof(spn_data));
        ret = -1;
    }
    else if(p1 != NULL)
    {
        p2 = strchr(p1,':');
        p2+=2;
        if(p2 != NULL)
        {
            memset(spn_data,0,sizeof(spn_data));
            strcpy(spn_data,p2);
            ret = 0;
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = -1;
    }
    return ret;
}

int read_SIM_RAT2(char *dest[], char *rat_data,int sz,int sim)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0;
    int i=0;
    char  rat[15] = {0};
    if(sim == 1  || sim == PRS_TYP_PRD)
        sprintf(rat,"SIM_RAT_%d",sim);
    else
    {
        return -1;
    }
    for(i=0;i<sz;i++)
    {
        p1 = strstr(dest[i],rat);
        if(p1 != NULL)
        {
            break;
        }
    }
    if(p1 == NULL)
    {
        memset(rat_data,0,sizeof(rat_data));
    }
    else if(p1 != NULL)
    {
        p2 = strchr(p1,':');
        p2+=2;
        if(p2 != NULL)
        {
            memset(rat_data,0,sizeof(rat_data));
            strcpy(rat_data,p2);
            ret = 0;
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = -1;
    }
    return ret;
}

int read_data_usage(char *dest[],uint32_t *data,int sz)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0;
    int i=0;
    for(i=0;i<sz;i++)
    {
        p1 = strstr(dest[i],"DATA_USAGE");
        if(p1 != NULL)
        {
            break;
        }
    }
    if(p1 == NULL)
    {
        *data = -1;
    }
    else if(p1 != NULL)
    {
        p2 = strchr(p1,':');
        p2+=2;
        if(p2 != NULL)
        {
            *data = atoi(p2);
            ret = 0;
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = -1;
    }
    return ret;
}

/*int prs_SIM_INFO_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    char imsi_data[20]={0};
    char ccid_data[20]={0};
    char spn_data[20]={0};
    char rat_data[20]={0};
    uint32_t data = 0;
    int i=0;
    int active_sim = 0;
    int ret = 0, r1 = 0, r2 = 0,r3 = 0,r4 = 0,r5 = 0;
    for(i=0;i<sz;i++)
    {
        p1 = strstr(dest[i],"ACTV_SIM_SLT");
        if(p1 != NULL)
        {
            break;
        }
    }
    if(p1 == NULL)
    {
        r1 = read_sim_imsi(dest,imsi_data,sz);
        if(r1 < 0)
        {
            ret = -1;
            memset(imsi_data,'0',5);
        }
        r2 = read_sim_ccid(dest,ccid_data,sz);
        if(r2 < 0)
        {
            ret = -1;
            memset(ccid_data,'0',5);
        }
        r3 = read_sim_spn(dest,spn_data,sz);
        if(r3 < 0)
        {
            ret = -1;
            memset(spn_data,'0',5);
        }
        r4 = read_sim_rat(dest,rat_data,sz);
        if(r4 < 0)
        {
            ret = -1;
            memset(rat_data,'0',5);
        }
        r5 = read_data_usage(dest,&data,sz);
        if(r5 < 0)
        {
            ret = -1;
            data = -1;
        }
        strcpy(obj->sSim_info[0].cIMSI,imsi_data);
        //printf("IMSI[0] = %s\n",obj->sSim_info[0].cIMSI);
        dbg_print(Bold_Cyan,"IMSI[0] = %s\n",obj->sSim_info[0].cIMSI);
        strcpy(obj->sSim_info[0].cCCID,ccid_data);
        //printf("CCID[0] = %s\n",obj->sSim_info[0].cCCID);
        dbg_print(Bold_Cyan,"CCID[0] = %s\n",obj->sSim_info[0].cCCID);
        strcpy(obj->sSim_info[0].cSpn,spn_data);
        //printf("Spn[0] = %s\n",obj->sSim_info[0].cSpn);
        dbg_print(Bold_Cyan,"Spn[0] = %s\n",obj->sSim_info[0].cSpn);
        strcpy(obj->sSim_info[0].cRAT,rat_data);
        //printf("RAT[0] = %s\n",obj->sSim_info[0].cRAT);
        dbg_print(Bold_Cyan,"RAT[0] = %s\n",obj->sSim_info[0].cRAT);
        obj->sSim_info[0].iData_usage = data; 
        //printf("Data Usage[0] = %d\n",obj->sSim_info[0].iData_usage); 
        dbg_print(Bold_Cyan,"Data Usage[0] = %d\n",obj->sSim_info[0].iData_usage);
    }
    if(p1 != NULL)
    {
        p2 = strchr(p1,':');
        p2+=2;
        if(p2 != NULL)
        {
            active_sim = atoi(p2);
            r1 = read_sim_imsi(dest,imsi_data,sz);
            if(r1 < 0)
            {
                ret = -1;
                memset(imsi_data,'0',5);
            }
            r2 = read_sim_ccid(dest,ccid_data,sz);
            if(r2 < 0)
            {
                ret = -1;
                memset(ccid_data,'0',5);
            }
            r3 = read_sim_spn(dest,spn_data,sz);
            if(r3 < 0)
            {
                ret = -1;
                memset(spn_data,'0',5);
            }
            r4 = read_sim_rat(dest,rat_data,sz);
            if(r4 < 0)
            {
                ret = -1;
                memset(rat_data,'0',5);
            }
            r5 = read_data_usage(dest,&data,sz);
            if(r5 < 0)
            {
                ret = -1;
                data = -1;
            }
            if ((active_sim == 1) || (active_sim == 2))
            {
                strcpy(obj->sSim_info[active_sim-1].cIMSI,imsi_data);
                //printf("IMSI[%d] = %s\n",(active_sim-1),obj->sSim_info[active_sim-1].cIMSI);
                strcpy(obj->sSim_info[active_sim-1].cCCID,ccid_data);
                //printf("CCID[%d] = %s\n",(active_sim-1),obj->sSim_info[active_sim-1].cCCID);
                strcpy(obj->sSim_info[active_sim-1].cSpn,spn_data);
                //printf("SPN[%d] = %s\n",(active_sim-1),obj->sSim_info[active_sim-1].cSpn);
                strcpy(obj->sSim_info[active_sim-1].cRAT,rat_data);
                //printf("RAT[%d] = %s\n",(active_sim-1),obj->sSim_info[active_sim-1].cRAT);
                obj->sSim_info[active_sim-1].iData_usage = data;
                //printf("Data Usage[%d] = %d\n",(active_sim-1),obj->sSim_info[active_sim-1].iData_usage); 

                dbg_print(Bold_Cyan,"IMSI[%d] = %s\n",(active_sim-1),obj->sSim_info[active_sim-1].cIMSI);
                dbg_print(Bold_Cyan,"CCID[%d] = %s\n",(active_sim-1),obj->sSim_info[active_sim-1].cCCID);
                dbg_print(Bold_Cyan,"SPN[%d] = %s\n",(active_sim-1),obj->sSim_info[active_sim-1].cSpn);
                dbg_print(Bold_Cyan,"RAT[%d] = %s\n",(active_sim-1),obj->sSim_info[active_sim-1].cRAT);
                dbg_print(Bold_Cyan,"Data Usage[%d] = %d\n",(active_sim-1),obj->sSim_info[active_sim-1].iData_usage); 
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = -1;
    }
    return ret;
}*/
            
int prs_mdm_RTC_TM_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0;
    int i=0;
    if((typ==0)||(typ==PRS_TYP_PRD))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"RTC_TM");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 == NULL)
        {
            memset(obj->cRtc_tm,0,sizeof(obj->cRtc_tm));
            //printf("RTC_TM = %s\n",obj->cRtc_tm);
            //dbg_print(Bold_Cyan,"RTC_TM = %s\n",obj->cRtc_tm);
        }
        if(p1 != NULL)
        { 
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                memset(obj->cRtc_tm,0,sizeof(obj->cRtc_tm));
                strcpy(obj->cRtc_tm,p2);
                //printf("RTC_TM = %s\n",obj->cRtc_tm);
                //dbg_print(Bold_Cyan,"RTC_TM = %s\n",obj->cRtc_tm);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    
    return ret;
}

int prs_mdm_netSts_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0;
    int i=0;
    if((typ == 0) || (typ==PRS_TYP_PRD))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"NET_STS");
            if(p1!=NULL)
            {
                break;
            }
        }
        if(p1 == NULL)
        {
            obj->iNet_sts = -1;
            //printf("NET_STS = %d\n",obj->iNet_sts);
            //dbg_print(Bold_Cyan,"NET_STS = %d\n",obj->iNet_sts);
            ret = -1;
        }
        else if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                obj->iNet_sts = atoi(p2);
                //printf("NET_STS = %d\n",obj->iNet_sts);
                //dbg_print(Bold_Cyan,"NET_STS = %d\n",obj->iNet_sts);
                ret = 0;
                //break;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }    
    return ret;
}

int prs_Btp_Slt_Sts_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0;
    int i=0;
    if((typ == 0) || (typ == PRS_TYP_BTP))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"BTP_SLT_STS");
            if(p1!=NULL)
            {
                break;
            }
        }
        if(p1 == NULL)
        {
            obj->iBtp_Slt_sts = -1;
            //printf("BTP_SLT_STS = %d\n",obj->iBtp_Slt_sts);
            //dbg_print(Bold_Cyan,"BTP_SLT_STS = %d\n",obj->iBtp_Slt_sts);
        }
        else if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                obj->iBtp_Slt_sts = atoi(p2);
                //printf("BTP_SLT_STS = %d\n",obj->iBtp_Slt_sts);
                //dbg_print(Bold_Cyan,"BTP_SLT_STS = %d\n",obj->iBtp_Slt_sts);
                ret = 0;
                //break;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    return ret;
}

int prs_Actv_Slt_Sts_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0;
    int i=0;
    if((typ == 0) || (typ == PRS_TYP_PRD))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"ACTV_SLT_STS");
            if(p1!=NULL)
            {
                break;
            }
        }
        if(p1 == NULL)
        {
            obj->iActv_Slt_sts = -1;
            ret = -1;
        }
        else if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                obj->iActv_Slt_sts = atoi(p2);
                //printf("ACTV_SLT_STS = %d\n",obj->iActv_Slt_sts);
                //dbg_print(Bold_Cyan,"ACTV_SLT_STS = %d\n",obj->iActv_Slt_sts);
                ret = 0;
                //break;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    
    return ret;
}


int prs_rstIntrvl_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0, len = 0;
    char *numstr[10]={0};
    int i=0;
    if((typ == 0) || (typ == PRS_TYP_PRD))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"RST_TIME");
            if(p1 != NULL)
            {
                break;
            }
        }

        if(p1 == NULL)
        {
            obj->sRst_Time.idom = -1;
            obj->sRst_Time.imonth = -1;
            obj->sRst_Time.ihrs = -1;
            obj->sRst_Time.iminute = -1;
            ret = -1;
        }
        else if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                len = str2numstr(p1,numstr,10,1);
                obj->sRst_Time.idom = (atoi(numstr[0]));
                obj->sRst_Time.imonth = (atoi(numstr[1]));
                obj->sRst_Time.ihrs = (atoi(numstr[2]));
                obj->sRst_Time.iminute = (atoi(numstr[3]));
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    
    return ret;
}


int prs_mdm_activeSim_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0;
    int i=0;
    if((typ == 0) || (typ == PRS_TYP_PRD))
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"ACTV_SIM_SLT");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 == NULL)
        {
            obj->iActv_sim_slt = -1;
            ret = -1;
        }
        else if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                obj->iActv_sim_slt = atoi(p2);
                //printf("ACTV_SIM_SLT = %d\n",obj->iActv_sim_slt);
                //dbg_print(Bold_Cyan,"ACTV_SIM_SLT = %d\n",obj->iActv_sim_slt);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
    
    return ret;
}

int prs_mdm_pngSts_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char *p1 = NULL, *p2= NULL;
    int ret = 0;
    int i=0;
    if( (typ==0) || (typ==PRS_TYP_PRD) )
    {
        for(i=0;i<sz;i++)
        {
            p1 = strstr(dest[i],"PNG_STS");
            if(p1 != NULL)
            {
                break;
            }
        }
        if(p1 == NULL)
        {
            obj->iPng_sts = -1;
            ret = -1;
            //printf("PNG_STS = %d\n",obj->iPng_sts);
            //dbg_print(Bold_Cyan,"PNG_STS = %d\n",obj->iPng_sts);
        }
        else if(p1 != NULL)
        {
            p2 = strchr(p1,':');
            p2+=2;
            if(p2 != NULL)
            {
                obj->iPng_sts = atoi(p2);
                //printf("PNG_STS = %d\n",obj->iPng_sts);
                //dbg_print(Bold_Cyan,"PNG_STS = %d\n",obj->iPng_sts);
                ret = 0;
            }
            else
            {
                ret = -1;
            }
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = 1;
    }
        
    return ret;
}

int prs_sim_rat1(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char rat_data[20]={0};
    int ret = 0, r=0;
    if((typ == 0) || (typ == PRS_TYP_PRD))
    {
        memset(rat_data,0,sizeof(rat_data));
        r = read_SIM_RAT2(dest,rat_data,sz,1);
        if(r < 0)
        {
            ret = -1;
        }
        strcpy(obj->sSim_info[0].cRAT,rat_data);
        ret = 0;
    }
    else
    {
        ret = 1;
    }
    return ret;
}

int prs_sim_rat2(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char rat_data[20]={0};
    int ret = 0, r=0;
    if((typ == 0) || (typ == PRS_TYP_PRD))
    {
        memset(rat_data,0,sizeof(rat_data));
        r = read_SIM_RAT2(dest,rat_data,sz,2);
        if(r < 0)
        {
            ret = -1;
        }
        strcpy(obj->sSim_info[1].cRAT,rat_data);
        ret = 0;
    }
    else
    {
        ret = 1;
    }
    return ret;
}

int prs_sim_imsi1(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char imsi_data[20]={0};
    int ret = 0, r=0;
    if((typ == 0) || (typ == PRS_TYP_BTP))
    {
        memset(imsi_data,0,sizeof(imsi_data));
        r = read_SIM_IMSI2(dest,imsi_data,sz,1);
        if(r < 0)
        {
            ret = -1;
        }
        strcpy(obj->sSim_info[0].cIMSI,imsi_data);
        ret = 0;
    }
    else
    {
        ret = 1;
    }
    return ret;
}

int prs_sim_imsi2(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char imsi_data[20]={0};
    int ret = 0, r=0;
    if((typ == 0) || (typ == PRS_TYP_BTP))
    {
        memset(imsi_data,0,sizeof(imsi_data));
        r = read_SIM_IMSI2(dest,imsi_data,sz,2);
        if(r < 0)
        {
            ret = -1;
        }
        strcpy(obj->sSim_info[1].cIMSI,imsi_data);
        ret = 0;
    }
    else
    {
        ret = 1;
    }
    return ret;
}

int prs_sim_ccid1(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
     char ccid_data[20]={0};
    int ret = 0, r=0;
    if((typ == 0) || (typ == PRS_TYP_BTP))
    {
        memset(ccid_data,0,sizeof(ccid_data));
        r = read_SIM_CCID2(dest,ccid_data,sz,1);
        if(r < 0)
        {
            ret = -1;
        }
        strcpy(obj->sSim_info[0].cCCID,ccid_data);
        ret = 0;
    }
    else
    {
        ret = 1;
    }
    return ret;
}

int prs_sim_ccid2(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char ccid_data[20]={0};
    int ret = 0, r=0;
    if((typ == 0) || (typ == PRS_TYP_BTP))
    {
        memset(ccid_data,0,sizeof(ccid_data));
        r = read_SIM_CCID2(dest,ccid_data,sz,2);
        if(r < 0)
        {
            ret = -1;
        }
        strcpy(obj->sSim_info[1].cCCID,ccid_data);
        ret = 0;
    }
    else
    {
        ret = 1;
    }
    return ret;
}

int prs_sim_spn1(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char spn_data[20]={0};
    int ret = 0, r=0;
    if((typ == 0) || (typ == PRS_TYP_BTP))
    {
        memset(spn_data,0,sizeof(spn_data));
        r = read_SIM_SPN2(dest,spn_data,sz,1);
        if(r < 0)
        {
            ret = -1;
        }
        strcpy(obj->sSim_info[0].cSpn,spn_data);
        ret = 0;
    }
    else
    {
        ret = 1;
    }
    return ret;
}

int prs_sim_spn2(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ)
{
    char spn_data[20]={0};
    int ret = 0, r=0;
    if((typ == 0) || (typ == PRS_TYP_BTP))
    {
        memset(spn_data,0,sizeof(spn_data));
        r = read_SIM_SPN2(dest,spn_data,sz,2);
        if(r < 0)
        {
            ret = -1;
        }
        strcpy(obj->sSim_info[1].cSpn,spn_data);
        ret = 0;
    }
    else
    {
        ret = 1;
    }
    return ret;
}


uint32_t prs_mdm_init(Parse_func_ *parse_func)
{
    uint32_t i = 0;
    parse_func[i++].f_prs_mdm = prs_mdm_sn_data ;
    parse_func[i++].f_prs_mdm = prs_mdm_model_data;
    parse_func[i++].f_prs_mdm = prs_mdm_mfn_data;
    parse_func[i++].f_prs_mdm = prs_mdmFMW_ver;
    parse_func[i++].f_prs_mdm = prs_rtrFMW_ver;
    parse_func[i++].f_prs_mdm = prs_appRLS_ver;
    parse_func[i++].f_prs_mdm = prs_appINV_ver;
    parse_func[i++].f_prs_mdm = prs_sim_imsi1;
    parse_func[i++].f_prs_mdm = prs_sim_imsi2;
    parse_func[i++].f_prs_mdm = prs_sim_ccid1;
    parse_func[i++].f_prs_mdm = prs_sim_ccid2;
    parse_func[i++].f_prs_mdm = prs_sim_spn1;
    parse_func[i++].f_prs_mdm = prs_sim_spn2;
    parse_func[i++].f_prs_mdm = prs_Btp_Slt_Sts_data;

    parse_func[i++].f_prs_mdm = prs_mdm_sys_uptime;
    parse_func[i++].f_prs_mdm = prs_mdm_csq_data;
    parse_func[i++].f_prs_mdm = prs_mdm_temp_data;
    parse_func[i++].f_prs_mdm = prs_mdm_netSts_data;
    parse_func[i++].f_prs_mdm = prs_mdm_pngSts_data;
    parse_func[i++].f_prs_mdm = prs_sim_rat1;
    parse_func[i++].f_prs_mdm = prs_sim_rat2;
    parse_func[i++].f_prs_mdm = prs_mdm_IP_ecm0_data;
    parse_func[i++].f_prs_mdm = prs_mdm_IP_eth0_data;
    parse_func[i++].f_prs_mdm = prs_mdm_IP_eth1_data;
    parse_func[i++].f_prs_mdm = prs_mdm_IP_eth2_data;
    parse_func[i++].f_prs_mdm = prs_mdm_memInfo_data;
    parse_func[i++].f_prs_mdm = prs_mdm_RTC_TM_data;
    parse_func[i++].f_prs_mdm = prs_mdm_activeSim_data;
    parse_func[i++].f_prs_mdm = prs_rstIntrvl_data;
    parse_func[i++].f_prs_mdm = prs_Actv_Slt_Sts_data;
    return i;
}
void print_btp_rcrd(sVEGA_PRD_ *sBtp)
{
    dbg_print(Bold_Yellow,"<Bootup record>\n");
    dbg_print(Bold_Green,"\nMDM_SN = %s",sBtp->cMdm_sn);
    dbg_print(Bold_Green,"\nMDM_MDL = %s",sBtp->cMdm_mdl);
    dbg_print(Bold_Green,"\nMFN = %s",sBtp->cMdm_mfn);
    dbg_print(Bold_Green,"\nVER_MDM_FMW = %s",sBtp->sVersion.cVER_MDM_FMW);
    dbg_print(Bold_Green,"\nVER_RTR_FMW = %s",sBtp->sVersion.cVER_RTR_FMW);
    dbg_print(Bold_Green,"\nVER_APP_RLS = %s",sBtp->sVersion.cVER_APP_RLS);
    dbg_print(Bold_Green,"\nVER_APP_INV = %s",sBtp->sVersion.cVER_APP_INV);
    dbg_print(Bold_Green,"\nIMSI[0] = %s",sBtp->sSim_info[0].cIMSI);
    dbg_print(Bold_Green,"\nIMSI[1] = %s",sBtp->sSim_info[1].cIMSI);
    dbg_print(Bold_Green,"\nCCID[0] = %s",sBtp->sSim_info[0].cCCID);
    dbg_print(Bold_Green,"\nCCID[1] = %s",sBtp->sSim_info[1].cCCID);
    dbg_print(Bold_Green,"\nSpn[0] = %s",sBtp->sSim_info[0].cSpn);
    dbg_print(Bold_Green,"\nSpn[1] = %s",sBtp->sSim_info[1].cSpn);
    dbg_print(Bold_Green,"\nBTP_SLT_STS = %d",sBtp->iBtp_Slt_sts);
    dbg_print(Bold_Yellow,"\n<End Bootup record>\n");
}

void print_periodic_rcrd(sVEGA_PRD_ *sPrd)
{
    dbg_print(Bold_Yellow, "\n<Periodic record>");
    dbg_print(Bold_Magenta,"\nupdate = %s",sPrd->sSys_uptime.cUpdate);
    dbg_print(Bold_Magenta,"\nuptime = %s",sPrd->sSys_uptime.cUptime);
    dbg_print(Bold_Magenta,"\ndiff_time = %s",sPrd->sSys_uptime.cDiff_time);
    dbg_print(Bold_Magenta,"\ndiff_day = %d",sPrd->sSys_uptime.iDiff_day);
    dbg_print(Bold_Magenta,"\ncsq[0]= %d",sPrd->cCsq[0]);
    dbg_print(Bold_Magenta,"\ncsq[1]= %d",sPrd->cCsq[1]);
    dbg_print(Bold_Magenta,"\nTotal= %d",sPrd->sMem_info.iTotal);
    dbg_print(Bold_Magenta,"\nUsed= %f",sPrd->sMem_info.fUsed);
    dbg_print(Bold_Magenta,"\nCached= %f",sPrd->sMem_info.fCached);
    dbg_print(Bold_Magenta,"\ntemp[0]= %.2f",sPrd->fTemp[0]);
    dbg_print(Bold_Magenta,"\ntemp[1]= %.2f",sPrd->fTemp[1]);
    dbg_print(Bold_Magenta,"\nIP_ecm0 = %s",sPrd->ip.cIP_ecm0);
    dbg_print(Bold_Magenta,"\nIP_eth0 = %s",sPrd->ip.cIP_eth0);
    dbg_print(Bold_Magenta,"\nIP_eth1 = %s",sPrd->ip.cIP_eth1);
    dbg_print(Bold_Magenta,"\nIP_eth2 = %s",sPrd->ip.cIP_eth2);
    dbg_print(Bold_Magenta,"\nRTC_TM = %s",sPrd->cRtc_tm);
    dbg_print(Bold_Magenta,"\nNET_STS = %d",sPrd->iNet_sts);
    dbg_print(Bold_Magenta,"\nPNG_STS = %d",sPrd->iPng_sts);
    dbg_print(Bold_Magenta,"\nRAT[0] = %s",sPrd->sSim_info[0].cRAT);
    dbg_print(Bold_Magenta,"\nRAT[1] = %s",sPrd->sSim_info[1].cRAT);
    dbg_print(Bold_Magenta,"\nACTV_SLT_STS = %d",sPrd->iActv_Slt_sts);
    dbg_print(Bold_Magenta,"\nRST_TIME_day = %d",sPrd->sRst_Time.idom);
    dbg_print(Bold_Magenta,"\nRST_TIME_month = %d",sPrd->sRst_Time.imonth);
    dbg_print(Bold_Magenta,"\nRST_TIME_hours = %d",sPrd->sRst_Time.ihrs);
    dbg_print(Bold_Magenta,"\nRST_TIME_minute = %d",sPrd->sRst_Time.iminute);
    dbg_print(Bold_Magenta,"\nACTV_SIM_SLT = %d",sPrd->iActv_sim_slt);
    dbg_print(Bold_Yellow, "\n<End Periodic record>\n");
}

int get_btp_rcrd(char *lns[], int sz_lns,int sz_func,\
            sVEGA_PRD_ *obj,Parse_func_ *parse_func, uint32_t pkt_typ)
{
    int i = 0;
    int ret = 0;
    int rt_prs = 0, cnt=0;
    if(pkt_typ == PRS_TYP_BTP)
    {
        for(i=0;i<sz_func;i++)//bootup
        {
            dbg_print(NULL,"i=%d\trt_prs=%d\tcnt=%d\n",i,rt_prs,cnt);
            rt_prs = parse_func[i].f_prs_mdm(lns,sz_lns,obj,PRS_TYP_BTP);
            if(rt_prs == 0)
            {
                cnt++;
            }
        }
        dbg_print(Cyan,"In get_btp_rcrd(), cnt = %d\n",cnt);
        if(cnt == BTP_PRM_CNT)
        {
            ret = 0;        
        }
        else
        {
            ret = -1;
        }
    }
    else
    {
        ret = -2;
    }
    
    return ret;
}

int get_prd_rcrd(char *lns[], int sz_lns,int sz_func,\
            sVEGA_PRD_ *obj,Parse_func_ *parse_func, uint32_t pkt_typ)
{
    int i = 0;
    int rt_prs = 0;
    if(pkt_typ == PRS_TYP_PRD)
    {
        for(i=12;i<sz_func;i++)//periodic
        {
            rt_prs = parse_func[i].f_prs_mdm(lns,sz_lns,obj,PRS_TYP_PRD);
        }
        i = 0;
    }
    else
    {
        i = -1;
    }
    return i;
}

int read_pkt_typ(char *pkt)
{
    int ret=0;
    if(strstr(pkt,PKT_BTP))
    {
        ret = PRS_TYP_BTP;
    }
    else if(strstr(pkt,PKT_PRD))
    {
        ret = PRS_TYP_PRD;
    }
    else
    {
        ret = -1;
    }
    
    return ret;
}
