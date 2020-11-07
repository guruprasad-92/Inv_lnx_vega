#ifndef PARSER_MDM__H
#define PARSER_MDM__H

#include "rtr_dtStruct.h"

//all the structru and function definetion
#define SZ_IP 20
#define PRS_TYP_BTP 1
#define PRS_TYP_PRD 2
#define BTP_PRM_CNT 14 // Total numbers of bootup parameters
#define RQ_PRD      "RQST_PRD"
#define RQ_BTP      "RQST_BTP"
#define PKT_BTP     "<PKT_Bootup>"
#define PKT_PRD     "<PKT_Periodic>"



/******************************
 * The folowing thread will be the argument 
 * of the instantaneous thread.
 * ***************************/
// typedef struct sTH_ARG_CMD__
// {
//     char cCmd_rsp[1024];
//     char cCmd_snd[100]; 
//     uint32_t iFlg_snd; //{ 0,1 }
//     uint32_t iFlg_rsp; //{ 0,1 }
// }sTH_ARG_AMD_;

// typedef struct sVERSN__
// {
//     char cVER_MDM_FMW [60];
//     char cVER_RTR_FMW [40];
//     char cVER_APP_RLS [10];
//     char cVER_APP_INV [10];
// }sVERSIONS_;

// typedef struct sSIM_INFO__
// {
//     char cIMSI[25];
//     char cCCID[25];
//     char cSpn[15];
//     char cRAT[15];//network type
//     int iData_usage;// (DATA_USAGE : ---)
// }sSIM_INFO_;

// typedef struct sIP__
// {
//     char cIP_ecm0[20];
//     char cIP_eth0[20];
//     char cIP_eth1[20];
//     char cIP_eth2[20];
// }sIP_;

// typedef struct sSYS_UPTIME__
// {
//     char cUpdate[15];
//     char cUptime[10];
//     char cDiff_time[5];
//     int iDiff_day;
// }sSYS_UPTIME_;

// typedef struct sMEM_INFO__
// {
//     int iTotal;
//     float fUsed;
//     float fCached;
// }sMEM_INFO_;


// /*****************************************
//  * This structure will be filled periodically
//  * **************************************/
// typedef struct sVEGA_PRD__
// {
// 	char cMdm_sn[25];
// 	char cMdm_mdl[15];
// 	char cMdm_mfn[35];
// 	int cCsq [2];
//     float fTemp[2];
// 	sVERSIONS_ sVersion;
//     sSIM_INFO_ sSim_info[2];
//     sSYS_UPTIME_ sSys_uptime;
// 	int iNet_sts;
//     int iPng_sts;
// 	char cRtc_tm[25];  
//     int iActv_sim_slt;  //
// 	sMEM_INFO_ sMem_info;   //
// 	char cUp_tm[15];   //
// 	sIP_ ip;   //
//     int iRst_interval;//
//     int iBtp_Slt_sts;
//     int iActv_Slt_sts;
//     /*
//         {0<no sim>, 1<sim-1>, 2<sim-2>, 3<both sims>}
//     */
// }sVEGA_PRD_;

// /***********************
//  * The following structure will be
//  * the argument of the periodic thread
//  * ********************/
// typedef struct sTH_ARG_PRD__
// {
//     char cTh_ip[SZ_IP];
//     int cTh_port;
//     sem_t *th_sem;
//     sVEGA_PRD_ *VEGA_PRD;
// }sTH_ARG_PRD;



typedef struct Parse_func__
{
    sVEGA_PRD_ obj;
    int(*f_prs_mdm)(char *[],int,sVEGA_PRD_ *,uint32_t typ);
}Parse_func_;

int prs_sim_rat(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);

int prs_mdm_sn_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdm_model_data(char *dest[], int sz, sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdm_mfn_data(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdmFMW_ver(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_rtrFMW_ver(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_appRLS_ver(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_appINV_ver(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdm_IP_ecm0_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdm_IP_eth0_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdm_IP_eth1_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdm_IP_eth2_data(char *dest[],int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdm_sys_uptime(char *dest[],int sz, sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdm_csq_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdm_memInfo_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdm_temp_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int read_SIM_IMSI2(char *dest[], char *imsi_data,int sz,int sim);
int read_SIM_CCID2(char *dest[], char *ccid_data,int sz,int sim);
int read_SIM_SPN2(char *dest[], char *spn_data,int sz,int sim);
int read_SIM_RAT2(char *dest[], char *rat_data,int sz,int sim);
int prs_sim_rat1(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_sim_rat2(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_sim_imsi1(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_sim_imsi2(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_sim_ccid1(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_sim_ccid2(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_sim_spn1(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_sim_spn2(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int read_data_usage(char *dest[],uint32_t *data,int sz);
int prs_SIM_INFO_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdm_RTC_TM_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdm_netSts_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_Btp_Slt_Sts_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_Actv_Slt_Sts_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_rstIntrvl_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdm_activeSim_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);
int prs_mdm_pngSts_data(char *dest[], int sz,sVEGA_PRD_ *obj,uint32_t typ);


uint32_t prs_mdm_init(Parse_func_ *parse_func);
void print_periodic_rcrd(sVEGA_PRD_ *sPrd);
void print_btp_rcrd(sVEGA_PRD_ *sBtp);

int get_btp_rcrd(char *lns[], int sz_lns,int sz_func,\
            sVEGA_PRD_ *obj,Parse_func_ *parse_func, uint32_t pkt_typ);

int get_prd_rcrd(char *lns[], int sz_lns,int sz_func,\
            sVEGA_PRD_ *obj,Parse_func_ *parse_func, uint32_t pkt_typ);

/***************************
 * Detect the packet type
 * ************************/
int read_pkt_typ(char *pkt);
#endif
