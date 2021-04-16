#include <mosquitto.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BROKER      "localhost"
#define PORT        1883
#define KEEPALIVE   5

#define TPK "Isensev2/IPKInfo/CommandRequest"
#define TPKR "Isensev2/IPKInfo/CommandResponse"
#define MSG "<,IPKVERSIONINFO,>"

void message_callback(struct mosquitto * mosq, void * obj , const struct mosquitto_message * message);
void publish_callback(struct mosquitto * mosq, void * pbj, int mid);
void subscribe_callback(struct mosquitto * mosq, void * obj, int mid, int qos, const int * granted_qos);
void cleanUp (struct mosquitto * pMosq);
void printError (int ec);

typedef struct stMQTT_TPQ__
{
    uint8_t topic;
    char message[200];
}stMQTT_TPQ_;


int main(void)
{
    struct mosquitto *m_pMqtt;
    int ret = 0;
    mosquitto_lib_init();

    stMQTT_TPQ_ stMQTT_TPQ[5];

    m_pMqtt = mosquitto_new(NULL, true, stMQTT_TPQ);
    if(m_pMqtt == NULL)
    {
        fprintf(stderr, "Failed to allocate mosquitto instance\r\n");
        mosquitto_lib_cleanup();
        printf("Returning ... .. .\n");
        return -1;
    }

    mosquitto_publish_callback_set(m_pMqtt, publish_callback);
    mosquitto_subscribe_callback_set(m_pMqtt, subscribe_callback);
    mosquitto_message_callback_set(m_pMqtt, message_callback);

    ret = mosquitto_connect(m_pMqtt, BROKER, PORT, KEEPALIVE);
    if(ret != MOSQ_ERR_SUCCESS)
    {
        printError(ret);
        cleanUp(m_pMqtt);
        return -1;
    }

    ret = mosquitto_loop_start(m_pMqtt);
    if(ret != MOSQ_ERR_SUCCESS)
    {
        printError(ret);
        cleanUp(m_pMqtt);
        return -1;
    }

    ret = mosquitto_subscribe(m_pMqtt,NULL,TPKR,1);
    if(ret != MOSQ_ERR_SUCCESS)
    {
        printError(ret);
        cleanUp(m_pMqtt);
        return -1;
    }

    // sleep(1);

    ret = mosquitto_publish(m_pMqtt,NULL,TPK,strlen(MSG),MSG,1,false);
    if(ret != MOSQ_ERR_SUCCESS)
    {
        printError(ret);
        cleanUp(m_pMqtt);
        return -1;
    }
    int i=0;
    do
    {
        i += 1;
        usleep(5*1000);
    } while ( (stMQTT_TPQ[0].topic==0) && (i<=100));
    

    ret = mosquitto_loop_stop(m_pMqtt, true);
    if (ret != MOSQ_ERR_SUCCESS)
    {
        printError(ret);
        cleanUp(m_pMqtt);
        return -1;
    }

    cleanUp(m_pMqtt);
}


//------------------------------------------

void message_callback(struct mosquitto * mosq, void * obj , const struct mosquitto_message * message)
{
    stMQTT_TPQ_ *stMQTT_TPQ = (stMQTT_TPQ_*)obj;
    printf("\r\nMessage Callback\r\n");
    printf("\tTopic: %s\r\n", message->topic);
    stMQTT_TPQ[0].topic = 1;
    if(strcmp((char*)message->topic,TPKR)==0)
    {
        memset(stMQTT_TPQ[0].message,0,strlen(stMQTT_TPQ[0].message));
        sprintf(stMQTT_TPQ[0].message,"%s",message->topic);
    }
    /* Assume the payload is a string in this example */
    printf("\tPayload: %s\r\n", (char *)message->payload);
}

void publish_callback(struct mosquitto * mosq, void * pbj, int mid)
{
    printf("\r\nPublish Callback\r\n");
}

void subscribe_callback(struct mosquitto * mosq, void * obj, int mid, int qos, const int * granted_qos)
{
    printf("\r\nSubscribe Callback\r\n");
}



void printError (int ec)
{
    fprintf(stderr, "ERROR: %s\r\n", mosquitto_strerror(ec));
}

void cleanUp (struct mosquitto * pMosq)
{
    /* destroy allocated object */
    mosquitto_destroy(pMosq);
    
    /* cleanup library */
    mosquitto_lib_cleanup();
}
