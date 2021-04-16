#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h> 
#include <errno.h> 
#include <termios.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#include "mqtt.h"

 #define TMP_BASIC 1

#ifdef TMP_BASIC
    #include "operation/mdm_payload.h"
    #include "basic/basic.h"
#else
    #include "./mdm_payload.h"
#endif

#include "operation/mdm_operation.h"



/*********************************************
 * Pack type : Bootup {0/1}
 *  Arguments:
 *      Size(str) : 25
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_sn(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret = 0, cnt = 0;;
    char tmp[30] = {0};
    if(sz < 25)
    {
        return -1;
    }
    memset(str,0,25);
    if( ((typ&3) == 0) || ((typ&3) == 1) )
    {
        
        //Use the function for both types
        do
        {
            ret = mdm_get_sn(fd,tmp);
            msleep(150);
            
        } while ( (ret !=0) && (cnt++ <= 10) );
        if(ret == 0)
        {
            sprintf(str,"MDM_SN : %s\n",tmp);
            ret = strlen(str);
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*********************************************
 * Pack type : Bootup {0/1}
 *  Arguments:
 *      Size(str) : 25
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_model(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret = 0;
    char tmp[10] = {0};
    memset(str,0,sz);
    if(((typ&3)==0) || ((typ&3)==1))
    {
        if(sz < 25)
        {
            return -1;
        }        
        ret = mdm_get_model(fd,tmp);
        if(ret == 0)
        {
            sprintf(str,"MDM_MDL : %s\n",tmp);
            ret = strlen(str);
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*********************************************
 * Pack type : Bootup {0/1}
 *  Arguments:
 *      Size(str) : 30
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_mfn(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret = 0;
    char tmp[30] = {0};
    char *ptr=NULL; 
    memset(str,0,sz);
    if( ((typ&3)==0) || ((typ&3)==1) )
    {
        if(sz < 30)
        {
            return -1;
        }
        memset(str,0,sz);
        ret = mdm_get_mfn(fd,tmp);
        if(ret == 0)
        {
            for(int i=0;i<strlen(tmp);i++)
            {
                if( (tmp[i]==',') || (tmp[i]==' ') )
                {
                    tmp[i] = '_';
                }
            }
            sprintf(str,"MFN : %s\n",tmp);
            ret = strlen(str);
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}


/*********************************************
 * Pack type : Bootup {0/1}
 *  Arguments:
 *      Size(str) : 60
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_fmwv(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret = 0;
    char tmp[60] = {0};
    memset(str,0,sz);
    if(((typ&3)==0)||((typ&3)==1))
    {
        if(sz < 60)
        {
            return -1;
        }
        memset(str,0,sz);
        ret = mdm_get_fmwv(fd,tmp);
        if(ret == 0)
        {
            sprintf(str,"FMWV : %s\n",tmp);
            ret = strlen(str);
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*********************************************
 * Pack type : Periodic {0/2}
 *  Arguments:
 *      Size(str) : 10
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_csq(int fd, char *str, uint32_t sz, uint32_t typ)
{
    
    int ret = 0;
    char tmp[10] = {0};
    int cnt = 0;
    memset(str,0,sz);
    if(((typ&3)==0)||((typ&3)==2))
    {
        if(sz < 10)
        {
            return -1;
        }
        memset(str,0,sz);
        do
        {
            ret = mdm_get_csq(fd,tmp);
            msleep(200);
        } while ((ret!=0) && (cnt++ < 5) );       
        
        if(ret == 0)
        {
            sprintf(str,"CSQ : %s\n",tmp);
            ret = strlen(str);
        }
        else
        {
            sprintf(str,"CSQ : \n");
            ret = strlen(str);
        }
        
    }
    else{
        ret = 0;
    }
    return ret;
}

/*********************************************
 * Pack type : Periodic {0/2}
 *  Arguments:
 *      Size(str) : 25
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_paTmp(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret = 0;
    char tmp[10] = {0};
    memset(str,0,sz);
    if(((typ&3)==0)||((typ&3)==2))
    {
        if(sz < 25)
        {
            return -1;
        }
        memset(str,0,sz);
        ret = mdm_get_paTmp(fd,tmp);
        if(ret == 0)
        {
            sprintf(str,"Power amp tmp : %s\n",tmp);
            ret = strlen(str);
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*********************************************
 * Pack type : Periodic {0/2}
 *  Arguments:
 *      Size(str) : 25
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_pcTmp(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret = 0;
    char tmp[10] = {0};
    memset(str,0,sz);
    if(((typ&3)==0)||((typ&3)==2))
    {
        if(sz < 25)
        {
            return -1;
        }
        memset(str,0,sz);
        ret = mdm_get_pcTmp(fd,tmp);
        if(ret == 0)
        {
            sprintf(str,"Power core tmp : %s\n",tmp);
            ret = strlen(str);
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*********************************************
 * Pack type : Bootup / sim specific {0/1/3}
 *  Arguments:
 *      Size(str) : 20
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_imsi(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret = 0;
    int cnt = 0;
    memset(str,0,sz);
    char tmp[20] = {0};
    // printf("typ = 0x%x\n",typ);
    if(((typ&3)==0)||((typ&3)==1)||((typ&3)==3))
    {
        if(sz < 20)
        {
            return -1;
        }
        memset(str,0,sz);
        do
        {
            ret = mdm_get_imsi(fd,tmp);
            if(ret!=0)
            {
                msleep(200);
                memset(tmp,0,20);
            }
        }
        while( (ret!=0) && (cnt++ < 3) );
        // printf("typ=0x%x",typ);
        // printf("ret = %d\n",ret);
        if(ret == 0 && ( ((typ&4)>>2)==1) )
        {
            sprintf(str,"IMSI_%d : %s\n",read_active_sim(),tmp);
            ret = strlen(str);
        }
        else
        {
            sprintf(str,"IMSI_%d : \n",read_active_sim());
            ret = -1;
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*********************************************
 * Pack type : Bootup / sim specific {0/1/3}
 *  Arguments:
 *      Size(str) : 25
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_ccid(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret = 0, cnt=0;;
    char tmp[25] = {0};
    memset(str,0,sz);
    // printf("typ = 0x%x\n",typ);
    if(((typ&3)==PACK_TYP_ALL) || ((typ&3)==((PACK_TYP_BTUP)&3)) \
             || ((typ&PACK_TYP_SIM&3)==3))
    {
        if(sz < 25)
        {
            return -1;
        }
        memset(str,0,sz);
        do
        {
            ret = mdm_get_ccid(fd,tmp);
            if(ret!=0)
            {
                memset(tmp,0,25);
                msleep(100);
            }
        }while( (ret!=0) && (cnt++ < 3) );

        if(ret == 0 && ( ((typ&4)>>2)==1) )
        {
            sprintf(str,"CCID_%d : %s\n",read_active_sim(),tmp);
            ret = strlen(str);
        }
        else
        {
        sprintf(str,"CCID_%d : \n",read_active_sim()); 
        ret = -1;
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*********************************************
 * Pack type : Bootup / sim specific {0/1/3}
 *  Arguments:
 *      Size(str) : 20
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_spn(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret = 0, cnt=0;
    char tmp[30] = {0};
    memset(str,0,sz);
    // printf("typ = 0x%x\n",typ);
    if(((typ&3)==0) || ((typ&3)==1) || ((typ&3)==3))
    {
        if(sz < 20)
        {
            return -1;
        }
        memset(str,0,sz);
        do
        {
            ret = mdm_get_spn(fd,tmp);
            if(ret != 0)
            {
                memset(tmp,0,30);
                msleep(100);
            }
        } while ( (ret!=0) && (cnt++ < 3) );
        
        
        // printf("ret = %d\n",ret);
        if(ret == 0 && ( ((typ&4)>>2)==1) )
        {
            sprintf(str,"SPN_%d : %s\n",read_active_sim(),tmp);
            ret = strlen(str);
        }
        else
        {
            sprintf(str,"SPN_%d : \n",read_active_sim());
            ret = -1;
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}


/*********************************************
 * Pack type : Periodic {0/2}
 *  Arguments:
 *      Size(str) : 15
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_netSts(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret = 0;
    memset(str,0,sz);
    if(((typ&3)==0) || ((typ&3)==2))
    {
        if(sz < 15)
        {
            return -1;
        }
        memset(str,0,sz);
        ret = mdm_get_netSts_withIP(); // We need to rewrite the definetion.
        if((ret == 0) || (ret == 1))
        {
            sprintf(str,"NET_STS(%d) : %d\n",read_active_simVal()+1,ret);
            ret = strlen(str);
        }
        else
        {
            sprintf(str,"NET_STS(%d) : %d\n",read_active_simVal()+1,-1);
            ret = -1;
        }
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*********************************************
 * Pack type : Periodic {0/2}
 *  Arguments:
 *      Size(str) : 15
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_pngSts(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret = 0;
    memset(str,0,15);
    if(((typ&3)==0) || ((typ&3)==2))
    {
        if(sz < 15)
        {
            return -1;
        }
        memset(str,0,15);
        ret = get_ping_sts();
        if(ret >= 0)
        {
            sprintf(str,"PNG_STS(%d) : %d\n",read_active_sim(),ret);
            ret = 0;
        }
        else
        {
            sprintf(str,"PNG_STS(%d) : %d\n",read_active_sim(),-1);
            ret = -1;
        }
    }
    else
    {
        ret = 0;
    }
    return ret;    
}


/***********************************************
 * Pack type : Bootup {0/1}
 * Arguments : 
 *  fd : required
 *  sz : min (250) 
 * ********************************************/
