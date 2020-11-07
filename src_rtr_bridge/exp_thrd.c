#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>

#include <time.h>

static sem_t sem;
static char buf[100]={0};

void get_time(char *str)
{
    time_t tm = time(NULL);
    memset(str,0,strlen(str));
    sprintf(str,"%s",ctime(&tm));
}

void* th1_producer(void *a)
{
    pthread_t th_id = pthread_self();
    while(1)
    {
        printf("\n\rTH-1 : thread-1_id : %ld\n\r",th_id);
        
        //--------critical section--------
        sem_wait(&sem);
        get_time(buf);
        sem_post(&sem);
        //--------xxxxxxxxxxxxxxxx--------

        printf("\nTH-1 : data has been produced\n");
        sleep(2);
    }
}

void* th2_consumer(void *a)
{
    
    pthread_t th_id = pthread_self();
    while(1)
    {
        printf("\n\rTH-2 : thread-2_id : %ld\n\r",th_id);
        
        //--------critical section--------
        sem_wait(&sem);
        printf("\nTH-2 : Time from TH-1 = %s\n",buf);
        sem_post(&sem);

        printf("\n\rTH-2 : Data has collected from th-1 here.\n\r");
        sleep(2);
    }
    //while(1);

}

int main(void)
{
    if(sem_init(&sem,0,1) < 0)
    {
        perror("ERR : sem_init() failed due to ");
        return (0);
    }
    printf("Creating producer and consumer threads.\n\r");
    pthread_t   th_id[2] = {0};
    int ret = 0;
    ret = pthread_create(&th_id[0],NULL,&th1_producer,NULL);
    if(ret < 0)
    {
        perror("ERR-THD : pthread_create(1) failed due to ");
        return 0;
    }
    ret = pthread_create(&th_id[0],NULL,&th2_consumer,NULL);
    if(ret <0)
    {
        perror("ERR-THD : pthread_create(2) failed due to ");
        return 0;
    }
    printf("\n\rThread crearted successfully\n\r");
    int rc = 0;
    void *status;
    for(int i=0;i<2;i++)
    {
        rc = pthread_join(th_id[i], &status);
        if (rc) {
            printf("ERROR; return code from pthread_join() is %d\n", rc);
            pthread_exit(NULL);
        }
        printf("Main: completed join with thread %d having a status of %ld\n",i,(long)status);

    }
    
    while(1)
    {
        printf("\n\rEnd of the main()\n\r");
        sleep(2);
    }
    return 0;
}