#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <thread>
#include <iostream>
#include <fstream>
#include <string>

#include "command.h"

#define SERV_PORT 7000
#define LISTEN_QUEUE 20

class Server {
    public:
        int listenfd;
        list* l;
        struct sockaddr_in server_sockaddr;
        Server(list* l_);
        ~Server();
};