int pack_rtr_fmwv(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret = 0;
    FMW_VER_ f_ver = {0};
    memset(str,0,sz);
    if(((typ&3)==0) || ((typ&3)==1))
    {
        if(sz < 250)
        {
            return -1;
        }
        memset(str,0,sz);
        ret = get_fmw_ver(fd,&f_ver);
        if(ret == 0)
        {
            sprintf(str,\
            "VER_MDM_FMW : %s\nVER_RTR_FMW : %s__%s\nVER_APP_RLS : %s\nVER_APP_INV : %s\n",\
                f_ver.v_mdm_fmw, f_ver.v_fs, f_ver.v_krn, f_ver.v_app_rls, f_ver.v_app_inv );
            ret = strlen(str);
        }
        else
        {
            ret = -1;
        }  
    }
    else
    {
        ret = 0;
    }
    return ret;
}


/***********************************************
 * Pack type : Periodic {0/2}
 * Arguments : 
 *  fd : required
 *  sz : min (20) 
 * ********************************************/
int pack_mdm_rat(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret;
    char tmp[50] = {0};
    memset(str,0,sz);
    if(((typ&3)==0) || ((typ&3)==2))
    {
        if(sz < 20)
        {
            return -1;
        }
        memset(str,0,sz);
        ret = mdm_get_rat(fd,tmp,10);
        if(ret == 0)
        {
            sprintf(str,"SIM_RAT_%d : %s\n",read_active_sim(),tmp);
        }
        else
        {
            sprintf(str,"SIM_RAT_%d : \n",read_active_sim());
        }
    }
    else
    {
        ret = 0;
    }
    
    return ret;
}

