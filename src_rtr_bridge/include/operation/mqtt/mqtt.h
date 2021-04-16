#ifndef MQTT__H
#define MQTT__H

#define MSQ_BROKER      "localhost"
#define MSQ_PORT        1883
#define MSQ_KEEPALIVE   500

#define TPQ_SIMPWR_RQST     "SIMPOWEROFFON"
#define TPQ_SIMPWR_MSGon    "<,1,SIMPOWERON,>"
#define TPQ_SIMPWR_MSGof    "<,1,SIMPOWEROFF,>"

#define TPQ_IPKINFO_NO      1
#define TPQ_IPKINFO_RQST    "Isensev2/IPKInfo/CommandRequest"
#define TPQ_IPKINFO_RESP    "Isensev2/IPKInfo/CommandResponse"
#define TPQ_IPKINFO_MSG     "<,IPKVERSIONINFO,>"

#define TPQ_UPGINST_NO      2
#define TPQ_UPGINST_RQST    "Isensev2/AutoUpgrade/CommandRequest"
#define TPQ_UPGINST_RESP    "Isensev2/AutoUpgrade/CommandResponse"
#define TPQ_UPGINST_MSG     "<,UPGRADE,>"

#define TPQ_UPGPRD_NO       3
#define TPQ_UPGPRD_RQST     "Isensev2/HTTPUpdateServerUrlConfigFile/CommandRequest"

// Periodic 

#define TPQ_UPGSRV_CFG_NO      4
#define TPQ_UPGSRV_CFG_RQST "Isensev2/AutoUpgradeUpdateServerConfig/CommandRequest"
#define TPQ_UPGSRV_CFG_RESP "Isensev2/AutoUpgradeUpdateServerConfig/CommandResponse"
#define TPQ_UPGSRV_CFG_MSG_MX   500
#define TPQ_UPGSRV_CFG_M1       "UPDATESERVERDETAILS"
//--- The message for the above topic will be collected from the user.

#define SZ_MQTT_TPQ 100
#define SZ_MQTT_MSG 500
typedef struct stMSQ_Uobj__
{
    char caTpq[SZ_MQTT_TPQ];
    char caMsg[SZ_MQTT_MSG];
    int iTpq_no;
    int iSzErr; // { bit(0) : caTpq } { bit(1) : caMsg}
    /*
        iSzErr defines wheather the topic / payload size 
        provided by the broker is greater that the 
        SZ_MQTT_TPQ / SZ_MQTT_MSG respectively.
    */
    
}stMSQ_Uobj_;

typedef struct stMSQ_DS__
{
    stMSQ_Uobj_ *spMSQ_Uobj;
    char *cpTpq_pub;
    char *cpTpq_sub;
    char *cpMsg;
    struct mosquitto *stpMsq_instns;
    int iCur_tpqNo;
}stMSQ_DS_;

typedef struct mosquitto msq_ins_;
typedef struct mosquitto_message stMsq_msg_;

typedef struct stMSQ_calbak__
{
    void (*mqtt_cbk_pub)(struct mosquitto *, void *, int);
    void (*mqtt_cbk_sub)(struct mosquitto *, void *, int, int, const int *);
    void (*mqtt_cbk_msg)(struct mosquitto *, void *, const stMsq_msg_ *);
}stMSQ_calbak_;

void mqtt_cleanup (struct mosquitto * pMosq);
void mqtt_printErr(int en, const char *str);
void mqtt_calBak_sub(struct mosquitto *pMoq, void *usr_obj, int mid, int qos, const int *granted_qos);
void mqtt_calBak_pub(struct mosquitto *pMosq, void *pbj, int mid);
void mqtt_calBak_msg(struct mosquitto *mosq, void *usr_obj , const stMsq_msg_ *message);

int mqtt_init(msq_ins_ **spMos_instns, stMSQ_Uobj_ *stMSQ_Uobj, stMSQ_calbak_ *spMSQ_calbak);
int mqtt_sub_pub(stMSQ_DS_ *stpMSQ_DS, uint32_t iTm_out);

#endif