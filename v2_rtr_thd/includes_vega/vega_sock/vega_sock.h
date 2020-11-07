#ifndef VEGA_SOCK__H
#define VEGA_SOCK__H

#include <stdint.h>

#ifndef SO_REUSEPORT
	#define SO_REUSEPORT 15
#endif

#define TH_BUF 1024
#define IN_BUF  200
#define PORT_40_CMD 40
#define PORT_41_PER 41
#define DEV_IP "10.1.1.107"

#define IP_SZMIN    ( 3+(4*1) )
#define IP_SZMAX    ( 3+(4*3) )


int sock_reconnect(int *fd_sock, const struct sockaddr_in *serv_addr);
int validate_ip(const char *ip);
int start_clnt(struct sockaddr_in *srvr_addr, char *str_ip, int port);
int restart_clnt(int *fd, char *str_ip, struct sockaddr_in *srvr_addr);


#endif