/***********************************************
 * Pack type : Periodic {0/2}
 * Arguments : 
 *  fd : not required
 *  sz : min (25) 
 * Return : {-1:0}
 * ********************************************/
int pack_mdm_sltSts(int fd, char *str, uint32_t sz_str, uint32_t typ)
{
    int ret = 0;
    memset(str,0,sz_str);
    if(((typ&3)==0) || ((typ&3)==2))
    {
        if(sz_str < 25)
        {
            return -1;
        }
        memset(str,0,sz_str);
        ret = mdm_get_Slt_Sts(fd);
        sprintf(str,"ACTV_SLT_STS : %d\n",ret);
    }
    else
    {
        ret = 0;
    }
    
    return ret;
}

uint32_t pack_func_init(Pack_func_ *pack_func, uint32_t *sz)
{
    uint32_t i = 0;

    //0
    pack_func[i].f_pack = pack_mdm_sn;
    sz[i] = pack_func[i].sz = 25;
    i+=1;

    //1
    pack_func[i].f_pack = pack_mdm_model;
    sz[i]= pack_func[i].sz = 25;
    i+=1;

    //2
    pack_func[i].f_pack = pack_mdm_mfn;
    sz[i] = pack_func[i].sz = 30;
    i+=1;

    //4
    pack_func[i].f_pack = pack_mdm_imsi;
    sz[i] = pack_func[i].sz = 20;
    i+=1;

    pack_func[i].f_pack = pack_mdm_ccid;
    sz[i] = pack_func[i].sz = 25;
    i+=1;

    pack_func[i].f_pack = pack_mdm_spn;
    sz[i] = pack_func[i].sz = 20;
    i+=1;

    pack_func[i].f_pack = pack_sys_uptime;
    sz[i] = pack_func[i].sz = 75;
    i+=1;

    pack_func[i].f_pack = pack_rtr_fmwv;
    sz[i] = pack_func[i].sz = 250;
    i+=1;

    //5
    pack_func[i].f_pack = pack_mdm_csq; //Periodic
    sz[i] = pack_func[i].sz = 25; 
    i+=1;

    //6
    pack_func[i].f_pack = pack_mdm_paTmp; //Periodic
    sz[i] = pack_func[i].sz = 25;  
    i+=1;

    //7
    pack_func[i].f_pack = pack_mdm_pcTmp; //Periodic
    sz[i] = pack_func[i].sz = 25;
    i+=1;

    pack_func[i].f_pack = pack_mdm_netSts;
    sz[i] = pack_func[i].sz = 15;
    i+=1;

    pack_func[i].f_pack = pack_mdm_pngSts;
    sz[i] = pack_func[i].sz = 15;
    i+=1;
    
    pack_func[i].f_pack = pack_eth_info;
    sz[i] = pack_func[i].sz = 120;
    i+=1;

    //13
    pack_func[i].f_pack = pack_meminfo;
    sz[i] = pack_func[i].sz = 70;
    i+=1;

    //14
    pack_func[i].f_pack = pack_sys_time;
    sz[i] = pack_func[i].sz = 30;
    i+=1;

    //16
    pack_func[i].f_pack = pack_sim_slot;
    sz[i] = pack_func[i].sz = 25;
    i+=1;

    //17
    pack_func[i].f_pack = pack_mdm_rat;
    sz[i] = pack_func[i].sz = 25;
    i+=1;

    //18
    pack_func[i].f_pack = pack_mdm_sltSts;
    sz[i] = pack_func[i].sz = 25;
    i+=1;

    //20
    pack_func[i].f_pack = pack_mdm_rstIntrvl;
    sz[i] = pack_func[i].sz = 25;
    i+=1;

    return (i);
}

