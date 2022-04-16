//
// Created by FlagerLee on 2022/3/26.
//

#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

/*
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  ECHO  |  identifier |  action  |
 * | 32-bit |    32-bit   |   8-bit  |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  payload length  |    payload   |
 * |      16-bit      |              |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#if defined(WINDOWS)
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")
#elif defined(LINUX)
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define SOCKET int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#endif

#include <stdio.h>

#define SERVER_PORT 8080
#define CLIENT_PORT 8002
#define BUFF_SIZE 65500

bool write_echo(char*, const char*, const char*, char, uint);
bool check_echo(const char*, char*, char*, char, uint&);

#endif //NETWORK_UTILS_H
