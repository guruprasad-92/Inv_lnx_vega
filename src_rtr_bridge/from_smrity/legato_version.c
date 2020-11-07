#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

typedef struct FMW_VER___
{
	char v_fs[50];
	char v_krn[50];
    char v_mdm_fmw[50];
	unsigned int v_app_rls;
    unsigned int v_app_inv;
}FMW_VER__;



int fmw_ver(FMW_VER__ *ver)
{
    int fp,ret;
    char str[420]={0};
    char tmp[20] ={0};
    char *p1 = NULL;
    char *p2 = NULL;
    fp = open("/etc/legato/version", O_RDONLY);
    if (fp < 0) 
    { 
        printf("open() failed due to : %s\n",strerror(errno));
        printf("Exiting ... .. .\n");
        close(fp);
        return -1;
    } 
    ret = read(fp,(void *)str, 410);
    if (ret < 0) 
    { 
        printf("read() failed due to : %s\n",strerror(errno));
        printf("Exiting ... .. .\n");
        close(fp);
        return -1;
    } 
        
    strcpy(tmp,"version: ");
    p1 = strstr(str,tmp);
    ret = strlen(tmp);
    p1 += ret ;
    p2 = strchr(p1,'\n');
    *p2 = 0;
    p2 += 1;
    strcpy(ver->v_fs,p1);
    
    memset(tmp,0,ret);
    strcpy(tmp,"linux-quic-quic: ");
    ret = strlen(tmp);
    p1 = strstr(p2,tmp);
    p1 += ret;
    p2 = strchr(p1,'\n');
    *p2 = 0;
    strcpy(ver->v_krn,p1);

    close(fp);

    FILE *fs;
    memset(str,0,400);
    fs = popen("app version RelCellularManagerApp", "r");
    if(fs == NULL)
    {
        ret = -1;
    }
    ret = fread(str,1,50,fs);
    if(ret > 0)
    {
        
    }
}

int main()
{
    FMW_VER__ ver;
    fmw_ver(&ver);
    printf("ver_fs = %s\n",ver.v_fs);
    printf("ver_krn = %s\n",ver.v_krn);
}