int mdm_pld_exp(char *pld, uint32_t sz_max, Pack_func_ *pack_func, uint32_t sz_pkFun)
{
    char tmp_str[300];
    uint32_t ret = 0;
    struct termios tty;
    int fd = mdm_init(&tty);

    memset(pld,0,strlen(pld));
    if(fd < 0)
    {
        printf("mdm_init() failed in mdm_pld_exp()\n");
        return -1;
    }
    printf("sz_pack_fun = %d\n",sz_pkFun);
    for(int i=0;i<sz_pkFun;i++)
    {
        ret = pack_func[i].f_pack(fd,tmp_str, pack_func[i].sz,0);
        if(ret < 0)
        {
            dbg_print(Bold_Red,"\nERR : error at idx-%d while creating pld in mdm_pld_exp()\n");
            dbg_print(Bold_Red,"Exiting ... .. .\n");
            return -1;
        }
        strcat(pld,tmp_str);
    }
    close(fd);
    return strlen(pld);
}


int mdm_pld_btup(char *pld, char *pld2, uint32_t sz_max, char *dlm)
{
    struct termios tty;
    int fd = mdm_init(&tty);
    int sts = 0;
    int ret = 0;
    uint8_t chk=0;
    memset(pld,0,sz_max);
    memset(pld2,0,sz_max);
    if(fd > 0)
    {
        char str[200]={0};
        printf("del-bug\n");
        sts = mdm_get_sn(fd,str);
        if(sts == 0)
        {
            sprintf(pld,"\n\r----<start>----\n\r");
            strcat(pld,"MDM_SN : ");
            strcat(pld,str);strcat(pld2,str);strcat(pld2,dlm);// printf("pld=%d,pld2=%d\n",strlen(pld),strlen(pld2));
            
            memset(str,0,strlen(str));
            sts = mdm_get_model(fd,str);
            if(sts == 0)
            {
                strcat(pld,"\n\rMDM_MDL : ");
                strcat(pld,str);strcat(pld2,str);strcat(pld2,dlm);// printf("pld=%d,pld2=%d\n",strlen(pld),strlen(pld2));
                
                memset(str,0,strlen(str));
                ret++;

                sts = mdm_get_mfn(fd,str);
                if(sts ==0)
                {
                    strcat(pld,"\n\rMFN : ");
                    strcat(pld,str);strcat(pld2,str);strcat(pld2,dlm);// printf("pld=%d,pld2=%d\n",strlen(pld),strlen(pld2));
                    
                    memset(str,0,strlen(str));
                    ret++;

                    sts = mdm_get_fmwv(fd,str);
                    if(sts == 0)
                    {
                        strcat(pld,"\n\rFMWV : ");
                        strcat(pld,str);strcat(pld2,str);strcat(pld2,dlm);// printf("pld=%d,pld2=%d\n",strlen(pld),strlen(pld2));
                        
                        memset(str,0,strlen(str));
                        ret++;

                        sts = mdm_get_csq(fd,str);
                        if(sts == 0)
                        {
                            strcat(pld,"\n\rCSQ : ");
                            strcat(pld,str);strcat(pld2,str);strcat(pld2,dlm);// printf("pld=%d,pld2=%d\n",strlen(pld),strlen(pld2));
                            
                            memset(str,0,strlen(str));
                            ret++;

                            sts = mdm_get_paTmp(fd,str);
                            if(sts == 0)
                            {
                                strcat(pld,"\n\rPower amp tmp : ");
                                strcat(pld,str);strcat(pld2,str);strcat(pld2,dlm);// printf("pld=%d,pld2=%d\n",strlen(pld),strlen(pld2));
                                
                                memset(str,0,strlen(str));
                                ret++;

                                sts = mdm_get_pcTmp(fd,str);
                                if(sts == 0)
                                {
                                    strcat(pld,"\n\rPower core tmp : ");
                                    strcat(pld,str);strcat(pld2,str);strcat(pld2,dlm);// printf("pld=%d,pld2=%d\n",strlen(pld),strlen(pld2));
                                    
                                    memset(str,0,strlen(str));
                                    ret++;

                                    sts = mdm_get_imsi(fd,str);
                                    if(sts == 0)
                                    {
                                        strcat(pld,"\n\rIMSI : ");
                                        strcat(pld,str);strcat(pld2,str);strcat(pld2,dlm);// printf("pld=%d,pld2=%d\n",strlen(pld),strlen(pld2));
                                        
                                        ret++;
                                        // printf("\n--------chk1--------\n");

                                        sts = mdm_get_ccid(fd,str);
                                        if(sts == 0)
                                        {
                                            strcat(pld,"\n\rCCID : ");
                                            strcat(pld,str);strcat(pld2,str);strcat(pld2,dlm);// printf("pld=%d,pld2=%d\n",strlen(pld),strlen(pld2));
                                            memset(str,0,strlen(str));
                                            ret++;

                                            sts = mdm_get_spn(fd,str);
                                            if(sts == 0)
                                            {
                                                strcat(pld,"\n\rSPN : ");
                                                strcat(pld,str);strcat(pld2,str);strcat(pld2,dlm);// printf("pld=%d,pld2=%d\n",strlen(pld),strlen(pld2));
                                                // printf("\n---chk%d---\n",ret);
                                                // printf("sz_str=%d\n",strlen(str));
                                                ret++;

                                                // printf("clearing str\n");
                                                memset(str,0,100);
                                                //sts = mdm_get_netSts();
                                                sts = 1;
                                                sprintf(str,"%d",sts);
                                                if(sts >= 0)
                                                {
                                                    strcat(pld,"\n\rNET_STS : ");
                                                    strcat(pld,str);strcat(pld2,str);strcat(pld2,dlm);// printf("pld=%d,pld2=%d\n",strlen(pld),strlen(pld2));
                                                    // printf("\n---chk%d---\n",ret);
                                                    // printf("sz_str=%d\n",strlen(str));
                                                    memset(str,0,strlen(str));
                                                    ret++;
                                                    
                                                    strcat(pld,"\n\r----<end>----\n\r");
                                                    ret = 100;
                                                }

                                            }
                                            else
                                            {
                                                #ifdef TMP_BASIC
                                                dbg_print(Bold_Red,"\n\rERR-PLD : mdm_get_spn() failed\n\r");
                                                #else
                                                printf("\n\rERR-PLD : mdm_get_spn() failed\n\r");
                                                #endif
                                            }
                                        }
                                        else
                                        {
                                            #ifdef TMP_BASIC
                                            dbg_print(Bold_Red,"\n\rERR-PLD : mdm_get_ccid() failed\n\r");
                                            #else
                                            printf("\n\rERR-PLD : mdm_get_ccid() failed\n\r");
                                            #endif
                                        }
                                    }
                                    else
                                    {
                                        #ifdef TMP_BASIC
                                        dbg_print(Bold_Red,"\n\rERR-PLD : mdm_get_imsi() failed\n\r");
                                        #else
                                        printf("\n\rERR-PLD : mdm_get_imsi() failed\n\r");
                                        #endif
                                    }                                    
                                }
                                else
                                {
                                    #ifdef TMP_BASIC
                                    dbg_print(Bold_Red,"\n\rERR-pcTMP : mdm_get_pcTmp() failed\n\r");
                                    #endif
                                    #ifndef TMP_BASIC
                                    printf("\n\rERR-pcTMP : mdm_get_pcTmp() failed\n\r");
                                    #endif

                                }                                
                            }
                            else
                            {
                                #ifdef TMP_BASIC
                                dbg_print(Bold_Red,"\n\rERR-paTMP : mdm_get_paTmp() failed\n\r");
                                #endif
                                #ifndef TMP_BASIC
                                printf("\n\rERR-paTMP : mdm_get_paTmp() failed\n\r");
                                #endif

                            }                            
                        }
                        else
                        {
                            #ifdef TMP_BASIC
                            dbg_print(Bold_Red,"\n\rERR-CSQ : mdm_get_csq() failed\n\r");
                            #endif
                            #ifndef TMP_BASIC
                            printf("\n\rERR-CSQ : mdm_get_csq() failed\n\r");
                            #endif
                        }                        
                    }
                    else
                    {
                        #ifdef TMP_BASIC
                        dbg_print(Bold_Red,"\n\rERR-FMWV : mdm_get_fmwv() failed\n\r");
                        #endif
                        #ifndef TMP_BASIC
                        printf("\n\rERR-FMWV : mdm_get_fmwv() failed\n\r");
                        #endif
                    }                    
                }
                else
                {
                    #ifdef TMP_BASIC
                    dbg_print(Bold_Red,"\n\rERR-MFN : mdm_get_mfn() failed\n\r");
                    #endif
                    #ifndef TMP_BASIC
                    printf("\n\rERR-MFN : mdm_get_mfn() failed\n\r");
                    #endif
                }                
            }
            else
            {
                #ifdef TMP_BASIC
                dbg_print(Bold_Red,"\n\rERR-MODEL : mdm_get_model() failed\n\r");
                #endif
                #ifndef TMP_BASIC
                printf("\n\rERR-MODEL : mdm_get_model() failed\n\r");
                #endif
            }            
        }
        else
        {
            #ifdef TMP_BASIC
            dbg_print(Bold_Red,"\n\rERR-SN : mdm_get_sn() failed\n\r");
            #else
            printf("\n\rERR-SN : mdm_get_sn() failed\n\r");
            #endif
        }        
    }
    else
    {
        #ifdef TMP_BASIC
        dbg_print(Bold_Red,"\n\rERR-MDM-INIT : mdm_init() failed\n\r");
        #else
        printf("\n\rERR-MDM-INIT : mdm_init() failed\n\r");
        #endif
    }
    close(fd);
    return ret;
}

