#ifndef DATA_STRUCT__H
#define DATA_STRUCT__H

#define SZ_IP 20
#define PRS_TYP_BTP 1
#define PRS_TYP_PRD 2

#define CMD_SNT         1
#define CMD_SNT_PRCS    2
#define CMD_SNT_SUCS    3

#define CMD_RQ  1
#define SZ_CMD_SND  1024
#define SZ_CMD_RSP  512

typedef struct sVERSN__
{
    char cVER_MDM_FMW [60];
    char cVER_RTR_FMW [40];
    char cVER_APP_RLS [10];
    char cVER_APP_INV [10];
}sVERSIONS_;

typedef struct sSIM_INFO__
{
    char cIMSI[25];
    char cCCID[25];
    char cSpn[15];
    char cRAT[15];//network type
    int iData_usage;// (DATA_USAGE : ---)
}sSIM_INFO_;

typedef struct sIP__
{
    char cIP_ecm0[20];
    char cIP_eth0[20];
    char cIP_eth1[20];
    char cIP_eth2[20];
}sIP_;

typedef struct sSYS_UPTIME__
{
    char cUpdate[15];
    char cUptime[10];
    char cDiff_time[5];
    int iDiff_day;
}sSYS_UPTIME_;

typedef struct sMEM_INFO__
{
    int iTotal;
    float fUsed;
    float fCached;
}sMEM_INFO_;


typedef struct sRST_INTVL__
{
    int idom; //day of month
    int imonth; 
    int ihrs;
    int iminute;
}sRST_INTVL_;



/*****************************************
 * This structure will be filled periodically
 * **************************************/
typedef struct sVEGA_PRD__
{
    char cMdm_sn[25];
    char cMdm_mdl[15];
    char cMdm_mfn[35];
    int cCsq [2];
    float fTemp[2];
    sVERSIONS_ sVersion;
    sSIM_INFO_ sSim_info[2];
    sSYS_UPTIME_ sSys_uptime;
    int iNet_sts;
    int iPng_sts;
    char cRtc_tm[25];  
    int iActv_sim_slt;  //
    sMEM_INFO_ sMem_info;   //
    char cUp_tm[15];   //
    sIP_ ip;   //
    sRST_INTVL_ sRst_Time;
    int iBtp_Slt_sts;
    int iActv_Slt_sts;
    /*
        {0<no sim>, 1<sim-1>, 2<sim-2>, 3<both sims>}
    */
}sVEGA_PRD_;

typedef struct sVEGA_CMD__
{
    char cCmd_rsp[SZ_CMD_RSP];
    char cCmd_snd[SZ_CMD_SND]; 
    int iFlg_snd; //{ 0,1 }
    int iFlg_rsp; //{ 0,1 }
}sVEGA_CMD_;

/******************************
 * The folowing thread will be the argument 
 * of the instantaneous thread.
 * ***************************/
typedef struct sTH_ARG_CMD__
{
    char cTh_ip[SZ_IP]; //V2 WR_ONLY
    int cTh_port; //V2 WR_ONLY
    // int iTh_bsy; //rtr WR_ONLY < Same can be achived by {sem_t}>
    sem_t *pth_Inst_Sem; //V2,rtr RD_WR <critical>
    sVEGA_CMD_ sVEGA_CMD; //V2,rtr RD_WR <critical>
}sTH_ARG_CMD_;

typedef struct sRtr_thd__
{
    void*(*func_thd)(void*);
}sRtr_thd_;

/***********************
 * The following structure will be
 * the argument of the periodic thread
 * ********************/
typedef struct sTH_ARG_PRD__
{
    char cTh_ip[SZ_IP]; //V2 WR_ONLY
    int cTh_port; //V2 WR_ONLY
    // int iTh_bsy; //rtr WR_ONLY < Same can be achived by {sem_t}>
    sem_t *pth_Prd_Sem; //V2,rtr RD_WR <critical>
    sVEGA_PRD_ VEGA_PRD; //V2,rtr RD_WR <critical>
}sTH_ARG_PRD_;

typedef struct sThreadARG
{
	sTH_ARG_PRD_ 	psPRDArg;
	sTH_ARG_CMD_ 	psCMDArg;
}sThreadARG;


#endif