//
// Created by FlagerLee on 2022/3/25.
//
#include <cstdio>
#include "utils.h"


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
        return 0;
    }

    // bind udp socket
    sockaddr_in addr = {AF_INET, htons(SERVER_PORT), {}, {}};
    if (bind(udp_server, (sockaddr *) &addr, sizeof(addr)) == SOCKET_ERROR) {
        // bind failed
        perror("Cannot bind udp socket");
        return 0;
    }

    // init receive buffer
    char buf[BUFF_SIZE + 1];
    char payload[BUFF_SIZE];
    char identifier[4];
    uint payload_length;
    sockaddr_in client_addr = {};
    uint buf_len = sizeof(client_addr);
    while (true) {
        memset(buf, 0, sizeof(buf));
        memset(identifier, 0, sizeof(identifier));
        size_t byte = recvfrom(udp_server, buf, BUFF_SIZE, 0, (sockaddr *) &client_addr, &buf_len);
        if (byte == 0) {
            printf("sockfd: %d read over\n", udp_server);
            break;
        }
        if (byte < 0) {
            break;
        }
        // start parsing content
        /*
         * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         * |  ECHO  |  identifier |  action  |
         * | 32-bit |    32-bit   |   8-bit  |
         * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         * |  payload length  |    payload   |
         * |      16-bit      |              |
         * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
         */

        if(!check_echo(buf, payload, identifier, (char)0, payload_length)) {
            // parse failed
            continue;
        }
        printf("received echo request \"%s\"\n", payload);
        memset(buf, 0, sizeof(buf));
        if(!write_echo(buf, payload, identifier, (char)1, payload_length)) {
            // write failed
            continue;
        }
        sendto(udp_server, buf, BUFF_SIZE, 0, (sockaddr*)&client_addr, buf_len);
        printf("send message \"%s\"\n", payload);
    }
}