/*****************************
 * Pack type : Bootup {0/2}
 * Minimum size of the 
 *  str = 75
 *  fd : not required
 * **************************/
int pack_sys_uptime(int fd, char *str, uint32_t sz, uint32_t typ)
{
    UPTM_INFO_ UP_tm = {0};
    int ret;
    if(((typ&3)==0) || ((typ&3)==2))
    {
        if(sz < 75)
        {
            return -1;
        }
        ret = get_sys_uptm(&UP_tm);
        if(ret == 0)
        {
            memset(str,0,sz);//19+5+2
            sprintf(str,"SYS_UPTIME : uptime=%s;diff_time=%s;diff_day=%d;\n",\
                    UP_tm.tm_up, UP_tm.tm_dif, UP_tm.day);
        }
        ret = strlen(str);
    }
    else
    {
        ret = 0;
    }
    return ret;
}

/*********************************************
 * Pack type : Periodic {0/2}
 * Arguments : 
 *  size(str) : (25*4) + (5*4) = 120
 *  fd : not required
 * ******************************************/
int pack_eth_info(int fd, char *str, uint32_t sz, uint32_t typ)
{
    ETH_INFO_ eth_info[3] = {0};
    char tmp[70] = {0};
    int ret;
    if(((typ&3)==0) || ((typ&3)==2))
    {
        if(sz < 120)
        {
            return -1;
        }
        memset(str,0,sz);
        ret = get_eth_info(eth_info);
        if(ret != 0)
        {
            return -1;
        }
        for(int i=0;i<3;i++)
        {
            if(eth_info[i].IP == NULL)
            {
                return -1;
            }
            if(i==0)
            {
                if(eth_info[i].ecm == NULL)
                {
                    return -1;
                }
                sprintf(str,"IP_ecm%d : %s\n",i,eth_info[i].ecm);
            }
            sprintf(tmp,"IP_eth%d : %s\n",i,eth_info[i].IP);
            strcat(str,tmp);
            memset(tmp,0,strlen(tmp));
        }
        ret = strlen(str);
    }
    else
    {
        ret = 0;
    }
    
    return ret;
}

