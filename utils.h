//
// Created by FlagerLee on 2022/3/26.
//

#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#if defined(WINDOWS)
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined(LINUX)
#endif

#include <stdio.h>

#define SERVER_PORT 8080
#define CLIENT_PORT 8002
#define BUFF_SIZE 1024

#endif //NETWORK_UTILS_H
