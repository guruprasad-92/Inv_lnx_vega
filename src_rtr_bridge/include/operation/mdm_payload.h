#ifndef MDM_PAYLOAD__H
#define MDM_PAYLOAD__H

#define PACK_TYP_BTUP   1 | 4

 // Recreate the bootup packet
#define PACK_TYP_BTUP2  4
#define PACK_TYP_PRD   2 | 4
#define PACK_TYP_ALL    0
#define PACK_TYP_SIM    3


typedef struct Pack_func__
{
    int(*f_pack)(int,char*,uint32_t,uint32_t);
    uint32_t sz;
}Pack_func_;

typedef struct st_pkFunc__
{
    Pack_func_ Spack_fun[20];
    uint32_t sz_btup;
    uint32_t sz_prd;
}st_pkFunc_;

/*********************************************
 *  Arguments:
 *      Size(str) : 25
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_sn(int fd, char *str, uint32_t sz, uint32_t typ);

/*********************************************
 *  Arguments:
 *      Size(str) : 25
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_model(int fd, char *str, uint32_t sz, uint32_t typ);

/*********************************************
 *  Arguments:
 *      Size(str) : 30
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_mfn(int fd, char *str, uint32_t sz, uint32_t typ);

/*********************************************
 *  Arguments:
 *      Size(str) : 60
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_fmwv(int fd, char *str, uint32_t sz, uint32_t typ);

/*********************************************
 *  Arguments:
 *      Size(str) : 25
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_csq(int fd, char *str, uint32_t sz, uint32_t typ);

/*********************************************
 *  Arguments:
 *      Size(str) : 25
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_paTmp(int fd, char *str, uint32_t sz, uint32_t typ);

/*********************************************
 *  Arguments:
 *      Size(str) : 10
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_pcTmp(int fd, char *str, uint32_t sz, uint32_t typ);

/*********************************************
 *  Arguments:
 *      Size(str) : 20
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_imsi(int fd, char *str, uint32_t sz, uint32_t typ);

/*********************************************
 *  Arguments:
 *      Size(str) : 25
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_ccid(int fd, char *str, uint32_t sz, uint32_t typ);

/*********************************************
 *  Arguments:
 *      Size(str) : 10
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_spn(int fd, char *str, uint32_t sz, uint32_t typ);

/*********************************************
 *  Arguments:
 *      Size(str) : 15
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_netSts(int fd, char *str, uint32_t sz, uint32_t typ);

/*********************************************
 *  Arguments:
 *      Size(str) : 15
 *      fd : Required {File Descriptor}
 * ******************************************/
int pack_mdm_pngSts(int fd, char *str, uint32_t sz, uint32_t typ);

int pack_sys_uptime(int fd, char *str, uint32_t sz, uint32_t typ);
int pack_eth_info(int fd, char *str, uint32_t sz, uint32_t typ);
int pack_sys_time(int fd, char *str, uint32_t sz, uint32_t typ);
int pack_meminfo(int fd, char *str, uint32_t sz, uint32_t typ);
int pack_rtr_fmwv(int fd, char *str, uint32_t sz, uint32_t typ);
uint32_t pack_func_init(Pack_func_ *pack_func, uint32_t *sz);

int mdm_pld_exp(char *pld, uint32_t sz_max, \
            Pack_func_ *pack_func, uint32_t sz_pkFun);

int pack_sim_slot(int fd, char *str, uint32_t sz, uint32_t typ);
int pack_mdm_rat(int fd, char *str, uint32_t sz, uint32_t typ);
int pack_mdm_sltSts(int fd, char *str, uint32_t sz_str, uint32_t typ);

int pkt_btup(char *str, const uint32_t sz, \
         Pack_func_ *pack_fun, const uint32_t sz_packFn);

int pkt_prd(char *str, const uint32_t sz_str, \
         Pack_func_ *pack_fun, const uint32_t sz_packFn);

#endif