/**************************************
 * Pack type : Periodic {0/2}
 * Arguments : 
 *  size(str) = 70
 *  fd : Not required.
 * ***********************************/
int pack_meminfo(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret = 0;
    MEM_INFO_ MEM_DT;

    if(((typ&3)==0) || ((typ&3)==2))
    {
        if(sz < 70)
        {
            return -1;
        }
        ret = read_meminfo(&MEM_DT);
        if(ret == 0)
        {
            sprintf(str,"MEM_INFO : Total=%d; used=%f; cached=%f\n",\
                        MEM_DT.f_memtotal, MEM_DT.r_memused, MEM_DT.r_cached);
            ret = strlen(str);
        }
    }
    else
    {
        ret = 0;
    }
    
    return ret;
}

/*********************************************
 * Pack type : Periodic {0/2}
 * Arguments :
 *  size(str) = 30
 *  fd : not required;
 * Return :
 *  Error = -1
 *  Success = size(str)
 * ******************************************/
int pack_sys_time(int fd, char *str, uint32_t sz, uint32_t typ)
{
    char tmp[30]={0};
    int ret;
    if(((typ&3)==0) || ((typ&3)==2))
    {
        if(sz < 30)
        {
            return -1;
        }
        memset(str,0,sz);
        get_formated_time(tmp,2);
        sprintf(str,"RTC_TM : %s\n",tmp);
        ret = strlen(str);
    }
    else
    {
        ret = 0;
    }
    
    return ret;
}

