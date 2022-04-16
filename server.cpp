//
// Created by FlagerLee on 2022/3/25.
//
#include <cstdio>
#include "utils.h"
#include "errno.h"


int main() {
#ifdef WINDOWS
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        printf("WSAStartup failed with error: %d\n", err);
        return 0;
    }
#endif

    // create udp socket
    SOCKET udp_server = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_server == INVALID_SOCKET) {
        // create failed
        perror("Invalid socket initialization");
        printf("%d", WSAGetLastError());
        return 0;
    }

    // bind udp socket
    sockaddr_in addr = {AF_INET, htons(SERVER_PORT), {}, {}};
    if (bind(udp_server, (sockaddr *) &addr, sizeof(addr)) == SOCKET_ERROR) {
        // bind failed
        perror("Cannot bind udp socket");
        closesocket(udp_server);
        WSACleanup();
        return 0;
    }

    // init receive buffer
    char* buf = new char[BUFF_SIZE];
    int buf_len = 0;
    auto client_addr = new sockaddr_in;
    while (true) {
        memset(buf, 0, BUFF_SIZE);
        int byte = recvfrom(udp_server, buf, BUFF_SIZE, 0, (sockaddr *) &client_addr, &buf_len);
        if (byte == 0) {
            printf("sockfd: %d read over\n", udp_server);
            break;
        }
        if (byte < 0) {
            printf("read failed: %d\n", WSAGetLastError());
            break;
        }
        printf("client IP: %s, port: %d, datalen: %d, info: %s\n", inet_ntoa(client_addr->sin_addr), client_addr->sin_port, byte, buf);
    }
}