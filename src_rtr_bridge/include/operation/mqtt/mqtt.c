#include <mosquitto.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include "mqtt.h"
#include "basic/basic.h"

void mqtt_cleanup (struct mosquitto * pMosq)
{
    mosquitto_destroy(pMosq);
    mosquitto_lib_cleanup();
}

void mqtt_printErr(int en, const char *str)
{
    dbg_print(Bold_Red,"ERR-MQTT : %s %s\n",\
        str, mosquitto_strerror(en));
}

void mqtt_calBak_sub(struct mosquitto *pMoq, void *usr_obj, int mid, int qos, const int * granted_qos)
{
    dbg_print(NULL,"DBG-MQTT : Subscribe call_back()\n");
}

void mqtt_calBak_pub(struct mosquitto *pMosq, void * pbj, int mid)
{
    dbg_print(NULL,"DBG-MQTT : Publish Call_back()\n");
}

void mqtt_calBak_msg(struct mosquitto *mosq, void *usr_obj , const stMsq_msg_ *message)
{
    stMSQ_Uobj_ *stMSQ_Uobj = (stMSQ_Uobj_*)usr_obj;
    size_t  sz_tpq = strlen(message->topic);
    memset(stMSQ_Uobj->caMsg,0,strlen(stMSQ_Uobj->caMsg));
    memset(stMSQ_Uobj->caTpq,0,strlen(stMSQ_Uobj->caTpq));
    if(message->payloadlen <= SZ_MQTT_MSG)
    {
        strncpy(stMSQ_Uobj->caMsg,message->payload,message->payloadlen);
        stMSQ_Uobj->iSzErr &= ~2;
    }
    else
    {
        dbg_print(Bold_Red, \
            "ERR-MQTT : size of payload(%s) exceeds the boundary.\n",stMSQ_Uobj->caMsg);
        stMSQ_Uobj->iSzErr |= 2;
    }

    if(sz_tpq <= SZ_MQTT_TPQ)
    {
        strncpy(stMSQ_Uobj->caTpq,message->topic,sz_tpq);
        stMSQ_Uobj->iSzErr &= ~1;
    }
    else
    {
        dbg_print(Bold_Red,\
            "ERR-MQTT : size of topic(%s) exceeds the boundary.\n",stMSQ_Uobj->caTpq);
        stMSQ_Uobj->iSzErr |= 1;
    }


    stMSQ_Uobj->iTpq_no = -1;
    if(strcmp((char*)message->topic,TPQ_IPKINFO_RESP) == 0)
    {
        stMSQ_Uobj->iTpq_no = TPQ_IPKINFO_NO;
    }
    else if(strcmp((char*)message->topic,TPQ_UPGINST_RESP) == 0)
    {
        stMSQ_Uobj->iTpq_no = TPQ_UPGINST_NO;
    }
    else if (strcmp((char*)message->topic,TPQ_UPGSRV_CFG_RESP) == 0)
    {
        stMSQ_Uobj->iTpq_no = TPQ_UPGSRV_CFG_NO;
    }
    printf("---mqtt_calBak_msg()----\n");
}


int mqtt_init(msq_ins_ **spMos_instns, stMSQ_Uobj_ *stMSQ_Uobj, stMSQ_calbak_ *spMSQ_calbak)
{
    int ret = 0;
    mosquitto_lib_init();

    *spMos_instns = mosquitto_new(NULL, true, stMSQ_Uobj);
    if(*spMos_instns == NULL)
    {
        dbg_print(Bold_Red,\
            "mosquitto_new() failed due to : %s\n",strerror(errno));
        mqtt_cleanup(*spMos_instns);
        dbg_print(Bold_Red,"Returning from(mqtt_init())...\n");
        return -1;
    }

    mosquitto_publish_callback_set(*spMos_instns,spMSQ_calbak->mqtt_cbk_pub);
    mosquitto_subscribe_callback_set(*spMos_instns,spMSQ_calbak->mqtt_cbk_sub);
    mosquitto_message_callback_set(*spMos_instns,spMSQ_calbak->mqtt_cbk_msg);

    ret = mosquitto_connect(*spMos_instns, MSQ_BROKER, MSQ_PORT, MSQ_KEEPALIVE);
    if(ret != MOSQ_ERR_SUCCESS)
    {
        mqtt_printErr(ret,"mosquitto_connect() failed due to :");
        mqtt_cleanup(*spMos_instns);
        dbg_print(Bold_Red,"Returning from mqtt_init()... .. .\n");
        return -1;
    }
    ret = mosquitto_loop_start(*spMos_instns);
    if(ret != MOSQ_ERR_SUCCESS)
    {
        mqtt_printErr(ret,"mosquitto_loop_start() failed due to :");
        mqtt_cleanup(*spMos_instns);
        dbg_print(Bold_Red,"Returning... .. .\n");
        return -1;
    }
    return 0;
}