/**********************************************
 * Pack type : Periodic {0/2}
 * Arguments :
 *  size(str) = 25
 *  fd : Required;
 * Return :
 *  Error = -1
 *  Success = size(str)
 * *******************************************/
int pack_sim_slot(int fd, char *str, uint32_t sz, uint32_t typ)
{
    char tmp[25] = {0};
    int ret;
    if(((typ&3)==0) || ((typ&3)==2))
    {
        if(sz<25)
        {
            return -1;
        }
        memset(str,0,25);
        ret = read_active_sim();
        if(ret < 0)
        {
            ret = -1;
        }
        else
        {
            sprintf(str,"ACTV_SIM_SLT : %d\n",ret);
            ret = strlen(str);
        }
    }
    else
    {
        ret = 0;
    }
    
    return ret;
}

int pkt_btup(char *str, const uint32_t sz, \
         Pack_func_ *pack_fun, const uint32_t sz_packFn, \
         stMSQ_DS_ *spMsq_DS)
{
    uint32_t BTP_SLT_STS = -1;
    int slt[2] = {0};
    int ret = 0, cnt = 0;;
    int tm_tkn = RTR_SIM_SWCH_TM;
    char tmp_str[500];
    int sim_sel = 0;

    uint8_t max_smRtry = 0;

    struct termios tty;
    int fd = mdm_init(&tty);
    dbg_print(Bold_Yellow,"PLD : Creating Bootup packet\n");
    if(fd < 0)
    {
        dbg_print(Bold_Red,"mdm_init() failed.\n");
        dbg_print(Bold_Red,"Returning... .. .\n");
        return -1;
    }

    cnt = 0;
    do
    {
        // ret = mdm_selSim(1,&tm_tkn);
        ret = mdm_selSim_ndt(spMsq_DS,1);
        if(ret != 1)
        {
            msleep(100);
            dbg_print(Bold_Red,"selSim(1) failed. Re-trying");
        }
        
    } while ((ret != 1) && (cnt++ < max_smRtry));
    
    cnt = 0;
    ret = 0;
    // slt[0]= mdm_get_sltSts();
    slt[0] = mdm_get_Slt_Sts(fd);
    dbg_print(NULL,"\nslot[0] = %d\n",slt[0]);
    memset(str,0,sz);
    sprintf(str,"\n<PKT_Bootup>\n");
    for(int i=0;i<sz_packFn;i++)
    {
        memset(tmp_str,0,strlen(tmp_str));
        ret = pack_fun[i].f_pack(fd,tmp_str,300,(PACK_TYP_BTUP));
        if(ret < 0)
        {
            dbg_print(Bold_Red,"Error in fun(%d)\n",i);
        }
        strcat(str,tmp_str);
    }
    cnt = 0;
    do
    {
        // ret = mdm_selSim(2,&tm_tkn);
        ret = mdm_selSim_ndt(spMsq_DS,2);
        if(ret != 2)
        {
            msleep(100);
            dbg_print(Bold_Red,"selSim(2) failed. Re-trying - %d\n",cnt);
        }
        
    } while ((ret != 2) && (cnt++ < max_smRtry));
    if(ret != 2)
    {
        dbg_print(Bold_Red,"mdm_selSim(2) failed\n");
        // dbg_print(Bold_Red,"Exiting... .. .\n");
        // close(fd);
        // return -1;
        sim_sel = -1;
    }
    ret = 0;
    cnt = 0;
    // slt[1] = mdm_get_sltSts();
    slt[1] = mdm_get_Slt_Sts(fd);
    printf("\nslot[1] = %d\n",slt[1]);
    for(int i=0;i<sz_packFn;i++)
    {
        memset(tmp_str,0,strlen(tmp_str));
        if(sim_sel == -1)
            ret = pack_fun[i].f_pack(fd,tmp_str,400,PACK_TYP_SIM); // Do not fill the values
        else
        {
            ret = pack_fun[i].f_pack(fd,tmp_str,400,PACK_TYP_SIM | 4); // FIll the values
        }
        
        if(ret < 0)
        {
            dbg_print(Bold_Red,"Error in fun(%d)\n",i);
        }
        strcat(str,tmp_str);
    }

    BTP_SLT_STS = (slt[1] << 1) | slt[0];
    memset(tmp_str,0,strlen(tmp_str));
    sprintf(tmp_str,"BTP_SLT_STS : %d\n",BTP_SLT_STS);
    strcat(str,tmp_str);
    strcat(str,"<END-Bootup>\n");
    
    cnt = 0;
    do
    {
        ret = mdm_selSim(1,&tm_tkn); // Required to switch the along with data.
        // ret = mdm_selSim_ndt(spMsq_DS,1);
        if(ret != 1)
        {
            msleep(100);
            dbg_print(Bold_Red,"TH-prd : selSim(1) failed. Retrying.\n");
        }
    } while ((ret != 1) && (cnt++ < max_smRtry));
    if(ret != 1)
    {
        dbg_print(Bold_Red,"mdm_selSim(1) failed\n");
        dbg_print(Bold_Red,"Exiting... .. .\n");
        close(fd);
        return -1;
    }
    ret = 0;
    close(fd);
    return (strlen(str));
}


