#include <stdio.h>
#include <string.h>
#include <time.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define ACTV_SIM_STS "/reap/etc/config/ActiveSIMStatus"
#define DMSG    "/dev/kmsg"
struct stat f_atribute = {0};

void wrt2dmsg(const char *preStr, char *asim, struct tm *tmv)
{
    int fd = 0;
    char caKlog[200] = {0};
    fd = open(DMSG,O_RDONLY);
    
    sprintf(caKlog,"ACTV_SIM : %s\n",asim);
    write(fd,caKlog,strlen(caKlog));
    memset(caKlog,0,sizeof(caKlog));

    tmv->tm_year += 1900;
    sprintf(caKlog,"%s = %02d/%02d/%04d %02d:%02d:%02d\n",preStr,tmv->tm_mday,\
            tmv->tm_mon, tmv->tm_year, tmv->tm_hour, tmv->tm_min, tmv->tm_sec);
    write(fd,caKlog,strlen(caKlog));
    close(fd);
}

int main(void)
{
    int fd=0,fd_klog=0;
    char caRd[5] = {0};
    char caKlog[200] = {0};
    struct tm tmv = {0};

    fd = open(ACTV_SIM_STS,O_RDWR);
    write(fd,"2",1);
    close(fd);
    for(int i=0;i<100;i++)
    {
        
        fd = open(ACTV_SIM_STS,O_RDONLY);
        memset(caRd,0,sizeof(caRd));
        read(fd,caRd,1);
        close(fd);

        stat(ACTV_SIM_STS,&f_atribute);
        localtime_r(&(f_atribute.st_ctime),&tmv);
        wrt2dmsg("C_TIME",caRd,&tmv);
        localtime_r(&(f_atribute.st_atime),&tmv);
        wrt2dmsg("A_TIME",caRd,&tmv);
        localtime_r(&(f_atribute.st_mtime),&tmv);
        wrt2dmsg("M_TIME",caRd,&tmv);

        sleep(40);

        // printf("c_time : %s\n",ctime(f_atribute.st_ctime));
        // printf("a_time : %s\n",ctime(f_atribute.st_atime));
        // printf("m_time : %s\n",ctime(f_atribute.st_mtime));

    }
    close(fd);
}