/*******************************************************
 * Description : 
 * subcribe and publish mqtt topics. Wait for the response 
 * of the broker upto iTmout(in seconds).
 *
 * Return :
 *      Success = 0
 *      error = -1
 * If subscription topic is NULL return -1;
 * 
 * ****************************************************/
int mqtt_sub_pub(stMSQ_DS_ *stpMSQ_DS, uint32_t iTm_out)
{
    int ret = 0;
    if(stpMSQ_DS->cpTpq_sub != NULL)
    {
        if(stpMSQ_DS->stpMsq_instns == NULL)
        {
            dbg_print(NULL,"stpMSQ_DS->stpMsq_instns is null\n");
        }
        dbg_print(NULL,"instanse : %p, sub = %p",stpMSQ_DS->stpMsq_instns, stpMSQ_DS->cpTpq_sub);
        ret = mosquitto_subscribe(stpMSQ_DS->stpMsq_instns,NULL,\
                            stpMSQ_DS->cpTpq_sub,1);
        if(ret != MOSQ_ERR_SUCCESS)
        {
            mqtt_printErr(ret,"In mqtt_sub_pub : mosquitto_subscribe() failed due to : ");
            mqtt_cleanup(stpMSQ_DS->stpMsq_instns);
            dbg_print(Bold_Red,"ERR : Returning from mqtt_sub_pub()... .. .\n");
            return -1;
        }
        
        ret = mosquitto_publish(stpMSQ_DS->stpMsq_instns,NULL,\
                stpMSQ_DS->cpTpq_pub,strlen(stpMSQ_DS->cpMsg),\
                stpMSQ_DS->cpMsg,1,false);
        if(ret != MOSQ_ERR_SUCCESS)
        {
            mqtt_printErr(ret,"In mqtt_sub_pub : mosquitto_publish() failed due to :");
            mqtt_cleanup(stpMSQ_DS->stpMsq_instns);
            dbg_print(Bold_Red,"Err : Returning from mqtt_sub_pub()\n");
            return -1;
        }

        iTm_out *= 1000; // In mili sec
        uint32_t iTpq_chk = 0;
        do
        {
            // dbg_print(NULL,"stpMSQ_DS->spMSQ_Uobj->iTpq_no = %d\n",stpMSQ_DS->spMSQ_Uobj->iTpq_no);
            // dbg_print(NULL,"stpMSQ_DS->iCur_tpqNo = %d\n",stpMSQ_DS->iCur_tpqNo);
            if(stpMSQ_DS->spMSQ_Uobj->iTpq_no != \
                stpMSQ_DS->iCur_tpqNo)
            {
                msleep(50);
                iTm_out -= 50;
            }
            else
            {
                iTpq_chk = 1;
            }

        } while ( (iTpq_chk != 1) && (iTm_out > 0) );
        printf("iTm_out = %d\n",3000-iTm_out);
        
        stpMSQ_DS->spMSQ_Uobj->iTpq_no = -1; // RFeverting back to use further.
        if(iTpq_chk == 1)
        {
            ret = 0; // Response collected.
        }
        ret = mosquitto_unsubscribe(stpMSQ_DS->stpMsq_instns, \
                                NULL, stpMSQ_DS->cpTpq_sub);
        if(ret != MOSQ_ERR_SUCCESS)
        {
            mqtt_printErr(ret,"In mqtt_sub_pub() : mosquitto_unsubscribe() failed due to :");
            dbg_print(Bold_Red,"ERR : Returning from mqtt_sub_pub()... .. .\n");
            ret = -1;
            // No need to destroy the object here.
        }
    }
    else
    {
        ret = -1;
    }
    return ret;
}