int pkt_prd(char *str, const uint32_t sz_str, \
         Pack_func_ *pack_fun, const uint32_t sz_packFn)
{
    int ret = 0;
    char Tstr[500] = {0};
    struct termios tty;
    int fd = mdm_init(&tty);
    if(fd < 0)
    {
        dbg_print(Bold_Red,"mdm_init() failed.\n");
        dbg_print(Bold_Red,"Returning... .. .\n");
        return -1;
    }
    memset(str,0,sz_str);
    sprintf(str,"<PKT_Periodic>\n");
    printf("sz_packFn = %d\n",sz_packFn);
    for(int i=0;i<sz_packFn;i++)
    {
        ret = pack_fun[i].f_pack(fd,Tstr,300,PACK_TYP_PRD);
        strcat(str,Tstr);
    }
    sprintf(Tstr,"<End-pkt-periodic>\n");
    strcat(str,Tstr);
    close(fd);
    return (strlen(str));
}

/***********************************************
 * Pack type : Periodic {0/2}
 * Arguments : 
 *  fd : not required
 *  sz : min (25) 
 * Return : {-1:0}
 * ********************************************/
int pack_mdm_rstIntrvl(int fd, char *str, uint32_t sz, uint32_t typ)
{
    int ret;
    char tmp[50] = {0};
    memset(str,0,sz);
    if( ( (typ&3)==0) || ((typ&3)==2) )
    {
        if(sz < 25)
        {
            return -1;
        }
        memset(str,0,sz);
        ret = mdm_get_rstIntrvl(tmp,45);
        if(ret == 0)
        {
            sprintf(str,"RST_TIME : %s\n",tmp);
            ret = strlen(str);
        }
    }
    else
    {
        ret = 0;
    }
    // printf("--Returned from pack_get_rstIntrvl()---\n");
    return ret;
}
