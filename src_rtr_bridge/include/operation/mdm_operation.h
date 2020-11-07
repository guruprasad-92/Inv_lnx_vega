#ifndef MDM_OPERATION__H
#define MDM_OPERATION__F

#define RTR_MDL "AT+GMM\r\0"
#define RTR_MFN "AT+GMI\r\0"
#define RTR_SN  "AT+GSN\r\0"
#define RTR_FWV "AT+GMR\r\0"
#define RTR_CSQ "AT+CSQ\r\0"
#define RTR_PA_TMP  "AT!PATEMP?\r\0"
#define RTR_PC_TMP  "AT!PCTEMP?\r\0"

#define RTR_SIM_SPN     "AT+CSPN\r\0"
#define RTR_SIM_CCID    "AT+CCID\r\0"
#define RTR_SIM_IMSI    "AT+CIMI\r\0"
#define RTR_GET_RAT     "AT!GETRAT?\r\0"

#define RTR_SIM_SWCH_TM     240

typedef struct termios Termios;

typedef struct FMW_VER__
{
	char v_fs[50];
	char v_krn[15];
    char v_mdm_fmw[70];
	char v_app_rls[10];
    char v_app_inv[10];
}FMW_VER_;


int read_active_sim(void);
int mdm_selSim(uint32_t sim, uint32_t *tm_tkn);

/***********************************************
 * 
 * mdm_init() : open "/dev/ttyAT" and set a 
 * proper termios struct.
 * 
 * ARG : termios struct pointer
 * RET : {+ve file descriptor on success;
 *          -1 on error}
 * *******************************************/
int mdm_init(Termios *tty);
int mdm_get_model(int fd, char *mdl);

/*****************************************
 * mdm_get_mfn() : get manufacturer name
 * return : success = 0
 *          error = -1
 * *************************************/
int mdm_get_mfn(int fd, char *mfn);
int mdm_get_fmwv(int fd, char *fmwv);
int mdm_get_sn(int fd, char *sn);
int mdm_get_csq(int fd, char *csq);
// int str2numstr(char *str, char *numstr[], int len);
int mdm_get_paTmp(int fd, char *paTmp);
int mdm_get_pcTmp(int fd, char *pcTmp);
int mdm_pld_btup(char *pld, char *pld2, uint32_t sz_max, char *dlm);
int mdm_rsp(int fd, char *cmd, char *rsp, uint32_t sz_rsp, uint64_t tm_out);
int mdm_get_ccid(int fd, char *ccid);
int mdm_get_spn(int fd, char *spn);
int mdm_get_imsi(int fd, char *imsi);
int mdm_get_netSts(void);
int mdm_get_rat(int fd, char *rat, uint32_t sz_rat);

int rsp_popen(const char *cmd, char *rsp, \
            char *rsp_ln[], uint32_t ln_max, uint32_t sz_rsp);


int fd_ctrl(int fd, int ctrl, Termios *tty);
int get_fmw_ver(int fd, FMW_VER_ *ver);
int mdm_get_sltSts(void);
#endif