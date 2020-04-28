#ifndef LIB_H_
#define LIB_H_

/***********INCLUDES****************/

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include <netdb.h>


/************DECKARATION***************/

int tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

#endif //LIB_H_
